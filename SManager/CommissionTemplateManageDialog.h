#pragma once
#include "afxcmn.h"


// CCommissionTemplateManageDialog 对话框
//#include "MyListCtrl.h"

class CCommissionTemplateManageDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCommissionTemplateManageDialog)

public:
	CCommissionTemplateManageDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCommissionTemplateManageDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_COMMISSION_TEMPLATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ResetList1();
	void ResetList2(CString ctno);
	bool RequestAddTemplate_851344(CString temp_no, CString temp_name);
	bool RequestDelTemplate_851344(CString temp_no);
private:
	CListCtrl m_listctrl1;
	CListCtrl m_listctrl2;
	//CMyListCtrl m_listctrl1;
	//CMyListCtrl m_listctrl2;
public:
//	afx_msg void OnNMClickListCommissionTemplate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListCommissionTemplate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAddTemplate();
	afx_msg void OnBnClickedButtonDeleteTemplate();
	afx_msg void OnBnClickedButtonAddCommission();
	afx_msg void OnBnClickedButtonModifyCommission();
	afx_msg void OnBnClickedButtonDeleteCommission();
};
