#pragma once
#include "afxwin.h"


// CResetCustFundPasswdDialog �Ի���
#include "KVhyphenComboBox.h"
class CResetCustFundPasswdDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CResetCustFundPasswdDialog)

public:
	CResetCustFundPasswdDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CResetCustFundPasswdDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESET_FUND_PASSWD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CCustComboBox m_ctlCust;
	CString m_Passwd;
	CString m_PasswdConfirm;

	bool ReqeustResetCustFundPasswd_851007();
	afx_msg void OnBnClickedOk();
};
