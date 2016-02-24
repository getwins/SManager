// ProductDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "ProductDialog.h"
#include "afxdialogex.h"
//#include <boost/lexical_cast.hpp>

// CProductDialog 对话框
#include "SMutility.h"


IMPLEMENT_DYNAMIC(CProductDialog, CDialogEx)

CProductDialog::CProductDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PRODUCT, pParent)
{

}

CProductDialog::~CProductDialog()
{
}

void CProductDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRODUCT, m_listctrl);
	DDX_Control(pDX, IDC_COMBO_EXCHANGE, m_ComboExchange);
	DDX_Control(pDX, IDC_COMBO_TRADE_STATUS, m_ctlStatus);
}


BEGIN_MESSAGE_MAP(CProductDialog, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_EXCHANGE, &CProductDialog::OnCbnSelchangeComboExchange)
	ON_CBN_SELCHANGE(IDC_COMBO_TRADE_STATUS, &CProductDialog::OnCbnSelchangeComboTradeStatus)
END_MESSAGE_MAP()


// CProductDialog 消息处理程序


BOOL CProductDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//m_listctrl
	//m_listctrl.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT, RECT(), this, ID_POSITION_LISTCTRL);
	//LONG style = GetWindowLong(m_listctrl.GetSafeHwnd(), GWL_STYLE);
	//style &= ~LVS_TYPEMASK;
	//style |= LVS_REPORT;
	//SetWindowLong(m_listctrl.GetSafeHwnd(), GWL_STYLE, style);

	//DWORD styleex = m_listctrl.GetExtendedStyle();
	//styleex |= LVS_EX_GRIDLINES;
	//styleex |= LVS_EX_DOUBLEBUFFER;
	//m_listctrl.SetExtendedStyle(styleex);

	LVCOLUMN cols[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "字头" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "名称" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "交易规则" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 200, "合约规则" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "币种" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "每手数量" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "最小变动价" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "最小手数" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "最大手数" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "最大持仓" }
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "涨停板率" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "跌停板率" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "手续费(手数)" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "手续费(金额)" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "保证金(手数）" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "保证金(金额)" }
	};

	for (int i = 0; i < sizeof(cols) / sizeof(cols[0]); ++i)
		m_listctrl.InsertColumn(i, &cols[i]);
	m_ComboExchange.InsertString(0, (CString("!-") + "全部"));
	m_ComboExchange.SetCurSel(0);
	//ResetList();
	m_ctlStatus.SetCurSel(0);
	ResetList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CProductDialog::OnCbnSelchangeComboExchange()
{
	// TODO: 在此添加控件通知处理程序代码
	//int sel = m_ComboExchange.GetCurSel();
	//m_ExchangeId = m_ComboExchange.GetSelExchangeId();
	ResetList();
}

void CProductDialog::ResetList()
{
	m_listctrl.DeleteAllItems();
	const std::vector<product_st> &products = theApp.m_BaseInfo.products;
	//const std::vector<
	CString exchangeid = m_ComboExchange.GetCurSelKey();
	CString status;
	m_ctlStatus.GetLBText(m_ctlStatus.GetCurSel(), status);
	hyphenstr h(status);
	status = h.left;

	for (int i = 0; i < products.size(); ++i)
	{
		char buff[256];
		const product_st &p = products[i];
		exchange_st e = theApp.GetProudctExchange(p.productid);
		//CString curExchangeid = 

		if (exchangeid.GetLength() > 0 && exchangeid != "!") {
			if (exchangeid != e.exchangeid)
				continue;
		}
		
		if (status.GetLength() > 0 && status != "!") {
			if (status != p.status)
				continue;
		}

		int index = m_listctrl.GetItemCount();
		m_listctrl.InsertItem(index, "");
		m_listctrl.SetItemText(index, 0, p.productid);
		m_listctrl.SetItemText(index, 1, p.name);

		//char *rule = products[index].trade_rule;
		//memset(buff, 0, sizeof(buff));
		//if (strcmp("1", rule) == 0)
		//	strcpy(buff, "每月第15日");
		//else if (strcmp("2", rule) == 0)
		//	strcpy(buff, "第10交易日");
		//else if (strcmp("3", rule) == 0)
		//	strcpy(buff, "倒数第七交易日");
		//else if (strcmp("4", rule) == 0)
		//	strcpy(buff, "交割期");
		//else if (strcmp("5", rule) == 0)
		//	strcpy(buff, "递延");
		//else if (strcmp("6", rule) == 0)
		//	strcpy(buff, "交割月前一交易日");
		//else if (strcmp("7", rule) == 0)
		//	strcpy(buff, "第三个星期五");
		//else if (strcmp("8", rule) == 0)
		//	strcpy(buff, "第二个星期五");
		//else if (strcmp("9", rule) == 0)
		//	strcpy(buff, "第5个交易日");
		//else if (strcmp("A", rule) == 0)
		//	strcpy(buff, "标的合约交割月前一个月的第15个交易日");
		//else if (strcmp("B", rule) == 0)
		//	strcpy(buff, "标的期货交割月前2个月的最后1个交易日");


		//m_listctrl.SetItemText(index, 2, buff);
		const char *trade_rule = theApp.DictToStr(DICT_CLASS_LAST_TRADEDAY_RULE, p.trade_rule);
		m_listctrl.SetItemText(index, 2, trade_rule);

		//memset(buff, 0, sizeof(buff));
		//const char *rule = p.inst_rule;
		//if (strcmp("01", rule) == 0)
		//	strcpy(buff, "连续月");
		//else if (strcmp("02", rule) == 0)
		//	strcpy(buff, "双月");
		//else if (strcmp("03", rule) == 0)
		//	strcpy(buff, "单月");
		//else if (strcmp("04", rule) == 0)
		//	strcpy(buff, "季月");
		//else if (strcmp("05", rule) == 0)
		//	strcpy(buff, "两个连续月及两个连续季月");
		//else if (strcmp("06", rule) == 0)
		//	strcpy(buff, "六个连续月及六个连续季月");
		//else if (strcmp("07", rule) == 0)
		//	strcpy(buff, "十二个连续月及十二个连续季月");
		//else if (strcmp("08", rule) == 0)
		//	strcpy(buff, "三个连续月及最近十一个月内的双月合约");
		//else if (strcmp("99", rule) == 0)
		//	strcpy(buff, "特殊合约");

		//m_listctrl.SetItemText(index, 3, buff);
		const char *inst_rule = theApp.DictToStr(DICT_CLASS_INST_RULE, p.inst_rule);
		m_listctrl.SetItemText(index, 3, inst_rule);

		//m_listctrl.SetItemText(index, 4, p.currency_type);
		m_listctrl.SetItemText(index, 4, theApp.DictToStr(DICT_CLASS_CURRENCY, p.currency_type));

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "%f", p.multiple);
		m_listctrl.SetItemText(index, 5, buff);

		//m_listctrl.SetItemText(index, 1, boost::lexical_cast<std::string>(static_cast<int>(p.multiple)).c_str());
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "%f", p.price_tick);
		m_listctrl.SetItemText(index, 6, buff);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "%d", p.min_lots);
		m_listctrl.SetItemText(index, 7, buff);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "%d", p.max_lots);
		m_listctrl.SetItemText(index, 8, buff);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "%d", p.max_position);
		m_listctrl.SetItemText(index, 9, buff);
	}
}


void CProductDialog::OnCbnSelchangeComboTradeStatus()
{
	// TODO: 在此添加控件通知处理程序代码
	ResetList();
}
