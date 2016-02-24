// OperLoginDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SMPerm.h"
#include "OperLoginDialog.h"
#include "afxdialogex.h"
#include "MyBCRequest.h"


// COperLoginDialog 对话框

IMPLEMENT_DYNAMIC(COperLoginDialog, CDialogEx)

COperLoginDialog::COperLoginDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPER_LOGIN, pParent)
	, m_OperCode(_T("777"))
	, m_OperPasswd(_T(""))
{

}

COperLoginDialog::~COperLoginDialog()
{
}

void COperLoginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OPER_CODE, m_OperCode);
	DDX_Text(pDX, IDC_EDIT_OPER_PASSWD, m_OperPasswd);
}


BEGIN_MESSAGE_MAP(COperLoginDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &COperLoginDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// COperLoginDialog 消息处理程序


void COperLoginDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	BCResult result = BCRequestOperLogin_854000(m_OperCode.GetBuffer(), m_OperPasswd.GetBuffer());
	if(result)
		CDialogEx::OnOK();
	else
	{
		MessageBox(result.errmsg);
	}
}
