// BankAccountMgrDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "BankAccountMgrDialog.h"
#include "afxdialogex.h"
#include "CommBCRequest.h"

// CBankAccountMgrDialog �Ի���

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


// CBankAccountMgrDialog ��Ϣ�������


void CBankAccountMgrDialog::OnBnClickedButtonQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString cust_no = m_ctlCust.GetCurSelKey();
	if (cust_no.GetLength() == 0)
	{
		MessageBox("��ѡ���ѯ�ͻ�");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString cust_no = m_ctlCust.GetCurSelKey();
	if (cust_no.GetLength() == 0)
	{
		MessageBox("��ѡ���ѯ�ͻ�");
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
		MessageBox("�������˻��ɹ�");
	}
	else
	{
		MessageBox(result.errmsg);
	}
}


void CBankAccountMgrDialog::OnBnClickedButtonSignout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString cust_no = m_ctlCust.GetCurSelKey();
	if (cust_no.GetLength() == 0)
	{
		MessageBox("��ѡ���ѯ�ͻ�");
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
		MessageBox("��������˻��ɹ�");
		m_BankAccount.Empty();
		UpdateData(FALSE);
	}
	else
	{
		MessageBox(result.errmsg);
	}
}
