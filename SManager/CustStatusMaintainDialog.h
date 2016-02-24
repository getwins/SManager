#pragma once
#include "afxwin.h"
#include "KVhyphenComboBox.h"

// CCustStatusMaintainDialog 对话框

class CCustStatusMaintainDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCustStatusMaintainDialog)

public:
	CCustStatusMaintainDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCustStatusMaintainDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUST_STATUS_MAINTAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CCustComboBox m_ctlCust;
private:
	bool RequestCustStatusMaintain_851005(char *cust_no, char *cust_status);
	//CComboBox m_ctlCust;
	
	CComboBox m_ctlStatus;
//	CEdit m_ctlCustName;
//	CEdit m_ctlCustStatus;
//	CEdit m_ctlCardType;
	CString m_CustName;
//	CEdit m_CustStatus;
	CString m_CardType;
	CString m_CustStatus;
	CString m_CardNo;
public:
	afx_msg void OnCbnSelchangeComboCust();
	afx_msg void OnBnClickedOk();
};
