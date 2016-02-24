#pragma once


// CModifyOperPasswdDialog 对话框

class CModifyOperPasswdDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyOperPasswdDialog)

public:
	CModifyOperPasswdDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModifyOperPasswdDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODIFY_OPER_PASSWD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_OperTitle;
	CString m_OldPasswd;
	
	CString m_NewConfirm;
	CBrush m_YellowBr;
public:
	CString m_NewPasswd;
	afx_msg void OnBnClickedOk();
	bool RequestModifyOperPasswd_851210();
	afx_msg void OnEnChangeEditNewConfirm();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnChangeEditNewPasswd();
};
