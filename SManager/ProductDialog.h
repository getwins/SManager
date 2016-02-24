#pragma once
#include "afxwin.h"

//#include "ExchangeComboBox.h"
// CProductDialog 对话框
#include "KVhyphenComboBox.h"
class CProductDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CProductDialog)

public:
	CProductDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProductDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRODUCT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listctrl;
	//CComboBox m_ComboExchange;
	CExchangeComboBox m_ComboExchange;
	//CString m_ExchangeId;
	//CString m_ProductStatus;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboExchange();
	void ResetList();
private:
	CComboBox m_ctlStatus;
public:
	afx_msg void OnCbnSelchangeComboTradeStatus();
};
