// LoginDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "LoginDialog.h"
#include "afxdialogex.h"
#include "SMutility.h"

// CLoginDialog �Ի���

IMPLEMENT_DYNAMIC(CLoginDialog, CDialogEx)

CLoginDialog::CLoginDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOGINDIALOG, pParent)
	, m_OperCode(_T(""))
	, m_OperPass(_T(""))
	, m_isRememberCode(FALSE)
	, m_isRememberPasswd(FALSE)
{
	m_OperCode = g_cfg.oper_code;
	m_OperPass = g_cfg.oper_passwd;

	if (!m_OperCode.IsEmpty())
		m_isRememberCode = TRUE;

	if (!m_OperPass.IsEmpty())
		m_isRememberPasswd = TRUE;

	//UpdateData(FALSE);
}

CLoginDialog::~CLoginDialog()
{
}

void CLoginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OPER_CODE, m_OperCode);
	DDV_MaxChars(pDX, m_OperCode, 11);
	DDX_Text(pDX, IDC_EDIT_OPER_PASSWD, m_OperPass);
	DDX_Check(pDX, IDC_CHECK_REMEMBER_CODE, m_isRememberCode);
	DDX_Check(pDX, IDC_CHECK_REMEMBER_PASSWD, m_isRememberPasswd);
}


BEGIN_MESSAGE_MAP(CLoginDialog, CDialogEx)
	ON_BN_CLICKED(ID_BTN_LOGIN, &CLoginDialog::OnBnClickedBtnLogin)
	ON_BN_CLICKED(ID_BTN_CANCEL, &CLoginDialog::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// CLoginDialog ��Ϣ�������


BOOL CLoginDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CLoginDialog::OnBnClickedBtnLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UpdateData(TRUE);

	//BCHANDLE handle = BCNewHandle(XpackDescribleFile);
	Scoped_BCHANDLE handle;
	BCResetHandle(handle);
	BCSetRequestType(handle, 854000);

	BCSetStringFieldByName(handle, 0, "scust_no", m_OperCode.GetBuffer());
	BCSetStringFieldByName(handle, 0, "semp_pwd", m_OperPass.GetBuffer());

	TCHAR host[MAX_PATH];
	::gethostname(host, MAX_PATH);
	BCSetStringFieldByName(handle, 0, "sphone", host);
	BCSetStringFieldByName(handle, 0, "sphone2", "34-02-86-70-4B-A2");
	BCSetStringFieldByName(handle, 0, "sholder_type2", "B"); //Ӧ�ó����ʶ

	
	//MyBCCallRequest(handle);
	if(MyBCRequest(handle, FetchRowFunc_t(), BCMSG_ERR_TO_MB))
		CDialog::OnOK();
	//int retcode;
	//BCGetRetCode(handle, &retcode);
	//if (retcode != 0) {
	//	char vsmess[256] = { 0 };
	//	BCGetStringFieldByName(handle, 0, "vsmess", vsmess, 256);
	//	MessageBox(vsmess, "��¼�쳣", MB_OK);
	//	BCDeleteHandle(handle);
	//	return;
	//}
	//BCDeleteHandle(handle);

	

}


void CLoginDialog::OnBnClickedBtnCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
