// BankAccountMgrDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "BankAccountMgrDialog.h"
#include "afxdialogex.h"
#include "CommBCRequest.h"

// CBankAccountMgrDialog 对话框

IMPLEMENT_DYNAMIC(CBankAccountMgrDialog, CDialogEx)

CBankAccountMgrDialog::CBankAccountMgrDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BANKACCOUNT, pParent)
	, m_BankAccount(_T(""))
	, m_MoneyPasswd(_T(""))
{

}

CBankAccountMgrDialog::~CBankAccountMgrDialog()
{
}

void CBankAccountMgrDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CUST, m_ctlCust);
	DDX_Text(pDX, IDC_EDIT_BANKACCOUNT, m_BankAccount);
	DDX_Text(pDX, IDC_EDIT_MONEY_PASSWD, m_MoneyPasswd);
}


BEGIN_MESSAGE_MAP(CBankAccountMgrDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CBankAccountMgrDialog::OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_SIGNIN, &CBankAccountMgrDialog::OnBnClickedButtonSignin)
	ON_BN_CLICKED(IDC_BUTTON_SIGNOUT, &CBankAccountMgrDialog::OnBnClickedButtonSignout)
END_MESSAGE_MAP()


// CBankAccountMgrDialog 消息处理程序


void CBankAccountMgrDialog::OnBnClickedButtonQuery()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cust_no = m_ctlCust.GetCurSelKey();
	if (cust_no.GetLength() == 0)
	{
		MessageBox("请选择查询客户");
		return;
	}
	bank_account_st ba = { 0 };
	Scoped_BCHANDLE handle;
	BCResult result;
	strcpy(ba.cust_no, cust_no);
	result = BCRequestQryBankFuture_858222(handle, ba);
	if (!result)
	{
		MessageBox(result.errmsg);
		return;
	}
	m_BankAccount = ba.bank_acc;
	UpdateData(FALSE);
}


void CBankAccountMgrDialog::OnBnClickedButtonSignin()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cust_no = m_ctlCust.GetCurSelKey();
	if (cust_no.GetLength() == 0)
	{
		MessageBox("请选择查询客户");
		return;
	}
	UpdateData(TRUE);
	bank_account_st ba = { 0 };
	Scoped_BCHANDLE handle;
	BCResult result;
	strcpy(ba.cust_no, cust_no);
	strcpy(ba.bank_acc, m_BankAccount);
	strcpy(ba.bank_code, "2");
	strcpy(ba.cert_type, "");
	strcpy(ba.currency_type, "1");
	strcpy(ba.cust_type, "Z");
	strcpy(ba.fund_passwd, m_MoneyPasswd);
	ba.ratio = 1.0;
	strcpy(ba.req_type, "3");
	result = BCRequestBankFutureSignin_858111(handle, ba);
	if (result)
	{
		MessageBox("绑定银行账户成功");
	}
	else
	{
		MessageBox(result.errmsg);
	}
}


void CBankAccountMgrDialog::OnBnClickedButtonSignout()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cust_no = m_ctlCust.GetCurSelKey();
	if (cust_no.GetLength() == 0)
	{
		MessageBox("请选择查询客户");
		return;
	}
	UpdateData(TRUE);
	bank_account_st ba = { 0 };
	Scoped_BCHANDLE handle;
	BCResult result;
	strcpy(ba.cust_no, cust_no);
	strcpy(ba.bank_acc, m_BankAccount);
	strcpy(ba.bank_code, "2");
	strcpy(ba.cert_type, "");
	strcpy(ba.currency_type, "1");
	strcpy(ba.cust_type, "Z");
	strcpy(ba.fund_passwd, m_MoneyPasswd);
	ba.ratio = 1.0;
	strcpy(ba.req_type, "3");
	result = BCRequestBankFutureSignout_858112(handle, ba);
	if (result)
	{
		MessageBox("解绑银行账户成功");
		m_BankAccount.Empty();
		UpdateData(FALSE);
	}
	else
	{
		MessageBox(result.errmsg);
	}
}
