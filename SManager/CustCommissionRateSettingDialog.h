#pragma once
#include "afxwin.h"


// CCustCommissionRateSettingDialog �Ի���
#include "KVhyphenComboBox.h"

class CCustCommissionRateSettingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCustCommissionRateSettingDialog)

public:
	CCustCommissionRateSettingDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCustCommissionRateSettingDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUST_COMMISSION_RATE_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	char * m_flag;
	CCustComboBox m_ctlCust;
	CExchangeComboBox m_ctlExchange;
	CProductComboBox m_ctlProduct;
	CString m_Date;
	double m_ByAmt;
//	CString m_byVol;
	double m_TdByAmt;
	double m_TdByVol;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	double m_ByVol;
};
