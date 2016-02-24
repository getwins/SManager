#pragma once


// CPositionView
#include "MyListCtrl.h"
//#include "PositionFormView.h"
//#include "PositionDialog.h"
#include <string>
#include <vector>
#include "struct_helper.h"
#include "nbccclt.h"


class CPositionViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CPositionView : public CDockablePane
{
	DECLARE_DYNAMIC(CPositionView)

public:
	CPositionView();
	virtual ~CPositionView();

protected:
	DECLARE_MESSAGE_MAP()
private:
	CMyListCtrl m_listctrl;
	//CPositionFormView *m_pPositionFormView;
	//CPositionDialog m_dialog;
	
	CPositionViewToolBar m_wndToolBar;
	//CMFCToolBar m_wndToolBar;
	//CMFCToolBarComboBoxButton m_wndToolBarCombox;

	//UINT m_nCurrSort;
	int m_ComboCurSel;
	std::string m_CurCustNo;
	PosiVec_t m_vPosition;
	PosiDetailVec_t m_vPosiDetail;
	//inst_market_st m_InstMarket;
	//CMFCToolBar m_wndToolBar;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnClickComboBox(){}
	afx_msg void OnComboBoxSelChangeClick();
	//afx_msg void OnNewFolder();
	//afx_msg void OnSort(UINT id);
	//afx_msg void OnUpdateSort(CCmdUI* pCmdUI);
	//afx_msg void OnShowPosition();
	//afx_msg void OnShowPosiDetail();
	afx_msg void OnOffsetDJ();
	afx_msg void OnOffsetSJ();
	afx_msg void OnOffsetAll();

	void AdjustLayout();
	void OnChangeVisualStyle();

	void ResetList();

	//price_type '1－市价' '2-最优价'
	bool RequestOffsetPosition_854002(char *PriceType);
	//bool RequestMarketData_851503(BCHANDLE handle, std::string inst);

	void initPosiDetailLastPrice();
protected:
	afx_msg LRESULT OnUserCustSelChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserCustDynamicInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserInstMarket(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnUserCustPositionFloatProfit(WPARAM wParam, LPARAM lParam);
};
//
//function 851503 description  "根据品种和交割期,查询行情信息"
///* 输入：操作员，品种，交割期，短信查询标志 */
//input:sorder2, sstock_code, sdate0, sstatus1
///* 输出：最新价，买价，卖价，涨跌，最高价，最低价，开盘价，涨停版，跌停版，买量，卖量，成交量，净持仓，昨收盘，今结算，史最高，史最低 */
//output : damt1, damt2, damt3, damt4, damt5, damt6, damt7, damt8, damt9, lvol1, lvol2, lvol3, lvol4, damt10, damt11, damt12, damt13
//
