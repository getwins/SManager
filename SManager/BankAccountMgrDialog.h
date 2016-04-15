#pragma once
#include "afxwin.h"
#include "KVhyphenComboBox.h"

// CBankAccountMgrDialog �Ի���

class CBankAccountMgrDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CBankAccountMgrDialog)

public:
	CBankAccountMgrDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBankAccountMgrDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BANKACCOUNT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CCustComboBox m_ctlCust;
	CString m_BankAccount;
	CString m_MoneyPasswd;
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnBnClickedButtonSignin();
	afx_msg void OnBnClickedButtonSignout();
};
