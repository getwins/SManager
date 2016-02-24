// ModifyOperPasswdDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "ModifyOperPasswdDialog.h"
#include "afxdialogex.h"
#include "SMutility.h"

// CModifyOperPasswdDialog �Ի���

IMPLEMENT_DYNAMIC(CModifyOperPasswdDialog, CDialogEx)

CModifyOperPasswdDialog::CModifyOperPasswdDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MODIFY_OPER_PASSWD, pParent)
	, m_OperTitle(g_cfg.oper_code)
	, m_OldPasswd(_T(""))
	, m_NewPasswd(_T(""))
	, m_NewConfirm(_T(""))
{
	m_YellowBr.CreateSolidBrush(RGB(255, 255, 0));
}

CModifyOperPasswdDialog::~CModifyOperPasswdDialog()
{
}

void CModifyOperPasswdDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OPER_TITLE, m_OperTitle);
	DDX_Text(pDX, IDC_EDIT_OLD_PASSWD, m_OldPasswd);
	DDX_Text(pDX, IDC_EDIT_NEW_PASSWD, m_NewPasswd);
	DDX_Text(pDX, IDC_EDIT_NEW_CONFIRM, m_NewConfirm);
	DDV_MaxChars(pDX, m_OldPasswd, 8);
	DDV_MaxChars(pDX, m_NewPasswd, 8);
	DDV_MaxChars(pDX, m_NewConfirm, 8);
}


BEGIN_MESSAGE_MAP(CModifyOperPasswdDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CModifyOperPasswdDialog::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_NEW_CONFIRM, &CModifyOperPasswdDialog::OnEnChangeEditNewConfirm)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_NEW_PASSWD, &CModifyOperPasswdDialog::OnEnChangeEditNewPasswd)
END_MESSAGE_MAP()


// CModifyOperPasswdDialog ��Ϣ�������


void CModifyOperPasswdDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_NewPasswd != m_NewConfirm)
		return;

	if (RequestModifyOperPasswd_851210()) {
		CDialogEx::OnOK();
	}	
}

bool CModifyOperPasswdDialog::RequestModifyOperPasswd_851210()
{
	if (m_NewPasswd != m_NewConfirm)
		return false;
	Scoped_BCHANDLE handle;
	BCSetRequestType(handle, 851210);
	BCSetStringFieldByName(handle, 0, "scust_no2", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "stx_pwd2", g_cfg.oper_passwd);
	BCSetStringFieldByName(handle, 0, "swithdraw_pwd2", m_NewPasswd.GetBuffer());
	return MyBCRequest(handle, FetchRowFunc_t(), BCMSG_TOALL);
	//return false;
}


void CModifyOperPasswdDialog::OnEnChangeEditNewConfirm()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//UpdateData(TRUE);
	////afxGlobalData
	//if (m_NewPasswd != m_NewConfirm) {
	//	GetDlgItem(IDC_EDIT_NEW_CONFIRM)->
	//	GetDlgItem(IDC_EDIT_NEW_CONFIRM)->SetBackgroundColor(RGB(255, 255, 0), TRUE);
	//}
	//else {
	//	GetDlgItem(IDC_EDIT_NEW_CONFIRM)->SetBackgroundColor(RGB(255, 255, 255), TRUE);
	//}
	//Invalidate(TRUE);
	GetDlgItem(IDC_EDIT_NEW_CONFIRM)->Invalidate(TRUE);
}


HBRUSH CModifyOperPasswdDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	//BLACK_BRUSH
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	if (nCtlColor == CTLCOLOR_EDIT && pWnd->GetDlgCtrlID() == IDC_EDIT_NEW_CONFIRM){
		UpdateData(TRUE);
		if (m_NewPasswd != m_NewConfirm) {
			pDC->SetBkColor(RGB(255, 255, 0));
			return (HBRUSH)::GetStockObject(GRAY_BRUSH);
		}
			
	}
	return hbr;
}


void CModifyOperPasswdDialog::OnEnChangeEditNewPasswd()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_EDIT_NEW_CONFIRM)->Invalidate(TRUE);
}
