// ResetCustFundPasswdDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "ResetCustFundPasswdDialog.h"
#include "afxdialogex.h"


// CResetCustFundPasswdDialog �Ի���

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
	BCSetStringFieldByName(handle, 0, "sstatus0", "2");
	BCSetStringFieldByName(handle, 0, "swithdraw_pwd2", m_Passwd.GetBuffer());

	return MyBCRequest(handle, FetchRowFunc_t(), BCMSG_TOALL);
}


BEGIN_MESSAGE_MAP(CResetCustFundPasswdDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CResetCustFundPasswdDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CResetCustFundPasswdDialog ��Ϣ�������


void CResetCustFundPasswdDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (ReqeustResetCustFundPasswd_851007()) {
		CDialogEx::OnOK();
	}
}
