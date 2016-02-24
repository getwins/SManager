#pragma once
#include "afxcmn.h"
#include "afxwin.h"

//#include "ExchangeComboBox.h"
// CInstrumentDialog �Ի���
#include "KVhyphenComboBox.h"

class CInstrumentDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CInstrumentDialog)

public:
	CInstrumentDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInstrumentDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSTRUMENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listctrl;
	virtual BOOL OnInitDialog();
//	afx_msg void OnNMDblclkListInstrument(NMHDR *pNMHDR, LRESULT *pResult);
private:
	//CComboBox m_ctlExchange;
	CExchangeComboBox m_ctlExchange;
	CProductComboBox m_ctlProduct;
	CComboBox m_ctlStatus;
public:
	afx_msg void OnCbnSelchangeComboExchange();
	afx_msg void OnCbnSelchangeComboProduct();
	afx_msg void OnCbnSelchangeComboTradeStatus();

	void ResetList();
};
