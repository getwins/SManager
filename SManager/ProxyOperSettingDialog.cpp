// ProxyOperSettingDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "ProxyOperSettingDialog.h"
#include "afxdialogex.h"
#include "CommBCRequest.h"
#include "SMutility.h"

#include <set>
#include <algorithm>
#include <iterator>

// CProxyOperSettingDialog 对话框

IMPLEMENT_DYNAMIC(CProxyOperSettingDialog, CDialogEx)

CProxyOperSettingDialog::CProxyOperSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROXY_OPER_SETTING, pParent)
	, m_ProxyOperCode(_T(""))
{

}

CProxyOperSettingDialog::~CProxyOperSettingDialog()
{
}

void CProxyOperSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PROXY_OPER, m_ProxyOperCode);
	DDX_Control(pDX, IDC_LIST_CUST, m_ctlCust);
	DDX_Control(pDX, IDC_LIST_PROXY_CUST, m_ctlProxyCust);
	DDV_MaxChars(pDX, m_ProxyOperCode, 15);
}


BEGIN_MESSAGE_MAP(CProxyOperSettingDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CProxyOperSettingDialog::OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CProxyOperSettingDialog::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_IN, &CProxyOperSettingDialog::OnBnClickedButtonIn)
	ON_BN_CLICKED(IDC_BUTTON_OUT, &CProxyOperSettingDialog::OnBnClickedButtonOut)
	ON_BN_CLICKED(IDC_BUTTON_ALL_IN, &CProxyOperSettingDialog::OnBnClickedButtonAllIn)
	ON_BN_CLICKED(IDC_BUTTON_ALL_OUT, &CProxyOperSettingDialog::OnBnClickedButtonAllOut)
	ON_BN_CLICKED(IDOK, &CProxyOperSettingDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CProxyOperSettingDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CProxyOperSettingDialog 消息处理程序


void CProxyOperSettingDialog::OnBnClickedButtonQuery()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ctlProxyCust.ResetContent();

	UpdateData(TRUE);
	if (m_ProxyOperCode.GetLength() == 0)
	{
		MessageBox("请输入代理操作员代码");
		return;
	}
	Scoped_BCHANDLE handle;
	std::vector<oper_cust_correspond_st> occs;
	BCResult result = BCRequestQryOperCustCorrespond_851243(handle, m_ProxyOperCode.GetBuffer(), occs);
	if (!result)
	{
		MessageBox(result.errmsg);
		return;
	}

	for each (oper_cust_correspond_st var in occs)
	{
		m_ctlProxyCust.AddString(var.cust_no);
	}
}


void CProxyOperSettingDialog::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_ProxyOperCode.GetLength() == 0)
	{
		MessageBox("请输入代理操作员代码");
		return;
	}

	Scoped_BCHANDLE handle;
	BCResult result;
	
	oper_basic_st ob = { 0 };
	strcpy(ob.oper_code, m_ProxyOperCode);
	strcpy(ob.oper_name, CString(g_cfg.oper_code) + "的代理");
	strcpy(ob.node_limit, "0");
	strcpy(ob.access_limit, "1");
	strcpy(ob.oper_status, "0");

	ob.menu_perm[0] = 192;
	ob.menu_perm[2] = 32;
	ob.menu_perm[5] = 253;
	ob.menu_perm[6] = 252;
	ob.menu_perm[23] = 8;
	ob.menu_perm[24] = 124;
	ob.menu_perm[27] = 2;
	ob.menu_perm[29] = 16;
	ob.menu_perm[31] = 64;
	ob.menu_perm[32] = 1;
	ob.menu_perm[33] = 160;
	ob.menu_perm[75] = 31;
	ob.menu_perm[77] = 15;
	ob.menu_perm[90] = 1;
	ob.menu_perm[91] = 28;

	ob.func_perm[1] = 32;
	ob.func_perm[65] = 3;
	ob.func_perm[66] = 128;

	result = BCRequestSetOperator_851200(handle, "1", ob);
	if (result)
	{
		MessageBox("新增代理操作员成功");
	}
	else
	{
		MessageBox(result.errmsg);
	}
	
}


void CProxyOperSettingDialog::OnBnClickedButtonIn()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_ctlCust.GetCurSel();
	if (sel < 0 || sel > m_ctlCust.GetCount() - 1)
		return;

	CString str;
	m_ctlCust.GetText(sel, str);
	if (m_ctlProxyCust.FindString(0, str) < 0)
		m_ctlProxyCust.AddString(str);
}


void CProxyOperSettingDialog::OnBnClickedButtonOut()
{
	// TODO: 在此添加控件通知处理程序代码
	//int sel = m_ctlCust.GetCurSel();
	int sel = m_ctlProxyCust.GetCurSel();
	if (sel < 0 || sel > m_ctlProxyCust.GetCount() - 1)
		return;
	m_ctlProxyCust.DeleteString(sel);
}


void CProxyOperSettingDialog::OnBnClickedButtonAllIn()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ctlProxyCust.ResetContent();
	for (int i = 0; i < m_ctlCust.GetCount(); i++)
	{
		CString str;
		m_ctlCust.GetText(i, str);
		m_ctlProxyCust.AddString(str);
	}
}


void CProxyOperSettingDialog::OnBnClickedButtonAllOut()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ctlProxyCust.ResetContent();
}


void CProxyOperSettingDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	UpdateData(TRUE);
	Scoped_BCHANDLE handle;
	oper_basic_st ob;
	BCResult result = BCRequestQryOperator_851201(handle, m_ProxyOperCode.GetBuffer(), ob);
	if (!result)
	{
		MessageBox(result.errmsg);
		return;
	}

	std::vector<oper_cust_correspond_st> occs;
	result = BCRequestQryOperCustCorrespond_851243(handle, m_ProxyOperCode.GetBuffer(), occs);
	//if (!result)
	//{
	//	MessageBox(result.errmsg);
	//	return;
	//}

	std::set<std::string> proxycust, cur_proxycust;
	//std::for_each()
	for each (oper_cust_correspond_st var in occs)
	{
		proxycust.insert(var.cust_no);
	}

	for (int i = 0; i < m_ctlProxyCust.GetCount(); i++)
	{
		CString str;
		m_ctlProxyCust.GetText(i, str);
		cur_proxycust.insert(str.GetBuffer());
	}

	if (proxycust == cur_proxycust)
		return;

	std::set<std::string> out_cust, in_cust;
	std::set_difference(proxycust.begin(), proxycust.end(), cur_proxycust.begin(), cur_proxycust.end(), std::inserter(out_cust, out_cust.begin()));
	std::set_difference(cur_proxycust.begin(), cur_proxycust.end(), proxycust.begin(), proxycust.end(), std::inserter(in_cust, in_cust.begin()));

	for each (std::string var in out_cust)
	{
		BCRequestSetOperCustCorrespond_851206(handle, "3", m_ProxyOperCode.GetBuffer(), (char*)(var.c_str()));
	}

	for each (std::string var in in_cust)
	{
		BCRequestSetOperCustCorrespond_851206(handle, "1", m_ProxyOperCode.GetBuffer(), (char*)(var.c_str()));
	}

	OnBnClickedButtonQuery();
}


void CProxyOperSettingDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL CProxyOperSettingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	for each (std::string var in theApp.GetCustnoList())
	{
		m_ctlCust.AddString(var.c_str());
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
