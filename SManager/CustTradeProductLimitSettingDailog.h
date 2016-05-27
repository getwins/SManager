#pragma once
#include "KVhyphenComboBox.h"
#include "afxwin.h"

// CCustTradeProductLimitSettingDailog 对话框

class CCustTradeProductLimitSettingDailog : public CDialogEx
{
	DECLARE_DYNAMIC(CCustTradeProductLimitSettingDailog)

public:
	CCustTradeProductLimitSettingDailog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCustTradeProductLimitSettingDailog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUST_TRADE_PRODUCT_LIMIT_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CCustComboBox m_ctlCust;
	CString m_flag;
	CCTComboBox m_ctlCommission;
	CExchangeComboBox m_ctlExchange;
	CProductComboBox m_ctlProduct;
	CString m_date;

	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboExchange();
	virtual BOOL OnInitDialog();

	CTradeLimitTypeComboBox m_LimitType;
};
