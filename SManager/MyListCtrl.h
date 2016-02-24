#pragma once

#include <map>
#include <vector>
#include <memory>
// CMyListCtrl
#define LVCELL_MASK_COLOR 0x00000001
#define LVCELL_MASK_FONT_WEIGHT 0x0000002

typedef struct _tag_LVCELL_ADDITIONAL
{
	int mask;
	int row;
	int col;
	COLORREF text_clr;
	int fw; //font weight;
	//CFont font;
}LVCELL;

typedef std::map<int, std::shared_ptr<CFont>> FontWeithtMap;

#define COLOR_BLACK		RGB(0, 0, 0)
#define COLOR_RED		RGB(255, 0, 0)
#define COLOR_GREEN		RGB(0, 255, 0)
#define COLOR_BLUE		RGB(0, 0, 255)
#define COLOR_YELLOW	RGB(255,255, 0)

class CMyListCtrl : public CMFCListCtrl
{
	DECLARE_DYNAMIC(CMyListCtrl)

public:
	CMyListCtrl();
	virtual ~CMyListCtrl();

	void SetCellTextColor(int row, int col, COLORREF clr);
	void SetCellFontWeigth(int row, int col, int fw);
	BOOL DeleteAllItems();
	// Support for individual cells text/background colors:
	virtual COLORREF OnGetCellTextColor(int nRow, int nColum); //{ return GetTextColor(); }
	//virtual COLORREF OnGetCellBkColor(int nRow, int nColum); //{ return GetBkColor(); }
	virtual HFONT OnGetCellFont(int nRow, int nColum, DWORD dwData = 0); //{ return NULL; }

	FontWeithtMap m_mfw;
	std::vector<LVCELL> m_vlvcell;
	LVCELL * find_lvcell(int row, int col);
protected:
	DECLARE_MESSAGE_MAP()
//	virtual void PreSubclassWindow();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


