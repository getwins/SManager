#pragma once
#include "afxwin.h"

#include "KVhyphenComboBox.h"
#include "MyBCRequest.h"
// COpenCustDialog �Ի���

class COpenCustDialog : public CDialogEx
{
	DECLARE_DYNAMIC(COpenCustDialog)

public:
	COpenCustDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COpenCustDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPEN_CUST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	//CComboBox m_ctlCommission;
	//CComboBox m_ctlMargin;
	CCTComboBox m_ctlCommission;
	CMTComboBox m_ctlMargin;
	CString m_CustNo;
	CString m_CustName;
	CString m_TradePasswd;
	CString m_MoneyPasswd;

	bool CallOpenCust_851000(BCHANDLE handle);
	bool SetCustCommissionTemplate_851342(BCHANDLE handle, char* template_no);
	bool SetCustMarginTemplate_851348(BCHANDLE handle, char* template_no);
	bool SetCustTradeCode_851010(BCHANDLE handle);
	bool SetCustAuthority_851012(BCHANDLE handle);
	BCResult SetCustMultiLogin_851278(BCHANDLE handle);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
//	CComboBox m_ctlCustClass;
	//CComboBox m_ctlCustClass;
	CCustClassComboBox m_ctlCustClass;
};
