#pragma once


// CAddTemplateDialog �Ի���

class CAddTemplateDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAddTemplateDialog)

public:
	CAddTemplateDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddTemplateDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADD_TEMPLATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_TempNo;
	CString m_TempName;
	afx_msg void OnBnClickedOk();
};
