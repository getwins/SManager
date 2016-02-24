// AddTemplateDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "AddTemplateDialog.h"
#include "afxdialogex.h"


// CAddTemplateDialog �Ի���

IMPLEMENT_DYNAMIC(CAddTemplateDialog, CDialogEx)

CAddTemplateDialog::CAddTemplateDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADD_TEMPLATE, pParent)
	, m_TempNo(_T(""))
	, m_TempName(_T(""))
{

}

CAddTemplateDialog::~CAddTemplateDialog()
{
}

void CAddTemplateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TEMPLATE_NO, m_TempNo);
	DDX_Text(pDX, IDC_EDIT_TEMPLATE_NAME, m_TempName);
	DDV_MaxChars(pDX, m_TempNo, 15);
	DDV_MaxChars(pDX, m_TempName, 200);
}


BEGIN_MESSAGE_MAP(CAddTemplateDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddTemplateDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddTemplateDialog ��Ϣ�������


void CAddTemplateDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CDialogEx::OnOK();
}
