// ProxyOperSettingDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "ProxyOperSettingDialog.h"
#include "afxdialogex.h"


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
}


void CProxyOperSettingDialog::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CProxyOperSettingDialog::OnBnClickedButtonIn()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CProxyOperSettingDialog::OnBnClickedButtonOut()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CProxyOperSettingDialog::OnBnClickedButtonAllIn()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CProxyOperSettingDialog::OnBnClickedButtonAllOut()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CProxyOperSettingDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CProxyOperSettingDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
