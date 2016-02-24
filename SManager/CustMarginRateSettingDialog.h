#pragma once
#include "afxwin.h"
#include "KVhyphenComboBox.h"

// CCustMarginRateSettingDialog 对话框

class CCustMarginRateSettingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCustMarginRateSettingDialog)

public:
	CCustMarginRateSettingDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCustMarginRateSettingDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUST_MARGIN_RATE_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CCustComboBox m_ctlCust;
	CExchangeComboBox m_ctlExchange;
	CProductComboBox m_ctlProduct;
	double m_ByAmt;
	double m_ByVol;
	char *m_flag;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
};
