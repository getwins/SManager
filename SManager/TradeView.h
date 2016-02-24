#pragma once
#include "MyListCtrl.h"
#include <string>
#include <vector>
#include "struct_helper.h"
class CTradeViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};
// CTradeView

class CTradeView : public CDockablePane
{
	DECLARE_DYNAMIC(CTradeView)

public:
	CTradeView();
	virtual ~CTradeView();

protected:
	CMyListCtrl m_listctrl;
	//CPositionFormView *m_pPositionFormView;
	//CPositionDialog m_dialog;

	CTradeViewToolBar m_wndToolBar;

	int m_ComboCurSel;
	std::string m_CurCustNo;
	TradeVec_t m_vTrade;
	TradeVec_t m_vSummary;
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void AdjustLayout();
	void SummaryTrade();
	void ResetList();
protected:
	afx_msg LRESULT OnUserCustSelChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserCustDynamicInfo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnComboBoxSelChangeClick();
	afx_msg void OnClickComboBox() {}
};


