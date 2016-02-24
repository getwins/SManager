// MarketWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "MarketWnd.h"
#include "string_identifer.h"
#include "cust_dynamic_info_worker.h"
#include "MdApi.h"
#include "Format.h"
#include "SMutility.h"
#include <boost/algorithm/string.hpp>

// CMarketWnd

IMPLEMENT_DYNAMIC(CMarketWnd, CDockablePane)

CMarketWnd::CMarketWnd()
	//: m_bSubscribled(false)
{

}

CMarketWnd::~CMarketWnd()
{
}


BEGIN_MESSAGE_MAP(CMarketWnd, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_MESSAGE(WM_USER_CUST_DYNAMIC_INFO, &CMarketWnd::OnUserCustDynamicInfo)
	ON_MESSAGE(WM_USER_INST_MARKET, &CMarketWnd::OnUserInstMarket)
END_MESSAGE_MAP()



// CMarketWnd 消息处理程序




void CMarketWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_listctrl.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	m_listctrl.ShowWindow(SW_SHOW);
}

void CMarketWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	AdjustLayout();
}


int CMarketWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_listctrl.Create(WS_VISIBLE | WS_CHILD, CRect(), this, 3);

	//LONG style = GetWindowLong(m_listctrl.GetSafeHwnd(), GWL_STYLE);
	//style &= ~LVS_TYPEMASK;
	//style |= LVS_REPORT;
	//SetWindowLong(m_listctrl.GetSafeHwnd(), GWL_STYLE, style);

	//DWORD styleex = m_listctrl.GetExtendedStyle();
	//styleex |= LVS_EX_GRIDLINES;
	//styleex |= LVS_EX_DOUBLEBUFFER;
	//m_listctrl.SetExtendedStyle(styleex);

	LVCOLUMN cols[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "合约" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "最新" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "现手" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "成交" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "卖量" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "卖价" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "买量" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "买价" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "成交量" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "涨跌" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "持仓" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "仓差" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "结算价" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "涨跌" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "开盘" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "最高" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "最低" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "昨结算" },
	};

	for (int i = 0; i < sizeof(cols) / sizeof(cols[0]); ++i)
		m_listctrl.InsertColumn(i, &cols[i]);

	return 0;
}


afx_msg LRESULT CMarketWnd::OnUserCustDynamicInfo(WPARAM wParam, LPARAM lParam)
{
	std::string cust_no = fetch_string(lParam);
	if (wParam == CUST_DYNAMIC_POSITION_CHANGED) {
		const std::vector<cust_position_st> vp = cust_dynamic_info_worker::instance()->get_cust_position(cust_no);
		for each (cust_position_st p in vp)
		{
			std::string inst = boost::trim_copy(std::string(p.instrumentid));
			if (m_SubscribeInst.find(inst) == m_SubscribeInst.end()) {
				m_SubscribeInst.insert(p.instrumentid);
				m_listctrl.InsertItem(m_listctrl.GetItemCount(), inst.c_str());
				MdApi_SubscribeMarketData(inst);
			}
		} 
	}
	else if (wParam == CUST_DYNAMIC_ORDER_CHANGED) {
		for each (cust_order_st o in cust_dynamic_info_worker::instance()->get_cust_order(cust_no))
		{
			std::string inst = boost::trim_copy(std::string(o.instrumentid));
			if (m_SubscribeInst.find(inst) == m_SubscribeInst.end()) {
				m_SubscribeInst.insert(inst);
				m_listctrl.InsertItem(m_listctrl.GetItemCount(), inst.c_str());
				MdApi_SubscribeMarketData(inst);
			}
		}
	}
	else if (wParam == CUST_DYNAMIC_TRADE_CHANGED) {
		for each (cust_trade_st t in cust_dynamic_info_worker::instance()->get_cust_trade(cust_no))
		{
			std::string inst = boost::trim_copy(std::string(t.instrumentid));
			if (m_SubscribeInst.find(inst) == m_SubscribeInst.end()) {
				m_SubscribeInst.insert(inst);
				m_listctrl.InsertItem(m_listctrl.GetItemCount(), inst.c_str());
				MdApi_SubscribeMarketData(inst);
			}
		}
	}

	return 0;
}

//void subscrible(CThostFtdcMdApi *pUserApi, const std::set<std::string> &insts)
//{
//	std::vector<char*> v;
//	for (const std::set<std::string>::iterator it = insts.begin(); it != insts.end(); ++it)
//	{
//		pUserApi->SubscribeMarketData()
//	}
//}

afx_msg LRESULT CMarketWnd::OnUserInstMarket(WPARAM wParam, LPARAM lParam)
{
	std::string inst = fetch_string(wParam);
	for (int i = 0; i < m_listctrl.GetItemCount(); i++) 
	{
		if (inst == m_listctrl.GetItemText(i, 0).GetBuffer())
		{
			depth_market_data_st m = MdApi_GetMarketData(inst);
			m_listctrl.SetCellTextColor(i, 1, comp_color(m.LastPrice, m.OpenPrice, COLOR_RED, COLOR_GREEN, COLOR_BLACK));
			m_listctrl.SetItemText(i, 1, inst_pricefmt(inst.c_str(), m.LastPrice));
			m_listctrl.SetItemText(i, 2, intfmt(m.Volume));
			m_listctrl.SetItemText(i, 3, intfmt(m.AskVolume1));

			m_listctrl.SetCellTextColor(i, 4, comp_color(m.AskPrice1, m.OpenPrice, COLOR_RED, COLOR_GREEN, COLOR_BLACK));
			m_listctrl.SetItemText(i, 4, inst_pricefmt(inst.c_str(), m.AskPrice1));

			m_listctrl.SetItemText(i, 5, intfmt(m.BidVolume1));
			m_listctrl.SetCellTextColor(i, 6, comp_color(m.BidPrice1, m.OpenPrice, COLOR_RED, COLOR_GREEN, COLOR_BLACK));
			m_listctrl.SetItemText(i, 6, inst_pricefmt(inst.c_str(), m.BidPrice1));

			//m_listctrl.SetItemText(i, 7, intfmt(m.Volume));
			//m_listctrl.SetItemText(i, 8, m.)
			m_listctrl.SetItemText(i, 7, inst_pricefmt(inst.c_str(), m.OpenInterest));
			//m_listctrl.SetItemText(i, 8, intfmt(m.CurrDelta));
			//m_listctrl.SetItemText(i, 8, inst_pricefmt(inst.c_str(), m.SettlementPrice));
			m_listctrl.SetItemText(i, 8, inst_pricefmt(inst.c_str(), m.LastPrice - m.PreSettlementPrice));
			m_listctrl.SetItemText(i, 9, inst_pricefmt(inst.c_str(), m.OpenPrice));
			m_listctrl.SetItemText(i, 10, inst_pricefmt(inst.c_str(), m.HighestPrice));
			m_listctrl.SetItemText(i, 11, inst_pricefmt(inst.c_str(), m.LowestPrice));
			m_listctrl.SetItemText(i, 12, inst_pricefmt(inst.c_str(), m.PreSettlementPrice));
		}
	}
	return 0;
}
