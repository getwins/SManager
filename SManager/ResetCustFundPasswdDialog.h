#pragma once
#include "afxwin.h"


// CResetCustFundPasswdDialog 对话框
#include "KVhyphenComboBox.h"
class CResetCustFundPasswdDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CResetCustFundPasswdDialog)

public:
	CResetCustFundPasswdDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CResetCustFundPasswdDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESET_FUND_PASSWD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CCustComboBox m_ctlCust;
	CString m_Passwd;
	CString m_PasswdConfirm;

	bool ReqeustResetCustFundPasswd_851007();
	afx_msg void OnBnClickedOk();
};
