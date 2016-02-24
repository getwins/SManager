// CustMoneyInOutDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "CustMoneyInOutDialog.h"
#include "afxdialogex.h"


// CCustMoneyInOutDialog 对话框

IMPLEMENT_DYNAMIC(CCustMoneyInOutDialog, CDialogEx)

CCustMoneyInOutDialog::CCustMoneyInOutDialog(/*const system_info_st &sysinfo,*/ CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CUST_MOENY_IN_OUT, pParent)
	//, m_Amount(_T(""))
	, m_Amount(0)
	//, m_SysInfo(sysinfo)
	, m_CustName(_T(""))
	, m_CustStatus(_T(""))
	, m_TradeWithdraw(0)
	//, m_SettleWithdraw(_T(""))
	, m_cash(0)
	, m_SettleWithdraw(0)
{

}

CCustMoneyInOutDialog::~CCustMoneyInOutDialog()
{
}

void CCustMoneyInOutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CUST, m_ctlCust);
	//  DDX_Text(pDX, IDC_EDIT_AMOUNT, m_Amount);
	DDX_Text(pDX, IDC_EDIT_AMOUNT, m_Amount);
	DDX_Text(pDX, IDC_EDIT_CUST_NAME, m_CustName);
	DDX_Text(pDX, IDC_EDIT_STATUS, m_CustStatus);
	DDX_Text(pDX, IDC_EDIT_TRADE_WITHDRAW, m_TradeWithdraw);
	//  DDX_Text(pDX, IDC_EDIT_SETTLE_WITHDRAW, m_SettleWithdraw);
	DDX_Text(pDX, IDC_EDIT_CASH, m_cash);
	DDX_Text(pDX, IDC_EDIT_SETTLE_WITHDRAW, m_SettleWithdraw);
}


BEGIN_MESSAGE_MAP(CCustMoneyInOutDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MONEY_IN, &CCustMoneyInOutDialog::OnBnClickedButtonMoneyIn)
	ON_BN_CLICKED(IDC_BUTTON_MONEY_OUT, &CCustMoneyInOutDialog::OnBnClickedButtonMoneyOut)
	ON_BN_CLICKED(IDC_BUTTON_FLUSH, &CCustMoneyInOutDialog::OnBnClickedButtonFlush)
	ON_CBN_SELCHANGE(IDC_COMBO_CUST, &CCustMoneyInOutDialog::OnCbnSelchangeComboCust)
END_MESSAGE_MAP()


// CCustMoneyInOutDialog 消息处理程序


BOOL CCustMoneyInOutDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	RequestSysInfo_100319();
	// TODO:  在此添加额外的初始化
	//for each(std::string cust_no in theApp.GetCustnoList())
	//{
	//	m_ctlCust.AddString(cust_no.c_str());
	//}
	OnCbnSelchangeComboCust();
	OnBnClickedButtonFlush();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

bool CCustMoneyInOutDialog::RequestSysInfo_100319()
{
	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
		BCGetStringFieldByName(handle, 0, "sdate0", m_SysInfo.system_date, sizeof(m_SysInfo.system_time));
		BCGetStringFieldByName(handle, 0, "sdate2", m_SysInfo.check_date, sizeof(m_SysInfo.check_date));
		BCGetStringFieldByName(handle, 0, "sdate1", m_SysInfo.next_tradingday, sizeof(m_SysInfo.next_tradingday));
		BCGetStringFieldByName(handle, 0, "sdate3", m_SysInfo.host_date, sizeof(m_SysInfo.host_date));
		BCGetStringFieldByName(handle, 0, "sstatus0", m_SysInfo.system_status, sizeof(m_SysInfo.system_status));
		BCGetStringFieldByName(handle, 0, "stime0", m_SysInfo.system_time, sizeof(m_SysInfo.system_time));
	};

	Scoped_BCHANDLE handle;
	BCResetHandle(handle);
	BCSetRequestType(handle, 100319);
	BCSetStringFieldByName(handle, 0, "sbranch_code0", "000");
	return MyBCRequest(handle, fetcher);
}

bool CCustMoneyInOutDialog::RequestAddCustMoneyIn_851100(BCHANDLE handle, char * direction)
{
	//CString cust_no = m_ctlCust.GetLBText(m)
	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
		BCGetIntFieldByName(handle, 0, "lserial0", &m_serial);
	};

	UpdateData(TRUE);
	//int sel = m_ctlCust.GetCurSel();
	//if (sel < 0 && sel >= m_ctlCust.GetCount())
	//	return false;
	//CString cust_no;
	//m_ctlCust.GetLBText(sel, cust_no);

	BCResetHandle(handle);
	BCSetRequestType(handle, 851100);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sstatus0", "1"); //操作标志

	BCSetStringFieldByName(handle, 0, "sdate0", m_SysInfo.next_tradingday); //日期
	BCSetIntFieldByName(handle, 0, "lserial0", 0);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", m_ctlCust.GetCurSelKey().GetBuffer());//客户号
	BCSetStringFieldByName(handle, 0, "sbank_code", "1");
	BCSetStringFieldByName(handle, 0, "scurrency_type", "1");
	BCSetStringFieldByName(handle, 0, "sstatus1", "1");
	BCSetStringFieldByName(handle, 0, "sstatus2", direction);
	BCSetStringFieldByName(handle, 0, "sbranch_code0", "000");
	BCSetDoubleFieldByName(handle, 0, "damt0", m_Amount);
	BCSetStringFieldByName(handle, 0, "snote", "");
	
	return MyBCRequest(handle, fetcher, BCMSG_NOR_NOMB);

}

bool CCustMoneyInOutDialog::RequestChkCustMoneyIn_851104(BCHANDLE handle, char * direction)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 851104);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sdate0", m_SysInfo.next_tradingday);
	BCSetStringFieldByName(handle, 0, "sbranch_code0", "000");
	BCSetIntFieldByName(handle, 0, "lserial0", m_serial);

	return MyBCRequest(handle, FetchRowFunc_t(), BCMSG_NOR_NOMB);
}

bool CCustMoneyInOutDialog::RequestCustWithdraw_851101(char *cust_no)
{
	auto fetcher = [this](const std::string &cust_no, BCHANDLE handle, int row) {
		cust_withdraw_st cw = { 0 };
		BCGetDoubleFieldByName(handle, 0, "damt0", &cw.withdraw_fund);
		BCGetDoubleFieldByName(handle, 0, "damt1", &cw.withdraw_cash);
		BCGetDoubleFieldByName(handle, 0, "damt2", &cw.trade_withdraw);
		BCGetDoubleFieldByName(handle, 0, "damt3", &cw.settle_withdraw);
		m_mWithdraw[cust_no] = cw;
	};
	Scoped_BCHANDLE handle;
	BCSetRequestType(handle, 851101);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sdate0", m_SysInfo.next_tradingday);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);
	BCSetStringFieldByName(handle, 0, "scurrency_type", "1");

	return MyBCRequest(handle, std::bind(fetcher, cust_no, std::placeholders::_1, std::placeholders::_2), BCMSG_NOR_NOMB);
}


void CCustMoneyInOutDialog::OnBnClickedButtonMoneyIn()
{
	// TODO: 在此添加控件通知处理程序代码
	Scoped_BCHANDLE handle;
	if (RequestAddCustMoneyIn_851100(handle, "1"))
		if (RequestChkCustMoneyIn_851104(handle, "1"))
		{
			MessageBox("入金成功");
		}

}


void CCustMoneyInOutDialog::OnBnClickedButtonMoneyOut()
{
	// TODO: 在此添加控件通知处理程序代码
	Scoped_BCHANDLE handle;
	if (RequestAddCustMoneyIn_851100(handle, "2"))
		if (RequestChkCustMoneyIn_851104(handle, "2"))
		{
			MessageBox("出金成功");
		}
}


void CCustMoneyInOutDialog::OnBnClickedButtonFlush()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cust_no = m_ctlCust.GetCurSelKey();
	if (cust_no.GetLength() == 0) {
		//MessageBox("请选择查询客户");
		return;
	}

	if (RequestCustWithdraw_851101(cust_no.GetBuffer()))
		UpdateCustWithDraw(cust_no.GetBuffer());
}

void CCustMoneyInOutDialog::UpdateCustWithDraw(char * cust_no)
{
	std::map<std::string, cust_withdraw_st>::iterator it = m_mWithdraw.find(cust_no);
	if (it != m_mWithdraw.end()) {
		m_TradeWithdraw = it->second.trade_withdraw;
		m_SettleWithdraw = it->second.settle_withdraw;
		m_cash = it->second.withdraw_cash;
		UpdateData(FALSE);
	}
}


void CCustMoneyInOutDialog::OnCbnSelchangeComboCust()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cust_no = m_ctlCust.GetCurSelKey();
	if (cust_no.GetLength()) {
		cust_base_info_st cbi =  theApp.GetCustBaseInfo(cust_no);
		m_CustName = cbi.basic.sname;
		m_CustStatus = theApp.DictToStr(DICT_CLASS_CUST_STATUS, cbi.basic.sstatus0);
		UpdateData(FALSE);

		std::map<std::string, cust_withdraw_st>::iterator it = m_mWithdraw.find(cust_no.GetBuffer());
		if (it != m_mWithdraw.end())
			UpdateCustWithDraw(cust_no.GetBuffer());
		else
			OnBnClickedButtonFlush();
	}
}
