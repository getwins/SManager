#pragma once


// CAddTemplateDialog 对话框

class CAddTemplateDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAddTemplateDialog)

public:
	CAddTemplateDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddTemplateDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADD_TEMPLATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_TempNo;
	CString m_TempName;
	afx_msg void OnBnClickedOk();
};
