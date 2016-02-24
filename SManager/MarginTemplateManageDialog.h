#pragma once
#include "afxcmn.h"


// CMarginTemplateManageDialog 对话框

class CMarginTemplateManageDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMarginTemplateManageDialog)

public:
	CMarginTemplateManageDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMarginTemplateManageDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MARGIN_TEMPLATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	bool RequestTemplate_851346(char *flag, char *temp_no, char *temp_name);

	void ResetList1();
	void ResetList2(CString mtno);

public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listctrl1;
	CListCtrl m_listctrl2;
	afx_msg void OnLvnItemchangedListMarginTemplate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAddTemplate();
	afx_msg void OnBnClickedButtonDeleteTemplate();
	afx_msg void OnBnClickedButtonAddMargin();
	afx_msg void OnBnClickedButtonModifyMargin();
	afx_msg void OnBnClickedButtonDeleteMargin();
};
