#include "stdafx.h"
#include "MdApi.h"
#include "SManager.h"
#include "string_identifer.h"
#include "KSMarketDataAPI.h"


#include <map>
#include <mutex>
#include <string>
#include <sstream>
#include <atomic>
#include <set>
#include <cassert>

//#include "CommBCRequest.h"

std::atomic_bool g_login(false);

using namespace KingstarAPI;

std::mutex g_MarketMutex;
std::set<std::string> g_SubInst;
std::map<std::string, CThostFtdcDepthMarketDataField> g_mMarket;

static CThostFtdcMdApi *g_pUserApi = NULL;



class CMdUserApiSample : public CThostFtdcMdSpi
{
public:
	// 构造函数
	CMdUserApiSample(CThostFtdcMdApi *pUserApi, TThostFtdcBrokerIDType chBrokerID, TThostFtdcUserIDType chUserID, TThostFtdcPasswordType chPassword)
	{
		m_nRequestID = 1;
		m_pUserApi = pUserApi;
		strncpy(m_chBrokerID, chBrokerID, sizeof(m_chBrokerID) - 1);
		strncpy(m_chUserID, chUserID, sizeof(m_chUserID) - 1);
		strncpy(m_chPassword, chPassword, sizeof(m_chPassword) - 1);
	}

	// 析构函数
	~CMdUserApiSample()
	{

	}

	// 连接信号
	virtual void OnFrontConnected()
	{
		//printf("OnFrontConnected:\n");
		PostOutputMsg("行情前置连接成功");
		CThostFtdcReqUserLoginField reqUserLogin = { 0 };
		strncpy(reqUserLogin.BrokerID, m_chBrokerID, sizeof(reqUserLogin.BrokerID) - 1);
		strncpy(reqUserLogin.UserID, m_chUserID, sizeof(reqUserLogin.UserID) - 1);
		strncpy(reqUserLogin.Password, m_chPassword, sizeof(reqUserLogin.Password) - 1);
		//strcpy(reqUserLogin.TradingDay, "20160325");

		// 发送登录请求
		m_pUserApi->ReqUserLogin(&reqUserLogin, m_nRequestID++);
	}

	// 断线信号
	virtual void OnFrontDisconnected(int nReason)
	{
		std::ostringstream oss;
		oss << "行情前置连接断开,thread_id=" << std::showbase << std::hex << std::this_thread::get_id();
		PostOutputMsg(oss.str());
	}

	// 登录应答处理
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		if (pRspInfo != NULL) {
			
			std::ostringstream oss;
			oss << "登录行情网关," << pRspInfo->ErrorID << "," << pRspInfo->ErrorMsg
				<< ",行情交易日期" << m_pUserApi->GetTradingDay()
				<< ",thread_id=" << std::showbase << std::hex << std::this_thread::get_id();

			PostOutputMsg(oss.str());
			if (pRspInfo->ErrorID == 0) {
				g_login = true;
				//std::vector<char*> pp;
				//{
				//	std::unique_lock<std::mutex> lkc(g_MarketMutex);
				//	for (std::set<std::string>::iterator it = g_SubInst.begin(); it != g_SubInst.end(); ++it)
				//	{
				//		pp.push_back((char*)it->c_str());
				//	}
				//}

				//if (!pp.empty())
				//	m_pUserApi->SubscribeMarketData(&pp[0], pp.size());
				std::unique_lock<std::mutex> lkc(g_MarketMutex);
				for (std::set<std::string>::iterator it = g_SubInst.begin(); it != g_SubInst.end(); ++it)
				{
					char *pp[] = { (char*)(it->c_str()) };
					m_pUserApi->SubscribeMarketData(pp, 1);
					std::ostringstream oss;
					oss << "订阅合约行情:" << *it;
					PostOutputMsg(oss.str());
					//pp.push_back((char*)it->c_str());
				}
			}
				
		}

	}

	///RspSubMarketData return
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		if (pRspInfo) {
			std::ostringstream oss;
			oss << "订阅合约行情应答,"
				<< (pSpecificInstrument ? pSpecificInstrument->InstrumentID : "")
				<< "," << pRspInfo->ErrorID << "," << pRspInfo->ErrorMsg
				<< ",thread_id=" << std::showbase << std::hex << std::this_thread::get_id();
			PostOutputMsg(oss.str());
		}
	}

	///OnRspUnSubMarketData return
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		if (pRspInfo) {
			std::ostringstream oss;
			oss << "退订合约行情," 
				<< (pSpecificInstrument ? pSpecificInstrument->InstrumentID : "")
				<< "," << pRspInfo->ErrorID << "," << pRspInfo->ErrorMsg;
			PostOutputMsg(oss.str());
		}
	}

	virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		if (pRspInfo) {
			std::ostringstream oss;
			oss << "订阅合约询价," 
				<< (pSpecificInstrument ? pSpecificInstrument->InstrumentID : "")
				<< "," << pRspInfo->ErrorID << "," << pRspInfo->ErrorMsg;
			PostOutputMsg(oss.str());
		}
	}

	// logout return
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		if (pRspInfo) {
			std::ostringstream oss;
			oss << "登出行情网关," 
				<< (pUserLogout ? pUserLogout->UserID : "")
				<< "," << pRspInfo->ErrorID << "," << pRspInfo->ErrorMsg;
			PostOutputMsg(oss.str());
		}
	}

	///OnRtnDepthMarketData
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
	{
		if (pDepthMarketData )
		{
			{
				std::unique_lock<std::mutex> lkc(g_MarketMutex);
				if (g_SubInst.find(pDepthMarketData->InstrumentID) == g_SubInst.end())
					return;
			}

			{
				std::unique_lock<std::mutex> lkc(g_MarketMutex);
				g_mMarket[pDepthMarketData->InstrumentID] = *pDepthMarketData;
				
			}
			::PostMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(),
				WM_USER_INST_MARKET,
				get_identifer(pDepthMarketData->InstrumentID), -1);
		}
	}

private:
	// 请求号
	int m_nRequestID;
	// 实例指针
	CThostFtdcMdApi *m_pUserApi;
	// 经济公司代码
	TThostFtdcBrokerIDType m_chBrokerID;
	// 用户名
	TThostFtdcUserIDType m_chUserID;
	// 用户密码
	TThostFtdcPasswordType m_chPassword;
};

int MdApi_init()
{
	if (g_pUserApi == NULL) {
		std::ostringstream oss;
		if ((g_pUserApi = CThostFtdcMdApi::CreateFtdcMdApi("bclog")) == NULL) {
			oss << "CThostFtdcMdApi::CreateFtdcMdApi failed,行情初始化失败.";
			PostOutputMsg(oss.str());
			return -1;
		}
		
		// 创建spi实例
		CMdUserApiSample* pSpi = new CMdUserApiSample(g_pUserApi, g_cfg.market_brokerid, 
			g_cfg.market_userid, g_cfg.market_passwd);
		// 注册spi实例
		g_pUserApi->RegisterSpi(pSpi);
		// 注册前置机	
		g_pUserApi->RegisterFront(g_cfg.market_front);

		g_pUserApi->Init();
	}
	return 0;
}

int MdApi_UnInit()
{
	if (g_pUserApi) {
		CThostFtdcUserLogoutField logout = { 0 };
		strcpy(logout.BrokerID, g_cfg.market_brokerid);
		strcpy(logout.UserID, g_cfg.market_userid);
		g_pUserApi->ReqUserLogout(&logout, 2);
		g_pUserApi->Release();
		g_pUserApi = NULL;
	}
	
	return 0;
}

int MdApi_SubscribeMarketData(const std::string &inst)
{
	//std::unique_lock<std::mutex> lkc(g_MarketMutex);
	//g_SubInst.insert(inst);
	if (g_login == false) {
		std::unique_lock<std::mutex> lkc(g_MarketMutex);
		g_SubInst.insert(inst);
		return 0;
	}
	else {
		std::unique_lock<std::mutex> lkc(g_MarketMutex);
		if (g_SubInst.find(inst) == g_SubInst.end())
		{
			g_SubInst.insert(inst);
			char *pp[] = { (char*)inst.c_str() };
			g_pUserApi->SubscribeMarketData(pp, 1);
			std::ostringstream oss;
			oss << "订阅合约行情:" << inst;
			PostOutputMsg(oss.str());
		}
		
	}
	
	return 0;
}

depth_market_data_st MdApi_GetMarketData(const std::string &inst)
{
	assert(sizeof(depth_market_data_st) == sizeof(CThostFtdcDepthMarketDataField));
	depth_market_data_st m = { 0 };
	std::map<std::string, CThostFtdcDepthMarketDataField>::iterator it;
	
	std::unique_lock<std::mutex> lkc(g_MarketMutex);
	if ((it = g_mMarket.find(inst)) != g_mMarket.end())
	{
		memcpy(&m, &(it->second), sizeof(m));
	}
	return m;
}