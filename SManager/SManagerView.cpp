
// SManagerView.cpp : CSManagerView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "SManager.h"
#endif

#include "SManagerDoc.h"
#include "SManagerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <boost/algorithm/string.hpp>

#include "string_identifer.h"
#include "cust_dynamic_info_worker.h"
//#include "MdApi.h"
#include "Format.h"
#include "struct_helper.h"
#include <fstream>
#include <iomanip>

#include "CustMoneyInOutDialog.h"
#include "CustStatusMaintainDialog.h"
#include "QryOrderDialg.h"
#include "QryPositionDialog.h"
#include "QryTradeDialog.h"
#include "QryMoneyInOutDialog.h"
#include "QrySettlementDialog.h"
#include "ResetCustTradePasswdDialog.h"
#include "ResetCustFundPasswdDialog.h"
#include "CustTemplateSettingDialog.h"
#include "CustRiskControlSettingDialog.h"
#include "CustCommissionDialog.h"
#include "CustMarginDialog.h"
#include "BankAccountMgrDialog.h"


// CSManagerView

IMPLEMENT_DYNCREATE(CSManagerView, CView)

BEGIN_MESSAGE_MAP(CSManagerView, CView)
	// 标准打印命令
	//ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSManagerView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CUST, &CSManagerView::OnNMDblclkListCust)
	ON_COMMAND(ID_MONEY_IN_OUT, &CSManagerView::OnMoneyInOut)
	//ON_COMMAND(ID_FROZEN_CUST, &CSManagerView::OnFrozenCust)
	ON_COMMAND(ID_CUST_STATUS_MAINTAIN, &CSManagerView::OnCustStatusMaintain)
	ON_COMMAND(ID_ORDER_QUERY, &CSManagerView::OnOrderQuery)
	ON_COMMAND(ID_POSITION_QUERY, &CSManagerView::OnPositionQuery)
	ON_COMMAND(ID_TRADE_QUERY, &CSManagerView::OnTradeQuery)
	ON_COMMAND(ID_QUERY_MONEY_IN_OUT, &CSManagerView::OnQueryMoneyInOut)
	ON_COMMAND(ID_SETTLEMENT_QUERY, &CSManagerView::OnSettlementQuery)
	ON_COMMAND(ID_RESET_TRADE_PASSWD, &CSManagerView::OnResetTradePasswd)
	ON_COMMAND(ID_RESET_FUND_PASSWD, &CSManagerView::OnResetFundPasswd)
	ON_COMMAND(ID_CANCEL_CUST, &CSManagerView::OnCancelCust)
	ON_COMMAND(ID_CUST_TEMPLATE_SETTING, &CSManagerView::OnCustTemplateSetting)
	//ON_MESSAGE(CUST_DYNAMIC_POSI_DETAIL_CHANGED, &CSManagerView::OnCustDynamicPosiDetailChanged)
	//ON_MESSAGE(WM_USER_CUST_DYNAMIC_INFO, &CSManagerView::OnUserCustDynamicInfo)
	//ON_MESSAGE(WM_USER_CUST_POSITION_FLOAT_PROFIT, &CSManagerView::OnUserCustPositionFloatProfit)
	ON_COMMAND(ID_CUST_RISK_CONTROL_SETTING, &CSManagerView::OnCustRiskControlSetting)
	//ON_COMMAND(ID_SUMMARY_FLOAT_PROFIT, OnSummaryFloatProfit)
	ON_COMMAND(ID_FP_CAPTION, &CSManagerView::OnFpCaption)
	ON_COMMAND(ID_OP_CAPTION, &CSManagerView::OnOpCaption)
	ON_COMMAND(ID_COMMISSION_CAPTION, &CSManagerView::OnCommissionCaption)
	ON_COMMAND(ID_SUMMARY_CAPTION, &CSManagerView::OnSummaryCaption)
	//ON_COMMAND(ID_MESSAGEBEE, &CSManagerView::OnMessagebee)
	//ON_UPDATE_COMMAND_UI(ID_MESSAGEBEE, &CSManagerView::OnUpdateMessagebee)
	ON_COMMAND(ID_CUST_COMMISSION_RATE_SETTING, &CSManagerView::OnCustCommissionRateSetting)
	ON_COMMAND(ID_CUST_MARGIN_RATE_SETTING, &CSManagerView::OnCustMarginRateSetting)
	ON_COMMAND(ID_BANKACCOUT, &CSManagerView::OnBankaccout)
	ON_COMMAND(ID_EXPORT_CUSTINFO, &CSManagerView::OnExportCustinfo)
END_MESSAGE_MAP()

// CSManagerView 构造/析构

CSManagerView::CSManagerView()
{
	// TODO: 在此处添加构造代码
	m_CurSel = -1;
	//m_MsgBee = false;
}

CSManagerView::~CSManagerView()
{
}

BOOL CSManagerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CSManagerView 绘制

void CSManagerView::OnDraw(CDC* /*pDC*/)
{
	CSManagerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CSManagerView 打印


//void CSManagerView::OnFilePrintPreview()
//{
//#ifndef SHARED_HANDLERS
//	AFXPrintPreview(this);
//#endif
//}

//BOOL CSManagerView::OnPreparePrinting(CPrintInfo* pInfo)
//{
//	// 默认准备
//	return DoPreparePrinting(pInfo);
//}
//
//void CSManagerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
//{
//	// TODO: 添加额外的打印前进行的初始化过程
//}
//
//void CSManagerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
//{
//	// TODO: 添加打印后进行的清理过程
//}

void CSManagerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSManagerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
	//if(g_cfg.oper_type == 0)
	//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_CUST_OPERATE, point.x, point.y, this, FALSE);
#ifdef _OPER_PROXY
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_CUST_PROXY_QUERY, point.x, point.y, this, FALSE);
#else
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_CUST_OPERATE, point.x, point.y, this, FALSE);
#endif
#endif
}


// CSManagerView 诊断

#ifdef _DEBUG
void CSManagerView::AssertValid() const
{
	CView::AssertValid();
}

void CSManagerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSManagerDoc* CSManagerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSManagerDoc)));
	return (CSManagerDoc*)m_pDocument;
}
#endif //_DEBUG


//// CSManagerView 消息处理程序
//static UINT indicators[] =
//{
//	ID_SEPARATOR,           // 状态行指示器
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
//};


int CSManagerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//CMFCListCtrl
	// TODO:  在此添加您专用的创建代码
	m_listctrl.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT, RECT(), this, IDC_LIST_CUST);
	////m_listctrl.SetExtendedStyle
	//LONG style = GetWindowLong(m_listctrl.GetSafeHwnd(), GWL_STYLE);
	////LONG style = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
	//style &= ~LVS_TYPEMASK;
	//style |= LVS_REPORT;
	//SetWindowLong(m_listctrl.GetSafeHwnd(), GWL_STYLE, style);

	//DWORD styleex = m_listctrl.GetExtendedStyle();
	//styleex |= LVS_EX_GRIDLINES;
	//styleex |= LVS_EX_DOUBLEBUFFER;
	//m_listctrl.SetExtendedStyle(styleex);
	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("未能创建状态栏\n");
	//	return -1;      // 未能创建
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_TOOLBAR_CAPITAL_SUMMARY);
	m_wndToolBar.LoadToolBar(IDR_TOOLBAR_CAPITAL_SUMMARY, 0, 0, TRUE /* 已锁定*/);
	//OnChangeVisualStyle();
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由: 
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	m_wndToolBar.ReplaceButton(ID_SUMMARY_FLOAT_PROFIT, 
		CMFCToolBarEditBoxButton(ID_SUMMARY_FLOAT_PROFIT, GetCmdMgr()->GetCmdImage(ID_SUMMARY_FLOAT_PROFIT, FALSE)));

	m_wndToolBar.ReplaceButton(ID_SUMMARY_OFFSET_PROFIT,
		CMFCToolBarEditBoxButton(ID_SUMMARY_OFFSET_PROFIT, GetCmdMgr()->GetCmdImage(ID_SUMMARY_OFFSET_PROFIT, FALSE)));

	m_wndToolBar.ReplaceButton(ID_SUMMARY_COMMISSION,
		CMFCToolBarEditBoxButton(ID_SUMMARY_COMMISSION, GetCmdMgr()->GetCmdImage(ID_SUMMARY_COMMISSION, FALSE)));
	


	LVCOLUMN cols[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "客户号" },  //0
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "客户名称" }, //1
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "状态" },  //2
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "上日结存" },//3
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "可用资金" },//4
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "动态权益" },//5
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "強平线" },//6
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "平仓盈亏" },//7
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "浮动盈亏" },//8
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "保证金" },//9
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "冻结保证金" },//10
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "手续费" },//11
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "冻结手续费" },//12
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "冻结资金" },//13
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "风险级别" },//14
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "登录" },	//15
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "货币" },//16
	};

	for (int i = 0; i < sizeof(cols) / sizeof(cols[0]); ++i)
		m_listctrl.InsertColumn(i, &cols[i]);

	return 0;
}


void CSManagerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	//m_listctrl.MoveWindow(0, 0, cx, cy, FALSE);
	CRect rect;
	GetClientRect(&rect);
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
	m_listctrl.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height() - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rect.left, rect.Height() - cyTlb, rect.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
}


void CSManagerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	

}

void CSManagerView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (!pHint)
		return;

	CViewHint *viewhint = (CViewHint*)pHint;
	std::string id = fetch_string(viewhint->identifer);
	if (lHint == WM_USER_CUST_BASE_INFO) {
		if (viewhint->type == CUST_BASE_COUNT_CHANGED) 
			HandleCustBaseInfo_CountChanged();
		else if (viewhint->type == CUST_BASE_INFO_CHANGED) 
			HandleCustBaseInfo_InfoChanged(id);
		
	}
	else if (lHint == WM_USER_CUST_DYNAMIC_INFO)
	{
		//std::string id = fetch_string(viewhint->identifer);
		if (viewhint->type == CUST_DYNAMIC_LOGIN_INFO_CHANGED) 
			HandleCustDynamicInfo_LoginInfo(id);
		else if (viewhint->type == CUST_DYNAMIC_CAPITAL)
			HandleCustDynamicInfo_Capital(id);
		//else if (viewhint->type == CUST_DYNAMIC_POSI_DETAIL_CHANGED)
		//	HandleCustDynamicInfo_PosiDetail(id);
		else if (viewhint->type == CUST_DYNAMIC_POSITION_CHANGED)
			HandleCustDynamicInfo_Position(id);
	}
	else if (lHint == WM_USER_INST_MARKET)
	{
		HandleInstMarket(id);
	}
	//else if (lHint == WM_USER_CUST_POSITION_FLOAT_PROFIT)
	//{
	//	cust_position_id_st posi_id;
	//	std::istringstream iss(fetch_string(viewhint->type));
	//	iss >> posi_id;
	//	std::vector<cust_position_st>::iterator it;
	//	std::map<std::string, std::vector<cust_position_st>>::iterator mit = m_mCustPosition.find(posi_id.cust_no);
	//	if (mit != m_mCustPosition.end())
	//	{
	//		it = std::find(mit->second.begin(), mit->second.end(), posi_id);
	//		if (it != mit->second.end())
	//		{
	//			it->float_profit = (viewhint->identifer / 100.0);
	//		}
	//	}
	//}
}

void CSManagerView::OnNMDblclkListCust(NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	int nItem = pNMItemActivate->iItem;
	if (m_CurSel != nItem)
	{
		if (nItem >= 0 && nItem < m_listctrl.GetItemCount())
		{
			CString cust_no = m_listctrl.GetItemText(nItem, 0);
			::PostMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(),
				WM_USER_CUST_SEL_CHANGED,
				0, get_identifer(cust_no.GetBuffer()));
		}
		m_CurSel = nItem;
	}
	
	*pResult = 0;
}

//bool CSManagerView::isInstInCustPosition(const std::string & inst, const std::string & cust_no)
//{
//	std::map<std::string, std::vector<cust_td_position_detail_st>>::iterator it = m_mCustPosiDetail.find(cust_no);
//	if (it != m_mCustPosiDetail.end())
//	{
//		for each (cust_td_position_detail_st var in it->second)
//		{
//			if (inst == boost::trim_copy(std::string(var.instrumentid)))
//				return true;
//		}
//	}
//	return false;
//}

//bool CSManagerView::canCalcCustFloatProfit(const std::string & cust_no)
//{
//	std::map<std::string, std::vector<cust_td_position_detail_st>>::iterator cpditer;
//	//std::map<std::string, depth_market_data_st>::iterator dmditer;
//
//	//客户权益数据不存丰，不能计算
//	if (m_mCapital.find(cust_no) == m_mCapital.end())
//		return false;
//
//	//客户持仓明细数据不存在，不能计算
//	cpditer = m_mCustPosiDetail.find(cust_no);
//	if(cpditer == m_mCustPosiDetail.end())
//		return false;
//
//	//客户持仓的合约的行情数据不存在，不能计算
//	for each (cust_td_position_detail_st var in cpditer->second)
//	{
//		std::string inst = boost::trim_copy(std::string(var.instrumentid));
//		if (m_mDmd.find(inst) == m_mDmd.end())
//			return false;
//	}
//
//	return true;
//}

//double CSManagerView::CalcCustFloatProfit(const std::string & cust_no)
//{
//	double profit = 0;
//	//const system_info_st &si = cust_dynamic_info_worker::instance()->get_sysinfo();
//	for each (cust_td_position_detail_st var in m_mCustPosiDetail[cust_no])
//	{
//		
//		std::string inst = boost::trim_copy(std::string(var.instrumentid));
//		depth_market_data_st &dmd = m_mDmd[inst];
//		if (strcmp(var.open_date, var.trade_date) == 0) //今仓
//		{
//			if (strcmp(var.direction, "0") == 0) //买
//			{
//				profit += (dmd.LastPrice - var.hold_price) * var.volume * var.multiple;
//			}
//			else if (strcmp(var.direction, "1") == 0) //卖
//			{
//				profit += (var.hold_price - dmd.LastPrice) * var.volume * var.multiple;
//			}
//		}
//		else //昨仓
//		{
//			if (strcmp(var.direction, "0") == 0) //买
//			{
//				profit += (dmd.LastPrice - dmd.PreSettlementPrice) * var.volume * var.multiple;
//			}
//			else if (strcmp(var.direction, "1") == 0) //卖
//			{
//				profit += (dmd.PreSettlementPrice - dmd.LastPrice) * var.volume * var.multiple;
//			}
//		}
//	}
//
//	for each (cust_position_st var in m_mCustPosition[cust_no])
//	{
//		profit += var.float_profit;
//	}
//	return profit;
//}

void CSManagerView::AdjustCustCapital(const std::string & cust_no, double float_profit)
{
	if (m_mCustPosition.find(cust_no) == m_mCustPosition.end())
		return;

	std::map<std::string, cust_capital_st>::iterator it = m_mCapital.find(cust_no);
	if (it == m_mCapital.end())
		return;

	for each (cust_position_st var in m_mCustPosition[cust_no])
	{
		float_profit += var.float_profit;
	}
	//if (float_profit == 0)
	//	return;
	it->second.available += float_profit - it->second.float_profit;
	it->second.dynamic_capital += float_profit - it->second.float_profit;
	it->second.float_profit = float_profit;

	for (int i = 0; i < m_listctrl.GetItemCount(); ++i)
	{
		CString text_cust = m_listctrl.GetItemText(i, 0);
		if (cust_no != text_cust.GetBuffer())
			continue;
		m_listctrl.SetItemText(i, 4, fundfmt(it->second.available));
		m_listctrl.SetItemText(i, 5, fundfmt(it->second.dynamic_capital));

		COLORREF clr;
		if (it->second.float_profit > 0)
			clr = COLOR_RED;
		else if (it->second.float_profit < 0)
			clr = COLOR_GREEN;
		else
			clr = COLOR_BLACK;
		m_listctrl.SetCellTextColor(i, 8, clr);
		m_listctrl.SetItemText(i, 8, fundfmt(it->second.float_profit));
	}
	
	DisplaySummary();
}

void CSManagerView::ResetListCustBaseInfo(const cust_base_info_st &cbi)
{
	for (int i = 0; i < m_listctrl.GetItemCount(); i++)
	{
		CString item_cust_no = m_listctrl.GetItemText(i, 0);
		if (item_cust_no != cbi.basic.scust_no)
			continue;

		COLORREF clr;
		int fw = FW_NORMAL;

		m_listctrl.SetItemText(i, 1, cbi.basic.sname);	
		if (strcmp(cbi.basic.sstatus0, "0") == 0) {
			clr = COLOR_BLACK;
			fw = FW_NORMAL;
		}
		else if (strcmp(cbi.basic.sstatus0, "4") == 0) {
			clr = COLOR_RED;
			fw = FW_MEDIUM;
		}
		else if (strcmp(cbi.basic.sstatus0, "5") == 0) {
			clr = COLOR_RED;
			fw = FW_SEMIBOLD;
		}
		else if (strcmp(cbi.basic.sstatus0, "6") == 0) {
			clr = COLOR_RED;
			fw = FW_HEAVY;
		}
		m_listctrl.SetCellTextColor(i, 2, clr);
		m_listctrl.SetCellFontWeigth(i, 2, fw);
		m_listctrl.SetItemText(i, 2, theApp.DictToStr(DICT_CLASS_CUST_STATUS, cbi.basic.sstatus0));

		std::vector<rc_desc_st>::const_iterator it = std::find_if(cbi.rcs.begin(), cbi.rcs.end(),
			[](const rc_desc_st &desc)->bool { return strcmp(desc.classflag, "0002") == 0; });
		if (it != cbi.rcs.end())
		{
			m_listctrl.SetItemText(i, 6, it->paramlist);
		}
		break;
	}
}

void CSManagerView::HandleCustBaseInfo_CountChanged()
{
	m_listctrl.DeleteAllItems();
	m_CurCBIs = theApp.GetCustBaseInfoList();

	for (int i = 0; i < m_CurCBIs.size(); i++)
	{
		m_listctrl.InsertItem(i, m_CurCBIs[i].basic.scust_no);
		ResetListCustBaseInfo(m_CurCBIs[i]);
		HandleCustDynamicInfo_LoginInfo(m_CurCBIs[i].basic.scust_no);
		HandleCustDynamicInfo_Capital(m_CurCBIs[i].basic.scust_no);
		HandleCustDynamicInfo_Position(m_CurCBIs[i].basic.scust_no);
	}
}

void CSManagerView::HandleCustBaseInfo_InfoChanged(const std::string & cust_no)
{
	for (cbi_iter_t it = m_CurCBIs.begin(); it != m_CurCBIs.end(); ++it)
	{
		if (cust_no == it->basic.scust_no)
		{
			*it = theApp.GetCustBaseInfo(it->basic.scust_no);
			ResetListCustBaseInfo(*it);
			break;
		}
	}

}

void CSManagerView::HandleCustDynamicInfo_LoginInfo(const std::string & cust_no)
{
	cust_login_info_st &cur_li = m_mLoginInfo[cust_no];
	const cust_login_info_st &new_li = cust_dynamic_info_worker::instance()->get_cust_login_info(cust_no);
	
	for (int i = 0; i < m_CurCBIs.size(); i++)
	{
		if (cust_no == m_CurCBIs[i].basic.scust_no) 
		{
			if (strcmp(cur_li.login_status, new_li.login_status))
			{
				if (new_li.login_status[0] == '1')
				{
					m_listctrl.SetCellTextColor(i, 14, COLOR_GREEN);
					m_listctrl.SetItemText(i, 14, "登入");
				}
				else
				{
					m_listctrl.SetCellTextColor(i, 13, COLOR_BLACK);
					m_listctrl.SetItemText(i, 14, "登出");
				}
				
			}

			cur_li = new_li;
			break;
		}
		
	}
}

void CSManagerView::HandleCustDynamicInfo_Capital(const std::string & cust_no)
{
	cust_capital_st &cur_cap = m_mCapital[cust_no];
	const cust_capital_st &new_cap = cust_dynamic_info_worker::instance()->get_cust_capital(cust_no);
	for (int i = 0; i < m_CurCBIs.size(); i++)
	{
		//CString str;
		if (cust_no == m_CurCBIs[i].basic.scust_no)
		{
			//COLORREF clr;
			//int fw;
			//if (cur_cap.yd_balance != new_cap.yd_balance) {
			//str.Format("%f", new_cap.yd_balance);
			m_listctrl.SetItemText(i, 3, fundfmt(new_cap.yd_balance));
			//}
			//if (cur_cap.available != new_cap.available) {
			//str.Format("%f", new_cap.available);
			m_listctrl.SetItemText(i, 4, fundfmt(new_cap.available));
			//}
			//if (cur_cap.dynamic_capital != new_cap.dynamic_capital) {
			//str.Format("%f", new_cap.dynamic_capital);
			m_listctrl.SetItemText(i, 5, fundfmt(new_cap.dynamic_capital));
			//}
			//if (cur_cap.offset_profit != new_cap.offset_profit) {
			if (new_cap.offset_profit > 0) {
				m_listctrl.SetCellTextColor(i, 7, COLOR_RED);
			}
			else if (new_cap.offset_profit < 0) {
				m_listctrl.SetCellTextColor(i, 7, COLOR_GREEN);
			}
			m_listctrl.SetCellFontWeigth(i, 7, FW_HEAVY);
			//str.Format("%f", new_cap.offset_profit);
			m_listctrl.SetItemText(i, 7, fundfmt(new_cap.offset_profit));
			//}
			//if (cur_cap.float_profit != new_cap.float_profit) {
			if (new_cap.float_profit > 0)
				m_listctrl.SetCellTextColor(i, 8, COLOR_RED);
			else if (new_cap.float_profit < 0)
				m_listctrl.SetCellTextColor(i, 8, COLOR_GREEN);
			m_listctrl.SetCellFontWeigth(i, 8, FW_HEAVY);
			//str.Format("%f", new_cap.float_profit);
			m_listctrl.SetItemText(i, 8, fundfmt(new_cap.float_profit));
			//}

			//if (cur_cap.margin != new_cap.margin) {
			//str.Format("%f", new_cap.margin);
			m_listctrl.SetItemText(i, 9, fundfmt(new_cap.margin));
			//}
			//if (cur_cap.buy_frzn_margin != new_cap.buy_frzn_margin || cur_cap.sell_frzn_margin != new_cap.sell_frzn_margin) {
			//str.Format("%f", new_cap.buy_frzn_margin + new_cap.sell_frzn_margin);
			m_listctrl.SetItemText(i, 10, fundfmt(new_cap.buy_frzn_margin + new_cap.sell_frzn_margin));
			//}
			//if (cur_cap.commission != new_cap.commission) {
			//str.Format("%f", new_cap.commission);
			m_listctrl.SetItemText(i, 11, fundfmt(new_cap.commission));
			//}
			//if (cur_cap.frzn_commission != new_cap.frzn_moeny) {
			//str.Format("%f", new_cap.frzn_commission);
			m_listctrl.SetItemText(i, 12, fundfmt(new_cap.frzn_commission));
			//}
			//str.Format("%f", new_cap.frzn_moeny);
			m_listctrl.SetItemText(i, 13, fundfmt(new_cap.frzn_moeny));

			//if (new_cap.risk_level[0] == '0') {
			//	m_listctrl.SetCellTextColor(i, 13, COLOR_BLACK);
			//	m_listctrl.SetCellFontWeigth(i, 13, FW_NORMAL);
			//}
			//else if (new_cap.risk_level[0] == '1') {
			//	m_listctrl.SetCellTextColor(i, 13, COLOR_BLUE);
			//	m_listctrl.SetCellFontWeigth(i, 13, FW_MEDIUM);
			//}else if (new_cap.risk_level[0] == '2') {
			//	m_listctrl.SetCellTextColor(i, 13, COLOR_RED);
			//	m_listctrl.SetCellFontWeigth(i, 13, FW_BOLD);
			//}else if (new_cap.risk_level[0] == '3') {
			//	m_listctrl.SetCellTextColor(i, 13, COLOR_RED);
			//	m_listctrl.SetCellFontWeigth(i, 13, FW_HEAVY);
			//}
			//m_listctrl.SetItemText(i, 13, theApp.DictToStr(DICT_CLASS_RISK_LEVEL, new_cap.risk_level));

			m_listctrl.SetItemText(i, 15, theApp.DictToStr(DICT_CLASS_CURRENCY, new_cap.scurrency_type));
			cur_cap = new_cap;
			break;
		}

	}
	DisplaySummary();
}

//void CSManagerView::_HandleCustDynamicInfo_Capital(const std::string & cust_no)
//{
//}

//void CSManagerView::HandleCustDynamicInfo_PosiDetail(const std::string & cust_no)
//{
//	m_mCustPosiDetail[cust_no] = cust_dynamic_info_worker::instance()->get_cust_posi_detail(cust_no);
//}

void CSManagerView::HandleCustDynamicInfo_Position(const std::string & cust_no)
{
	m_mCustPosition[cust_no] = cust_dynamic_info_worker::instance()->get_cust_position(cust_no);
	AdjustCustCapital(cust_no, 0.0);
	//DisplaySummary();
}

void CSManagerView::HandleInstMarket(const std::string & inst)
{
	//std::map<std::string, std::vector<cust_td_position_detail_st>>::iterator it;
	depth_market_data_st dmd = MdApi_GetMarketData(inst);
	//m_mDmd[inst] = MdApi_GetMarketData(inst);

	//for each (cust_base_info_st var in m_CurCBIs)
	//{
	//	if (!isInstInCustPosition(inst, var.basic.scust_no))
	//		continue;
	//	if (!canCalcCustFloatProfit(var.basic.scust_no))
	//		continue;
	//	double profit = CalcCustFloatProfit(var.basic.scust_no);
	//	AdjustCustCapital(var.basic.scust_no, profit);
	//	//HandleCustDynamicInfo_Capital(var.basic.scust_no);
	//}
	for (CustPosi_t::iterator mit = m_mCustPosition.begin(); mit != m_mCustPosition.end(); ++mit)
	{
		bool adjust = false;
		for (PosiVec_t::iterator vit = mit->second.begin(); vit != mit->second.end(); ++vit)
		{
			if (vit->instrumentid == inst && vit->last_price != dmd.LastPrice)
			{
				vit->float_profit = calcProfit(*vit, dmd.LastPrice);
				vit->last_price = dmd.LastPrice;
				adjust = true;
			}		
		}
		if (adjust)
			AdjustCustCapital(mit->first, 0.0);
	}
}

//void CSManagerView::HnadleCustPositionFloatProfit(const cust_position_id_st & posi_id, double profit)
//{
//	std::vector<cust_position_st>::iterator it;
//	std::map<std::string, std::vector<cust_position_st>>::iterator mit = m_mCustPosition.find(posi_id.cust_no);
//	if (mit != m_mCustPosition.end())
//	{
//		it = std::find(mit->second.begin(), mit->second.end(), posi_id);
//		if (it != mit->second.end())
//		{
//			it->float_profit = profit;
//			AdjustCustCapital(posi_id.cust_no, 0.0);
//		}
//	}
//}


void CSManagerView::OnMoneyInOut()
{
	// TODO: 在此添加命令处理程序代码
	
	CCustMoneyInOutDialog dlg;
	int sel = m_listctrl.GetSelectionMark();
	if (sel >= 0 && sel < m_listctrl.GetItemCount())
	{
		CString cust_no = m_listctrl.GetItemText(sel, 0);
		dlg.m_ctlCust.SelectKey(cust_no);
	}
	dlg.DoModal();
}

cbi_iter_t CSManagerView::FindCurCBI(CString cust_no)
{
	cbi_iter_t it;
	for (it = m_CurCBIs.begin(); it != m_CurCBIs.end(); ++it)
	{
		if (cust_no == it->basic.scust_no) break;
	}
	return it;
}

//void CSManagerView::OnFrozenCust()
//{
//	// TODO: 在此添加命令处理程序代码
//	int sel = m_listctrl.GetSelectionMark();
//	if (sel >= 0 && sel < m_listctrl.GetItemCount())
//	{
//		CString cust_no = m_listctrl.GetItemText(sel, 0);
//		cust_base_info_st cbi = theApp.GetCustBaseInfo(cust_no);
//		if (strcmp(cbi.sstatus0, "6") == 0)
//			return;
//		Scoped_BCHANDLE handle;
//		BCResetHandle(handle);
//		BCSetRequestType(handle, 851005);
//		BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
//		BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no.GetBuffer());
//		BCSetStringFieldByName(handle, 0, "sstatus1", "6");
//		if (MyBCRequest(handle, FetchRowFunc_t(), true))
//		{
//			strcpy(cbi.sstatus0, "6");
//			theApp.SetCustBaseInfo(cbi);
//		}
//	}
//	
//}


void CSManagerView::OnCustStatusMaintain()
{
	// TODO: 在此添加命令处理程序代码
	CCustStatusMaintainDialog dlg;
	int sel = m_listctrl.GetSelectionMark();
	if (sel >= 0 && sel < m_listctrl.GetItemCount())
	{
		CString cust_no = m_listctrl.GetItemText(sel, 0);
		dlg.m_ctlCust.SelectKey(cust_no);
	}
	dlg.DoModal();
}


void CSManagerView::OnOrderQuery()
{
	// TODO: 在此添加命令处理程序代码
	CQryOrderDialg dlg;
	int sel = m_listctrl.GetSelectionMark();
	if (sel >= 0 && sel < m_listctrl.GetItemCount())
	{
		CString cust_no = m_listctrl.GetItemText(sel, 0);
		dlg.m_ctlCust.SelectKey(cust_no);
	}
	dlg.DoModal();
}


void CSManagerView::OnPositionQuery()
{
	// TODO: 在此添加命令处理程序代码
	CQryPositionDialog dlg;
	int sel = m_listctrl.GetSelectionMark();
	if (sel >= 0 && sel < m_listctrl.GetItemCount())
	{
		CString cust_no = m_listctrl.GetItemText(sel, 0);
		dlg.m_ctlCust.SelectKey(cust_no);
	}
	dlg.DoModal();
}


void CSManagerView::OnTradeQuery()
{
	// TODO: 在此添加命令处理程序代码
	CQryTradeDialog dlg;
	int sel = m_listctrl.GetSelectionMark();
	if (sel >= 0 && sel < m_listctrl.GetItemCount())
	{
		CString cust_no = m_listctrl.GetItemText(sel, 0);
		dlg.m_ctlCust.SelectKey(cust_no);
	}
	dlg.DoModal();
}


void CSManagerView::OnQueryMoneyInOut()
{
	// TODO: 在此添加命令处理程序代码
	CQryMoneyInOutDialog dlg;
	int sel = m_listctrl.GetSelectionMark();
	if (sel >= 0 && sel < m_listctrl.GetItemCount())
	{
		CString cust_no = m_listctrl.GetItemText(sel, 0);
		dlg.m_ctlCust.SelectKey(cust_no);
	}
	dlg.DoModal();
}


void CSManagerView::OnSettlementQuery()
{
	// TODO: 在此添加命令处理程序代码
	CQrySettlementDialog dlg;
	int sel = m_listctrl.GetSelectionMark();
	if (sel >= 0 && sel < m_listctrl.GetItemCount())
	{
		CString cust_no = m_listctrl.GetItemText(sel, 0);
		dlg.m_ctlCust.SelectKey(cust_no);
	}
	dlg.DoModal();
}


void CSManagerView::OnResetTradePasswd()
{
	// TODO: 在此添加命令处理程序代码
	CResetCustTradePasswdDialog dlg;
	int sel = m_listctrl.GetSelectionMark();
	if (sel >= 0 && sel < m_listctrl.GetItemCount())
	{
		CString cust_no = m_listctrl.GetItemText(sel, 0);
		dlg.m_ctlCust.SelectKey(cust_no);
	}
	dlg.DoModal();
}


void CSManagerView::OnResetFundPasswd()
{
	// TODO: 在此添加命令处理程序代码
	CResetCustFundPasswdDialog dlg;
	int sel = m_listctrl.GetSelectionMark();
	if (sel >= 0 && sel < m_listctrl.GetItemCount())
	{
		CString cust_no = m_listctrl.GetItemText(sel, 0);
		dlg.m_ctlCust.SelectKey(cust_no);
	}
	dlg.DoModal();
}

bool CSManagerView::RequestCancelCust_851018(CString cust_no)
{
	Scoped_BCHANDLE handle;
	BCSetRequestType(handle, 851018);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no.GetBuffer());
	BCSetStringFieldByName(handle, 0, "sstatus0", "1");

	BCResult ret = MyBCRequest(handle, FetchRowFunc_t(), BCMSG_ERR_TO_OP);
	if (ret.errcode == 5108003 || ret.errcode == 0)
	{
		MessageBox("客户销户成功");
		return true;
	}
	else
	{
		MessageBox(ret.errmsg);
		return false;
	}
}


void CSManagerView::OnCancelCust()
{
	// TODO: 在此添加命令处理程序代码
	int sel = m_listctrl.GetSelectionMark();
	if (sel < 0 || sel >= m_listctrl.GetItemCount())
	{
		MessageBox("请单击选中客户");
		return;
	}

	CString cust_no = m_listctrl.GetItemText(sel, 0);
	
	CString text;
	text.Format("确认要对%s销户吗？", cust_no);
	if (MessageBox(text, "提示", MB_YESNO) == IDNO)
		return;
	cust_base_info_st cbi = theApp.GetCustBaseInfo(cust_no.GetBuffer());
	assert(cust_no == cbi.basic.scust_no);
	if (RequestCancelCust_851018(cust_no))
	{
		//strcpy(cbi.sstatus0, "7");
		//theApp.SetCustBaseInfo(cbi);
		theApp.DelCust(cbi.basic.scust_no);
	}
}


void CSManagerView::OnCustTemplateSetting()
{
	// TODO: 在此添加命令处理程序代码
	CCustTemplateSettingDialog dlg;
	int sel = m_listctrl.GetSelectionMark();
	if (sel >= 0 && sel < m_listctrl.GetItemCount())
	{
		CString cust_no = m_listctrl.GetItemText(sel, 0);
		dlg.m_ctlCust.SelectKey(cust_no);
	}
	dlg.DoModal();
}


//afx_msg LRESULT CSManagerView::OnCustDynamicPosiDetailChanged(WPARAM wParam, LPARAM lParam)
//{
//	//std::string cust_no = fetch_string(lParam)
//	return 0;
//}


//afx_msg LRESULT CSManagerView::OnUserCustDynamicInfo(WPARAM wParam, LPARAM lParam)
//{
//	if (wParam == CUST_DYNAMIC_POSI_DETAIL_CHANGED)
//	{
//		//std::string cust_no = fetch_string(lP)
//	}
//	return 0;
//}


//afx_msg LRESULT CSManagerView::OnUserCustPositionFloatProfit(WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}


void CSManagerView::OnCustRiskControlSetting()
{
	// TODO: 在此添加命令处理程序代码
	CCustRiskControlSettingDialog dlg;
	int sel = m_listctrl.GetSelectionMark();
	if (sel >= 0 && sel < m_listctrl.GetItemCount())
	{
		CString cust_no = m_listctrl.GetItemText(sel, 0);
		dlg.m_ctlCust.SelectKey(cust_no);
	}
	dlg.DoModal();
}

void CSManagerView::DisplaySummary()
{
	double s_float_profit = 0.0, s_offset_profit = 0.0, s_commission = 0.0;
	for each (auto var in m_mCapital)
	{
		s_float_profit += var.second.float_profit;
		s_offset_profit += var.second.offset_profit;
		s_commission += var.second.commission;
	}
	
	CMFCToolBarEditBoxButton* pSrcEdit = CMFCToolBarEditBoxButton::GetByCmd(ID_SUMMARY_FLOAT_PROFIT);
	if (pSrcEdit)
	{
		pSrcEdit->SetContents(fundfmt(s_float_profit));
		//m_wndToolBar.ReplaceButton(ID_SUMMARY_FLOAT_PROFIT, *pSrcEdit);
	}

	CMFCToolBarEditBoxButton *pSrcEdit2 = CMFCToolBarEditBoxButton::GetByCmd(ID_SUMMARY_OFFSET_PROFIT);
	if (pSrcEdit2)
	{
		pSrcEdit2->SetContents(fundfmt(s_offset_profit));
		//m_wndToolBar.ReplaceButton(ID_SUMMARY_OFFSET_PROFIT, *pSrcEdit2);
	}
	
	CMFCToolBarEditBoxButton *pSrcEdit3 = CMFCToolBarEditBoxButton::GetByCmd(ID_SUMMARY_COMMISSION);
	if (pSrcEdit3)
	{
		pSrcEdit3->SetContents(fundfmt(s_commission));
	}

}


void CSManagerView::OnFpCaption()
{
	// TODO: 在此添加命令处理程序代码
}


void CSManagerView::OnOpCaption()
{
	// TODO: 在此添加命令处理程序代码
}


void CSManagerView::OnCommissionCaption()
{
	// TODO: 在此添加命令处理程序代码
}

void CSManagerView::OnSummaryCaption()
{
	// TODO: 在此添加命令处理程序代码
}


//void CSManagerView::OnMessagebee()
//{
//	// TODO: 在此添加命令处理程序代码
//	m_MsgBee = !m_MsgBee;
//}
//
//
//void CSManagerView::OnUpdateMessagebee(CCmdUI *pCmdUI)
//{
//	// TODO: 在此添加命令更新用户界面处理程序代码
//	//pCmdUI->
//	pCmdUI->SetCheck(m_MsgBee);
//}


void CSManagerView::OnCustCommissionRateSetting()
{
	// TODO: 在此添加命令处理程序代码
	CCustCommissionDialog dlg;
	int sel = m_listctrl.GetSelectionMark();
	if (sel >= 0 && sel < m_listctrl.GetItemCount())
	{
		CString cust_no = m_listctrl.GetItemText(sel, 0);
		dlg.m_ctlCust.SelectKey(cust_no);
	}
	dlg.DoModal();
}


void CSManagerView::OnCustMarginRateSetting()
{
	// TODO: 在此添加命令处理程序代码
	CCustMarginDialog dlg;
	int sel = m_listctrl.GetSelectionMark();
	if (sel >= 0 && sel < m_listctrl.GetItemCount())
	{
		CString cust_no = m_listctrl.GetItemText(sel, 0);
		dlg.m_ctlCust.SelectKey(cust_no);
	}
	dlg.DoModal();
}


void CSManagerView::OnBankaccout()
{
	// TODO: 在此添加命令处理程序代码
	CBankAccountMgrDialog dlg;
	int sel = m_listctrl.GetSelectionMark();
	if (sel >= 0 && sel < m_listctrl.GetItemCount())
	{
		CString cust_no = m_listctrl.GetItemText(sel, 0);
		dlg.m_ctlCust.SelectKey(cust_no);
	}
	dlg.DoModal();
}


void CSManagerView::OnExportCustinfo()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(FALSE, "txt", NULL, OFN_OVERWRITEPROMPT, NULL, this);
	if (dlg.DoModal() != IDOK)
		return;

	CString path = dlg.GetPathName();
	std::ofstream ofs(path.GetBuffer());
	if (!ofs.good())
	{
		MessageBox("打开文件失败");
		return;
	}

	int rows = m_listctrl.GetItemCount();
	int cols = m_listctrl.GetHeaderCtrl().GetItemCount();
	for (int i = 0; i < cols; i++)
	{
		HDITEM   hdi;
		char     lpBuffer[256];
		LPCTSTR   lpszmyString;
		hdi.mask = HDI_TEXT;
		hdi.pszText = lpBuffer;
		hdi.cchTextMax = 256;
		m_listctrl.GetHeaderCtrl().GetItem(i, &hdi);
		ofs << std::left << std::setw(10) << lpBuffer << ',';
	}
	ofs << std::endl;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			CString str = m_listctrl.GetItemText(i, j);
			ofs << std::left << std::setw(10) << str.GetBuffer() << ',';
		}
		ofs << std::endl;
	}

	ofs.close();
}
