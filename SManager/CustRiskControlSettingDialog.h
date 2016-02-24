#pragma once
#include "afxwin.h"
#include "KVhyphenComboBox.h"


// CCustRiskControlSettingDialog 对话框

class CCustRiskControlSettingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCustRiskControlSettingDialog)

public:
	CCustRiskControlSettingDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCustRiskControlSettingDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUST_RISK_CONTROL_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CCustComboBox m_ctlCust;
//	CButton m_btnAllow;
	CEdit m_ctlCapital;
	afx_msg void OnBnClickedCheckNight();
	afx_msg void OnBnClickedCheckForce();
	BOOL m_bClear;
	BOOL m_bForce;
	afx_msg void OnCbnSelchangeComboCust();
	afx_msg void OnBnClickedSet();
	afx_msg void OnBnClickedDelete();
	virtual BOOL OnInitDialog();
//	BOOL m_bAllow;
};
