#pragma once
#include "afxwin.h"


// CResetCustTradePasswdDialog �Ի���
#include "KVhyphenComboBox.h"

class CResetCustTradePasswdDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CResetCustTradePasswdDialog)

public:
	CResetCustTradePasswdDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CResetCustTradePasswdDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESET_TRADE_PASSWD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	bool ReqeustResetCustTradePasswd_851007();
public:
	CCustComboBox m_ctlCust;
	CString m_Passwd;
	CString m_PasswdConfirm;
	afx_msg void OnBnClickedOk();
};
