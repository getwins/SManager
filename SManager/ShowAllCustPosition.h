#pragma once
#include "afxcmn.h"


// CShowAllCustPosition 对话框
#include "struct_helper.h"

//struct depth_market_data_st;
//
//class CSAPListctrl : public CListCtrl
//{
//	DECLARE_DYNAMIC(CSAPListctrl)
//	DECLARE_MESSAGE_MAP()
//
////	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//	virtual void PreSubclassWindow();
//public:
//	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
//};

class CShowAllCustPosition : public CDialogEx
{
	DECLARE_DYNAMIC(CShowAllCustPosition)

public:
	CShowAllCustPosition(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowAllCustPosition();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHOW_ALL_CUST_POSOTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//void SetCustsPoition(const std::map<std::string, std::vector<cust_position_st>> &custPosition);
	void ShowPosition();
	//void UpdateCustPositionProfit(int posiid, double profit);
	void UpdateInstMarket(const std::string &inst);
private:
	//CSAPListctrl m_listctrl;
	CListCtrl m_listctrl;
	CustPosi_t m_mCustPosition;
	PosiVec_t m_vPosi;
public:
//	afx_msg void OnNMRClickListAllCustPosition(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnContextMenu(CWnd* /* pWnd */, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnCloseAll();
//	afx_msg void OnUpdateUICloseAll(CCmdUI* pCmdUI) {};
	afx_msg void OnCloseSingle();
};
