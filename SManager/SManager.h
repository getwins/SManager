
// SManager.h : SManager 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include <string>
#include <map>
#include <thread>
#include <mutex>
//#include <queue>

#include "nbccclt.h"

#include "struct.h"
#include "MyBCRequest.h"

#define DICT_CLASS_CUST_STATUS 1
#define DICT_CALSS_BUY_SELL 4
#define DICT_CLASS_OFFSET_FLAG 5
#define DICT_CLASS_HEDGE_FLAG 6
#define DICT_CLASS_ORDER_STATUS 9
#define DICT_CLASS_MONEYINOUT_TYPE 11
#define DICT_CLASS_FUND_TYPE 13
#define DICT_CLASS_RISK_LEVEL 17
#define DICT_CLASS_APP_IDENTIFIER 18
#define DICT_CLASS_LAST_TRADEDAY_RULE 27
#define DICT_CLASS_SYSTEM_STATUS 31
#define DICT_CLASS_INST_STATUS 85
#define DICT_CLASS_INST_RULE 182
#define DICT_CLASS_CARD_TYPE 1009
#define DICT_CLASS_FORCE_CLOSE 1025
#define DICT_CLASS_CURRENCY 1229
//#define DICT_CLASS_LOGIN_STATUS 

#define WM_USER_OUTPUT	WM_USER+1
#define WM_USER_CUST_DYNAMIC_INFO WM_USER+2
#define WM_USER_CUST_BASE_INFO	WM_USER+3
#define WM_USER_CUST_SEL_CHANGED WM_USER+4
#define WM_USER_INST_MARKET WM_USER+5
//#define WM_USER_REQUEST_INST_DONE WM_USER+6
#define WM_USER_SYSTEM_INFO WM_USER+7
#define WM_USER_CUST_POSITION_FLOAT_PROFIT WM_USER+8

#define CUST_BASE_COUNT_CHANGED 1
#define CUST_BASE_INFO_CHANGED 2

#define CUST_DYNAMIC_CAPITAL 6
#define CUST_DYNAMIC_LOGIN_INFO_CHANGED 5
#define CUST_DYNAMIC_POSITION_CHANGED 1
#define CUST_DYNAMIC_POSI_DETAIL_CHANGED 2
#define CUST_DYNAMIC_ORDER_CHANGED 3
#define CUST_DYNAMIC_TRADE_CHANGED 4
#define CUST_DYNAMIC_TRADE_DETAIL_CHANGED 7


//extern std::shared_ptr<std::thread> g_thr_custinfo;

struct user_config
{
	char host[31];
	int port;
	int branchno;
	int drtpno;
	int funcno;
	//std::string oper_code;
	//std::string oper_passwd;
	int oper_type;
	char oper_code[11];
	char oper_passwd[9];

	int bccall_timeout;
	int login_interval;
	int capital_interval;
	int position_interval;
	int posidetail_interval;
	int order_interval;
	int trade_interval;

	char market_brokerid[11];
	char market_userid[11];
	char market_passwd[9];
	char market_front[80];

	int wnd_float;
};

extern user_config g_cfg;
//#define XpackDescribleFile "cpack.dat"



struct CViewHint : public CObject
{
	int type;
	int identifer;
};
//extern std::map<std::string, custinfo_st> g_map_custinfo;
//typedef std::map<std::string, custinfo_st>::iterator iter_t;
//extern std::mutex g_custinfo_mutex;


extern const char *cache_file_dict;
extern const char *cache_file_exchange;
extern const char *cache_file_seat;
extern const char *cache_file_product;
extern const char *cache_file_exchange_product;
 
//void MyBCCallRequest(BCHANDLE handle);
//void MyBCCLTInit();
//typedef std::function<void(BCHANDLE,int)> FetchRowFunc_t;
//bool MyBCRequest(BCHANDLE handle, FetchRowFunc_t func, bool ErrMsgToMessageBox = false, bool ErrMsgToOutput = true,
//	std::string *ret_msg = NULL);


void PostOutputMsg(const std::string &msg);
std::vector<std::string> GetOutputMsgs();
// CSManagerApp:
// 有关此类的实现，请参阅 SManager.cpp
//

class CSManagerApp : public CWinAppEx
{
public:
	CSManagerApp();

	//std::shared_ptr<std::thread> m_SystemInfoThr;
	//std::shared_ptr<std::thread> m_ProductInstrumentThr;
	//std::shared_ptr<std::thread>
	//void AsyncRequestSystemInfo();

	//void AsyncRequestCustomerByOperator();
	//std::vector<

	//void AddOutputMsg(std::string msg);
	//std::vector<std::string> GetOutputMsg();

	const char* DictToStr(int DictClass, const char *DictCode);

	std::vector<std::string> GetCustnoList();
	std::vector<cust_base_info_st> GetCustBaseInfoList();
	cust_base_info_st GetCustBaseInfo(const char *cust_no);
	void SetCustBaseInfo(const cust_base_info_st &cbi);
	void AddCust(cust_base_info_st &cbi);
	void DelCust(char *cust_no);
	//void UpdateCust(cust_base_info_st &cbi);
	std::mutex m_mutex;
	std::vector<cust_base_info_st> m_CustBaseInfos;

	baseinfo_st m_BaseInfo;

	//system_info_st GetSysInfo() const;
	void WaitCustInfoReady();
	void WaitBaseInfoReady();

	exchange_st GetProudctExchange(const std::string &productid);

	std::vector<oper_sm_perm_st> m_perms;
	int m_CustNum;

	bool m_bMsgBee;

private:
	//std::shared_ptr<std::thread> m_SysInfoThr;
	//void RequestSysInfo();
	//system_info_st m_SysInfo;
	//std::mutex m_SysInfoMutex;

	//std::shared_ptr<std::thread> m_CustInfoThr;
	void PreRequestCustInfo();
	void PreRequestCustCommissionTemplate(BCHANDLE handle, cust_base_info_st &cbi);
	void PreRequestCustMarginTemplate(BCHANDLE handle, cust_base_info_st &cbi);

	std::shared_ptr<std::thread> m_BaseInfoThr;
	void AsyncRequestBaseInfo();
	//void RequestSystemInfo();
	void PreRequestDict();
	//void FetchDictRow(BCHANDLE handle, int row);
	void RequestExchange(BCHANDLE handle);
	void RequestSeat(BCHANDLE handle);
	//void FetchExchangeRow(BCHANDLE handle, int row);
	void RequestProduct(BCHANDLE handle);
	//void FetchProductRow(BCHANDLE handle, int row);
	void RequestExchangeProduct(BCHANDLE handle);
	//void GetExchangeProductData(BCHANDLE handle);
	void RequestInstrument(BCHANDLE handle);
	//void FetchInstrumentRow(BCHANDLE handle, int row);
	void RequestCommissionTemplateNo(BCHANDLE handle);
	void RequestCommissionTemplate(BCHANDLE handle);
	//void FetchCommissionTemplateRow(BCHANDLE handle, int row);
	void RequestMarginTemplateNo(BCHANDLE handle);
	void RequestMarginTemplate(BCHANDLE handle);

private:

	//std::queue<std::string> m_OutputMsg;
	//std::vector<std::string> m_OutputMsg;
	//std::mutex m_OutputMsgMutex;

	//std::vector<std::
// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	

//	afx_msg void OnSystemInfo();
	afx_msg void OnProductData();
	afx_msg void OnInstrumentData();
	afx_msg void OnCommissionManage();
	afx_msg void OnMarginManage();
//	afx_msg void OnHisTrade();
	afx_msg void OnAddAccount();
//	afx_msg void OnCustStatusMaintain();
//	afx_msg void OnOrderQuery();
//	afx_msg void OnTradeQuery();
//	afx_msg void OnPositionQuery();
//	afx_msg void OnQueryMoneyInOut();
	afx_msg void OnClearCache();
	afx_msg void OnModifyPasswd();
	afx_msg void OnSettlementQuery();
	afx_msg void OnSystemInfo();
//	afx_msg void OnMoneyInOut();
//	afx_msg void OnShowAllCustPosition();
	afx_msg void OnMessagebee();
	afx_msg void OnUpdateMessagebee(CCmdUI *pCmdUI);
	afx_msg void OnProxyOperSetting();
};

extern CSManagerApp theApp;
