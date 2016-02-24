#pragma once
#include "afxwin.h"


// CMarginTemplateSettingDialog 对话框
#include "KVhyphenComboBox.h"
class CMarginTemplateSettingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMarginTemplateSettingDialog)

public:
	CMarginTemplateSettingDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMarginTemplateSettingDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MARGIN_SETTING };
#endif
	
	CString m_flag;
	//std::vector<margin_template_st>::iterator m_mtit;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//CComboBox m_ctlMargin;
	//CComboBox m_ctlExchange;
	//CComboBox m_ctlProduct;
	CMTComboBox m_ctlMargin;
	CExchangeComboBox m_ctlExchange;
	CProductComboBox m_ctlProduct;
	double m_ByAmt;
//	CEdit m_ByVol;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	double m_ByVol;

	bool RequestMargin_851340(char *mtno, char *exchangeid, char *productid);
	afx_msg void OnCbnSelchangeComboExchange();
};
