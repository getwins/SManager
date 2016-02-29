
// SManager.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "SManager.h"
#include "MainFrm.h"

#include "SManagerDoc.h"
#include "SManagerView.h"

#include "string_identifer.h"
//#include "SysInfoDialog.h"
#include "LoginDialog.h"
#include "cust_dynamic_info_worker.h"
#include "ProductDialog.h"
#include "InstrumentDialog.h"
#include "CommissionTemplateManageDialog.h"
#include "MarginTemplateManageDialog.h"
#include "OpenCustDialog.h"
//#include "CustStatusMaintainDialog.h"
//#include "TradeRecordQueryDialog.h"
//#include "QryOrderDialg.h"
//#include "QryTradeDialog.h"
//#include "QryPositionDialog.h"
//#include "QryMoneyInOutDialog.h"
#include "DeleteCacheDialog.h"
#include "ModifyOperPasswdDialog.h"
#include "QrySettlementDialog.h"
#include "SysInfoDialog.h"
#include "ProxyOperSettingDialog.h"
//#include "CustMoneyInOutDialog.h"
//#include "ShowAllCustPosition.h"

#include "MdApi.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
//#include <boost/filesystem.hpp>
#include <chrono>
#include <cassert>
#include <vector>
#include <fstream>
#include <iterator>
#include <ctime>
//#include <stdio.h>
//#include <stdlib.h>
#include <io.h>
#include <regex>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "CommBCRequest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

user_config g_cfg = { 0 };
//bool g_exit = false;
//std::shared_ptr<std::thread> g_thr_custinfo;
//std::map<std::string, custinfo_st> g_map_custinfo;
//std::mutex g_custinfo_mutex;
const std::string g_conf = "conf.ini";
//thread_local int drtp_no;
const char *cache_file_dict = "dict.dat";
const char *cache_file_exchange = "exchange.dat";
const char *cache_file_seat = "seat.dat";
const char *cache_file_product = "product.dat";
const char *cache_file_exchange_product = "exchange_product.dat";

std::vector<std::string> g_OutputMsg;
std::mutex g_OutputMsgMutex;

template<typename T>
void read_cache(const char *file, std::vector<T> &dest)
{
	FILE *f = fopen(file, "rb");
	T item;
	while (fread(&item, sizeof(T), 1, f) == 1) {
		dest.push_back(item);
	}
	fclose(f);
}

template<typename T>
void write_cache(const char *file, std::vector<T> &src)
{
	FILE *f = fopen(file, "wb");
	//fwrite(src.data(), sizeof(T), src., f);
	for (std::vector<T>::iterator it = src.begin(); it != src.end(); ++it) {
		fwrite(&(*it), sizeof(T), 1, f);
	}
	fclose(f);
}

void load_conf()
{
	boost::property_tree::ptree ptree;
	boost::property_tree::read_ini(g_conf, ptree);

	std::string host = ptree.get<std::string>("drtp.host");
	host.copy(g_cfg.host, sizeof(g_cfg.host));
	g_cfg.port = ptree.get<int>("drtp.port", 3000);
	g_cfg.drtpno = ptree.get<int>("drtp.drtpno", 0);
	g_cfg.branchno = ptree.get<int>("drtp.branchno", 0);
	g_cfg.funcno = ptree.get<int>("drtp.funcno", 19900);

	//g_cfg.oper_code = ptree.get<std::string>("oper.code");
	//g_cfg.oper_passwd = ptree.get<std::string>("oper.passwd");
	//ptree.get<char*>("oper.code");
	std::string code = ptree.get<std::string>("oper.code");
	code.copy(g_cfg.oper_code, sizeof(g_cfg.oper_code) - 1);
	std::string passwd = ptree.get<std::string>("oper.passwd");
	passwd.copy(g_cfg.oper_passwd, sizeof(g_cfg.oper_passwd) - 1);
	g_cfg.oper_type = ptree.get<int>("oper.type", 0);

	g_cfg.bccall_timeout = ptree.get<int>("timeout.bccall", 20000);
	g_cfg.login_interval = ptree.get<int>("interval.login", 10);
	g_cfg.capital_interval = ptree.get<int>("interval.captial", 3);
	g_cfg.position_interval = ptree.get<int>("interval.position", 60);
	g_cfg.posidetail_interval = ptree.get<int>("interval.posi_detail", 60);
	g_cfg.order_interval = ptree.get<int>("interval.order", 60);
	g_cfg.trade_interval = ptree.get<int>("interval.trade", 60);
	//g_cfg.oper_passwd = ptree.get<std::string>("oper.passwd");

	ptree.get<std::string>("market.brokerid").copy(g_cfg.market_brokerid, sizeof(g_cfg.market_brokerid));
	ptree.get<std::string>("market.userid").copy(g_cfg.market_userid, sizeof(g_cfg.market_userid));
	ptree.get<std::string>("market.passwd").copy(g_cfg.market_passwd, sizeof(g_cfg.market_passwd));
	ptree.get<std::string>("market.front").copy(g_cfg.market_front, sizeof(g_cfg.market_front));

	g_cfg.wnd_float = 0;
}


void write_conf(const std::string &nodepath, const std::string &nodevalue)
{
	boost::property_tree::ptree ptree;
	boost::property_tree::read_ini(g_conf, ptree);
	ptree.put(nodepath, nodevalue);
	boost::property_tree::write_ini(g_conf, ptree);
}

void MyBCCLTInit()
{
	if (!BCCCLTInit(100))
		throw std::runtime_error("BCCCTLInit failed");
	
	g_cfg.drtpno = BCAddDrtpNode(g_cfg.host, g_cfg.port);


	//if (g_cfg.drtpno< 0)
	//	throw std::runtime_error("BCAddDrtpNode failed");

	BCSetDebugSwitch(true);

	BCHANDLE handle = BCNewHandle("cpack.dat");
	if(handle == NULL)
		throw std::runtime_error("BCNewHandle failed");
	BCDeleteHandle(handle);
}

//void MyBCCallRequest(BCHANDLE handle)
//{
//	int retcode;
//	char retmsg[1024] = { 0 };
//	if (!BCCallRequest(handle, g_cfg.drtpno, g_cfg.branchno, g_cfg.funcno, 10000, &retcode, retmsg))
//	{
//		AfxMessageBox("�ͷ�����ͨ���쳣�������˳���", MB_ICONSTOP | MB_OK);
//		::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_CLOSE, 0, 0);
//	}
//}

//
//bool MyBCRequest(BCHANDLE handle, FetchRowFunc_t func, bool ErrMsgToMessageBox, bool ErrMsgToOutput, std::string *ret_msg)
//{
//	int errcode;
//	char errmsg[256];
//	int record;
//	int retcode;
//
//	if (!BCCallRequest(handle, g_cfg.drtpno, g_cfg.branchno, g_cfg.funcno, 10000, &errcode, errmsg))
//	//if (!BCCallRequest(handle, drtp_no, g_cfg.branchno, g_cfg.funcno, 10000, &errcode, errmsg))
//	{
//		//theApp.addoutput
//		if (ErrMsgToMessageBox)
//			::AfxMessageBox(errmsg, MB_ICONSTOP | MB_OK);
//		if (ErrMsgToOutput)
//			PostOutputMsg(errmsg);
//		if (ret_msg)
//			*ret_msg = errmsg;
//		return false;
//	}
//
//	BCGetRetCode(handle, &retcode);
//	if (retcode)
//	{
//		BCGetStringFieldByName(handle, 0, "vsmess", errmsg, sizeof(errmsg));
//		if (ErrMsgToMessageBox)
//			::AfxMessageBox(errmsg, MB_ICONSTOP | MB_OK);
//		if (ErrMsgToOutput)
//			PostOutputMsg(errmsg);
//		if (ret_msg)
//			*ret_msg = errmsg;
//		return false;
//	}
//
//	BCGetRecordCount(handle, &record);
//
//	for (int i = 0; i < record; i++)
//	{
//		if(func)
//			func(handle, i);
//	}
//
//	while (BCCallNext(handle, 10000, &errcode, errmsg))
//	{
//		BCGetRetCode(handle, &retcode);
//		if (retcode)
//		{
//			BCGetStringFieldByName(handle, 0, "vsmess", errmsg, sizeof(errmsg));
//			if (ErrMsgToMessageBox)
//				::AfxMessageBox(errmsg, MB_ICONSTOP | MB_OK);
//			if (ErrMsgToOutput)
//				PostOutputMsg(errmsg);
//			if (ret_msg)
//				*ret_msg = errmsg;
//			return false;
//		}
//
//		BCGetRecordCount(handle, &record);
//		for (int i = 0; i < record; i++)
//		{
//			if(func)
//				func(handle, i);
//		}
//	}
//	
//	if (ret_msg)
//	{
//		BCGetStringFieldByName(handle, 0, "vsmess", errmsg, sizeof(errmsg));
//		*ret_msg = errmsg;
//	}
//	return true;
//}

void PostOutputMsg(const std::string &msg)
{
	std::unique_lock<std::mutex> lkc(g_OutputMsgMutex);
	time_t t;
	::time(&t);
	char buf[64] = { 0 };
	::strftime(buf, sizeof(buf), "[%T]", ::localtime(&t));
	g_OutputMsg.push_back(msg + buf);
	::PostMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), WM_USER_OUTPUT, 0, 0);
	//::PostMessage(theApp.GetMainWnd()->GetSafeHwnd(), WM_USER_OUTPUT, 0, 0);
}

std::vector<std::string> GetOutputMsgs()
{
	std::unique_lock<std::mutex> lkc(g_OutputMsgMutex);

	std::vector<std::string> ret(g_OutputMsg);
	g_OutputMsg.clear();
	return ret;
}


void RequestCustOrder(BCHANDLE handle, char *cust_no, char *date)
{
	FetchRowFunc_t fetcher = [](BCHANDLE handle, int row) {
		cust_order_st ord = { 0 };
		BCGetStringFieldByName(handle, row, "sdate0", ord.date, sizeof(ord.date));
		BCGetIntFieldByName(handle, row, "lserial0", &ord.orderseq);
		BCGetStringFieldByName(handle, row, "smarket_code", ord.exchangeid, sizeof(ord.exchangeid));
		BCGetStringFieldByName(handle, row, "sstock_code", ord.productid, sizeof(ord.productid));
		BCGetStringFieldByName(handle, row, "sdate1", ord.delivery_date, sizeof(ord.delivery_date));
		BCGetStringFieldByName(handle, row, "sstatus0", ord.order_status, sizeof(ord.order_status));
		BCGetStringFieldByName(handle, row, "sstatus1", ord.offset_flag, sizeof(ord.offset_flag));
		BCGetStringFieldByName(handle, row, "sstatus2", ord.hedge_flag, sizeof(ord.hedge_flag));
		BCGetStringFieldByName(handle, row, "sstatus3", ord.direction, sizeof(ord.direction));
		BCGetDoubleFieldByName(handle, row, "damt0", &ord.order_price);
		BCGetIntFieldByName(handle, row, "lvol0", &ord.origin_total_volume);
		BCGetIntFieldByName(handle, row, "lvol1", &ord.total_volume);
		BCGetDoubleFieldByName(handle, row, "damt1", &ord.traded_price);
		BCGetIntFieldByName(handle, row, "lvol2", &ord.traded_volume);
		BCGetStringFieldByName(handle, row, "sorder0", ord.sysid, sizeof(ord.sysid));
		BCGetStringFieldByName(handle, row, "stime0", ord.order_time, sizeof(ord.order_time));
		BCGetStringFieldByName(handle, row, "stime1", ord.insert_time, sizeof(ord.insert_time));
		BCGetStringFieldByName(handle, row, "sholder_ac_no2", ord.trade_no, sizeof(ord.trade_no));
		BCGetStringFieldByName(handle, row, "sholder_type", ord.order_way, sizeof(ord.order_way));
		BCGetStringFieldByName(handle, row, "scust_no2", ord.cancel_cust, sizeof(ord.cancel_cust));
		BCGetStringFieldByName(handle, row, "stime2", ord.cancel_time, sizeof(ord.cancel_time));
		BCGetStringFieldByName(handle, row, "sstatus4", ord.order_type, sizeof(ord.order_type));
		BCGetStringFieldByName(handle, row, "sserial2", ord.seatid, sizeof(ord.seatid));
		BCGetStringFieldByName(handle, row, "scurrency_type", ord.currency, sizeof(ord.currency));
	};
}

void RequestCustTrade(BCHANDLE handle, char *cust_no, char *date)
{
	FetchRowFunc_t fetcher = [](BCHANDLE handle, int row) {
		cust_order_st ord = { 0 };
		cust_trade_st tra = { 0 };
		BCGetStringFieldByName(handle, row, "sdate0", tra.date, sizeof(tra.date));
		BCGetIntFieldByName(handle, row, "lserial0", &tra.orderseq);
		BCGetStringFieldByName(handle, row, "sholder_ac_no2", tra.trade_no, sizeof(tra.trade_no));
		BCGetStringFieldByName(handle, row, "smarket_code", tra.exchangeid, sizeof(tra.exchangeid));
		BCGetStringFieldByName(handle, row, "sstock_code", tra.productid, sizeof(tra.productid));
		BCGetStringFieldByName(handle, row, "sdate1", tra.delivery_date, sizeof(tra.delivery_date));
		BCGetStringFieldByName(handle, row, "sstatus3", tra.direction, sizeof(tra.direction));
		BCGetDoubleFieldByName(handle, row, "damt1", &tra.price);
		BCGetIntFieldByName(handle, row, "lvol2", &tra.volume);
		BCGetStringFieldByName(handle, row, "sstatus1", tra.offset_flag, sizeof(tra.offset_flag));
		BCGetStringFieldByName(handle, row, "sstatus2", tra.hedge_flag, sizeof(tra.hedge_flag));
		BCGetStringFieldByName(handle, row, "sorder0", tra.sysid, sizeof(tra.sysid));
		BCGetStringFieldByName(handle, row, "sserial2", tra.seatid, sizeof(tra.seatid));
		BCGetStringFieldByName(handle, row, "scurrency_type", tra.currency, sizeof(tra.currency));
		BCGetStringFieldByName(handle, row, "sstatus0", tra.force_close, sizeof(tra.force_close));

	};
}
// CSManagerApp

BEGIN_MESSAGE_MAP(CSManagerApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CSManagerApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	//	ON_COMMAND(ID_SYSTEM_INFO, &CSManagerApp::OnSystemInfo)
	ON_COMMAND(ID_PRODUCT_DATA, &CSManagerApp::OnProductData)
	ON_COMMAND(ID_INSTRUMENT_DATA, &CSManagerApp::OnInstrumentData)
	ON_COMMAND(ID_COMMISSION_MANAGE, &CSManagerApp::OnCommissionManage)
	ON_COMMAND(ID_MARGIN_MANAGE, &CSManagerApp::OnMarginManage)
	//ON_COMMAND(ID_HIS_TRADE, &CSManagerApp::OnHisTrade)
	ON_COMMAND(ID_ADD_ACCOUNT, &CSManagerApp::OnAddAccount)
	//ON_COMMAND(ID_CUST_STATUS_MAINTAIN, &CSManagerApp::OnCustStatusMaintain)
	//ON_COMMAND(ID_ORDER_QUERY, &CSManagerApp::OnOrderQuery)
	//ON_COMMAND(ID_TRADE_QUERY, &CSManagerApp::OnTradeQuery)
	//ON_COMMAND(ID_POSITION_QUERY, &CSManagerApp::OnPositionQuery)
	//ON_COMMAND(ID_QUERY_MONEY_IN_OUT, &CSManagerApp::OnQueryMoneyInOut)
	ON_COMMAND(ID_CLEAR_CACHE, &CSManagerApp::OnClearCache)
	ON_COMMAND(ID_MODIFY_PASSWD, &CSManagerApp::OnModifyPasswd)
	ON_COMMAND(ID_SETTLEMENT_QUERY, &CSManagerApp::OnSettlementQuery)
	ON_COMMAND(ID_SYSTEM_INFO, &CSManagerApp::OnSystemInfo)
	//ON_COMMAND(ID_MONEY_IN_OUT, &CSManagerApp::OnMoneyInOut)
	//ON_COMMAND(ID_SHOW_ALL_CUST_POSITION, &CSManagerApp::OnShowAllCustPosition)
	ON_COMMAND(ID_MESSAGEBEE, &CSManagerApp::OnMessagebee)
	ON_UPDATE_COMMAND_UI(ID_MESSAGEBEE, &CSManagerApp::OnUpdateMessagebee)
	ON_COMMAND(ID_PROXY_OPER_SETTING, &CSManagerApp::OnProxyOperSetting)
END_MESSAGE_MAP()


// CSManagerApp ����

CSManagerApp::CSManagerApp()
{
	m_bHiColorIcons = TRUE;

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���: 
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("SManager.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��

	m_CustNum = -1;
	m_bMsgBee = false;
}

// Ψһ��һ�� CSManagerApp ����

CSManagerApp theApp;


// CSManagerApp ��ʼ��

//system_info_st CSManagerApp::GetSysInfo() const
//{
//	std::unique_lock<std::mutex> lkc(m_SysInfoMutex)
//	return m_SysInfo;
//}

//void CSManagerApp::RequestSysInfo()
//{
//	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
//		std::unique_lock<std::mutex> lkc(m_SysInfoMutex);
//		BCGetStringFieldByName(handle, 0, "sdate0", m_SysInfo.system_date, sizeof(m_SysInfo.system_time));
//		BCGetStringFieldByName(handle, 0, "sdate2", m_SysInfo.check_date, sizeof(m_SysInfo.check_date));
//		BCGetStringFieldByName(handle, 0, "sdate1", m_SysInfo.next_tradingday, sizeof(m_SysInfo.next_tradingday));
//		BCGetStringFieldByName(handle, 0, "sdate3", m_SysInfo.host_date, sizeof(m_SysInfo.host_date));
//		BCGetStringFieldByName(handle, 0, "sstatus0", m_SysInfo.system_status, sizeof(m_SysInfo.system_status));
//		BCGetStringFieldByName(handle, 0, "stime0", m_SysInfo.system_time, sizeof(m_SysInfo.system_time));
//	};
//	//system_info_st sysinfo;
//	//BCHANDLE handle = BCNewHandle(XpackDescribleFile);
//	Scoped_BCHANDLE handle;
//	BCResetHandle(handle);
//	BCSetRequestType(handle, 100319);
//	BCSetStringFieldByName(handle, 0, "sbranch_code0", "000");
//	MyBCRequest(handle, fetcher);
//
//	//BCDeleteHandle(handle);
//}

void CSManagerApp::PreRequestCustInfo()
{
	//drtp_no = BCAddDrtpNode(g_cfg.host, g_cfg.port);

	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
		char cust_open_oper[11] = { 0 };
		BCGetStringFieldByName(handle, row, "scust_no", cust_open_oper, 11);
		if (strcmp(cust_open_oper, g_cfg.oper_code) == 0) {
			cust_base_info_st bi = { 0 };
			BCGetStringFieldByName(handle, row, "sholder_ac_no", bi.basic.scust_no, sizeof(bi.basic.scust_no));
			BCGetStringFieldByName(handle, row, "sname", bi.basic.sname, sizeof(bi.basic.sname));
			BCGetStringFieldByName(handle, row, "sstatus3", bi.basic.sstatus0, sizeof(bi.basic.sstatus0));
			BCGetStringFieldByName(handle, row, "scust_type2", bi.basic.card_type, sizeof(bi.basic.card_type));
			BCGetStringFieldByName(handle, row, "scert_no", bi.basic.card_no, sizeof(bi.basic.card_no));
			BCGetStringFieldByName(handle, row, "sserial1", bi.basic.cust_class, sizeof(bi.basic.cust_class));
			AddCust(bi);
		}
		
	};


	Scoped_BCHANDLE handle;

	if (g_cfg.oper_type == 0)
	{
		BCSetRequestType(handle, 859999);

		BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);//����Ա
																	   //BCSetStringFieldByName(handle, 0, "sstatus1", "0"); //�ͻ�״̬
		BCSetStringFieldByName(handle, 0, "sstatus0", "1"); //�ͻ�����
		BCSetStringFieldByName(handle, 0, "scust_no2", g_cfg.oper_code);//��������Ա
		BCSetStringFieldByName(handle, 0, "sstatus2", "0"); //�Ƿ���������ͻ�
		BCSetStringFieldByName(handle, 0, "sstatus3", "1"); //�Ƿ��������ͻ�
		if (MyBCRequest(handle, fetcher))
			PostOutputMsg("��ѯ�ͻ����ϳɹ�.");
		else
			PostOutputMsg("��ѯ�ͻ�����ʧ��.");
		//for each (cust_base_info_st cbi in m_CustBaseInfos)
		
	}
	else if (g_cfg.oper_type == 1)
	{
		BCResult result;
		std::vector<oper_cust_correspond_st> occs;
		result = BCRequestQryOperCustCorrespond_851243(handle, g_cfg.oper_code, occs);
		for each (oper_cust_correspond_st var in occs)
		{
			cust_base_info_st bi = { 0 };
			result = BCRequestCustBasic_850003(handle, var.cust_no, bi.basic);
			AddCust(bi);
		}
	}

	for (auto it = m_CustBaseInfos.begin(); it != m_CustBaseInfos.end(); ++it)
	{

		PreRequestCustCommissionTemplate(handle, *it);
		PreRequestCustMarginTemplate(handle, *it);
		BCRequestRC_860000(handle, it->basic.scust_no, "", "", it->rcs);
	}
	
	//BCDeleteHandle(handle);

	//cust_dynamic_info_worker::instance()->start();
	//while (!theApp.m_pMainWnd) {
	//	std::this_thread::sleep_for(std::chrono::microseconds(1));
	//}
	//::PostMessage(theApp.m_pMainWnd->GetSafeHwnd(), WM_USER_CUST_BASE_INFO, CUST_BASE_COUNT_CHANGED, 0);
	//cust_dynamic_info_worker::instance()->start();
}

void CSManagerApp::PreRequestCustCommissionTemplate(BCHANDLE handle, cust_base_info_st &cbi)
{
	//Scoped_BCHANDLE handle;
	auto fetcher = [](cust_base_info_st &cbi, BCHANDLE handle, int row) {
		BCGetStringFieldByName(handle, 0, "sstation0", cbi.commission_temp.template_no,
			sizeof(cbi.commission_temp.template_no));
		BCGetStringFieldByName(handle, 0, "scusttypes", cbi.commission_temp.template_name,
			sizeof(cbi.commission_temp.template_name));
	};
	BCResetHandle(handle);
	BCSetRequestType(handle, 851343);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cbi.basic.scust_no);
	MyBCRequest(handle, std::bind(fetcher, std::ref(cbi), std::placeholders::_1, std::placeholders::_2));

}

void CSManagerApp::PreRequestCustMarginTemplate(BCHANDLE handle, cust_base_info_st & cbi)
{
	//Scoped_BCHANDLE handle;
	auto fetcher = [](cust_base_info_st &cbi, BCHANDLE handle, int row) {
		BCGetStringFieldByName(handle, 0, "sstation0", cbi.margin_temp.template_no,
			sizeof(cbi.commission_temp.template_no));
		BCGetStringFieldByName(handle, 0, "scusttypes", cbi.margin_temp.template_name,
			sizeof(cbi.commission_temp.template_name));
	};
	BCResetHandle(handle);
	BCSetRequestType(handle, 851349);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cbi.basic.scust_no);
	MyBCRequest(handle, std::bind(fetcher, std::ref(cbi), std::placeholders::_1, std::placeholders::_2));
}


void CSManagerApp::AsyncRequestBaseInfo()
{
	//RequestSystemInfo();
	//MyBCCLTInit();
	//drtp_no = BCAddDrtpNode(g_cfg.host, g_cfg.port);
	//BCHANDLE handle = BCNewHandle(XpackDescribleFile);
	Scoped_BCHANDLE handle;

	//RequestDict(handle);
	
	RequestExchange(handle);
	RequestSeat(handle);
	RequestProduct(handle);
	RequestExchangeProduct(handle);
	RequestInstrument(handle);

	RequestCommissionTemplateNo(handle);
	RequestCommissionTemplate(handle);
	RequestMarginTemplateNo(handle);
	RequestMarginTemplate(handle);

	BCRequestCustClassInfo_851028(handle, 1, m_BaseInfo.cust_classes);
	//BCDeleteHandle(handle);
}

void CSManagerApp::PreRequestDict()
{

	//if (boost::filesystem::exists("cache/dict.dat")) {
	//	//std::istream_iterator<dict_st> it_beg("cache/dict.dat");
	//}
	if (access(cache_file_dict, 00) == 0) {
	//if (access("dict.dat", 00) == 0) {
		//FILE *f = fopen("dict.dat", "r");
		//dict_st d = { 0 };
		//while (fread(&d, sizeof(d), 1, f) == 1) {
		//	m_BaseInfo.dicts.push_back(d);
		//}
		//fclose(f);
		read_cache(cache_file_dict, m_BaseInfo.dicts);
	}
	else {
		FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
			dict_st dict = { 0 };
			BCGetIntFieldByName(handle, row, "lcert_code", &dict.dict_class);
			BCGetStringFieldByName(handle, row, "sname", dict.dict_code, sizeof(dict.dict_code));
			BCGetStringFieldByName(handle, row, "sall_name", dict.dict_note, sizeof(dict.dict_note));
			m_BaseInfo.dicts.push_back(dict);
		};
		Scoped_BCHANDLE handle;
		//BCResetHandle(handle);
		BCSetRequestType(handle, 300402);
		BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
		if (MyBCRequest(handle, fetcher)) {
			std::sort(m_BaseInfo.dicts.begin(), m_BaseInfo.dicts.end());
			write_cache(cache_file_dict, m_BaseInfo.dicts);
		}
		
		//FILE *f = fopen("dict.dat", "w");
		//fwrite(m_BaseInfo.dicts.data(), sizeof(dict_st), m_BaseInfo.dicts.size(), f);
		//fclose(f);
	}
	
}

//void CSManagerApp::FetchDictRow(BCHANDLE handle, int row)
//{
//	dict_st dict = { 0 };
//	BCGetIntFieldByName(handle, row, "lcert_code", &dict.dict_class);
//	BCGetStringFieldByName(handle, row, "sname", dict.dict_code, sizeof(dict.dict_code));
//	BCGetStringFieldByName(handle, row, "sall_name", dict.dict_note, sizeof(dict.dict_note));
//	m_BaseInfo.dicts.push_back(dict);
//}

void CSManagerApp::RequestExchange(BCHANDLE handle)
{
	if (access(cache_file_exchange, 00) == 0) {
		//FILE *f = fopen(cache_file_exchange, "r");
		//exchange_st e;
		//while (fread(&e, sizeof(e), 1) == 1) {
		//	m_BaseInfo.exchanges.push_back(e);
		//}
		//fclose(f);
		read_cache(cache_file_exchange, m_BaseInfo.exchanges);
	}
	else {
		FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
			exchange_st exchange = { 0 };
			BCGetStringFieldByName(handle, row, "smarket_code", exchange.exchangeid, sizeof(exchange.exchangeid));
			BCGetStringFieldByName(handle, row, "sbank_acc", exchange.chinese_name, sizeof(exchange.chinese_name));
			BCGetStringFieldByName(handle, row, "sbank_acc2", exchange.chinese_all_name,
				sizeof(exchange.chinese_all_name));
			BCGetStringFieldByName(handle, row, "scard1", exchange.english_name, sizeof(exchange.english_name));
			BCGetStringFieldByName(handle, row, "sstatus1", exchange.status, sizeof(exchange.status));
			m_BaseInfo.exchanges.push_back(exchange);
		};

		BCResetHandle(handle);
		BCSetRequestType(handle, 851230);
		BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
		if (MyBCRequest(handle, fetcher)) {
			//FILE *f = fopen(cache_file_exchange, "w");
			//fwrite(m_BaseInfo.exchanges.data(), sizeof(exchange_st), m_BaseInfo.exchanges.size(), f);
			//fclose(f);
			write_cache(cache_file_exchange, m_BaseInfo.exchanges);
		}
	}
}
void CSManagerApp::RequestSeat(BCHANDLE handle)
{
	if (access(cache_file_seat, 00) == 0) {
		read_cache(cache_file_seat, m_BaseInfo.seats);
	}
	else {
		FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
			seat_st s = { 0 };
			BCGetStringFieldByName(handle, row, "sserial2", s.seat_no, sizeof(s.seat_no));
			BCGetStringFieldByName(handle, row, "sname", s.seat_name, sizeof(s.seat_name));
			BCGetStringFieldByName(handle, row, "sname2", s.member_no, sizeof(s.member_no));
			BCGetStringFieldByName(handle, row, "sstatus2", s.seat_status, sizeof(s.seat_status));
			m_BaseInfo.seats.push_back(s);
		};
		BCResetHandle(handle);
		BCSetRequestType(handle, 851233);
		BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
		if (MyBCRequest(handle, fetcher)) {
			write_cache(cache_file_seat, m_BaseInfo.seats);
		}
	}
}
//
//void CSManagerApp::FetchExchangeRow(BCHANDLE handle, int row)
//{
//	exchange_st exchange = { 0 };
//	BCGetStringFieldByName(handle, row, "smarket_code", exchange.exchangeid, sizeof(exchange.exchangeid));
//	BCGetStringFieldByName(handle, row, "sbank_acc", exchange.chinese_name, sizeof(exchange.chinese_name));
//	BCGetStringFieldByName(handle, row, "sbank_acc2", exchange.chinese_all_name,
//		sizeof(exchange.chinese_all_name));
//	BCGetStringFieldByName(handle, row, "scard1", exchange.english_name, sizeof(exchange.english_name));
//	BCGetStringFieldByName(handle, row, "sstatus1", exchange.status, sizeof(exchange.status));
//	m_BaseInfo.exchanges.push_back(exchange);
//}

void CSManagerApp::RequestProduct(BCHANDLE handle)
{
	if (access(cache_file_product, 00) == 0) {
		read_cache(cache_file_product, m_BaseInfo.products);
	}
	else {
		FetchRowFunc_t fetcher = [this](HANDLE handle, int row) {
			product_st product = { 0 };
			BCGetStringFieldByName(handle, row, "sstock_code", product.productid, sizeof(product.productid));
			BCGetStringFieldByName(handle, row, "sname", product.name, sizeof(product.name));
			BCGetStringFieldByName(handle, row, "sholder_ac_no2", product.inst_rule, sizeof(product.inst_rule));
			BCGetStringFieldByName(handle, row, "sstatus1", product.trade_rule, sizeof(product.trade_rule));//27
			BCGetStringFieldByName(handle, row, "scurrency_type", product.currency_type, sizeof(product.currency_type));//182
			BCGetStringFieldByName(handle, row, "sstatus2", product.type, sizeof(product.type));
			BCGetStringFieldByName(handle, row, "sstatus3", product.status, sizeof(product.status));
			BCGetDoubleFieldByName(handle, row, "damt13", &product.multiple);
			BCGetDoubleFieldByName(handle, row, "damt12", &product.price_tick);
			BCGetIntFieldByName(handle, row, "lvol0", &product.min_lots);
			BCGetIntFieldByName(handle, row, "lvol1", &product.max_lots);
			BCGetIntFieldByName(handle, row, "lvol2", &product.max_position);
			BCGetDoubleFieldByName(handle, row, "damt0", &product.upper_limit_rate);
			BCGetDoubleFieldByName(handle, row, "damt1", &product.lower_limit_rate);
			BCGetDoubleFieldByName(handle, row, "damt2", &product.commission_rate_by_money);
			BCGetDoubleFieldByName(handle, row, "damt3", &product.commission_rate_by_volume);
			BCGetDoubleFieldByName(handle, row, "damt8", &product.margin_rate_by_money);
			BCGetDoubleFieldByName(handle, row, "damt9", &product.margin_rate_by_volume);
			m_BaseInfo.products.push_back(product);
		};
		BCResetHandle(handle);
		BCSetRequestType(handle, 851238);
		BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
		if (MyBCRequest(handle, fetcher)) {
			write_cache(cache_file_product, m_BaseInfo.products);
		}
	}
	
	//MyBCCallRequest(handle);
	//int record = 0;
	//BCGetRecordCount(handle, &record);
	//for (int i = 0; i < record; i++)
	//{
	//	product_st product = { 0 };
	//	BCGetStringFieldByName(handle, i, "sstock_code", product.productid, sizeof(product.productid));
	//	BCGetStringFieldByName(handle, i, "sname", product.name, sizeof(product.name));
	//	BCGetStringFieldByName(handle, i, "sholder_ac_no2", product.inst_rule,sizeof(product.inst_rule));
	//	BCGetStringFieldByName(handle, i, "sstatus1", product.trade_rule, sizeof(product.trade_rule));//27
	//	BCGetStringFieldByName(handle, i, "scurrency_type", product.currency_type, sizeof(product.currency_type));//182
	//	BCGetStringFieldByName(handle, i, "sstatus2", product.type, sizeof(product.type));
	//	BCGetDoubleFieldByName(handle, i, "damt13", &product.multiple);
	//	BCGetDoubleFieldByName(handle, i, "damt12", &product.price_tick);
	//	BCGetIntFieldByName(handle, i, "lvol0", &product.min_lots);
	//	BCGetIntFieldByName(handle, i, "lvol1", &product.max_lots);
	//	BCGetIntFieldByName(handle, i, "lvol2", &product.max_position);
	//	BCGetDoubleFieldByName(handle, i, "damt0", &product.upper_limit_rate);
	//	BCGetDoubleFieldByName(handle, i, "damt1", &product.lower_limit_rate);
	//	BCGetDoubleFieldByName(handle, i, "damt2", &product.commission_rate_by_money);
	//	BCGetDoubleFieldByName(handle, i, "damt3", &product.commission_rate_by_volume);
	//	BCGetDoubleFieldByName(handle, i, "damt8", &product.margin_rate_by_money);
	//	BCGetDoubleFieldByName(handle, i, "damt9", &product.margin_rate_by_volume);
	//	
	//	m_BaseInfo.products.push_back(product);
	//}
	//int errcode;
	//char errmsg[256];
	//while (BCCallNext(handle, 10000, &errcode, errmsg))
	//{
	//	BCGetRecordCount(handle, &record);
	//	for (int i = 0; i < record; i++)
	//	{
	//		product_st product = { 0 };
	//		BCGetStringFieldByName(handle, i, "sstock_code", product.productid, sizeof(product.productid));
	//		BCGetStringFieldByName(handle, i, "sname", product.name, sizeof(product.name));
	//		BCGetStringFieldByName(handle, i, "sholder_ac_no2", product.inst_rule, sizeof(product.inst_rule));
	//		BCGetStringFieldByName(handle, i, "sstatus1", product.trade_rule, sizeof(product.trade_rule));//27
	//		BCGetStringFieldByName(handle, i, "scurrency_type", product.currency_type, sizeof(product.currency_type));//182
	//		BCGetStringFieldByName(handle, i, "sstatus2", product.type, sizeof(product.type));
	//		BCGetDoubleFieldByName(handle, i, "damt13", &product.multiple);
	//		BCGetDoubleFieldByName(handle, i, "damt12", &product.price_tick);
	//		BCGetIntFieldByName(handle, i, "lvol0", &product.min_lots);
	//		BCGetIntFieldByName(handle, i, "lvol1", &product.max_lots);
	//		BCGetIntFieldByName(handle, i, "lvol2", &product.max_position);
	//		BCGetDoubleFieldByName(handle, i, "damt0", &product.upper_limit_rate);
	//		BCGetDoubleFieldByName(handle, i, "damt1", &product.lower_limit_rate);
	//		BCGetDoubleFieldByName(handle, i, "damt2", &product.commission_rate_by_money);
	//		BCGetDoubleFieldByName(handle, i, "damt3", &product.commission_rate_by_volume);
	//		BCGetDoubleFieldByName(handle, i, "damt8", &product.margin_rate_by_money);
	//		BCGetDoubleFieldByName(handle, i, "damt9", &product.margin_rate_by_volume);

	//		m_BaseInfo.products.push_back(product);
	//	}
	//}
}

void CSManagerApp::RequestExchangeProduct(BCHANDLE handle)
{
	if (access(cache_file_exchange_product, 00) == 0) {
		read_cache(cache_file_exchange_product, m_BaseInfo.exchange_product);
	}
	else {
		FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
			exchange_product_st ep = { 0 };
			BCGetStringFieldByName(handle, row, "smarket_code", ep.exchangeid, sizeof(ep.exchangeid));
			BCGetStringFieldByName(handle, row, "sstock_code", ep.productid, sizeof(ep.productid));
			BCGetStringFieldByName(handle, row, "sname", ep.product_name, sizeof(ep.product_name));
			this->m_BaseInfo.exchange_product.push_back(ep);
		};
		//Scoped_BCHANDLE handle;
		BCResetHandle(handle);
		BCSetRequestType(handle, 851240);
		BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
		if (MyBCRequest(handle, fetcher)) {
			write_cache(cache_file_exchange_product, m_BaseInfo.exchange_product);
		}
	}
	
}

//void CSManagerApp::FetchProductRow(BCHANDLE handle, int row)
//{
//	product_st product = { 0 };
//	BCGetStringFieldByName(handle, row, "sstock_code", product.productid, sizeof(product.productid));
//	BCGetStringFieldByName(handle, row, "sname", product.name, sizeof(product.name));
//	BCGetStringFieldByName(handle, row, "sholder_ac_no2", product.inst_rule, sizeof(product.inst_rule));
//	BCGetStringFieldByName(handle, row, "sstatus1", product.trade_rule, sizeof(product.trade_rule));//27
//	BCGetStringFieldByName(handle, row, "scurrency_type", product.currency_type, sizeof(product.currency_type));//182
//	BCGetStringFieldByName(handle, row, "sstatus2", product.type, sizeof(product.type));
//	BCGetDoubleFieldByName(handle, row, "damt13", &product.multiple);
//	BCGetDoubleFieldByName(handle, row, "damt12", &product.price_tick);
//	BCGetIntFieldByName(handle, row, "lvol0", &product.min_lots);
//	BCGetIntFieldByName(handle, row, "lvol1", &product.max_lots);
//	BCGetIntFieldByName(handle, row, "lvol2", &product.max_position);
//	BCGetDoubleFieldByName(handle, row, "damt0", &product.upper_limit_rate);
//	BCGetDoubleFieldByName(handle, row, "damt1", &product.lower_limit_rate);
//	BCGetDoubleFieldByName(handle, row, "damt2", &product.commission_rate_by_money);
//	BCGetDoubleFieldByName(handle, row, "damt3", &product.commission_rate_by_volume);
//	BCGetDoubleFieldByName(handle, row, "damt8", &product.margin_rate_by_money);
//	BCGetDoubleFieldByName(handle, row, "damt9", &product.margin_rate_by_volume);
//
//	m_BaseInfo.products.push_back(product);
//}

void CSManagerApp::RequestInstrument(BCHANDLE handle)
{
	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row){
		instrument_st inst = { 0 };
		BCGetStringFieldByName(handle, row, "smarket_code", inst.exchangeid, sizeof(inst.exchangeid));
		BCGetStringFieldByName(handle, row, "sstock_code", inst.productid, sizeof(inst.productid));
		BCGetStringFieldByName(handle, row, "sname2", inst.product_name, sizeof(inst.product_name));
		BCGetStringFieldByName(handle, row, "sdate1", inst.delivery_date, sizeof(inst.delivery_date));
		BCGetStringFieldByName(handle, row, "scurrency_type2", inst.delivery_rule, sizeof(inst.delivery_rule));
		BCGetStringFieldByName(handle, row, "scust_auth", inst.instrumentid, sizeof(inst.instrumentid));
		BCGetStringFieldByName(handle, row, "scurrency_type", inst.currency_type, sizeof(inst.currency_type));
		BCGetIntFieldByName(handle, row, "lvol5", &inst.max_position);
		BCGetIntFieldByName(handle, row, "lvol10", &inst.multiple);
		BCGetDoubleFieldByName(handle, row, "damt15", &inst.min_lots);
		BCGetDoubleFieldByName(handle, row, "damt14", &inst.max_lots);
		BCGetDoubleFieldByName(handle, row, "damt6", &inst.price_tick);
		BCGetStringFieldByName(handle, row, "sstatus1", inst.status, sizeof(inst.status));
		m_BaseInfo.instruments.push_back(inst);
	};

	//Scoped_BCHANDLE handle;
	BCResetHandle(handle);
	BCSetRequestType(handle, 854088);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	//BCSetStringFieldByName(handle, 0, "scust_auth", ""); //��Լ��
	//BCSetStringFieldByName(handle, 0, "smarket_code", ""); //����������

	//MyBCRequest(handle, std::bind(&CSManagerApp::FetchInstrumentRow, this, std::placeholders::_1,
	//	std::placeholders::_2));

	if (MyBCRequest(handle, fetcher))
	{
		std::sort(m_BaseInfo.instruments.begin(), m_BaseInfo.instruments.end());
		PostOutputMsg("ȡ��Լ�������.");
	}
	//PostMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), WM_USER_REQUEST_INST_DONE, 0, 0);
}

//void CSManagerApp::FetchInstrumentRow(BCHANDLE handle, int row)
//{
//	instrument_st inst = { 0 };
//	BCGetStringFieldByName(handle, row, "smarket_code", inst.exchangeid, sizeof(inst.exchangeid));
//	BCGetStringFieldByName(handle, row, "sstock_code", inst.productid, sizeof(inst.productid));
//	BCGetStringFieldByName(handle, row, "sname2", inst.product_name, sizeof(inst.product_name));
//	BCGetStringFieldByName(handle, row, "sdate1", inst.delivery_date, sizeof(inst.delivery_date));
//	BCGetStringFieldByName(handle, row, "scurrency_type2", inst.delivery_rule, sizeof(inst.delivery_rule));
//	BCGetStringFieldByName(handle, row, "scust_auth", inst.instrumentid, sizeof(inst.instrumentid));
//	BCGetStringFieldByName(handle, row, "scurrency_type", inst.currency_type, sizeof(inst.currency_type));
//	BCGetIntFieldByName(handle, row, "lvol5", &inst.max_position);
//	BCGetIntFieldByName(handle, row, "lvol10", &inst.multiple);
//	BCGetDoubleFieldByName(handle, row, "damt15", &inst.min_lots);
//	BCGetDoubleFieldByName(handle, row, "damt14", &inst.max_lots);
//	BCGetDoubleFieldByName(handle, row, "damt6", &inst.price_tick);
//	BCGetStringFieldByName(handle, row, "sstatus1", inst.status, sizeof(inst.status));
//	m_BaseInfo.instruments.push_back(inst);
//}

void CSManagerApp::RequestCommissionTemplateNo(BCHANDLE handle)
{
	//struct FetchRow
	//{
	//	FetchRow(std::vector<commission_template_no_st> &v_ctn)
	//		: v_ctn_(v_ctn){}
	//	void operator() (BCHANDLE handle, int row)
	//	{
	//		commission_template_no_st ctn = { 0 };
	//		BCGetStringFieldByName(handle, row, "sholder_ac_no", ctn.template_no, sizeof(ctn.template_no));
	//		BCGetStringFieldByName(handle, row, "scusttypes", ctn.template_name, sizeof(ctn.template_name));
	//		v_ctn_.push_back(ctn);
	//	}
	//	std::vector<commission_template_no_st> &v_ctn_;
	//}fetcher(m_BaseInfo.commission_no);

	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
		commission_template_no_st ctn = { 0 };
		BCGetStringFieldByName(handle, row, "sholder_ac_no", ctn.template_no, sizeof(ctn.template_no));
		BCGetStringFieldByName(handle, row, "scusttypes", ctn.template_name, sizeof(ctn.template_name));
		m_BaseInfo.commission_no.push_back(ctn);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 851345);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", ""); //
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", ""); //

	if (MyBCRequest(handle, fetcher))
		PostOutputMsg("��ѯ��������ģ���ܱ�ɹ�");
	else
		PostOutputMsg("��ѯ��������ģ���ܱ�ʧ��");
}

void CSManagerApp::RequestCommissionTemplate(BCHANDLE handle)
{
	//struct FetchRow
	//{
	//	FetchRow(StrCtMap_t &m_ct) : m_ct_(m_ct){}
	//	void operator()(BCHANDLE handle, int row)
	//	{
	//		commission_template_st ct = { 0 };
	//		BCGetStringFieldByName(handle, row, "sholder_ac_no", ct.template_no, sizeof(ct.template_no));
	//		BCGetStringFieldByName(handle, row, "scusttypes", ct.template_name, sizeof(ct.template_name));
	//		BCGetStringFieldByName(handle, row, "smarket_code", ct.exchangeid, sizeof(ct.exchangeid));
	//		BCGetStringFieldByName(handle, row, "sstock_code", ct.productid, sizeof(ct.productid));
	//		BCGetStringFieldByName(handle, row, "sdate0", ct.delivery_date, sizeof(ct.delivery_date));
	//		BCGetDoubleFieldByName(handle, row, "damt2", &ct.commission_rate_by_money);
	//		BCGetDoubleFieldByName(handle, row, "damt3", &ct.commission_rate_by_volume);
	//		BCGetDoubleFieldByName(handle, row, "damt8", &ct.offset_td_commission_rate_by_money);
	//		BCGetDoubleFieldByName(handle, row, "damt9", &ct.offset_td_commission_rate_by_volume);
	//		//m_ct_[ct.template_no].push_back(ct);
	//		std::vector<commission_template_st> &v_ct = m_ct_[ct.template_no];
	//		v_ct.push_back(ct);
	//	}
	//	StrCtMap_t &m_ct_;
	//}fetcher(m_BaseInfo.commissions);

	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
		commission_template_st ct = { 0 };
		BCGetStringFieldByName(handle, row, "sholder_ac_no", ct.template_no, sizeof(ct.template_no));
		BCGetStringFieldByName(handle, row, "scusttypes", ct.template_name, sizeof(ct.template_name));
		BCGetStringFieldByName(handle, row, "smarket_code", ct.exchangeid, sizeof(ct.exchangeid));
		BCGetStringFieldByName(handle, row, "sstock_code", ct.productid, sizeof(ct.productid));
		BCGetStringFieldByName(handle, row, "sdate0", ct.delivery_date, sizeof(ct.delivery_date));
		BCGetDoubleFieldByName(handle, row, "damt2", &ct.commission_rate_by_money);
		BCGetDoubleFieldByName(handle, row, "damt3", &ct.commission_rate_by_volume);
		BCGetDoubleFieldByName(handle, row, "damt8", &ct.offset_td_commission_rate_by_money);
		BCGetDoubleFieldByName(handle, row, "damt9", &ct.offset_td_commission_rate_by_volume);
		m_BaseInfo.commissions[ct.template_no].push_back(ct);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 851339);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	//BCSetStringFieldByName(handle, 0, "sholder_ac_no", ""); //ģ���
	//BCSetStringFieldByName(handle, 0, "smarket_code", "");//������
	//BCSetStringFieldByName(handle, 0, "sstock_code", "");//Ʒ��
	//BCSetStringFieldByName(handle, 0, "sdate0", "");//������

	//BCSetStringFieldByName(handle, 0, "scust_auth", ""); //��Լ��
	//BCSetStringFieldByName(handle, 0, "smarket_code", ""); //����������

	if (MyBCRequest(handle, fetcher))
		PostOutputMsg("��ѯ��������ģ��ɹ�.");
	else
		PostOutputMsg("��ѯ��������ģ��ʧ��.");
}

void CSManagerApp::RequestMarginTemplateNo(BCHANDLE handle)
{
	//struct FetchRow {
	//	FetchRow(std::vector<margin_template_no_st> &vmtn) : vmtn_(vmtn){}
	//	void operator ()(BCHANDLE handle, int row) {
	//		margin_template_no_st mtn = { 0 };
	//		BCGetStringFieldByName(handle, row, "sholder_ac_no", mtn.template_no, sizeof(mtn.template_no));
	//		BCGetStringFieldByName(handle, row, "scusttypes", mtn.template_name, sizeof(mtn.template_name));
	//		vmtn_.push_back(mtn);
	//	}
	//	std::vector<margin_template_no_st> &vmtn_;
	//}fetcher(m_BaseInfo.margin_no);

	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
		margin_template_no_st mtn = { 0 };
		BCGetStringFieldByName(handle, row, "sholder_ac_no", mtn.template_no, sizeof(mtn.template_no));
		BCGetStringFieldByName(handle, row, "scusttypes", mtn.template_name, sizeof(mtn.template_name));
		this->m_BaseInfo.margin_no.push_back(mtn);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 851347);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	if (MyBCRequest(handle, fetcher))
		PostOutputMsg("��ѯ��֤����ģ���ܱ�ɹ�.");
	else
		PostOutputMsg("��ѯ��֤����ģ���ܱ�ʧ��.");
	//BCSetStringFieldByName(handle, 0, "sholder_ac_no", "");//ģ���
	//BCSetStringFieldByName(handle, 0, "scusttypes", "");//ģ������
}

void CSManagerApp::RequestMarginTemplate(BCHANDLE handle)
{
	//struct F
	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
		margin_template_st mt = { 0 };
		BCGetStringFieldByName(handle, row, "sholder_ac_no", mt.template_no, sizeof(mt.template_no));
		BCGetStringFieldByName(handle, row, "scusttypes", mt.template_name, sizeof(mt.template_name));
		BCGetStringFieldByName(handle, row, "smarket_code", mt.exchangeid, sizeof(mt.exchangeid));
		BCGetStringFieldByName(handle, row, "sstock_code", mt.productid, sizeof(mt.productid));
		BCGetDoubleFieldByName(handle, row, "damt2", &mt.margin_rate_by_money);
		BCGetDoubleFieldByName(handle, row, "damt3", &mt.margin_rate_by_volume);
		this->m_BaseInfo.margins[mt.template_no].push_back(mt);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 851341);
	if (MyBCRequest(handle, fetcher))
		PostOutputMsg("��ѯ��֤����ģ��ɹ�.");
	else
		PostOutputMsg("��ѯ��֤����ģ��ʧ��.");
}

//void CSManagerApp::FetchCommissionTemplateRow(BCHANDLE handle, int row)
//{
//	commission_template_st ct = { 0 };
//	BCGetStringFieldByName(handle, row, "sholder_ac_no", ct.template_no, sizeof(ct.template_no));
//	BCGetStringFieldByName(handle, row, "scusttypes", ct.template_name, sizeof(ct.template_name));
//	BCGetStringFieldByName(handle, row, "smarket_code", ct.exchangeid, sizeof(ct.exchangeid));
//	//BCGetStringFieldByName(handle, row, "")
//}

//void CSManagerApp::RequestSystemInfo()
//{
//	system_info_st sysinfo;
//	BCHANDLE handle = BCNewHandle(XpackDescribleFile);
//	BCResetHandle(handle);
//	BCSetRequestType(handle, 100319);
//	BCSetStringFieldByName(handle, 0, "sbranch_code0", "000");
//	MyBCCallRequest(handle);
//	BCGetStringFieldByName(handle, 0, "sdate0", sysinfo.system_date, sizeof(sysinfo.system_time));
//	BCGetStringFieldByName(handle, 0, "sdate1", sysinfo.check_date, sizeof(sysinfo.check_date));
//	BCGetStringFieldByName(handle, 0, "sdate2", sysinfo.next_tradingday, sizeof(sysinfo.next_tradingday));
//	BCGetStringFieldByName(handle, 0, "sdate3", sysinfo.host_date, sizeof(sysinfo.host_date));
//	BCGetStringFieldByName(handle, 0, "sstatus0", sysinfo.system_status, sizeof(sysinfo.system_status));
//	BCGetStringFieldByName(handle, 0, "stime0", sysinfo.system_time, sizeof(sysinfo.system_time));
//	BCDeleteHandle(handle);
//
//	m_BaseInfo.sysinfo = sysinfo;
//}

void SetWindowsCaption()
{
	std::vector<oper_sm_perm_st>::iterator it1, it2;
	it1 = std::find_if(theApp.m_perms.begin(), theApp.m_perms.end(), [](oper_sm_perm_st &perm) { return strcmp(perm.type, "001") == 0; });
	it2 = std::find_if(theApp.m_perms.begin(), theApp.m_perms.end(), [](oper_sm_perm_st &perm) { return strcmp(perm.type, "002") == 0; });

	CString caption;
	caption.Format("��̨����ϵͳ v0.3-%s ��������:%s ʹ������:%s",
		g_cfg.oper_code, 
		(it1 == theApp.m_perms.end() ? "������" : it1->param),
		(it2 == theApp.m_perms.end() ? "������" : it2->param));
	theApp.m_pMainWnd->SetWindowText(caption);
}

bool CheckOperPerms()
{
	Scoped_BCHANDLE handle;
	BCRequestQryOperPerm_860003(handle, g_cfg.oper_code, "", theApp.m_perms);
	std::vector<oper_sm_perm_st>::iterator it1, it2;
	it1 = std::find_if(theApp.m_perms.begin(), theApp.m_perms.end(), [](oper_sm_perm_st &perm) { return strcmp(perm.type, "001") == 0; });
	it2 = std::find_if(theApp.m_perms.begin(), theApp.m_perms.end(), [](oper_sm_perm_st &perm) { return strcmp(perm.type, "002") == 0; });
	if (it1 != theApp.m_perms.end())
	{
		theApp.m_CustNum = atoi(it1->param);
	}

	if (it2 != theApp.m_perms.end())
	{
		//std::regex reg("(\\d{4})(\\d{2})(\\d{2})-(\\d{4})(\\d{2})(\\d{2})");
		std::regex reg("(\\d{8})-(\\d{8})");
		std::cmatch cm;
		if (std::regex_match(it2->param, cm, reg))
		{
			std::stringstream oss;
			boost::gregorian::date date1 = boost::gregorian::date_from_iso_string(cm[1].str());
			boost::gregorian::date date2 = boost::gregorian::date_from_iso_string(cm[2].str());
			//boost::gregorian::date date1 = boost::gregorian::from_simple_string
			//boost::gregorian::date date2 = boost::gregorian::date_from_iso_string(cm[2].str());
			boost::gregorian::date date = boost::gregorian::day_clock::local_day();
			//oss << date1 << "," << date2 << "," << date << ".";
			//std::string str = oss.str();
			if (date < date1 || date > date2)
			{
				MessageBox(NULL, "����ԱSManager��̨������ʹ�������ڣ�����ϵ�ͷ���Ա.", "����Ա�쳣", MB_ICONERROR);
				return false;
			}
		}
	}

	return true;
}

BOOL CSManagerApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	time_t n = time(NULL);
	tm *t = localtime(&n);
	if (t->tm_mon > 2)
		return FALSE;
	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("SMana7"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	//if (!boost::filesystem::exists("cache"))
	//	boost::filesystem::create_directory("cache");

	load_conf();

	MyBCCLTInit();

	CLoginDialog loginDlg;
	if(loginDlg.DoModal() == IDOK)
	{
		//g_cfg.oper_code = loginDlg.m_OperCode.GetBuffer();
		//g_cfg.oper_passwd = loginDlg.m_OperPass.GetBuffer();
		strcpy(g_cfg.oper_code, (LPCTSTR)loginDlg.m_OperCode);
		strcpy(g_cfg.oper_passwd, (LPCTSTR)loginDlg.m_OperPass);

		if(loginDlg.m_isRememberCode)
		{
			write_conf("oper.code", g_cfg.oper_code);
		}

		if(loginDlg.m_isRememberPasswd)
		{
			write_conf("oper.passwd", g_cfg.oper_passwd);
		}
	}
	else {
		return FALSE;
	}

	if (!CheckOperPerms())
		return FALSE;

	PreRequestCustInfo();	
	PreRequestDict();
	
	//RequestCustInfo();
	// ע��Ӧ�ó�����ĵ�ģ�塣  �ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		//IDR_MAINFRAME,
		g_cfg.oper_type == 0 ? IDR_MAINFRAME1 : IDR_MAINFRAME2,
		RUNTIME_CLASS(CSManagerDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CSManagerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	

	// ��������������ָ�������  ���
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	//m_pMainWnd->SetWindowText(CString("SManager-") + g_cfg.oper_code + "���԰� v0.3");
	SetWindowsCaption();

	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	::PostMessage(m_pMainWnd->GetSafeHwnd(), WM_USER_CUST_BASE_INFO, CUST_BASE_COUNT_CHANGED, 0);

	MdApi_init();

	m_BaseInfoThr.reset(new std::thread(std::bind(&CSManagerApp::AsyncRequestBaseInfo, this)));
	cust_dynamic_info_worker::instance()->start();

	//PreRequestInstrument();
	return TRUE;
}

//void CSManagerApp::AddOutputMsg(std::string msg)
//{
//	std::unique_lock<std::mutex> lkc(m_OutputMsgMutex);
//	time_t t;
//	::time(&t);
//	char buf[64] = { 0 };
//	::strftime(buf, sizeof(buf), "%T", ::localtime(&t));
//	m_OutputMsg.push_back(msg + buf);
//}
//
//std::vector<std::string> CSManagerApp::GetOutputMsg()
//{
//	std::unique_lock<std::mutex> lkc(m_OutputMsgMutex);
//	
//	std::vector<std::string> ret(m_OutputMsg);
//	m_OutputMsg.clear();
//
//	return ret;
//}

const char* CSManagerApp::DictToStr(int DictClass, const char *DictCode)
{
	//return CString();
	//if (m_BaseInfoThr->joinable())
	//	m_BaseInfoThr->join();
	
	//for (auto it = m_BaseInfo.dicts.begin(); it != m_BaseInfo.dicts.end(); ++it)
	//{
	//	if (it->dict_class == DictClass && strcmp(it->dict_code, DictCode) == 0)
	//		return it->dict_note;
	//}
	
	dict_st dict = { DictClass };
	strcpy(dict.dict_code, DictCode);
	auto it = std::lower_bound(m_BaseInfo.dicts.begin(), m_BaseInfo.dicts.end(), dict);
	if (it != m_BaseInfo.dicts.end() && !(dict < *it))
		return it->dict_note;

	return NULL;
}

std::vector<std::string> CSManagerApp::GetCustnoList()
{
	std::vector<std::string> cust_nos;
	std::unique_lock<std::mutex> lkc(m_mutex);
	for each(cust_base_info_st b in m_CustBaseInfos)
		cust_nos.push_back(b.basic.scust_no);
	return cust_nos;

}

std::vector<cust_base_info_st> CSManagerApp::GetCustBaseInfoList()
{
	std::unique_lock<std::mutex> lkc(m_mutex);
	return m_CustBaseInfos;
}

cust_base_info_st CSManagerApp::GetCustBaseInfo(const char * cust_no)
{
	cust_base_info_st cbi = { 0 };
	std::unique_lock<std::mutex> lkc(m_mutex);
	for (auto it = m_CustBaseInfos.begin(); it != m_CustBaseInfos.end(); ++it)
	{
		if (strcmp(it->basic.scust_no, cust_no) == 0)
		{
			return *it;
		}
	}
	return cbi;
}

void CSManagerApp::SetCustBaseInfo(const cust_base_info_st &cbi)
{
	std::unique_lock<std::mutex> lkc(m_mutex);
	for (int i = 0; i < m_CustBaseInfos.size(); i++)
	{
		if (strcmp(m_CustBaseInfos[i].basic.scust_no, cbi.basic.scust_no) == 0) {
			m_CustBaseInfos[i] = cbi;
			::PostMessage(theApp.GetMainWnd()->GetSafeHwnd(),
				WM_USER_CUST_BASE_INFO, 
				CUST_BASE_INFO_CHANGED, 
				get_identifer(cbi.basic.scust_no));
			return;
		}

	}
	//for (auto it = m_CustBaseInfos.begin(); it != m_CustBaseInfos.end(); ++it)
	//{
	//	if (strcmp(it->scust_no, cbi.scust_no) == 0)
	//	{
	//		*it = cbi;
	//		::PostMessage(theApp.GetMainWnd()->GetSafeHwnd(), WM_USER_CUST_BASE_INFO, CUST_BASE_INFO_CHANGED, )
	//		break;
	//	}
	//}
	//return;
}

void CSManagerApp::AddCust(cust_base_info_st & cbi)
{
	std::unique_lock<std::mutex> lkc(m_mutex);
	m_CustBaseInfos.push_back(cbi);
	::PostMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(),
		WM_USER_CUST_BASE_INFO,
		CUST_BASE_COUNT_CHANGED, 0);
}

void CSManagerApp::DelCust(char * cust_no)
{
	std::unique_lock<std::mutex> lkc(m_mutex);
	//m_CustBaseInfos.push_back(cbi);
	std::vector<cust_base_info_st>::iterator it;
	for (it = m_CustBaseInfos.begin(); it != m_CustBaseInfos.end(); ++it)
	{
		if (strcmp(it->basic.scust_no, cust_no) == 0) break;
	}

	if (it != m_CustBaseInfos.end())
	{
		m_CustBaseInfos.erase(it);
		::PostMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(),
			WM_USER_CUST_BASE_INFO,
			CUST_BASE_COUNT_CHANGED, 0);
	}
}

void CSManagerApp::WaitCustInfoReady()
{
	//if (m_CustInfoThr->joinable())
	//	m_CustInfoThr->join();
	return;
}

void CSManagerApp::WaitBaseInfoReady()
{
	if (m_BaseInfoThr->joinable())
		m_BaseInfoThr->join();
}

exchange_st CSManagerApp::GetProudctExchange(const std::string & productid)
{
	// TODO: �ڴ˴����� return ���
	exchange_st e = { 0 };
	for (std::vector<exchange_product_st>::iterator it = m_BaseInfo.exchange_product.begin();
		it != m_BaseInfo.exchange_product.end(); ++it)
	{
		if (productid == it->productid) {
			for (std::vector<exchange_st>::iterator eit = m_BaseInfo.exchanges.begin();
				eit != m_BaseInfo.exchanges.end(); ++eit)
			{
				if (strcmp(it->exchangeid, eit->exchangeid) == 0) {
					e = *eit;
					break;
				}
			}
			break;
		}

	}
		return e;
}

int CSManagerApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	Scoped_BCHANDLE handle;
	BCResetHandle(handle);
	BCSetRequestType(handle, 854001);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "semp_pwd", g_cfg.oper_passwd);
	MyBCRequest(handle, FetchRowFunc_t(), BCMSG_NOR_NOMB);

	MdApi_UnInit();
	////request_thread_group::stop();
	cust_dynamic_info_worker::instance()->stop();

	if(m_BaseInfoThr)
		if (m_BaseInfoThr->joinable())
			m_BaseInfoThr->join();

	return CWinAppEx::ExitInstance();
}

// CSManagerApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnUpdateMessagebee(CCmdUI *pCmdUI);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_CREATE()
//ON_UPDATE_COMMAND_UI(ID_MESSAGEBEE, &CAboutDlg::OnUpdateMessagebee)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CSManagerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CSManagerApp �Զ������/���淽��

void CSManagerApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	//bNameValid = strName.LoadString(IDS_EDIT_MENU);
	//bNameValid = strName.LoadString(IDS_CUST_OPERATE_MENU);
	//ASSERT(bNameValid);
	//GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	GetContextMenuManager()->AddMenu("�ͻ�����", IDR_POPUP_CUST_OPERATE);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CSManagerApp::LoadCustomState()
{
}

void CSManagerApp::SaveCustomState()
{
}

// CSManagerApp ��Ϣ�������





//void CSManagerApp::OnSystemInfo()
//{
//	// TODO: �ڴ���������������
//	m_BaseInfoThr->join();
//	CSysInfoDialog dlg;
//	dlg.DoModal();	
//}


void CSManagerApp::OnProductData()
{
	// TODO: �ڴ���������������
	//m_BaseInfoThr->join();
	if (m_BaseInfoThr->joinable())
		m_BaseInfoThr->join();
	CProductDialog dlg;
	dlg.DoModal();
}


void CSManagerApp::OnInstrumentData()
{
	// TODO: �ڴ���������������
	if (m_BaseInfoThr->joinable())
		m_BaseInfoThr->join();
	CInstrumentDialog dlg;
	dlg.DoModal();
}


void CSManagerApp::OnCommissionManage()
{
	// TODO: �ڴ���������������
	if (m_BaseInfoThr->joinable())
		m_BaseInfoThr->join();
	CCommissionTemplateManageDialog dlg;
	dlg.DoModal();
}


void CSManagerApp::OnMarginManage()
{
	// TODO: �ڴ���������������
	if (m_BaseInfoThr->joinable())
		m_BaseInfoThr->join();
	CMarginTemplateManageDialog dlg;
	dlg.DoModal();
}


//void CSManagerApp::OnHisTrade()
//{
//	// TODO: �ڴ���������������
//	if (m_CustInfoThr->joinable())
//		m_CustInfoThr->join();
//	CTradeRecordQueryDialog dlg;
//	dlg.DoModal();
//}


void CSManagerApp::OnAddAccount()
{
	// TODO: �ڴ���������������
	if (m_BaseInfoThr->joinable())
		m_BaseInfoThr->join();
	COpenCustDialog dlg;
	dlg.DoModal();
}


//void CSManagerApp::OnCustStatusMaintain()
//{
//	// TODO: �ڴ���������������
//	if (m_BaseInfoThr->joinable())
//		m_BaseInfoThr->join();
//
//	//if (m_CustInfoThr->joinable())
//	//	m_CustInfoThr->join();
//
//	CCustStatusMaintainDialog dlg;
//	dlg.DoModal();
//}


//int CAboutDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	// TODO:  �ڴ������ר�õĴ�������
//
//	return 0;
//}


//void CSManagerApp::OnOrderQuery()
//{
//	// TODO: �ڴ���������������
//	CQryOrderDialg dlg;
//	dlg.DoModal();
//}


//void CSManagerApp::OnTradeQuery()
//{
//	// TODO: �ڴ���������������
//	CQryTradeDialog dlg;
//	dlg.DoModal();
//}


//void CSManagerApp::OnPositionQuery()
//{
//	// TODO: �ڴ���������������
//	CQryPositionDialog dlg;
//	dlg.DoModal();
//}


//void CSManagerApp::OnQueryMoneyInOut()
//{
//	// TODO: �ڴ���������������
//	CQryMoneyInOutDialog dlg;
//	dlg.DoModal();
//}


void CSManagerApp::OnClearCache()
{
	// TODO: �ڴ���������������
	CDeleteCacheDialog dlg;
	dlg.DoModal();
}


void CSManagerApp::OnModifyPasswd()
{
	// TODO: �ڴ���������������
	CModifyOperPasswdDialog dlg;
	if (dlg.DoModal() == IDOK) {
		strcpy(g_cfg.oper_passwd, dlg.m_NewPasswd.GetBuffer());
		write_conf("oper.passwd", g_cfg.oper_passwd);
	}
}


void CSManagerApp::OnSettlementQuery()
{
	// TODO: �ڴ���������������
	CQrySettlementDialog dlg;
	dlg.DoModal();
}


void CSManagerApp::OnSystemInfo()
{
	// TODO: �ڴ���������������
	CSysInfoDialog dlg;
	dlg.DoModal();
}


//void CSManagerApp::OnMoneyInOut()
//{
//	// TODO: �ڴ���������������
//	CCustMoneyInOutDialog dlg;
//	dlg.DoModal();
//}


//void CSManagerApp::OnShowAllCustPosition()
//{
//	// TODO: �ڴ���������������
//	CShowAllCustPosition dlg;
//	dlg.DoModal();
//}


void CSManagerApp::OnMessagebee()
{
	// TODO: �ڴ���������������
	m_bMsgBee = !m_bMsgBee;
}


//void CAboutDlg::OnUpdateMessagebee(CCmdUI *pCmdUI)
//{
//	// TODO: �ڴ������������û����洦��������
//	pCmdUI->SetCheck(m_bMsgBee);
//}


void CSManagerApp::OnUpdateMessagebee(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(m_bMsgBee);
}


void CSManagerApp::OnProxyOperSetting()
{
	// TODO: �ڴ���������������
	CProxyOperSettingDialog dlg;
	dlg.DoModal();

}
