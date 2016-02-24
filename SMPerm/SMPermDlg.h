
// SMPermDlg.h : ͷ�ļ�
//

#pragma once
#include "ATLComTime.h"

#include "MyBCRequest.h"

// CSMPermDlg �Ի���
class CSMPermDlg : public CDialogEx
{
// ����
public:
	CSMPermDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMPERM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	Scoped_BCHANDLE m_handle;
	CString m_OperCode;
//	int m_CustNum;
//	COleDateTime m_DateFrom;
//	COleDateTime m_DateTo;
	afx_msg void OnBnClickedBtnQuery();
	afx_msg void OnBnClickedSet();
	afx_msg void OnBnClickedDel();
	CString m_CustNum;
	CString m_DateFrom;
	CString m_DateTo;
};
