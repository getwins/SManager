#pragma once


// COperLoginDialog �Ի���

class COperLoginDialog : public CDialogEx
{
	DECLARE_DYNAMIC(COperLoginDialog)

public:
	COperLoginDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COperLoginDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPER_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_OperCode;
	CString m_OperPasswd;
};
