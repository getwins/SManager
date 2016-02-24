#pragma once
#include "afxwin.h"

#include "KVhyphenComboBox.h"
// CCustTemplateSettingDialog �Ի���

class CCustTemplateSettingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCustTemplateSettingDialog)

public:
	CCustTemplateSettingDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCustTemplateSettingDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUST_TEMPLATE_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CCustComboBox m_ctlCust;
	afx_msg void OnBnClickedCheckCommission();
	afx_msg void OnBnClickedCheckMargin();
	CCTComboBox m_ctlCommission;
	CMTComboBox m_ctlMargin;
	BOOL m_IsSettingCommission;
	BOOL m_IsSettingMargin;
	afx_msg void OnCbnSelchangeComboCust();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	bool SetCustCommissionTemplate_851342(char *cust_no, char* template_no, char *flag);
	bool SetCustMarginTemplate_851348(char *cust_no, char* template_no, char *flag);
};
