#pragma once
#include "afxwin.h"
#include "KVhyphenComboBox.h"

// CCommissionTemplateSettingDialog 对话框
#include "struct.h"

class CCommissionTemplateSettingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCommissionTemplateSettingDialog)

public:
	CCommissionTemplateSettingDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCommissionTemplateSettingDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMISSION_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
public:
	//std::vector<commission_template_st>::iterator m_ctit;
	CString m_flag;
	CCTComboBox m_ctlCommission;
	CExchangeComboBox m_ctlExchange;
	CProductComboBox m_ctlProduct;
	CString m_date;
	double m_TdByAmt;
	double m_ByAmt;
	double m_TdByVol;
	double m_ByVol;
	afx_msg void OnBnClickedOk();

	bool RequestCommissionSetting_851338(CString &ctno, CString &exchangeid, CString &productid);
	afx_msg void OnCbnSelchangeComboExchange();
	
};
