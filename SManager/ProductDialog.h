#pragma once
#include "afxwin.h"

//#include "ExchangeComboBox.h"
// CProductDialog �Ի���
#include "KVhyphenComboBox.h"
class CProductDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CProductDialog)

public:
	CProductDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProductDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRODUCT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
