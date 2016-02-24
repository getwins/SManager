#pragma once
#include "afxwin.h"
#include "ATLComTime.h"


// CQryOrderDialg 对话框
#include "KVhyphenComboBox.h"
#include "afxcmn.h"
#include "struct.h"

class CQryOrderDialg : public CDialogEx
{
	DECLARE_DYNAMIC(CQryOrderDialg)

public:
	CQryOrderDialg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQryOrderDialg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QUERY_ORDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	//CComboBox m_ctlCust;
	std::vector<cust_order_st> m_vOrder;
	
	COleDateTime m_DateTime;
	CListCtrl m_listctrl;

public:
	CCustComboBox m_ctlCust;
	afx_msg void OnBnClickedButtonQuery();
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	bool RequestOrder_852102();

	
};
