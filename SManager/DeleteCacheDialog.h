#pragma once
#include "afxwin.h"


// CDeleteCacheDialog �Ի���

class CDeleteCacheDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CDeleteCacheDialog)

public:
	CDeleteCacheDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDeleteCacheDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEL_CACHE_FILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CListBox m_ctlOutput;
public:
	afx_msg void OnBnClickedButtonDelete();
	//bool DeleteCacheFiel(const char *file);
};
