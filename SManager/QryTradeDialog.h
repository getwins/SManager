#pragma once
#include "afxwin.h"
#include "ATLComTime.h"
#include "afxcmn.h"

#include "KVhyphenComboBox.h"

// CQryTradeDialog �Ի���

class CQryTradeDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CQryTradeDialog)

public:
	CQryTradeDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQryTradeDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QUERY_TRADE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	
	COleDateTime m_DateTime;
	CListCtrl m_listctrl;

	std::vector<cust_trade_st> m_vTrade;
public:
	CCustComboBox m_ctlCust;
	afx_msg void OnBnClickedButtonQuery();
	bool RequestTrade_852103();
};
