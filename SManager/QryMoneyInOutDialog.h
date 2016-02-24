#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "ATLComTime.h"
#include "afxcmn.h"

#include "KVhyphenComboBox.h"

// CQryMoneyInOutDialog 对话框
struct cust_money_in_out_st
{
	char date[9];
	int serial;
	char cust_no[16];
	char currency[2];
	char type[2];
	char money_type[2];
	double in_amt;
	double out_amt;
	char time[9];
};

class CQryMoneyInOutDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CQryMoneyInOutDialog)

public:
	CQryMoneyInOutDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQryMoneyInOutDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MONEY_QUERY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	
//	CDateTimeCtrl m_DateBegin;
	COleDateTime m_DateBegin;
	COleDateTime m_DateEnd;
	CListCtrl m_listctrl;

	std::vector<cust_money_in_out_st> m_vMoney;
public:
	CCustComboBox m_ctlCust;
	afx_msg void OnBnClickedButtonQuery();
	virtual BOOL OnInitDialog();
	bool RequestMoneyInOut_851403();
	
};
