// ShowAllCustPosition.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "ShowAllCustPosition.h"
#include "afxdialogex.h"
#include "cust_dynamic_info_worker.h"
#include "string_identifer.h"
#include <sstream>
#include <iterator>
#include "Format.h"
#include "CommBCRequest.h"
#include "SMutility.h"
#include "MdApi.h"

//IMPLEMENT_DYNAMIC(CSAPListctrl, CListCtrl)
//
//BEGIN_MESSAGE_MAP(CSAPListctrl, CListCtrl)
//	ON_WM_CONTEXTMENU()
//END_MESSAGE_MAP()

// CShowAllCustPosition 对话框

IMPLEMENT_DYNAMIC(CShowAllCustPosition, CDialogEx)

CShowAllCustPosition::CShowAllCustPosition(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SHOW_ALL_CUST_POSOTION, pParent)
{

}

CShowAllCustPosition::~CShowAllCustPosition()
{
}

void CShowAllCustPosition::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ALL_CUST_POSITION, m_listctrl);
}


BEGIN_MESSAGE_MAP(CShowAllCustPosition, CDialogEx)
//	ON_WM_CONTEXTMENU()
//	ON_NOTIFY(NM_RCLICK, IDC_LIST_ALL_CUST_POSITION, &CShowAllCustPosition::OnNMRClickListAllCustPosition)
#ifndef  _OPER_PROXY
	ON_WM_CONTEXTMENU()
#endif
	ON_COMMAND(ID_CLOSE_ALL, &CShowAllCustPosition::OnCloseAll)
	//ON_UPDATE_COMMAND_UI(ID_CLOSE_ALL, &CShowAllCustPosition::OnUpdateUICloseAll)
	ON_COMMAND(ID_CLOSE_SINGLE, &CShowAllCustPosition::OnCloseSingle)
END_MESSAGE_MAP()


// CShowAllCustPosition 消息处理程序


BOOL CShowAllCustPosition::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LVCOLUMN cols[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "客户号" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "客户名称" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "合约" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "买卖" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "总持仓" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "昨仓" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "可平量" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "持仓均价" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "持仓盈亏" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "最新价" },
	};

	for (int i = 0; i < sizeof(cols) / sizeof(cols[0]); ++i)
		m_listctrl.InsertColumn(i, &cols[i]);
	m_listctrl.SetExtendedStyle(m_listctrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	//CString str;
	ShowPosition();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//void CShowAllCustPosition::SetCustsPoition(const std::map<std::string, std::vector<cust_position_st>>& custPosition)
//{
//	m_mCustPosition = custPosition;
//}

void CShowAllCustPosition::ShowPosition()
{
	m_vPosi.clear();
	m_listctrl.DeleteAllItems();
	for each (cust_base_info_st cbi in theApp.GetCustBaseInfoList())
	{
		//for each (cust_position_st p in cust_dynamic_info_worker::instance()->get_cust_position(cbi.scust_no))
		//m_vPosi += cust_dynamic_info_worker::instance()->get_cust_position(cbi.scust_no);
		//m_vPosi.insert()
		const PosiVec_t &pv = cust_dynamic_info_worker::instance()->get_cust_position(cbi.basic.scust_no);
		std::copy(pv.begin(), pv.end(), std::back_inserter(m_vPosi));
	}

	int t_p = 0;
	double t_profit = 0.0;

	for (PosiVec_t::iterator it = m_vPosi.begin(); it != m_vPosi.end(); ++it)
	{
		int index = std::distance(m_vPosi.begin(), it);
		m_listctrl.InsertItem(index, it->cust_no);
		//m_listctrl.SetItemText(index, 1, cbi.sname);
		m_listctrl.SetItemText(index, 1, it->instrumentid);
		m_listctrl.SetItemText(index, 2, theApp.DictToStr(DICT_CLASS_BUY_SELL, it->direction));
		//str.Format("%d", it->positions);
		m_listctrl.SetItemText(index, 3, intfmt(it->positions));
		//str.Format("%d", it->positions - it->td_positions);
		m_listctrl.SetItemText(index, 4, intfmt(it->positions - it->td_positions));
		m_listctrl.SetItemText(index, 5, intfmt(it->positions));
		//str.Format("%.2f", it->hold_average_price);
		m_listctrl.SetItemText(index, 6, inst_pricefmt(it->instrumentid, it->hold_average_price));
		//str.Format("%.2f", it->float_profit);
		m_listctrl.SetItemText(index, 7, fundfmt(it->float_profit));
		m_listctrl.SetItemText(index, 8, inst_pricefmt(it->instrumentid, it->last_price));
		t_p += it->positions;
		t_profit += it->float_profit;
	}

	int item = m_listctrl.GetItemCount();
	m_listctrl.InsertItem(item, "汇总");
	m_listctrl.SetItemText(item, 3, intfmt(t_p));
	m_listctrl.SetItemText(item, 7, fundfmt(t_profit));
	//for each (cust_base_info_st cbi in theApp.GetCustBaseInfoList())
	//{
	//	int index = m_listctrl.GetItemCount();
	//	for each (cust_position_st p in cust_dynamic_info_worker::instance()->get_cust_position(cbi.scust_no))
	//	{
	//		m_listctrl.InsertItem(index, p.cust_no);
	//		m_listctrl.SetItemText(index, 1, cbi.sname);
	//		m_listctrl.SetItemText(index, 2, p.instrumentid);
	//		m_listctrl.SetItemText(index, 3, theApp.DictToStr(DICT_CLASS_BUY_SELL, p.direction));
	//		//str.Format("%d", p.positions);
	//		m_listctrl.SetItemText(index, 4, intfmt(p.positions));
	//		//str.Format("%d", p.positions - p.td_positions);
	//		m_listctrl.SetItemText(index, 5, intfmt(p.positions - p.td_positions));
	//		m_listctrl.SetItemText(index, 6, intfmt(p.positions));
	//		//str.Format("%.2f", p.hold_average_price);
	//		m_listctrl.SetItemText(index, 7, inst_pricefmt(p.instrumentid, p.hold_average_price));
	//		//str.Format("%.2f", p.float_profit);
	//		m_listctrl.SetItemText(index, 8, inst_pricefmt(p.instrumentid, p.float_profit));
	//		//m_listctrl.setitem
	//		std::ostringstream oss;
	//		oss << get_posiid(p);
	//		m_listctrl.SetItemData(index, get_identifer(oss.str()));
	//	}
	//}

}


void CShowAllCustPosition::UpdateInstMarket(const std::string & inst)
{
	depth_market_data_st dmd = MdApi_GetMarketData(inst);
	double t_profit = 0.0;
	for (PosiVec_t::iterator it = m_vPosi.begin(); it != m_vPosi.end(); ++it)
	{
		if(inst == it->instrumentid && it->last_price != dmd.LastPrice)
		{
			int index = std::distance(m_vPosi.begin(), it);
			it->float_profit = it->float_profit = calcProfit(*it, dmd.LastPrice);
			it->last_price = dmd.LastPrice;
			m_listctrl.SetItemText(index, 7, inst_pricefmt(it->instrumentid, it->float_profit));
			m_listctrl.SetItemText(index, 8, inst_pricefmt(it->instrumentid, it->last_price));
		}
		t_profit += it->float_profit;
	}
	m_listctrl.SetItemText(m_listctrl.GetItemCount() - 1, 7, fundfmt(t_profit));
}

//void CShowAllCustPosition::UpdateCustPositionProfit(int posiid, double profit)
//{
//	//auto it = m_mCustPosition.find(posi_id.cust_no);
//	//if (it == m_mCustPosition.end())
//	//	return;
//	for (int i = 0; i < m_listctrl.GetItemCount(); ++i)
//	{
//		int itemdata = static_cast<int>(m_listctrl.GetItemData(i));
//		if (itemdata == posiid)
//		{
//			m_listctrl.SetItemText(i, 8, inst_pricefmt(NULL, profit));
//			break;
//		}
//	}
//}

//
//void CShowAllCustPosition::OnNMRClickListAllCustPosition(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	ScreenToClient(&pNMItemActivate->ptAction);
//
//#ifndef SHARED_HANDLERS
//	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_CLOSE_POSITION, pNMItemActivate->ptAction.x, pNMItemActivate->ptAction.y, this, TRUE);
//#endif
//	*pResult = 0;
//}

//void CShowAllCustPosition::OnContextMenu(CWnd* /* pWnd */, CPoint point)
//{
//#ifndef SHARED_HANDLERS
//	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_CLOSE_POSITION, point.x, point.y, this, TRUE);
//#endif
//}

void CShowAllCustPosition::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_CLOSE_POSITION, point.x, point.y, this, TRUE);
	CMenu   menu;   //定义下面要用到的cmenu对象
	menu.LoadMenu(IDR_POPUP_CLOSE_POSITION); //装载自定义的右键菜单 
	CMenu   *pContextMenu = menu.GetSubMenu(0); //获取第一个弹出菜单，所以第一个菜单必须有子菜单 
	CPoint point1;//定义一个用于确定光标位置的位置  
	GetCursorPos(&point1);//获取当前光标的位置，以便使得菜单可以跟随光标  
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this); //在指定位置显示弹出菜单

}


//BOOL CSAPListctrl::PreCreateWindow(CREATESTRUCT& cs)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	
//	return CListCtrl::PreCreateWindow(cs);
//}


//void CSAPListctrl::PreSubclassWindow()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	//this->mo
//	CListCtrl::PreSubclassWindow();
//}
//
//
//void CSAPListctrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
//{
//	// TODO: 在此处添加消息处理程序代码
//	//CMenu menu;
//	//menu.LoadMenu(IDR_POPUP_CLOSE_POSITION);
//
//	//CMenu* pSumMenu = menu.GetSubMenu(0);
//
//	//if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
//	//{
//	//	CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;
//
//	//	if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
//	//		return;
//
//	//	((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
//	//	UpdateDialogControls(this, FALSE);
//	//}
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_CLOSE_POSITION, point.x, point.y, this, TRUE);
//	//SetFocus();
//}


void CShowAllCustPosition::OnCloseAll()
{
	// TODO: 在此添加命令处理程序代码
	Scoped_BCHANDLE handle;
	std::map<std::string, inst_market_st> mcache;
	for (PosiVec_t::iterator it = m_vPosi.begin(); it != m_vPosi.end(); ++it)
	{
		inst_market_st m;
		if (mcache.find(it->instrumentid) == mcache.end())
		{
			if (!BCRequestInstMarketData_851503(handle, it->instrumentid, m))
				continue;
		}
		else
			m = mcache[it->instrumentid];
		
		BCRequestOffsetOrderInsert(handle, *it, m);
		//char *dir = strcmp(it->direction, "0") ? "0" : "1";
		////double price = strcmp(it->direction, "0") ? m.bid_price : m.ask_price;
		//double price = (strcmp(dir, "0") == 0) ? m.upperlimit_price : m.lowerlimit_price;
		//BCRequestOrderInsert_854002(handle, it->cust_no, it->instrumentid, it->exchangeid, "1", dir, price, it->positions);
	}
}


void CShowAllCustPosition::OnCloseSingle()
{
	// TODO: 在此添加命令处理程序代码
	int sel = m_listctrl.GetSelectionMark();
	if (sel < 0 || sel >= m_vPosi.size())
	{
		MessageBox("请选中持仓");
		return;
	}
	Scoped_BCHANDLE handle;
	inst_market_st m;
	cust_position_st &p = m_vPosi[sel];
	if (!BCRequestInstMarketData_851503(handle, p.instrumentid, m))
		return;
	char *dir = strcmp(p.direction, "0") ? "0" : "1";
	//double price = strcmp(p.direction, "0") ? m.bid_price : m.ask_price;
	double price = (strcmp(dir, "0") == 0) ? m.upperlimit_price : m.lowerlimit_price;
	if (BCRequestOffsetOrderInsert(handle, p, m))
	{
		std::ostringstream oss;
		oss << "平价指令发送成功,";
		oss << p.cust_no << "|" << p.instrumentid << "|" << p.exchangeid << "|" << dir << "|" << price << "|" << p.positions << "|";
		MessageBox(oss.str().c_str());
	}
		
}
