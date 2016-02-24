#pragma once

#include <set>

// CMarketWnd
#include "MyListCtrl.h"



class CMarketWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CMarketWnd)

public:
	CMarketWnd();
	virtual ~CMarketWnd();

protected:
	CMyListCtrl m_listctrl;
	//CThostFtdcMdApi *m_pUserApi;
	std::set<std::string> m_SubscribeInst;
	DECLARE_MESSAGE_MAP()
public:
	void AdjustLayout();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnUserCustDynamicInfo(WPARAM wParam, LPARAM lParam);
protected:
	afx_msg LRESULT OnUserInstMarket(WPARAM wParam, LPARAM lParam);
};


