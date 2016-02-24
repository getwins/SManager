#pragma once

#include <string>
// CCustInfoWnd
#include "struct.h"
class CCustInfoWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CCustInfoWnd)

public:
	CCustInfoWnd();
	virtual ~CCustInfoWnd();

protected:
	CFont m_fntPropList;
	CMFCPropertyGridCtrl m_wndPropList;
	std::string m_CurCustNo;

	CMFCPropertyGridProperty *m_pCustNo;
	CMFCPropertyGridProperty *m_pCustName;
	CMFCPropertyGridProperty *m_pCustStatus;
	CMFCPropertyGridProperty *m_pCustType;

	CMFCPropertyGridProperty *m_pLoginStatus;
	CMFCPropertyGridProperty *m_pLoginDate;
	CMFCPropertyGridProperty *m_pLoginTime;
	CMFCPropertyGridProperty *m_pLoginIp;
	CMFCPropertyGridProperty *m_pLoginApp;

	CMFCPropertyGridProperty *m_pCommissionNo;
	CMFCPropertyGridProperty *m_pCommissionName;

	CMFCPropertyGridProperty *m_pMarginNo;
	CMFCPropertyGridProperty *m_pMarginName;

	CMFCPropertyGridProperty *m_pPosi;
	CMFCPropertyGridProperty *m_pYdPosi;
	CMFCPropertyGridProperty *m_pTdPosi;
	CMFCPropertyGridProperty *m_pFloatProfit;
	CMFCPropertyGridProperty *m_pPosiInst;

	CMFCPropertyGridProperty *m_pClear;
	CMFCPropertyGridProperty *m_pLowerCapitalForce;;

	DECLARE_MESSAGE_MAP()
public:
	void InitPropList();
	void SetPropListFont();
	void AdjustLayout();

	bool RequestCustInfoMaintenance_851001(cust_base_info_st cbi);

	virtual void Dump(CDumpContext& dc) const;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	afx_msg LRESULT OnUserCustSelChanged(WPARAM wParam, LPARAM lParam);
	void SetPositionInfo();
	void SetBaseInfo();
	afx_msg LRESULT OnAfxWmPropertyChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserCustBaseInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserCustDynamicInfo(WPARAM wParam, LPARAM lParam);
};


