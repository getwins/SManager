#pragma once

#include "KVhyphenComboBox.h"
// CCustTradeProductLimitDialog �Ի���

class CCustTradeProductLimitDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCustTradeProductLimitDialog)

public:
	CCustTradeProductLimitDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCustTradeProductLimitDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUST_TRADE_PRODUCT_LIMIT };
#endif


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	std::vector<trade_product_limit_st> m_tpls;
	CCustComboBox m_ctlCust;
	CListCtrl m_listctrl;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnBnClickedButtonDelete();
};
