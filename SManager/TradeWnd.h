#pragma once


// CTradeWnd
#include <string>

class CTradeWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CTradeWnd)

public:
	CTradeWnd();
	virtual ~CTradeWnd();

	CFont m_fntPropList;
	CMFCPropertyGridCtrl m_wndPropList;
	CMFCButton m_wndButton;
	//CButton m_wndButton;
	//CMFCCaptionButton m_wndButton;
	std::string m_CurCustNo;
	CMFCPropertyGridProperty *m_pCustProp;
	CMFCPropertyGridProperty *m_pInstProp;
	CMFCPropertyGridProperty *m_pDirectionProp;
	CMFCPropertyGridProperty *m_pOffsetProp;
	CMFCPropertyGridProperty *m_pVolProp;
	CMFCPropertyGridProperty *m_pPriceProp;
protected:
	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
	void AdjustLayout();

	bool Request_854057();//"查询可开手数"
	bool Request_854058();//"查询可平数量"
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg LRESULT OnUserCustSelChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonOrder();
	afx_msg void OnUpdateButtonUI(CCmdUI* pCmdUI);

//	afx_msg LRESULT OnUserRequestInstDone(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
};


