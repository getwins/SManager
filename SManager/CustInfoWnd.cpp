// CustInfoWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "CustInfoWnd.h"
#include "string_identifer.h"
#include "cust_dynamic_info_worker.h"
#include "Format.h"
#include "SMutility.h"
#include <algorithm>
#include <set>
#include <cassert>

// CCustInfoWnd

IMPLEMENT_DYNAMIC(CCustInfoWnd, CDockablePane)

CCustInfoWnd::CCustInfoWnd()
{

}

CCustInfoWnd::~CCustInfoWnd()
{
}


BEGIN_MESSAGE_MAP(CCustInfoWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_USER_CUST_SEL_CHANGED, &CCustInfoWnd::OnUserCustSelChanged)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, &CCustInfoWnd::OnAfxWmPropertyChanged)
	ON_MESSAGE(WM_USER_CUST_BASE_INFO, &CCustInfoWnd::OnUserCustBaseInfo)
	ON_MESSAGE(WM_USER_CUST_DYNAMIC_INFO, &CCustInfoWnd::OnUserCustDynamicInfo)
END_MESSAGE_MAP()



// CCustInfoWnd 消息处理程序




void CCustInfoWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty *pGroup1 = new CMFCPropertyGridProperty(_T("基本信息"));
	//CMFCPropertyGridProperty *pProp;
	m_pCustNo = new CMFCPropertyGridProperty("交易账户", "", "单击客户权益列表选中要显示的客户");
	m_pCustNo->AllowEdit(FALSE);
	pGroup1->AddSubItem(m_pCustNo);
	m_pCustName = new CMFCPropertyGridProperty("名称", "", "显示并可以修改客户名称");
	pGroup1->AddSubItem(m_pCustName);
	m_pCustStatus = new CMFCPropertyGridProperty("客户状态", "", "0-正常,4-只可平仓,5-禁取,6-冻结,7-销户");
	m_pCustStatus->AllowEdit(FALSE);
	pGroup1->AddSubItem(m_pCustStatus);
	m_pCustType = new CMFCPropertyGridProperty("客户性质", "1-自然人", "1-自然人,2-法人,3-基金户");
	m_pCustType->AllowEdit(FALSE);
	pGroup1->AddSubItem(m_pCustType);
	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty *pGroup2 = new CMFCPropertyGridProperty(_T("登录信息"));
	m_pLoginStatus = new CMFCPropertyGridProperty("登录状态", "0-登出", "0-登出,1-登入");
	m_pLoginStatus->AllowEdit(FALSE);
	pGroup2->AddSubItem(m_pLoginStatus);
	m_pLoginDate = new CMFCPropertyGridProperty("登录日期", "", "");
	m_pLoginDate->AllowEdit(FALSE);
	pGroup2->AddSubItem(m_pLoginDate);
	m_pLoginTime = new CMFCPropertyGridProperty("登录时间", "", "");
	m_pLoginTime->AllowEdit(FALSE);
	pGroup2->AddSubItem(m_pLoginTime);
	m_pLoginIp = new CMFCPropertyGridProperty("登录IP", "", "");
	m_pLoginIp->AllowEdit(FALSE);
	pGroup2->AddSubItem(m_pLoginIp);
	m_pLoginApp = new CMFCPropertyGridProperty("登录终端", "", "");
	m_pLoginApp->AllowEdit(FALSE);
	pGroup2->AddSubItem(m_pLoginApp);
	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty *pGroup3 = new CMFCPropertyGridProperty("手续费模板");
	m_pCommissionNo = new CMFCPropertyGridProperty("模板号", "", "");
	m_pCommissionNo->AllowEdit(FALSE);
	pGroup3->AddSubItem(m_pCommissionNo);
	m_pCommissionName = new CMFCPropertyGridProperty("模板名称", "", "");
	m_pCommissionName->AllowEdit(FALSE);
	pGroup3->AddSubItem(m_pCommissionName);
	m_wndPropList.AddProperty(pGroup3);

	CMFCPropertyGridProperty *pGroup4 = new CMFCPropertyGridProperty("保证金模板");
	m_pMarginNo = new CMFCPropertyGridProperty("模板号", "", "");
	m_pMarginNo->AllowEdit(FALSE);
	pGroup4->AddSubItem(m_pMarginNo);
	m_pMarginName = new CMFCPropertyGridProperty("模板名称", "", "");
	m_pMarginName->AllowEdit(FALSE);
	pGroup4->AddSubItem(m_pMarginName);
	m_wndPropList.AddProperty(pGroup4);

	CMFCPropertyGridProperty *pGroup5 = new CMFCPropertyGridProperty("持仓信息");
	m_pPosi = new CMFCPropertyGridProperty("总持仓", "0", "客户持仓总和");
	m_pPosi->AllowEdit(FALSE);
	pGroup5->AddSubItem(m_pPosi);
	m_pTdPosi = new CMFCPropertyGridProperty("今仓", "0", "客户今仓总和");
	m_pTdPosi->AllowEdit(FALSE);
	pGroup5->AddSubItem(m_pTdPosi);
	m_pYdPosi = new CMFCPropertyGridProperty("昨仓", "0", "客户昨仓总和");
	m_pYdPosi->AllowEdit(FALSE);
	pGroup5->AddSubItem(m_pYdPosi);
	m_pFloatProfit = new CMFCPropertyGridProperty("浮动盈亏", "0.0", "客户仓位浮动盈亏");
	m_pFloatProfit->AllowEdit(FALSE);
	pGroup5->AddSubItem(m_pFloatProfit);
	m_pPosiInst = new CMFCPropertyGridProperty("持仓合约","0", "客户持仓合约数量");
	m_pPosiInst->AllowEdit(FALSE);
	pGroup5->AddSubItem(m_pPosiInst);
	m_wndPropList.AddProperty(pGroup5);
	
	CMFCPropertyGridProperty *pGroup6 = new CMFCPropertyGridProperty("风控信息");
	m_pClear = new CMFCPropertyGridProperty("盘终清仓", "无设置", "夜盘和日盘盘终是否强平客户的所有持仓，默认禁止盘终清仓");
	m_pClear->AllowEdit(FALSE);
	pGroup6->AddSubItem(m_pClear);
	m_pLowerCapitalForce = new CMFCPropertyGridProperty("权益小于强平", "无设置", "客户权益小于设置的阀值时强平客户持仓");
	m_pLowerCapitalForce->AllowEdit(FALSE);
	pGroup6->AddSubItem(m_pLowerCapitalForce);
	m_wndPropList.AddProperty(pGroup6);
}

void CCustInfoWnd::SetPropListFont()
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
}

void CCustInfoWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	//m_wndButton.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 40, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top , rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

bool CCustInfoWnd::RequestCustInfoMaintenance_851001(cust_base_info_st cbi)
{
	Scoped_BCHANDLE handle;
	BCSetRequestType(handle, 851001);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cbi.basic.scust_no);
	BCSetStringFieldByName(handle, 0, "sname", cbi.basic.sname);
	BCSetStringFieldByName(handle, 0, "sall_name", cbi.basic.sname);
	BCSetStringFieldByName(handle, 0, "scust_type2", cbi.basic.card_type);
	BCSetStringFieldByName(handle, 0, "scert_no", cbi.basic.card_no);
	BCSetStringFieldByName(handle, 0, "sstatus1", "1"); //客户性质 自然人
	//BCSetStringFieldByName(handle, 0, "scust_type2", "1"); //证件类型 身份证
	
	return MyBCRequest(handle, FetchRowFunc_t(), true);
}

void CCustInfoWnd::Dump(CDumpContext& dc) const
{
	//CDockablePane::Dump(dc);

	// TODO: 在此添加专用代码和/或调用基类

}


int CCustInfoWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_PROPERTYGRID_CUSTINFO))
	{
		TRACE0("未能创建帐户信息属性网格\n");
		return -1;      // 未能创建
	}

	InitPropList();
	AdjustLayout();
	return 0;
}


void CCustInfoWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	AdjustLayout();
}


afx_msg LRESULT CCustInfoWnd::OnUserCustSelChanged(WPARAM wParam, LPARAM lParam)
{
	std::string cust_no = fetch_string(lParam);
	if (m_CurCustNo == cust_no)
		return 0;
	m_CurCustNo = cust_no;

	SetBaseInfo();

	cust_login_info_st cli = cust_dynamic_info_worker::instance()->get_cust_login_info(m_CurCustNo);
	m_pLoginStatus->SetValue((cli.login_status[0] == '1' ? "登入" : "登出"));
	m_pLoginDate->SetValue(cli.cur_login_date);
	m_pLoginTime->SetValue(cli.cur_login_time);
	m_pLoginIp->SetValue(cli.cur_login_ip);
	m_pLoginApp->SetValue(theApp.DictToStr(DICT_CLASS_APP_IDENTIFIER, cli.cur_app_name));



	SetPositionInfo();
	return 0;
}

void CCustInfoWnd::SetPositionInfo()
{
	//cust_dynamic_info_worker::instance()->get_cust_position(m_CurCustNo);
	long orgin = 0, td = 0, yd = 0;
	double float_profit = 0.0;
	std::set<std::string> insts;
	for each (cust_position_st p in cust_dynamic_info_worker::instance()->get_cust_position(m_CurCustNo))
	{
		orgin += p.positions;
		td += p.td_positions;
		//yd += p.
		float_profit += p.float_profit;
		insts.insert(p.instrumentid);
	}
	yd = orgin - td;
	//CString str;
	//str.Format("%d", orgin);
	m_pPosi->SetValue(intfmt(orgin));

	//str.Format("%d", td);
	m_pTdPosi->SetValue(intfmt(td));

	//str.Format("%d", yd);
	m_pYdPosi->SetValue(intfmt(yd));

	//str.Format("%f", float_profit);
	m_pFloatProfit->SetValue(fundfmt(float_profit));

	//str.Format("%d", insts.size());
	m_pPosiInst->SetValue(intfmt(insts.size()));
	//m_pPosi->SetValue(orgin);
	//m_pTdPosi->SetValue(td);
	//m_pYdPosi->SetValue(yd);
	//m_pFloatProfit->SetValue(float_profit);
	//m_pPosiInst->SetValue((long)insts.size());

}

void CCustInfoWnd::SetBaseInfo()
{
	cust_base_info_st cbi = theApp.GetCustBaseInfo(m_CurCustNo.c_str());
	m_pCustNo->SetValue(cbi.basic.scust_no);
	m_pCustName->SetValue(cbi.basic.sname);
	m_pCustStatus->SetValue(theApp.DictToStr(DICT_CLASS_CUST_STATUS, cbi.basic.sstatus0));

	m_pCommissionNo->SetValue(cbi.commission_temp.template_no);
	m_pCommissionName->SetValue(cbi.commission_temp.template_name);
	m_pMarginNo->SetValue(cbi.margin_temp.template_no);
	m_pMarginName->SetValue(cbi.margin_temp.template_name);

	std::vector<rc_desc_st>::iterator it;
	it = find_if(cbi.rcs.begin(), cbi.rcs.end(), [](rc_desc_st &rc) {
		return strcmp(rc.classflag, "0001") == 0;
	});
	if (it != cbi.rcs.end())
	{
		if (strcmp(it->paramlist, "1") == 0)
			m_pClear->SetValue("允许");
		else
			m_pClear->SetValue("禁止");
	}
	else
	{
		m_pClear->SetValue("无设置");
	}

	it = find_if(cbi.rcs.begin(), cbi.rcs.end(), [](rc_desc_st &rc) {
		return strcmp(rc.classflag, "0002") == 0;
	});
	if (it != cbi.rcs.end())
	{
		m_pLowerCapitalForce->SetValue(it->paramlist);
	}
	else
	{
		m_pLowerCapitalForce->SetValue("无设置");
	}
}


afx_msg LRESULT CCustInfoWnd::OnAfxWmPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;
	CString cust_no = m_pCustNo->GetValue();
	assert(m_CurCustNo == cust_no.GetBuffer());
	if (cust_no.GetLength() == 0)
		return 0;

	cust_base_info_st cbi = theApp.GetCustBaseInfo(cust_no);
	assert(cust_no == cbi.basic.scust_no);

	if (pProp == m_pCustName) {
		CString cust_name = pProp->GetValue();
		if (cust_name != cbi.basic.sname) {
			strcpy(cbi.basic.sname, cust_name);
			if (RequestCustInfoMaintenance_851001(cbi)) {
				theApp.SetCustBaseInfo(cbi);
			}
		}
	}
	else if (pProp == m_pCustStatus)
	{
		CString status =  hyphenstr(pProp->GetValue()).left;
		if (status != cbi.basic.sstatus0) {
			strcpy(cbi.basic.sstatus0, status);
		}
	}
	else if (pProp == m_pCommissionNo)
	{
		CString commission_no = pProp->GetValue();
		if (commission_no != cbi.commission_temp.template_no) {
			//strcpy(cib.)
		}
	}
	else if (pProp == m_pMarginNo)
	{

	}
	return 0;
}


afx_msg LRESULT CCustInfoWnd::OnUserCustBaseInfo(WPARAM wParam, LPARAM lParam)
{
	if (wParam == CUST_BASE_INFO_CHANGED)
	{
		std::string cust_no = fetch_string(lParam);
		if (cust_no == m_CurCustNo)
		{
			SetBaseInfo();
			//cust_base_info_st cbi = theApp.GetCustBaseInfo(m_CurCustNo.c_str());
			//assert(m_CurCustNo == cbi.basic.scust_no);
			//m_pCustName->SetValue(cbi.basic.sname);
			//m_pCustStatus->SetValue(theApp.DictToStr(DICT_CLASS_CUST_STATUS, cbi.basic.sstatus0));
			//m_pCommissionNo->SetValue(cbi.commission_temp.template_no);
			//m_pCommissionName->SetValue(cbi.commission_temp.template_name);
			//m_pMarginNo->SetValue(cbi.margin_temp.template_no);
			//m_pMarginName->SetValue(cbi.margin_temp.template_name);

			//std::vector<rc_desc_st>::iterator it;
			//it = find_if(cbi.rcs.begin(), cbi.rcs.end(), [](rc_desc_st &rc) {
			//	return strcmp(rc.classflag, "0001") == 0;
			//});
			//if (it != cbi.rcs.end())
			//{
			//	if (strcmp(it->paramlist, "1") == 0)
			//		m_pClear->SetValue("允许");
			//	else
			//		m_pClear->SetValue("禁止");
			//}
			//else
			//{
			//	m_pClear->SetValue("无设置");
			//}

			//it = find_if(cbi.rcs.begin(), cbi.rcs.end(), [](rc_desc_st &rc) {
			//	return strcmp(rc.classflag, "0002") == 0;
			//});
			//if (it != cbi.rcs.end())
			//{
			//	m_pLowerCapitalForce->SetValue(it->paramlist);
			//}
			//else
			//{
			//	m_pLowerCapitalForce->SetValue("无设置");
			//}
			
		}

	}
	return 0;
}


afx_msg LRESULT CCustInfoWnd::OnUserCustDynamicInfo(WPARAM wParam, LPARAM lParam)
{
	if (wParam == CUST_DYNAMIC_POSITION_CHANGED)
	{
		std::string cust_no = fetch_string(lParam);
		if (cust_no == m_CurCustNo)
		{
			SetPositionInfo();
		}
	}
	return 0;
}
