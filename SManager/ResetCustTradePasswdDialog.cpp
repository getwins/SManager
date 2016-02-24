// ResetCustTradePasswdDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "ResetCustTradePasswdDialog.h"
#include "afxdialogex.h"

#include "MyBCRequest.h"

// CResetCustTradePasswdDialog 对话框

IMPLEMENT_DYNAMIC(CResetCustTradePasswdDialog, CDialogEx)

CResetCustTradePasswdDialog::CResetCustTradePasswdDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RESET_TRADE_PASSWD, pParent)
	, m_Passwd(_T(""))
	, m_PasswdConfirm(_T(""))
{

}

CResetCustTradePasswdDialog::~CResetCustTradePasswdDialog()
{
}

void CResetCustTradePasswdDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CUST, m_ctlCust);
	DDX_Text(pDX, IDC_EDIT_TRADE_PASSWD, m_Passwd);
	DDX_Text(pDX, IDC_EDIT_PASSWD_CONFIRM, m_PasswdConfirm);
}

bool CResetCustTradePasswdDialog::ReqeustResetCustTradePasswd_851007()
{
	CString cust_no = m_ctlCust.GetCurSelKey();
	if (cust_no.GetLength() == 0) 
	{
		MessageBox("请先选择客户");
		return false;
	}

	if (m_Passwd != m_PasswdConfirm)
	{
		MessageBox("密码和密码确认不一致");
		return false;
	}

	Scoped_BCHANDLE handle;
	BCSetRequestType(handle, 851007);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no.GetBuffer());
	BCSetStringFieldByName(handle, 0, "sstatus0", "1");
	BCSetStringFieldByName(handle, 0, "swithdraw_pwd2", m_Passwd.GetBuffer());

	return MyBCRequest(handle, FetchRowFunc_t(), BCMSG_TOALL);
}


BEGIN_MESSAGE_MAP(CResetCustTradePasswdDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CResetCustTradePasswdDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CResetCustTradePasswdDialog 消息处理程序


void CResetCustTradePasswdDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (ReqeustResetCustTradePasswd_851007()) {
		CDialogEx::OnOK();
	}
}
