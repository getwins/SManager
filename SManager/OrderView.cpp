// OrderView.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "OrderView.h"
#include "string_identifer.h"
#include "cust_dynamic_info_worker.h"
#include "SMutility.h"
#include "Format.h"
#include <sstream>

//class COrderViewMenuButton : public CMFCToolBarMenuButton
//{
//	friend class COrderView;
//
//	DECLARE_SERIAL(COrderViewMenuButton)
//
//public:
//	COrderViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
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
//IMPLEMENT_SERIAL(COrderViewMenuButton, CMFCToolBarMenuButton, 1)
// COrderView

IMPLEMENT_DYNAMIC(COrderView, CDockablePane)

COrderView::COrderView()
{
	//m_nCurrSort = ID_ORDER_STATUS_A;
	m_ComboCurSel = -1;
}

COrderView::~COrderView()
{
}


BEGIN_MESSAGE_MAP(COrderView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_OS_MENU, OnClickComboBox)
	ON_CBN_SELCHANGE(ID_OS_MENU, OnComboBoxSelChangeClick)
	ON_MESSAGE(WM_USER_CUST_SEL_CHANGED, &COrderView::OnUserCustSelChanged)
#ifndef  _OPER_PROXY
	ON_COMMAND(ID_CANCEL_ORDER, &COrderView::OnCancelOrder)
	ON_UPDATE_COMMAND_UI(ID_CANCEL_ORDER, &COrderView::OnCancelOrderUpdateUI)
	ON_COMMAND(ID_CANCEL_ALL_ORDER, &COrderView::OnCancelAllOrder)
	ON_UPDATE_COMMAND_UI(ID_CANCEL_ALL_ORDER, &COrderView::OnCancelAllOrderUpdateUI)
#endif
	ON_MESSAGE(WM_USER_CUST_DYNAMIC_INFO, &COrderView::OnUserCustDynamicInfo)
END_MESSAGE_MAP()



// COrderView 消息处理程序




int COrderView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CMFCToolBarMenuButton buttotn;

	//m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	//m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* 已锁定*/);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_TOOLBAR_ORDER);
	m_wndToolBar.LoadToolBar(IDR_TOOLBAR_ORDER, 0, 0, TRUE /* 已锁定*/);


	//OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由: 
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	//CMenu menuSort;
	////menuSort.LoadMenu(IDR_POPUP_SORT);
	//menuSort.LoadMenu(IDR_POPUP_ORDER_SHOW);

	////m_wndToolBar.ReplaceButton(ID_SORT_MENU, CPositionViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));
	//m_wndToolBar.ReplaceButton(ID_OS_MENU, COrderViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	//COrderViewMenuButton* pButton = DYNAMIC_DOWNCAST(COrderViewMenuButton, m_wndToolBar.GetButton(0));

	//if (pButton != NULL)
	//{
	//	pButton->m_bText = FALSE;
	//	pButton->m_bImage = TRUE;
	//	pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
	//	pButton->SetMessageWnd(this);
	//}

	//CMFCToolBarComboBoxButton *pCombo = new CMFCToolBarComboBoxButton(ID_OS_MENU, 
	//	GetCmdMgr()->GetCmdImage(ID_OS_MENU, FALSE), CBS_DROPDOWNLIST, 80);
	//pCombo->AddItem("全部");
	//pCombo->AddItem("可撤");
	//pCombo->SelectItem((m_ComboCurSel = 0));
	//m_wndToolBar.ReplaceButton(ID_OS_MENU, *pCombo);


	CMFCToolBarComboBoxButton Combo(ID_OS_MENU, GetCmdMgr()->GetCmdImage(ID_OS_MENU, FALSE), CBS_DROPDOWNLIST, 80);
	Combo.AddItem("全部");
	Combo.AddItem("可撤");
	Combo.SelectItem((m_ComboCurSel = 0));
	m_wndToolBar.ReplaceButton(ID_OS_MENU, Combo);


	m_listctrl.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT, RECT(), this, ID_POSITION_LISTCTRL);

	LVCOLUMN cols[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "报单编号" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "合约" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "买卖" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "开平" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "报单状态" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "报单价格" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 60, "报单量" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 60, "未成交" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 60, "已成交" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "详细状态" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "席位" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "报单时间" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "强平" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "风控信息" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "交易所" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "交易账户" },
	};

	for (int i = 0; i < sizeof(cols) / sizeof(cols[0]); ++i)
		m_listctrl.InsertColumn(i, &cols[i]);
	return 0;
}


void COrderView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	AdjustLayout();
	//CRect rect;
	//GetClientRect(&rect);
	//m_listctrl.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

void COrderView::AdjustLayout()
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

void COrderView::ResetList()
{
	m_listctrl.DeleteAllItems();
	CString str;
	//for (int i = 0; i < m_vOrder.size(); i++)
	for each (cust_order_st o in m_vOrder)
	{	
		//if (m_ComboCurSel == 1 && !(o.order_status[0] == 'n' || o.order_status[0] == 's' || o.order_status[0] == 'a' || o.order_status[0] == 'p'))
		if (m_ComboCurSel == 1 && !canCancel(o))
			continue;
		int item = m_listctrl.GetItemCount();
		str.Format("%d", o.orderseq);
		m_listctrl.InsertItem(item, str);
		m_listctrl.SetItemText(item, 1, o.instrumentid);
		m_listctrl.SetItemText(item, 2, theApp.DictToStr(DICT_CLASS_BUY_SELL, o.direction));
		m_listctrl.SetItemText(item, 3, theApp.DictToStr(DICT_CLASS_OFFSET_FLAG, o.offset_flag));
		const char *order_status = theApp.DictToStr(DICT_CLASS_ORDER_STATUS, o.order_status);
		m_listctrl.SetItemText(item, 4, order_status);
		//str.Format("%f", o.order_price);
		m_listctrl.SetItemText(item, 5, inst_pricefmt(o.instrumentid, o.order_price));
		str.Format("%d", o.origin_total_volume);
		m_listctrl.SetItemText(item, 6, str);
		str.Format("%d", o.total_volume);
		m_listctrl.SetItemText(item, 7, str);
		str.Format("%d", o.origin_total_volume - o.total_volume);
		m_listctrl.SetItemText(item, 8, str);
		//m_listctrl.SetItemText(item, 9, order_status);
		m_listctrl.SetItemText(item, 9, o.seatid);
		m_listctrl.SetItemText(item, 10, o.insert_time);

		if (o.force_close[0] == '0')
		{
			m_listctrl.SetCellFontWeigth(item, 11, FW_NORMAL);
			m_listctrl.SetCellTextColor(item, 11, COLOR_BLACK);
		}
		else
		{
			m_listctrl.SetCellFontWeigth(item, 11, FW_HEAVY);
			m_listctrl.SetCellTextColor(item, 11, COLOR_RED);
		}

		m_listctrl.SetItemText(item, 11, theApp.DictToStr(DICT_CLASS_FORCE_CLOSE, o.force_close));
		m_listctrl.SetItemText(item, 12, o.exchangeid);
		m_listctrl.SetItemText(item, 13, m_CurCustNo.c_str());
	}

}

afx_msg
void COrderView::OnComboBoxSelChangeClick()
{
	CMFCToolBarComboBoxButton* pSrcCombo = CMFCToolBarComboBoxButton::GetByCmd(ID_OS_MENU, TRUE);

	if (pSrcCombo->GetCurSel() == m_ComboCurSel)
		return;

	m_ComboCurSel = pSrcCombo->GetCurSel();
	ResetList();
}

LRESULT COrderView::OnUserCustSelChanged(WPARAM wParam, LPARAM lParam)
{
	std::string cust_no = fetch_string(lParam);
	if (cust_no == m_CurCustNo)
		return 0;
	m_CurCustNo = cust_no;
	m_vOrder = cust_dynamic_info_worker::instance()->get_cust_order(m_CurCustNo);
	ResetList();
	return 0;
}

afx_msg LRESULT COrderView::OnUserCustDynamicInfo(WPARAM wParam, LPARAM lParam)
{
	std::string cust_no = fetch_string(lParam);
	if (cust_no != m_CurCustNo)
		return 0;
	m_vOrder = cust_dynamic_info_worker::instance()->get_cust_order(m_CurCustNo);
	ResetList();
	return 0;
}

void COrderView::OnCancelOrder()
{
	//if (RequestCancelOrder_854018()) {
	//	MessageBox("撤单成功");
	//	PostOutputMsg("撤单成功");
	//}

	int sel = m_listctrl.GetSelectionMark();
	if (sel < 0 || sel >= m_listctrl.GetItemCount())
	{
		MessageBox("请在委托列表中选中要撤消的委托");
		return;
	}

	CString order_seq = m_listctrl.GetItemText(sel, 0);
	
	for (std::vector<cust_order_st>::iterator it = m_vOrder.begin(); it != m_vOrder.end(); ++it)
	{
		if (order_seq == intfmt(it->orderseq))
		{
			Scoped_BCHANDLE handle;
			if (RequestCancelOrder_854018(handle, *it))
			{
				MessageBox("撤单成功");
				PostOutputMsg("撤单成功");
			}
			break;
		}
	}
}

void COrderView::OnCancelOrderUpdateUI(CCmdUI* pCmdUI)
{
}


void COrderView::OnCancelAllOrder()
{
	Scoped_BCHANDLE handle;
	BCResult result;
	for (std::vector<cust_order_st>::iterator it = m_vOrder.begin(); it != m_vOrder.end(); ++it)
	{
		if (canCancel(*it))
		{
			std::ostringstream oss;
			BCResult result = RequestCancelOrder_854018(handle, *it);
			oss << "委托编号" << it->orderseq << "撤单结果 " << result;
			PostOutputMsg(oss.str());
		}	
	}
}

void COrderView::OnCancelAllOrderUpdateUI(CCmdUI * pCmdUI)
{
}

BCResult COrderView::RequestCancelOrder_854018(BCHANDLE handle, cust_order_st &odr)
{
	//int sel = m_listctrl.GetSelectionMark();
	//if (sel < 0 || sel >= m_listctrl.GetItemCount())
	//{
	//	MessageBox("请在委托列表中选中要撤消的委托");
	//	return false;
	//}
	////CString status = 
	//CString order_seq = m_listctrl.GetItemText(sel, 0);
	//CString seatid = m_listctrl.GetItemText(sel, 9);
	//Scoped_BCHANDLE handle;
	BCResetHandle(handle);
	BCSetRequestType(handle, 854018);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	//BCSetStringFieldByName(handle, 0, "lserial0", order_seq.GetBuffer());
	//BCSetStringFieldByName(handle, 0, "sholder_ac_no", (char*)m_CurCustNo.c_str());
	//BCSetStringFieldByName(handle, 0, "sserial2", seatid.GetBuffer());
	BCSetIntFieldByName(handle, 0, "lserial0", odr.orderseq);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", odr.cust_no);
	BCSetStringFieldByName(handle, 0, "sserial2", odr.seatid);
	BCSetStringFieldByName(handle, 0, "smain_flag2", "1"); //下单人类型 操作员
	
	return MyBCRequest(handle, FetchRowFunc_t(), BCMSG_NOR_NOMB);
}



