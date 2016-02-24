#pragma once


// COperLoginDialog 对话框

class COperLoginDialog : public CDialogEx
{
	DECLARE_DYNAMIC(COperLoginDialog)

public:
	COperLoginDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COperLoginDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPER_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_OperCode;
	CString m_OperPasswd;
};
