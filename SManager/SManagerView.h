
// SManagerView.h : CSManagerView 类的接口
//

#pragma once
#include "MyListCtrl.h"
#include <map>
#include "MdApi.h"
#include "struct_helper.h"

class CSManagerViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};


class CSManagerView : public CView
{
protected: // 仅从序列化创建
	CSManagerView();
	DECLARE_DYNCREATE(CSManagerView)

// 特性
public:
	CSManagerDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	//virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	//virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CSManagerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
private:
	CMyListCtrl m_listctrl;
	CSManagerViewToolBar m_wndToolBar;
	//CMFCToolBar m_wndToolBar;
	//CMFCStatusBar     m_wndStatusBar;
	//CListCtrl m_listctrl;
	//CMFCListCtrl m_listctrl;

	std::vector<cust_base_info_st> m_CurCBIs;
	std::map<std::string, cust_login_info_st> m_mLoginInfo;
	std::map<std::string, cust_capital_st> m_mCapital;
	//std::map<std::string, std::vector<cust_td_position_detail_st>> m_mCustPosiDetail;
	//std::map<std::string, depth_market_data_st> m_mDmd;
	//std::map<std::string, std::vector<cust_position_st>> m_mCustPosition;
	CustPosi_t m_mCustPosition;
	int m_CurSel;
	//bool m_MsgBee;
// 生成的消息映射函数
protected:
//	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	afx_msg void OnNMDblclkListCust(NMHDR *pNMHDR, LRESULT *pResult);

	//bool isInstInCustPosition(const std::string &inst, const std::string &cust_no);
	//bool canCalcCustFloatProfit(const std::string &cust_no);
	//double CalcCustFloatProfit(const std::string &cust_no);

	void AdjustCustCapital(const std::string &cust_no, double float_profit);
	void ResetListCustBaseInfo(const cust_base_info_st &cbi);
	void HandleCustBaseInfo_CountChanged();
	void HandleCustBaseInfo_InfoChanged(const std::string &cust_no);

	//void SetListCustLoginInfo(const std::string &cust_no, const cust_login_info_st &cli);
	void HandleCustDynamicInfo_LoginInfo(const std::string &cust_no);
	void HandleCustDynamicInfo_Capital(const std::string &cust_no);
	//void _HandleCustDynamicInfo_Capital(const std::string &cust_no);
	//void HandleCustDynamicInfo_PosiDetail(const std::string &cust_no);
	void HandleCustDynamicInfo_Position(const std::string &cust_no);
	void HandleInstMarket(const std::string &inst);
	//void HnadleCustPositionFloatProfit(const cust_position_id_st &posi_id, double profit);



	cbi_iter_t FindCurCBI(CString cust_no);

	afx_msg void OnMoneyInOut();
//	afx_msg void OnFrozenCust();
	afx_msg void OnCustStatusMaintain();
	afx_msg void OnOrderQuery();
	afx_msg void OnPositionQuery();
	afx_msg void OnTradeQuery();
	afx_msg void OnQueryMoneyInOut();
	afx_msg void OnSettlementQuery();
	afx_msg void OnResetTradePasswd();
	afx_msg void OnResetFundPasswd();
	bool RequestCancelCust_851018(CString cust_no);
	afx_msg void OnCancelCust();
	afx_msg void OnCustTemplateSetting();
protected:
//	afx_msg LRESULT OnCustDynamicPosiDetailChanged(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnUserCustDynamicInfo(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnUserCustPositionFloatProfit(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnCustRiskControlSetting();
	afx_msg void OnSummaryFloatProfit() {};

	void DisplaySummary();
	afx_msg void OnFpCaption();
	afx_msg void OnOpCaption();
	afx_msg void OnCommissionCaption();
	afx_msg void OnSummaryCaption();
	//afx_msg void OnMessagebee();
	//afx_msg void OnUpdateMessagebee(CCmdUI *pCmdUI);
	afx_msg void OnCustCommissionRateSetting();
	afx_msg void OnCustMarginRateSetting();
	afx_msg void OnBankaccout();
	afx_msg void OnExportCustinfo();
};

#ifndef _DEBUG  // SManagerView.cpp 中的调试版本
inline CSManagerDoc* CSManagerView::GetDocument() const
   { return reinterpret_cast<CSManagerDoc*>(m_pDocument); }
#endif

