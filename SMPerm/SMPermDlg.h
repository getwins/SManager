
// SMPermDlg.h : 头文件
//

#pragma once
#include "ATLComTime.h"

#include "MyBCRequest.h"

// CSMPermDlg 对话框
class CSMPermDlg : public CDialogEx
{
// 构造
public:
	CSMPermDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMPERM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
