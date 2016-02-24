
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
//#include "FileView.h"
//#include "ClassView.h"
#include "PositionView.h"
#include "OrderView.h"
#include "TradeView.h"

#include "CustInfoWnd.h"
#include "TradeWnd.h"
#include "MarketWnd.h"
#include "OutputWnd.h"
//#include "PropertiesWnd.h"
//#include "struct.h"
#include "ShowAllCustPosition.h"



class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	//CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	//CFileView         m_wndFileView;
	//CClassView        m_wndClassView;


	CTabbedPane m_wndTabbe1;
	CPositionView m_wndPosition;
	COrderView m_wndOrder;
	CTradeView m_wndTradeQry;

	CTabbedPane m_wndTabbe2;
	CTradeWnd m_wndTrade;
	CMarketWnd m_wndMarket;

	CCustInfoWnd m_wndCustInfo;
	COutputWnd        m_wndOutput;
	//CPropertiesWnd    m_wndProperties;

	//system_info_st m_SysInfo;

	//std::map<std::string, std::vector<cust_td_position_detail_st>> m_mCustPosiDetail;
	CShowAllCustPosition m_ShowAllCustPosiDlg;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnViewCustomize();
	//afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

	//void calcCustFloatProfit(const std::string &inst);
	
protected:
//	afx_msg LRESULT OnUserOuput(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserOutput(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserCustDynamicInfo(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnUserCustBaseInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserCustSelChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserInstMarket(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnUserRequestInstDone(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnShowAllCustPosition();
};


