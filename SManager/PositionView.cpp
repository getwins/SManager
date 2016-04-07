// PositionView.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "PositionView.h"
//#include "PositionFormView.h"
#include "string_identifer.h"
#include "cust_dynamic_info_worker.h"
#include "MdApi.h"
#include "Format.h"
#include "SMutility.h"
#include "struct_helper.h"
#include "CommBCRequest.h"
#include <algorithm>
//#include <sstream>

////CMFCToolBarComboBoxButton
//class CPositionViewMenuButton : public CMFCToolBarMenuButton
//{
//	friend class CPositionView;
//
//	DECLARE_SERIAL(CPositionViewMenuButton)
//
//public:
//	CPositionViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
//	{
//	}
//
//	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
//		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
//	{
//		pImages = CMFCToolBar::GetImages();
//
//		CAfxDrawState ds;
//		pImages->PrepareDrawImage(ds);
//
//		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);
//
//		pImages->EndDrawImage(ds);
//	}
//};
//
//IMPLEMENT_SERIAL(CPositionViewMenuButton, CMFCToolBarMenuButton, 1)

//
// CPositionView

IMPLEMENT_DYNAMIC(CPositionView, CDockablePane)

CPositionView::CPositionView()
{
	//m_nCurrSort = ID_PS_POSITION;
	m_ComboCurSel = -1;
}

CPositionView::~CPositionView()
{
}


BEGIN_MESSAGE_MAP(CPositionView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	//(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	//ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
	//ON_COMMAND(ID_PS_POSITION, OnShowPosition)
	//ON_COMMAND(ID_PS_POSI_DETAIL, OnShowPosiDetail)
	//ON_UPDATE_COMMAND_UI(ID_PS_POSITION, )
	ON_COMMAND(ID_PS_MENU, OnClickComboBox)
	ON_CBN_SELCHANGE(ID_PS_MENU, OnComboBoxSelChangeClick)
#ifndef _OPER_PROXY
	ON_COMMAND(ID_OFFSET_DJ, OnOffsetDJ)
	ON_COMMAND(ID_OFFSET_SJ, OnOffsetSJ)
	ON_COMMAND(ID_OFFSET_ALL, OnOffsetAll)
#endif
	//ON_COMMAND_RANGE(ID_PS_POSITION, ID_PS_POSI_DETAIL, OnSort)
	//ON_UPDATE_COMMAND_UI_RANGE(ID_PS_POSITION, ID_PS_POSI_DETAIL, OnUpdateSort)
	ON_MESSAGE(WM_USER_CUST_SEL_CHANGED, &CPositionView::OnUserCustSelChanged)
	ON_MESSAGE(WM_USER_CUST_DYNAMIC_INFO, &CPositionView::OnUserCustDynamicInfo)
	ON_MESSAGE(WM_USER_INST_MARKET, &CPositionView::OnUserInstMarket)
	//ON_MESSAGE(WM_USER_CUST_POSITION_FLOAT_PROFIT, &CPositionView::OnUserCustPositionFloatProfit)
END_MESSAGE_MAP()



// CPositionView 消息处理程序




int CPositionView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//CCreateContext *pContext = NULL;

	//m_pPositionFormView = CPositionFormView::CreateOne(this);

	//CMFCToolBarMenuButton buttotn;

	//m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	//m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* 已锁定*/);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_TOOLBAR_POSITION);
	m_wndToolBar.LoadToolBar(IDR_TOOLBAR_POSITION, 0, 0, TRUE /* 已锁定*/);
	

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由: 
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	//CMenu menuSort;
	////menuSort.LoadMenu(IDR_POPUP_SORT);
	//menuSort.LoadMenu(IDR_POPUP_POSITION_SHOW);

	////m_wndToolBar.ReplaceButton(ID_SORT_MENU, CPositionViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));
	//m_wndToolBar.ReplaceButton(ID_PS_MENU, CPositionViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));
	//CPositionViewMenuButton* pButton = DYNAMIC_DOWNCAST(CPositionViewMenuButton, m_wndToolBar.GetButton(0));
	//if (pButton != NULL)
	//{
	//	pButton->m_bText = FALSE;
	//	pButton->m_bImage = TRUE;
	//	pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
	//	pButton->SetMessageWnd(this);
	//}
	
	//CMFCToolBarComboBoxButton *pCombo= new CMFCToolBarComboBoxButton(ID_PS_MENU, 
	//	GetCmdMgr()->GetCmdImage(ID_PS_MENU, FALSE), CBS_DROPDOWNLIST, 80);
	//pCombo->AddItem(_T("持仓"));
	//pCombo->AddItem(_T("持仓明细"));
	//m_wndToolBar.ReplaceButton(ID_PS_MENU, *pCombo);
	//pCombo->EnableWindow(TRUE);
	CMFCToolBarComboBoxButton Combo(ID_PS_MENU,GetCmdMgr()->GetCmdImage(ID_PS_MENU, FALSE), CBS_DROPDOWNLIST, 80);
	Combo.AddItem(_T("持仓"));
	Combo.AddItem(_T("持仓明细"));
	Combo.SelectItem((m_ComboCurSel = 0));
	m_wndToolBar.ReplaceButton(ID_PS_MENU, Combo);
	
	

	m_listctrl.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT, RECT(), this, ID_POSITION_LISTCTRL);

	LVCOLUMN cols[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "合约" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "买卖" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "总持仓" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "昨仓" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "可平量" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "最新" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "持仓均价" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "持仓盈亏" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "点数(持)" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "平仓盈亏" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "点数(平)" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "交易账户" }
	};

	for (int i = 0; i < sizeof(cols) / sizeof(cols[0]); ++i)
		m_listctrl.InsertColumn(i, &cols[i]);

	return 0;
}


void CPositionView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	//CRect rect;
	//GetClientRect(&rect);
	//m_listctrl.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	//m_pPositionFormView->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER); 
	
	//m_pPositionFormView->SetWindowPos(this, -1,-1, cx ,cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	//m_pPositionFormView->ShowWindow(SW_SHOW);
	
	//m_dialog.SetWindowPos(this, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE |SWP_NOZORDER);
	//m_dialog.SetWindowPos(this, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE |SWP_NOZORDER);
	//m_dialog.ShowWindow(SW_SHOW);

	//CBasePane::AdjustLayout
	AdjustLayout();
}

void CPositionView::OnComboBoxSelChangeClick()
{
	CMFCToolBarComboBoxButton* pSrcCombo = CMFCToolBarComboBoxButton::GetByCmd(ID_PS_MENU, TRUE);
	if (!pSrcCombo)
		return;
	if (pSrcCombo->GetCurSel() == m_ComboCurSel)
		return;
	
	m_ComboCurSel = pSrcCombo->GetCurSel();
	ResetList();
}

//void CPositionView::OnNewFolder()
//{
//	AfxMessageBox(_T("新建文件夹..."));
//}

void CPositionView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);

	m_listctrl.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_listctrl.ShowWindow(SW_SHOW);
}

void CPositionView::OnChangeVisualStyle()
{

	//m_wndToolBar.CleanUpLockedImages();
	//m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* 锁定*/);
}

void CPositionView::ResetList()
{
	//CString str;
	//if (m_nCurrSort == ID_PS_POSITION)
	if(m_ComboCurSel == 0)
	{
		m_listctrl.DeleteAllItems();
		for (int i = 0; i < m_vPosition.size(); i++)
		{
			cust_position_st &p = m_vPosition[i];
			m_listctrl.InsertItem(i, p.instrumentid);
			m_listctrl.SetCellFontWeigth(i, 1, FW_HEAVY);
			m_listctrl.SetItemText(i, 1, theApp.DictToStr(DICT_CALSS_BUY_SELL, p.direction));
			m_listctrl.SetItemText(i, 2, intfmt(p.positions));
			m_listctrl.SetItemText(i, 3, intfmt(p.positions - p.td_positions));
			m_listctrl.SetItemText(i, 4, intfmt(p.can_offset_vol));
			//if (strcmp(p.direction, "0") == 0) 
			//{	
			//	m_listctrl.SetCellTextColor(i, 5,
			//		comp_color(p.last_price, p.hold_average_price, COLOR_RED, COLOR_GREEN, COLOR_BLACK));
			//}
			//else if (strcmp(p.direction, "1") == 0)
			//{		
			//	m_listctrl.SetCellTextColor(i, 5,
			//		comp_color(p.hold_average_price, p.last_price, COLOR_RED, COLOR_GREEN, COLOR_BLACK));
			//}
			m_listctrl.SetCellTextColor(i, 5,
				comp_color(p.direction, p.last_price, p.hold_average_price,  COLOR_RED, COLOR_GREEN, COLOR_BLACK));
			m_listctrl.SetItemText(i, 5, inst_pricefmt(p.instrumentid, p.last_price));
			m_listctrl.SetItemText(i, 6, inst_pricefmt(p.instrumentid, p.hold_average_price));
			
			m_listctrl.SetCellTextColor(i, 7,
				comp_color(p.float_profit, 0.0, COLOR_RED, COLOR_GREEN, COLOR_BLACK));
			m_listctrl.SetCellFontWeigth(i, 7, FW_BOLD);
			m_listctrl.SetItemText(i, 7, inst_pricefmt(p.instrumentid, p.float_profit));
			m_listctrl.SetItemText(i, 8, inst_pricefmt(p.instrumentid, p.open_average_price));
			m_listctrl.SetItemText(i, 9, m_CurCustNo.c_str());

		}
	}
	//else if (m_nCurrSort == ID_PS_POSI_DETAIL)
	if (m_ComboCurSel == 1)
	{
		m_listctrl.DeleteAllItems();
		for (int i = 0; i < m_vPosiDetail.size(); i++) 
		{
			cust_td_position_detail_st &pd = m_vPosiDetail[i];
			m_listctrl.InsertItem(i, pd.instrumentid);
			m_listctrl.SetCellFontWeigth(i, 1, FW_HEAVY);
			m_listctrl.SetItemText(i, 1, theApp.DictToStr(DICT_CALSS_BUY_SELL, pd.direction));
			//str.Format("%d", pd.volume);
			m_listctrl.SetItemText(i, 2, intfmt(pd.volume));
			if (strcmp(pd.trade_date, pd.open_date))
			{
				m_listctrl.SetItemText(i, 3, intfmt(pd.volume));
			}
			else
			{
				m_listctrl.SetItemText(i, 3, "0");
			}
			m_listctrl.SetItemText(i, 4, intfmt(pd.volume));
			//str.Format("%f", pd.hold_price);
			m_listctrl.SetCellTextColor(i, 5,
				comp_color(pd.direction, pd.last_price, pd.hold_price, COLOR_RED, COLOR_GREEN, COLOR_BLACK));
			m_listctrl.SetItemText(i, 5, inst_pricefmt(pd.instrumentid, pd.last_price));
			m_listctrl.SetItemText(i, 6, inst_pricefmt(pd.instrumentid, pd.hold_price));
			m_listctrl.SetCellTextColor(i, 7,
				comp_color(pd.float_profit, 0.0, COLOR_RED, COLOR_GREEN, COLOR_BLACK));
			m_listctrl.SetCellFontWeigth(i, 7, FW_BOLD);
			m_listctrl.SetItemText(i, 7, inst_pricefmt(pd.instrumentid, pd.float_profit));
			m_listctrl.SetItemText(i, 8, inst_pricefmt(pd.instrumentid, pd.open_price));
			m_listctrl.SetItemText(i, 9, m_CurCustNo.c_str());
		}

	}
}

bool CPositionView::RequestOffsetPosition_854002(char *PriceType)
{
	
	if (m_CurCustNo.empty())
		return false;
	int sel = m_listctrl.GetSelectionMark();
	if (sel < 0 || sel > m_listctrl.GetItemCount())
	{
		MessageBox("请单击选中持仓");
		return false;
	}

	Scoped_BCHANDLE handle;
	inst_market_st m;
	//char *cust_no = NULL;
	//char *inst = NULL;
	//char *exchangeid = NULL;
	//int vol = 0;
	//double price = 0.0;
	//char *dir = NULL;
	//std::string inst, dir;
	//int vol;
	//if (m_nCurrSort == ID_PS_POSITION)
	if (m_ComboCurSel == 0)
	{
		//cust_no = m_vPosition[sel].cust_no;
		//inst =  m_vPosition[sel].instrumentid;
		//exchangeid = m_vPosition[sel].exchangeid;
		//dir = m_vPosition[sel].direction;
		//vol = m_vPosition[sel].positions;
		if (!BCRequestInstMarketData_851503(handle, m_vPosition[sel].instrumentid, m))
			return false;
		BCRequestOffsetOrderInsert(handle, m_vPosition[sel], m);
	}
	//else if (m_nCurrSort == ID_PS_POSI_DETAIL)
	else if (m_ComboCurSel == 1)
	{
		//cust_no = m_vPosiDetail[sel].cust_no;
		//inst = m_vPosiDetail[sel].instrumentid;
		//exchangeid = m_vPosiDetail[sel].exchangeid;
		//dir = m_vPosiDetail[sel].direction;
		//vol = m_vPosiDetail[sel].volume;
		if (!BCRequestInstMarketData_851503(handle, m_vPosition[sel].instrumentid, m))
			return false;
		BCRequestOffsetOrderInsert(handle, m_vPosition[sel], m);
	}

	return false;
	//if (!RequestMarketData_851503(handle, inst))
	//	return false;
	
	

	//BCResetHandle(handle);
	//BCSetRequestType(handle, 854002);
	//
	////if (m_nCurrSort == ID_PS_POSITION) 

	//if (m_ComboCurSel == 0)
	//{
	//	BCSetStringFieldByName(handle, 0, "smarket_code", m_vPosition[sel].exchangeid); 
	//	BCSetStringFieldByName(handle, 0, "scust_auth", m_vPosition[sel].instrumentid);
	//	//(handle, 0, "sstatus2", m_vPosition[sel].direction);
	//	BCSetIntFieldByName(handle, 0, "lvol0", m_vPosition[sel].positions);
	//	//dir = m_vPosition[sel].direction;

	//}
	////else if (m_nCurrSort == ID_PS_POSI_DETAIL)
	//else if (m_ComboCurSel == 1)
	//{
	//	BCSetStringFieldByName(handle, 0, "smarket_code", m_vPosiDetail[sel].exchangeid);
	//	BCSetStringFieldByName(handle, 0, "scust_auth", m_vPosiDetail[sel].instrumentid);
	//	BCSetIntFieldByName(handle, 0, "lvol0", m_vPosiDetail[sel].volume);
	//}

	//if (strcmp(dir, "0") == 0)
	//	dir = "1";
	//else
	//	dir = "0";

	//BCSetStringFieldByName(handle, 0, "sstatus2", (dir == "0" ? "1" : "0"));

	//BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	//BCSetStringFieldByName(handle, 0, "sholder_ac_no", (char*)m_CurCustNo.c_str());

	////市价
	//if (strcmp(PriceType, "1") == 0) {
	//	if (dir == "0")//平多单
	//		//BCSetDoubleFieldByName(handle, 0, "damt0", m.ask_price);
	//		price = m.bid_price;
	//	else if (dir == "1")//平空单
	//		price = m.ask_price;
	//}
	//else if (strcmp(PriceType, "2") == 0)//对价
	//{
	//	if (dir == "0")//买
	//		//BCSetDoubleFieldByName(handle, 0, "damt0", m.bid_price);
	//		price = m.ask_price;
	//	else if (dir == "1")//卖
	//		//BCSetDoubleFieldByName(handle, 0, "damt0", m.ask_price);
	//		price = m.bid_price;
	//}

	//if(strcmp(dir, "0") == 0)
	//price = (strcmp(dir, "0") == 0) ? m.upperlimit_price : m.lowerlimit_price;
	//BCSetStringFieldByName(handle, 0, "sstatus3", "1");//开平
	//BCSetStringFieldByName(handle, 0, "sstatus4", "0");//投保
	//BCSetStringFieldByName(handle, 0, "smain_flag2", "1");//下单人类型
	//BCSetStringFieldByName(handle, 0, "sholder_type", "1");//委托方式 柜台委托
	//BCSetStringFieldByName(handle, 0, "scurrency_type", "1");//scurrency_type = '', 币种
	//BCSetStringFieldByName(handle, 0, "sstatus0", "0");//强平
	//BCSetStringFieldByName(handle, 0, "scust_type", "0");//定单类型 限价
	////BCSetStringFieldByName(handle, 0, "scust_type", PriceType);//定单类型 限价
	//BCSetStringFieldByName(handle, 0, "scust_type2", "0");//成交属性 //'0', 'GFD当日有效'
	//BCSetStringFieldByName(handle, 0, "sstatus1", "A");	//sstatus1 = '', 委托单类型
	//BCSetIntFieldByName(handle, 0, "lvol8", vol);//lvol8 = '', 最小成交数量

	//return MyBCRequest(handle, FetchRowFunc_t(), BCMSG_NOR_NOMB);
	//return BCRequestOrderInsert_854002(handle, cust_no, inst, exchangeid, "1", dir, price, vol);
}

///* 输出：最新价		output:damt1
//买价		damt2
//卖价		damt3
//涨跌		damt4
//最高价		damt5
//最低价		damt6
//开盘价		damt7
//涨停版		damt8
//跌停版		damt9
//买量		lvol1
//卖量		lvol2
//成交量		lvol3
//净持仓		lvol4
//昨收盘		damt10
//今结算		damt11
//史最高		damt12
//史最低		damt13

//bool CPositionView::RequestMarketData_851503(BCHANDLE handle, std::string inst)
//{
//	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
//		BCGetDoubleFieldByName(handle, 0, "damt1", &m_InstMarket.last_price);
//		BCGetDoubleFieldByName(handle, 0, "damt2", &m_InstMarket.bid_price);
//		BCGetDoubleFieldByName(handle, 0, "damt3", &m_InstMarket.ask_price);
//		BCGetDoubleFieldByName(handle, 0, "damt4", &m_InstMarket.range);
//		BCGetDoubleFieldByName(handle, 0, "damt5", &m_InstMarket.highest_price);
//		BCGetDoubleFieldByName(handle, 0, "damt6", &m_InstMarket.lowest_price);
//		BCGetDoubleFieldByName(handle, 0, "damt7", &m_InstMarket.open_price);
//		BCGetDoubleFieldByName(handle, 0, "damt8", &m_InstMarket.upperlimit_Price);
//		BCGetDoubleFieldByName(handle, 0, "damt9", &m_InstMarket.lowerlimit_price);
//		BCGetIntFieldByName(handle, 0, "lvol1", &m_InstMarket.bid_vol);
//		BCGetIntFieldByName(handle, 0, "lvol2", &m_InstMarket.ask_vol);
//		BCGetIntFieldByName(handle, 0, "lvol3", &m_InstMarket.volume);
//		BCGetIntFieldByName(handle, 0, "lvol4", &m_InstMarket.delta);
//		BCGetDoubleFieldByName(handle, 0, "damt10", &m_InstMarket.pre_close_price);
//		BCGetDoubleFieldByName(handle, 0, "damt11", &m_InstMarket.settlement_price);
//		BCGetDoubleFieldByName(handle, 0, "damt12", &m_InstMarket.his_highest_price);
//		BCGetDoubleFieldByName(handle, 0, "damt13", &m_InstMarket.his_lowest_price);
//	};
//	BCResetHandle(handle);
//	BCSetRequestType(handle, 851503);
//	BCSetStringFieldByName(handle, 0, "sorder2", g_cfg.oper_code);
//	std::string::iterator it = std::find_if(inst.begin(), inst.end(), ::isdigit);
//	std::string product(inst.begin(), it);
//	std::string delivery(it, inst.end());
//	BCSetStringFieldByName(handle, 0, "sstock_code", (char*)product.c_str());
//	BCSetStringFieldByName(handle, 0, "sdate0", (char*)delivery.c_str());
//	
//	memset(&m_InstMarket, 0, sizeof(m_InstMarket));
//
//	return MyBCRequest(handle, fetcher, BCMSG_NOR_NOMB);
//}

void CPositionView::initPosiDetailLastPrice()
{
	for (PosiDetailVec_t::iterator it = m_vPosiDetail.begin(); it != m_vPosiDetail.end(); ++it)
	{
		it->last_price = getLastPrice(m_vPosition, it->instrumentid, it->direction);
		it->float_profit = calcProfit(*it, it->last_price);
	}
}

//void CPositionView::OnSort(UINT id)
//{
//	if (m_nCurrSort == id)
//	{
//		return;
//	}
//
//	m_nCurrSort = id;
//
//	////CPositionViewMenuButton* pButton = DYNAMIC_DOWNCAST(CPositionViewMenuButton, m_wndToolBar.GetButton(0));
//	//CPositionViewMenuButton* pButton = DYNAMIC_DOWNCAST(CPositionViewMenuButton, m_wndToolBar.GetButton(0));
//	//if (pButton != NULL)
//	//{
//	//	pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
//	//	m_wndToolBar.Invalidate();
//	//	m_wndToolBar.UpdateWindow();
//	//}
//
//	ResetList();
//}

//void CPositionView::OnUpdateSort(CCmdUI * pCmdUI)
//{
//	//pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
//}

//void CPositionView::OnShowPosition()
//{
//	m_nCurrSort = ID_PS_POSITION;
//	CString str;
//	m_listctrl.DeleteAllItems();
//	for (int i = 0; i < m_vPosition.size(); i++)
//	{
//		cust_position_st &p = m_vPosition[i];
//		m_listctrl.InsertItem(i, p.instrumentid);
//		//str.Format("%d", p.)
//		m_listctrl.SetItemText(i, 1, theApp.DictToStr(DICT_CALSS_BUY_SELL, p.direction));
//		str.Format("%d", p.positions);
//		m_listctrl.SetItemText(i, 2, str);
//		str.Format("%d", p.positions - p.td_positions);
//		m_listctrl.SetItemText(i, 3, str);
//		str.Format("%f", p.hold_average_price);
//		m_listctrl.SetItemText(i, 6, str);
//		str.Format("%f", p.float_profit);
//		m_listctrl.SetItemText(i, 7, str);
//		m_listctrl.SetItemText(i, 11, m_CurCustNo.c_str());
//
//	}
//}
//
//void CPositionView::OnShowPosiDetail()
//{
//	m_nCurrSort = ID_PS_POSI_DETAIL;
//	CString str;
//	m_listctrl.DeleteAllItems();
//	for (int i = 0; i < m_vPosiDetail.size(); i++) {
//		cust_td_position_detail_st &pd = m_vPosiDetail[i];
//		m_listctrl.InsertItem(i, pd.instrumentid);
//		m_listctrl.SetItemText(i, 1, theApp.DictToStr(DICT_CALSS_BUY_SELL, pd.direction));
//		str.Format("%d", pd.volume);
//		m_listctrl.SetItemText(i, 2, str);
//		str.Format("%f", pd.hold_price);
//		m_listctrl.SetItemText(i, 6, str);
//		m_listctrl.SetItemText(i, 11, m_CurCustNo.c_str());
//	}
//}

void CPositionView::OnOffsetDJ()
{
	if (RequestOffsetPosition_854002("2")) {
		//MessageBox("对价平仓指令发送成功");
		MessageBox("平仓指令发送成功");
	}
}

void CPositionView::OnOffsetSJ()
{
	if (RequestOffsetPosition_854002("1"))
	{
		MessageBox("市价平仓指令发送成功");
	}
}

void CPositionView::OnOffsetAll()
{
	Scoped_BCHANDLE handle;
	if(m_ComboCurSel == 0)
	{
		for (PosiVec_t::iterator it = m_vPosition.begin(); it != m_vPosition.end(); ++it)
		{
			inst_market_st m;
			if (BCRequestInstMarketData_851503(handle, it->instrumentid, m))
				BCRequestOffsetOrderInsert(handle, *it, m);
		}
	}
	else if (m_ComboCurSel == 1)
	{
		for (PosiDetailVec_t::iterator it = m_vPosiDetail.begin(); it != m_vPosiDetail.end(); ++it)
		{
			inst_market_st m;
			if (BCRequestInstMarketData_851503(handle, it->instrumentid, m))
				BCRequestOffsetOrderInsert(handle, *it, m);
		}
	}
	
}


afx_msg LRESULT CPositionView::OnUserCustSelChanged(WPARAM wParam, LPARAM lParam)
{
	std::string cust_no = fetch_string(lParam);
	m_CurCustNo = cust_no;
	m_vPosition =  cust_dynamic_info_worker::instance()->get_cust_position(cust_no);
	m_vPosiDetail = cust_dynamic_info_worker::instance()->get_cust_posi_detail(cust_no);
	initPosiDetailLastPrice();
	ResetList();
	return 0;
}


afx_msg LRESULT CPositionView::OnUserCustDynamicInfo(WPARAM wParam, LPARAM lParam)
{
	std::string cust_no = fetch_string(lParam);
	if (m_CurCustNo != cust_no)
		return 0;
	if(wParam == CUST_DYNAMIC_POSITION_CHANGED)
	{ 
		m_vPosition = cust_dynamic_info_worker::instance()->get_cust_position(m_CurCustNo);
		initPosiDetailLastPrice();
		//if (m_nCurrSort == ID_PS_POSITION)
		if(m_ComboCurSel == 0)
			ResetList();
	}
	else if (wParam == CUST_DYNAMIC_POSI_DETAIL_CHANGED)
	{
		m_vPosiDetail = cust_dynamic_info_worker::instance()->get_cust_posi_detail(m_CurCustNo);
		initPosiDetailLastPrice();
		//if (m_nCurrSort == ID_PS_POSI_DETAIL)
		if(m_ComboCurSel == 1)
			ResetList();
	}
	return 0;
}


afx_msg LRESULT CPositionView::OnUserInstMarket(WPARAM wParam, LPARAM lParam)
{
	std::string inst = fetch_string(wParam);
	depth_market_data_st m = MdApi_GetMarketData(inst);
	////CString str;
	////COLORREF clr;
	for (PosiVec_t::iterator it = m_vPosition.begin(); it != m_vPosition.end(); ++it) {
		if (inst == it->instrumentid) 
		{
			it->last_price = m.LastPrice;
			it->float_profit = calcProfit(*it, m.LastPrice);
			int item = std::distance(m_vPosition.begin(), it);
			if (m_ComboCurSel == 0)
			{
				//if (strcmp(it->direction, "0") == 0)
				//{
				//	m_listctrl.SetCellTextColor(item, 5,
				//		comp_color(it->last_price, it->hold_average_price, COLOR_RED, COLOR_GREEN, COLOR_BLACK));
				//}
				//else if (strcmp(it->direction, "1") == 0)
				//{
				//	m_listctrl.SetCellTextColor(item, 5,
				//		comp_color(it->hold_average_price, it->last_price, COLOR_RED, COLOR_GREEN, COLOR_BLACK));
				//}
				m_listctrl.SetCellTextColor(item, 5,
					comp_color(it->direction, it->last_price, it->hold_average_price, COLOR_RED, COLOR_GREEN, COLOR_BLACK));
				m_listctrl.SetItemText(item, 5, inst_pricefmt(it->instrumentid, it->last_price));
				m_listctrl.SetCellTextColor(item, 7,
					comp_color(it->float_profit, 0.0, COLOR_RED, COLOR_GREEN, COLOR_BLACK));
				m_listctrl.SetCellFontWeigth(item, 7, FW_BOLD);
				m_listctrl.SetItemText(item, 7, inst_pricefmt(it->instrumentid, it->float_profit));

			}
		}
		
	}

	for (PosiDetailVec_t::iterator it = m_vPosiDetail.begin(); it != m_vPosiDetail.end(); ++it) 
	{
		if (inst == it->instrumentid)
		{
			it->last_price = m.LastPrice;
			it->float_profit = calcProfit(*it, m.LastPrice);
			int item = std::distance(m_vPosiDetail.begin(), it);

			if (m_ComboCurSel == 1)
			{
				//if (strcmp(it->direction, "0") == 0)
				//{
				//	m_listctrl.SetCellTextColor(item, 5,
				//		comp_color(m.LastPrice, it->hold_price, COLOR_RED, COLOR_GREEN, COLOR_BLACK));
				//}
				//else if (strcmp(it->direction, "1") == 0)
				//{
				//	m_listctrl.SetCellTextColor(item, 5,
				//		comp_color(it->hold_price, m.LastPrice, COLOR_RED, COLOR_GREEN, COLOR_BLACK));
				//}
				m_listctrl.SetCellTextColor(item, 5,
					comp_color(it->direction, m.LastPrice, it->hold_price,  COLOR_RED, COLOR_GREEN, COLOR_BLACK));
				m_listctrl.SetItemText(item, 5, inst_pricefmt(it->instrumentid, m.LastPrice));

				m_listctrl.SetCellTextColor(item, 7,
					comp_color(it->float_profit, 0.0, COLOR_RED, COLOR_GREEN, COLOR_BLACK));
				m_listctrl.SetItemText(item, 7, fundfmt(it->float_profit));
			}
		}
		
	}

	return 0;
}

//afx_msg LRESULT CPositionView::OnUserCustPositionFloatProfit(WPARAM wParam, LPARAM lParam)
//{
//	std::istringstream iss(fetch_string(wParam));
//	cust_position_id_st posi_id;
//	iss >> posi_id;
//	//fetch_string(wParam)
//	std::vector<cust_position_st>::iterator it;
//	it = std::find(m_vPosition.begin(), m_vPosition.end(), posi_id);
//	if (it != m_vPosition.end())
//	{
//		it->float_profit = (lParam / 100.0);
//		if(m_ComboCurSel == 0)
//		{
//			int item = std::distance(m_vPosition.begin(), it);
//			m_listctrl.SetCellTextColor(item, 7,
//				comp_color(it->float_profit, 0.0, COLOR_RED, COLOR_GREEN, COLOR_BLACK));
//			m_listctrl.SetItemText(item, 7, fundfmt(it->float_profit));
//		}
//	}
//	return 0;
//}
