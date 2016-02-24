#pragma once


// COrderView
#include "MyListCtrl.h"
#include <string>
#include <vector>
#include "struct_helper.h"
#include "MyBCRequest.h"

class COrderViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class COrderView : public CDockablePane
{
	DECLARE_DYNAMIC(COrderView)

public:
	COrderView();
	virtual ~COrderView();

protected:
	DECLARE_MESSAGE_MAP()

private:
	CMyListCtrl m_listctrl;
	COrderViewToolBar m_wndToolBar;
	//UINT m_nCurrSort;
	int m_ComboCurSel;
	std::string m_CurCustNo;
	std::vector<cust_order_st> m_vOrder;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void COrderView::AdjustLayout();
	void ResetList();
	BCResult RequestCancelOrder_854018(BCHANDLE handle, cust_order_st &odr);
protected:
	afx_msg void OnClickComboBox() {}
	afx_msg void OnComboBoxSelChangeClick();
	afx_msg LRESULT OnUserCustSelChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserCustDynamicInfo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCancelOrder();
	afx_msg void OnCancelOrderUpdateUI(CCmdUI* pCmdUI);
	afx_msg void OnCancelAllOrder();
	afx_msg void OnCancelAllOrderUpdateUI(CCmdUI* pCmdUI);
};


