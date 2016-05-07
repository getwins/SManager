// TradeView.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "TradeView.h"
#include "string_identifer.h"
#include "cust_dynamic_info_worker.h"
#include "Format.h"

class CTradeViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CPositionView;

	DECLARE_SERIAL(CTradeViewMenuButton)

public:
	CTradeViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};
IMPLEMENT_SERIAL(CTradeViewMenuButton, CMFCToolBarMenuButton, 1)
// CTradeView

IMPLEMENT_DYNAMIC(CTradeView, CDockablePane)

CTradeView::CTradeView()
{
	m_ComboCurSel = -1;
}

CTradeView::~CTradeView()
{
}


BEGIN_MESSAGE_MAP(CTradeView, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_MESSAGE(WM_USER_CUST_SEL_CHANGED, &CTradeView::OnUserCustSelChanged)
	ON_MESSAGE(WM_USER_CUST_DYNAMIC_INFO, &CTradeView::OnUserCustDynamicInfo)
	ON_COMMAND(ID_TS_MENU, OnClickComboBox)
	ON_CBN_SELCHANGE(ID_TS_MENU, OnComboBoxSelChangeClick)
END_MESSAGE_MAP()



// CTradeView 消息处理程序

void CTradeView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	AdjustLayout();
}


int CTradeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	//CMFCToolBarMenuButton buttotn;

	////m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	////m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* 已锁定*/);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_TOOLBAR_TRADE);
	m_wndToolBar.LoadToolBar(IDR_TOOLBAR_TRADE, 0, 0, TRUE /* 已锁定*/);


	//OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由: 
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);
	
	//CMFCToolBarComboBoxButton *pCombo = new CMFCToolBarComboBoxButton(ID_TS_MENU,
	//	GetCmdMgr()->GetCmdImage(ID_TS_MENU, FALSE), CBS_DROPDOWNLIST, 80);
	//pCombo->AddItem(_T("成交明细"));
	//pCombo->AddItem(_T("成交汇总"));
	////pCombo->EnableWindow(TRUE);

	//pCombo->SelectItem((m_ComboCurSel = 0));

	//m_wndToolBar.ReplaceButton(ID_TS_MENU, *pCombo);

	CMFCToolBarComboBoxButton Combo(ID_TS_MENU, GetCmdMgr()->GetCmdImage(ID_TS_MENU, FALSE), CBS_DROPDOWNLIST, 80);
	Combo.AddItem(_T("成交明细"));
	Combo.AddItem(_T("成交汇总"));
	//Combo.EnableWindow(TRUE);
	Combo.SelectItem((m_ComboCurSel = 0));
	m_wndToolBar.ReplaceButton(ID_TS_MENU, Combo);

	//CMenu menuSort;
	////menuSort.LoadMenu(IDR_POPUP_SORT);
	//menuSort.LoadMenu(IDR_POPUP_TRADE_SHOW);

	////m_wndToolBar.ReplaceButton(ID_SORT_MENU, CPositionViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));
	//m_wndToolBar.ReplaceButton(ID_TS_MENU, CTradeViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	//CTradeViewMenuButton* pButton = DYNAMIC_DOWNCAST(CTradeViewMenuButton, m_wndToolBar.GetButton(0));

	//if (pButton != NULL)
	//{
	//	pButton->m_bText = FALSE;
	//	pButton->m_bImage = TRUE;
	//	pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
	//	pButton->SetMessageWnd(this);
	//}


	m_listctrl.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT, RECT(), this, ID_POSITION_LISTCTRL);
	LVCOLUMN cols[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "成交编号" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 60, "合约" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "买卖" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "开平" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "成交价格" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "成交手数" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "成交时间" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "报单编号" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "交易所" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "手续费" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "盈亏" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "交易账户" }
	};

	for (int i = 0; i < sizeof(cols) / sizeof(cols[0]); ++i)
		m_listctrl.InsertColumn(i, &cols[i]);

	return 0;
}

void CTradeView::AdjustLayout()
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

	//m_listctrl.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	//m_listctrl.ShowWindow(SW_SHOW);
}

//TradeVec_t::iterator findTrade(TradeVec_t &tv, char *inst)
#include <algorithm>
void CTradeView::SummaryTrade()
{
	std::function<bool(cust_trade_st, char*)> fn = [](cust_trade_st t, char *inst)
	{
		return strcmp(t.instrumentid, inst) == 0;
	};

	m_vSummary.clear();
	TradeVec_t::iterator sit;
	for (TradeVec_t::iterator it = m_vTrade.begin(); it != m_vTrade.end(); ++it)
	{
		sit = std::find_if(m_vSummary.begin(), m_vSummary.end(), std::bind(fn, std::placeholders::_1, it->instrumentid));
		if (sit == m_vSummary.end())
		{
			cust_trade_st t = *it;
			t.orderseq = 0;
			memset(t.direction, 0, sizeof(t.direction));
			t.price = 0;
			memset(t.offset_flag, 0, sizeof(t.offset_flag));
			memset(t.hedge_flag, 0, sizeof(t.hedge_flag));
			t.tradeseq = 0;
			memset(t.force_close, 0, sizeof(t.force_close));
			m_vSummary.push_back(t);
		}
		else
		{
			sit->volume += it->volume;
			sit->commission += it->commission;
		}
	}
}

void CTradeView::ResetList()
{
	m_listctrl.DeleteAllItems();
	TradeVec_t *tv = NULL;
	if (m_ComboCurSel == 0)
	{
		tv = &m_vTrade;
		
	}
	else if (m_ComboCurSel == 1)
	{
		tv = &m_vSummary;
	}
	
	if (tv != NULL)
	{
		for (int i = 0; i < tv->size(); i++)
		{
			cust_trade_st &t = (*tv)[i];
			m_listctrl.InsertItem(i, intfmt(t.tradeseq));
			m_listctrl.SetItemText(i, 1, t.instrumentid);
			m_listctrl.SetItemText(i, 2, theApp.DictToStr(DICT_CLASS_BUY_SELL, t.direction));
			m_listctrl.SetItemText(i, 3, theApp.DictToStr(DICT_CLASS_OFFSET_FLAG, t.offset_flag));
			m_listctrl.SetItemText(i, 4, inst_pricefmt(t.instrumentid, t.price));
			m_listctrl.SetItemText(i, 5, intfmt(t.volume));
			m_listctrl.SetItemText(i, 6, t.time);
			m_listctrl.SetItemText(i, 7, intfmt(t.orderseq));
			m_listctrl.SetItemText(i, 8, t.exchangeid);
			m_listctrl.SetItemText(i, 9, fundfmt(t.commission));
			m_listctrl.SetItemText(i, 10, t.cust_no);
		}
	}
}

afx_msg LRESULT CTradeView::OnUserCustSelChanged(WPARAM wParam, LPARAM lParam)
{
	std::string cust_no = fetch_string(lParam);
	if (m_CurCustNo == cust_no)
		return 0;
	m_CurCustNo = cust_no;
	m_vTrade = cust_dynamic_info_worker::instance()->get_cust_trade(m_CurCustNo);
	SummaryTrade();
	ResetList();
	return 0;
}


afx_msg LRESULT CTradeView::OnUserCustDynamicInfo(WPARAM wParam, LPARAM lParam)
{
	std::string cust_no = fetch_string(lParam);
	if (m_CurCustNo != cust_no)
		return 0;
	m_vTrade = cust_dynamic_info_worker::instance()->get_cust_trade(m_CurCustNo);
	SummaryTrade();
	ResetList();

	return 0;
}

void CTradeView::OnComboBoxSelChangeClick()
{
	CMFCToolBarComboBoxButton* pSrcCombo = CMFCToolBarComboBoxButton::GetByCmd(ID_TS_MENU, TRUE);
	if (!pSrcCombo)
		return;
	if (pSrcCombo->GetCurSel() == m_ComboCurSel)
		return;

	m_ComboCurSel = pSrcCombo->GetCurSel();
	ResetList();
}
