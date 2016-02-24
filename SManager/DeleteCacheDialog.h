#pragma once
#include "afxwin.h"


// CDeleteCacheDialog 对话框

class CDeleteCacheDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CDeleteCacheDialog)

public:
	CDeleteCacheDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDeleteCacheDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEL_CACHE_FILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CListBox m_ctlOutput;
public:
	afx_msg void OnBnClickedButtonDelete();
	//bool DeleteCacheFiel(const char *file);
};
