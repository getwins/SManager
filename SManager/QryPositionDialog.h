#pragma once
#include "afxwin.h"
#include "ATLComTime.h"
#include "afxcmn.h"

#include "KVhyphenComboBox.h"

// CQryPositionDialog �Ի���

class CQryPositionDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CQryPositionDialog)

public:
	CQryPositionDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQryPositionDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QUERY_POSITION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	//CComboBox m_ctlCust;
	
	COleDateTime m_DateTime;
	CListCtrl m_listctrl;
	std::vector<cust_position_st> m_vPosition;
public:
	CCustComboBox m_ctlCust;
	afx_msg void OnBnClickedButtonQuery();
	virtual BOOL OnInitDialog();

	bool RequestPosition_851401();
};
