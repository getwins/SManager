// TradeWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "TradeWnd.h"
#include "string_identifer.h"
#include <algorithm>
#include "SMutility.h"
#include "CommBCRequest.h"
// CTradeWnd


IMPLEMENT_DYNAMIC(CTradeWnd, CDockablePane)

CTradeWnd::CTradeWnd()
	: m_pCustProp(NULL)
	, m_pInstProp(NULL)
	, m_pDirectionProp(NULL)
	, m_pOffsetProp(NULL)
	, m_pVolProp(NULL)
	, m_pPriceProp(NULL)
{

}

CTradeWnd::~CTradeWnd()
{
}



BEGIN_MESSAGE_MAP(CTradeWnd, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_MESSAGE(WM_USER_CUST_SEL_CHANGED, &CTradeWnd::OnUserCustSelChanged)
	//ON_BN_CLICKED(IDC_BUTTON_ORDER, &CTradeWnd::OnBnClickedButtonOrder)
#ifndef  _OPER_PROXY
	ON_COMMAND(IDC_BUTTON_ORDER, &CTradeWnd::OnBnClickedButtonOrder)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_ORDER, &OnUpdateButtonUI)
#endif
	//ON_MESSAGE(WM_USER_REQUEST_INST_DONE, &CTradeWnd::OnUserRequestInstDone)
	//ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, &CTradeWnd::OnPropertyChanged)
END_MESSAGE_MAP()

void CTradeWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	//m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty *pGroup1 = new CMFCPropertyGridProperty(_T("委托"));
	//CMFCPropertyGridProperty *pProp;

	m_pCustProp = new CMFCPropertyGridProperty(_T("账户"), _T("客户号"), "双击客户权益列表选择要交易的客户");
	m_pCustProp->AllowEdit(FALSE);
	pGroup1->AddSubItem(m_pCustProp);

	m_pInstProp = new CMFCPropertyGridProperty(_T("合约"), "", "请输入要委托的合约");
	//for each (instrument_st inst in theApp.m_BaseInfo.instruments)
	//{
	//	m_pInstProp->AddOption(CString(inst.product_name) + "-" + CString(inst.instrumentid));
	//}
	//m_pInstProp->SetData(reinterpret_cast<int>(m_pInstProp));
	//m_pInstProp->AllowEdit(FALSE);
	pGroup1->AddSubItem(m_pInstProp);

	m_pDirectionProp = new CMFCPropertyGridProperty("方向", "0-买入", "请在下拉列表中选择委托方向");
	m_pDirectionProp->AddOption("0-买入");
	m_pDirectionProp->AddOption("1-卖出");
	m_pDirectionProp->AllowEdit(FALSE);
	pGroup1->AddSubItem(m_pDirectionProp);

	m_pOffsetProp = new CMFCPropertyGridProperty("开平", "0-开仓", "请在下拉列表中选择开平标志");
	m_pOffsetProp->AllowEdit(FALSE);
	m_pOffsetProp->AddOption("0-开仓");
	m_pOffsetProp->AddOption("1-平仓");
	m_pOffsetProp->AddOption("3-平今");
	pGroup1->AddSubItem(m_pOffsetProp);

	m_pVolProp = new CMFCPropertyGridProperty("手数", (_variant_t)1, "调整委托手数");
	//m_pVolProp->EnableSpinControl(TRUE, 0, 0);
	pGroup1->AddSubItem(m_pVolProp);

	m_pPriceProp = new CMFCPropertyGridProperty("价格", (_variant_t)0, "请输入委托价格");
	//m_pPriceProp->EnableSpinControl(TRUE, 0, 0);
	pGroup1->AddSubItem(m_pPriceProp);

	m_wndPropList.AddProperty(pGroup1);
}

void CTradeWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	//m_wndObjectCombo.SetFont(&m_fntPropList);
}

void CTradeWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	m_wndButton.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 40, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + 41, rectClient.Width(), rectClient.Height() - 41, SWP_NOACTIVATE | SWP_NOZORDER);
}


// CTradeWnd 消息处理程序




void CTradeWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	AdjustLayout();
}


int CTradeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndButton.Create("下单", WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_BUTTON_ORDER))
	{
		TRACE0("未能创建交易面板下单按钮\n");
		return -1;      // 未能创建
	}

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_PROPERTYGRID_ORDER))
	{
		TRACE0("未能创建交易面板属性网格\n");
		return -1;      // 未能创建
	}

	InitPropList();
	AdjustLayout();
	return 0;
}


afx_msg LRESULT CTradeWnd::OnUserCustSelChanged(WPARAM wParam, LPARAM lParam)
{
	std::string cust_no = fetch_string(lParam);
	if (m_CurCustNo == cust_no)
		return 0;
	m_CurCustNo = cust_no;
	cust_base_info_st cbi = theApp.GetCustBaseInfo(m_CurCustNo.c_str());
	m_pCustProp->SetValue(cbi.basic.scust_no);
	return 0;
}

//scust_no = '', 操作员
//sholder_ac_no = '', 客户号
//sholder_ac_no2 = '', 交易编码
//smarket_code = '', 交易所代码
//sstatus2 = '', 买卖方向
//scust_auth = '', 合约号
//lvol0 = '', 申报手数
//damt0 = '', 申报价格
//sstatus3 = '', 开平标记
//sstatus4 = '', 投保标记
//sserial2 = '', 席位号
//smain_flag2 = '', 下单人类型
//sholder_type = '', 委托方式
//scurrency_type = '', 币种
//sstatus0 = '', 强平标志
//scust_type = '', 定单类型
//scust_type2 = '', 成交属性
//damt3 = '', 止损止盈价
//sstation1 = '', 私有信息
//semp_no = '', 私有标识
//scust_limit = '', 私有流水号
//sstatus1 = '', 委托单类型
//smarket_code2 = '', 拆分平今单标记
//lvol8 = '', 最小成交数量
void CTradeWnd::OnBnClickedButtonOrder()
{
	Scoped_BCHANDLE handle;
	//BCResetHandle(handle)
	//BCSetRequestType(handle, 854002);
	if (m_CurCustNo.empty())
		return;

	//hyphenstr hs_inst(m_pInstProp->GetValue());
	instrument_st tmp = { 0 };
	CString inst = m_pInstProp->GetValue();
	strcpy(tmp.instrumentid, inst);
	auto inst_it = std::lower_bound(theApp.m_BaseInfo.instruments.begin(), theApp.m_BaseInfo.instruments.end(), tmp);
	if (inst_it == theApp.m_BaseInfo.instruments.end() || strcmp(tmp.instrumentid, inst_it->instrumentid)) {
		MessageBox(inst + "合约不存在");
		return;
	}

	//BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	//BCSetStringFieldByName(handle, 0, "sholder_ac_no", (char*)m_CurCustNo.c_str());
	//BCSetStringFieldByName(handle, 0, "smarket_code", inst_it->exchangeid);

	hyphenstr hs_dir(m_pDirectionProp->GetValue());
	//BCSetStringFieldByName(handle, 0, "sstatus2", hs_dir.left.GetBuffer());
	//BCSetStringFieldByName(handle, 0, "scust_auth", inst_it->instrumentid);

	CString vol = m_pVolProp->GetValue();
	//BCSetStringFieldByName(handle, 0, "lvol0", vol.GetBuffer());
	CString price = m_pPriceProp->GetValue();
	//BCSetStringFieldByName(handle, 0, "damt0", price.GetBuffer());
	hyphenstr hs_offset(m_pOffsetProp->GetValue());
	//BCSetStringFieldByName(handle, 0, "sstatus3", hs_offset.left.GetBuffer());//开平
	//BCSetStringFieldByName(handle, 0, "sstatus4", "0");//投保
	//BCSetStringFieldByName(handle, 0, "smain_flag2", "1");//下单人类型
	//BCSetStringFieldByName(handle, 0, "sholder_type", "1");//委托方式 柜台委托
	//BCSetStringFieldByName(handle, 0, "scurrency_type", inst_it->currency_type);//scurrency_type = '', 币种
	//BCSetStringFieldByName(handle, 0, "sstatus0", "0");//强平
	//BCSetStringFieldByName(handle, 0, "scust_type", "0");//定单类型 限价
	//BCSetStringFieldByName(handle, 0, "scust_type2", "0");//成交属性 //'0', 'GFD当日有效'
	//BCSetStringFieldByName(handle, 0, "sstatus1", "A");	//sstatus1 = '', 委托单类型
	//BCSetIntFieldByName(handle, 0, "lvol8", inst_it->min_lots);//lvol8 = '', 最小成交数量

	

	CString str = CString(m_CurCustNo.c_str()) + "|" + hs_dir.right + "|" + vol + "|" + price + "|";
	if (BCRequestOrderInsert_854002(handle, (char*)m_CurCustNo.c_str(), inst_it->instrumentid, inst_it->exchangeid, hs_offset.left.GetBuffer(),
		hs_dir.left.GetBuffer(), atof(price), atoi(vol)))
	{
		MessageBox(str + "\r\n委托成功");
		PostOutputMsg((str + "委托成功").GetBuffer());
	}
	//else
	//{
	//	PostOutputMsg((str + "委托失败").GetBuffer());
	//}

	//if (MyBCRequest(handle, FetchRowFunc_t(), true)) {
	//	//CString text = st"|\r\n委托成功";
	//	MessageBox(str + "\r\n委托成功");
	//	PostOutputMsg((str+"委托成功").GetBuffer());
	//}
	//else {
	//	PostOutputMsg((str + "委托失败").GetBuffer());
	//}
}

void CTradeWnd::OnUpdateButtonUI(CCmdUI* pCmdUI)
{
}


//afx_msg LRESULT CTradeWnd::OnUserRequestInstDone(WPARAM wParam, LPARAM lParam)
//{
//	m_pInstProp->RemoveAllOptions();
//	for each (instrument_st inst in theApp.m_BaseInfo.instruments)
//	{
//		m_pInstProp->AddOption(CString(inst.product_name) + "-" + CString(inst.instrumentid));
//	}
//	return 0;
//}


//afx_msg LRESULT CTradeWnd::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
//{
//	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;
//	if (pProp == m_pInstProp)
//	{
//		m_pVolProp->EnableSpinControl(FALSE);
//		hyphenstr h(pProp->GetValue());
//		instrument_st inst;
//		instrument_st tmp = { 0 };
//		strcpy(tmp.instrumentid, h.right);
//		auto inst_it = std::lower_bound(theApp.m_BaseInfo.instruments.begin(), theApp.m_BaseInfo.instruments.end(), tmp);
//		if (inst_it == theApp.m_BaseInfo.instruments.end() || strcmp(tmp.instrumentid, inst_it->instrumentid))
//			return 0;
//		m_pVolProp->EnableSpinControl(TRUE, inst_it->min_lots, inst_it->max_lots);
//	}
//	return 0;
//}
