#pragma once

#include "KVhyphenComboBox.h"
#include "afxwin.h"
#include "afxcmn.h"

#include "MyBCRequest.h"

// CCustCommissionDialog �Ի���

class CCustCommissionDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCustCommissionDialog)

public:
	CCustCommissionDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCustCommissionDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUST_COMMISSION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CCustComboBox m_ctlCust;
	CListCtrl m_listctrl;
	Scoped_BCHANDLE m_handle;
	std::vector<cust_commission_rate_st> m_ccrs;
	virtual BOOL OnInitDialog();
	void ResetList();
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnBnClickedButtonDelete();
	
};
