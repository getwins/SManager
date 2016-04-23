// MyListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "MyListCtrl.h"

#include <boost/lexical_cast.hpp>
// CMyListCtrl

IMPLEMENT_DYNAMIC(CMyListCtrl, CMFCListCtrl)

CMyListCtrl::CMyListCtrl()
{

}

CMyListCtrl::~CMyListCtrl()
{
}

void CMyListCtrl::SetCellTextColor(int row, int col, COLORREF clr)
{
	LVCELL *cell;
	if (cell = find_lvcell(row, col)) {
		//if ((cell->mask && LVCELL_MASK_COLOR) == 0)
		//{
			cell->mask |= LVCELL_MASK_COLOR;
		//}
		cell->text_clr = clr;
	}
	else {
		LVCELL c = { 0 };
		c.mask |= LVCELL_MASK_COLOR;
		c.row = row;
		c.col = col;
		c.text_clr = clr;
		m_vlvcell.push_back(c);
	}
}

void CMyListCtrl::SetCellFontWeigth(int row, int col, int fw)
{
	LVCELL *cell;
	if (cell = find_lvcell(row, col)) {
		//cell->mask |= LVCELL_MASK_FONT_WEIGHT;
		//if ((cell->mask && LVCELL_MASK_FONT_WEIGHT) == 0)
		//{
			cell->mask |= LVCELL_MASK_FONT_WEIGHT;
		//}
		cell->fw = fw;
	}
	else {
		LVCELL c = { 0 };
		c.mask |= LVCELL_MASK_FONT_WEIGHT;
		c.row = row;
		c.col = col;
		c.fw = fw;
		m_vlvcell.push_back(c);
	}
}

BOOL CMyListCtrl::DeleteAllItems()
{
	m_vlvcell.clear();
	return CMFCListCtrl::DeleteAllItems();
}

COLORREF CMyListCtrl::OnGetCellTextColor(int nRow, int nColum)
{
	LVCELL *cell;
	if (cell = find_lvcell(nRow, nColum)) {
		//return cell->text_clr;
		if (cell->mask & LVCELL_MASK_COLOR)
			return cell->text_clr;
	}
	return GetTextColor();

}

HFONT CMyListCtrl::OnGetCellFont(int nRow, int nColum, DWORD dwData)
{
	LVCELL *cell;
	if (cell = find_lvcell(nRow, nColum)) {
		if (cell->mask & LVCELL_MASK_FONT_WEIGHT) {
			FontWeithtMap::iterator it = m_mfw.find(cell->fw);
			if (it != m_mfw.end())
				return *(it->second);
		}
	}
	return NULL;
}

int CMyListCtrl::OnCompareItems(LPARAM lParam1, LPARAM lParam2, int iColumn)
{
	//return CMFCListCtrl::CompareProc(lParam1, lParam2, iColumn);
	CString strItem1, strItem2;
	LVFINDINFO lvfi;

	lvfi.flags = LVFI_PARAM;
	lvfi.lParam = lParam1;
	strItem1 = GetItemText(FindItem(&lvfi, -1), iColumn);

	lvfi.lParam = lParam2;
	strItem2 = GetItemText(FindItem(&lvfi, -1), iColumn);

	try {
		double d1 = boost::lexical_cast<double>(strItem1.GetBuffer());
		double d2 = boost::lexical_cast<double>(strItem2.GetBuffer());
		if (d1 > d2)
			return 1;
		else if (d1 < d2)
			return -1;
	}
	catch (std::exception &err)
	{
		return strcmp(strItem1, strItem2);
	}
	//字符串比较
	

	return 0;
}


BEGIN_MESSAGE_MAP(CMyListCtrl, CMFCListCtrl)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CMyListCtrl 消息处理程序


//#define FW_DONTCARE         0
//#define FW_THIN             100
//#define FW_EXTRALIGHT       200
//#define FW_LIGHT            300
//#define FW_NORMAL           400
//#define FW_MEDIUM           500
//#define FW_SEMIBOLD         600
//#define FW_BOLD             700
//#define FW_EXTRABOLD        800
//#define FW_HEAVY            900

static int fw_array[] = {
	FW_THIN,
	FW_EXTRALIGHT,
	FW_LIGHT,
	FW_NORMAL,
	FW_MEDIUM,
	FW_SEMIBOLD,
	FW_BOLD,
	FW_EXTRABOLD,
	FW_HEAVY
};

LVCELL * CMyListCtrl::find_lvcell(int row, int col)
{
	//for each(LVCELL var in m_vlvcell) {
	//	if(var.row == row && var.col = col)
	//		
	//}
	for (std::vector<LVCELL>::iterator it = m_vlvcell.begin(); it != m_vlvcell.end(); ++it) {
		if (it->row == row && it->col == col)
			return &(*it);
	}
	return NULL;
}

//void CMyListCtrl::PreSubclassWindow()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
//	//LONG style = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
//	style &= ~LVS_TYPEMASK;
//	style |= LVS_REPORT;
//	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);
//
//	DWORD styleex = GetExtendedStyle();
//	styleex |= LVS_EX_GRIDLINES;
//	styleex |= LVS_EX_DOUBLEBUFFER;
//	styleex |= LVS_EX_FULLROWSELECT;
//	SetExtendedStyle(styleex);
//
//	LOGFONT lf;
//	GetFont()->GetLogFont(&lf);
//	for (int i = 0; i < sizeof(fw_array) / sizeof(fw_array[0]); i++)
//	{
//		std::shared_ptr<CFont> font(new CFont);
//		lf.lfWidth = fw_array[i];
//		font->CreateFontIndirect(&lf);
//		m_mfw.insert(std::make_pair(fw_array[i], font));
//	}
//
//	CMFCListCtrl::PreSubclassWindow();
//}


int CMyListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	LONG style = GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	//LONG style = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
	style &= ~LVS_TYPEMASK;
	style |= LVS_REPORT;
	SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);

	DWORD styleex = GetExtendedStyle();
	styleex |= LVS_EX_GRIDLINES;
	//styleex |= LVS_EX_DOUBLEBUFFER;
	styleex |= LVS_EX_FULLROWSELECT;
	SetExtendedStyle(styleex);

	LOGFONT lf;
	GetFont()->GetLogFont(&lf);
	for (int i = 0; i < sizeof(fw_array) / sizeof(fw_array[0]); i++)
	{
		std::shared_ptr<CFont> font(new CFont);
		lf.lfWeight = fw_array[i];
		font->CreateFontIndirect(&lf);
		m_mfw.insert(std::make_pair(fw_array[i], font));
	}
	return 0;
}
