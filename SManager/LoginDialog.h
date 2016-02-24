#pragma once

#include "struct_helper.h"
#include "MyBCRequest.h"
// CLoginDialog �Ի���

class CLoginDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDialog)

public:
	CLoginDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLoginDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGINDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
