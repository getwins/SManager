// ResetCustTradePasswdDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "ResetCustTradePasswdDialog.h"
#include "afxdialogex.h"

#include "MyBCRequest.h"

// CResetCustTradePasswdDialog �Ի���

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
		MessageBox("����ѡ��ͻ�");
		return false;
	}

	if (m_Passwd != m_PasswdConfirm)
	{
		MessageBox("���������ȷ�ϲ�һ��");
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


// CResetCustTradePasswdDialog ��Ϣ�������


void CResetCustTradePasswdDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (ReqeustResetCustTradePasswd_851007()) {
		CDialogEx::OnOK();
	}
}
