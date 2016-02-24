#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CCustMarginDialog �Ի���
#include "KVhyphenComboBox.h"
#include "struct_helper.h"

class CCustMarginDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCustMarginDialog)

public:
	CCustMarginDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCustMarginDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUST_MARGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
