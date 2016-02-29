#pragma once
#include "afxwin.h"


// CProxyOperSettingDialog 对话框

class CProxyOperSettingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CProxyOperSettingDialog)

public:
	CProxyOperSettingDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProxyOperSettingDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROXY_OPER_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonIn();
	afx_msg void OnBnClickedButtonOut();
	afx_msg void OnBnClickedButtonAllIn();
	afx_msg void OnBnClickedButtonAllOut();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	CString m_ProxyOperCode;
	CListBox m_ctlCust;
	CListBox m_ctlProxyCust;
	virtual BOOL OnInitDialog();
};
