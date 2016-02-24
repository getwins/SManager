#pragma once
#include "afxwin.h"


// CResetCustTradePasswdDialog 对话框
#include "KVhyphenComboBox.h"

class CResetCustTradePasswdDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CResetCustTradePasswdDialog)

public:
	CResetCustTradePasswdDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CResetCustTradePasswdDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESET_TRADE_PASSWD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	bool ReqeustResetCustTradePasswd_851007();
public:
	CCustComboBox m_ctlCust;
	CString m_Passwd;
	CString m_PasswdConfirm;
	afx_msg void OnBnClickedOk();
};
