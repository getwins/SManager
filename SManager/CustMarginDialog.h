#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CCustMarginDialog 对话框
#include "KVhyphenComboBox.h"
#include "struct_helper.h"

class CCustMarginDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCustMarginDialog)

public:
	CCustMarginDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCustMarginDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUST_MARGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CCustComboBox m_ctlCust;
	CListCtrl m_listctrl;
	std::vector<cust_margin_rate_st> m_cmrs;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnBnClickedButtonDelete();
	
};
