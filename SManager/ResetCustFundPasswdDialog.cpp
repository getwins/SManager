// ResetCustFundPasswdDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "ResetCustFundPasswdDialog.h"
#include "afxdialogex.h"


// CResetCustFundPasswdDialog 对话框

IMPLEMENT_DYNAMIC(CResetCustFundPasswdDialog, CDialogEx)

CResetCustFundPasswdDialog::CResetCustFundPasswdDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RESET_FUND_PASSWD, pParent)
	, m_Passwd(_T(""))
	, m_PasswdConfirm(_T(""))
{

}

CResetCustFundPasswdDialog::~CResetCustFundPasswdDialog()
{
}

void CResetCustFundPasswdDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CUST, m_ctlCust);
	DDX_Text(pDX, IDC_EDIT_FUND_PASSWD, m_Passwd);
	DDX_Text(pDX, IDC_EDIT_PASSWD_CONFIRM, m_PasswdConfirm);
}

bool CResetCustFundPasswdDialog::ReqeustResetCustFundPasswd_851007()
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
	BCSetStringFieldByName(handle, 0, "sstatus0", "2");
	BCSetStringFieldByName(handle, 0, "swithdraw_pwd2", m_Passwd.GetBuffer());

	return MyBCRequest(handle, FetchRowFunc_t(), BCMSG_TOALL);
}


BEGIN_MESSAGE_MAP(CResetCustFundPasswdDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CResetCustFundPasswdDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CResetCustFundPasswdDialog 消息处理程序


void CResetCustFundPasswdDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (ReqeustResetCustFundPasswd_851007()) {
		CDialogEx::OnOK();
	}
}
