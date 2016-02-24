// AddTemplateDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "AddTemplateDialog.h"
#include "afxdialogex.h"


// CAddTemplateDialog 对话框

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


// CAddTemplateDialog 消息处理程序


void CAddTemplateDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CDialogEx::OnOK();
}
