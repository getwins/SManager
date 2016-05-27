#pragma once
#include "KVhyphenComboBox.h"
#include "afxwin.h"

// CCustTradeProductLimitSettingDailog �Ի���

class CCustTradeProductLimitSettingDailog : public CDialogEx
{
	DECLARE_DYNAMIC(CCustTradeProductLimitSettingDailog)

public:
	CCustTradeProductLimitSettingDailog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCustTradeProductLimitSettingDailog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUST_TRADE_PRODUCT_LIMIT_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
