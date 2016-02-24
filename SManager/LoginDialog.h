#pragma once

#include "struct_helper.h"
#include "MyBCRequest.h"
// CLoginDialog 对话框

class CLoginDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDialog)

public:
	CLoginDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGINDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_OperCode;
	CString m_OperPass;
	BOOL m_isRememberCode;
	BOOL m_isRememberPasswd;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedBtnCancel();
};
