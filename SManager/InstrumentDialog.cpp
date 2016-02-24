// InstrumentDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "InstrumentDialog.h"
#include "afxdialogex.h"
#include "SMutility.h"

// CInstrumentDialog 对话框

IMPLEMENT_DYNAMIC(CInstrumentDialog, CDialogEx)

CInstrumentDialog::CInstrumentDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INSTRUMENT, pParent)
{

}

CInstrumentDialog::~CInstrumentDialog()
{
}

void CInstrumentDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INSTRUMENT, m_listctrl);
	DDX_Control(pDX, IDC_COMBO_EXCHANGE, m_ctlExchange);
	DDX_Control(pDX, IDC_COMBO_PRODUCT, m_ctlProduct);
	DDX_Control(pDX, IDC_COMBO_TRADE_STATUS, m_ctlStatus);
}


BEGIN_MESSAGE_MAP(CInstrumentDialog, CDialogEx)
//	ON_NOTIFY(NM_DBLCLK, IDC_LIST_INSTRUMENT, &CInstrumentDialog::OnNMDblclkListInstrument)
ON_CBN_SELCHANGE(IDC_COMBO_EXCHANGE, &CInstrumentDialog::OnCbnSelchangeComboExchange)
ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT, &CInstrumentDialog::OnCbnSelchangeComboProduct)
ON_CBN_SELCHANGE(IDC_COMBO_TRADE_STATUS, &CInstrumentDialog::OnCbnSelchangeComboTradeStatus)
END_MESSAGE_MAP()


// CInstrumentDialog 消息处理程序


BOOL CInstrumentDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LVCOLUMN cols[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "合约" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "品种" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "交割期" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "币种" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "每手数量" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "最小变动价" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "最小手数" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "最大手数" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "最大持仓" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "合约状态" }
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "涨停板率" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "跌停板率" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "手续费(手数)" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "手续费(金额)" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "保证金(手数）" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "保证金(金额)" }
	};

	for (int i = 0; i < sizeof(cols) / sizeof(cols[0]); ++i)
		m_listctrl.InsertColumn(i, &cols[i]);
	m_ctlExchange.InsertString(0, CString("!-全部"));
	m_ctlExchange.SetCurSel(0);
	m_ctlProduct.SetCurSel(0);
	m_ctlStatus.SetCurSel(0);
	OnCbnSelchangeComboExchange();
	

	ResetList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//void CInstrumentDialog::OnNMDblclkListInstrument(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}


void CInstrumentDialog::OnCbnSelchangeComboExchange()
{
	// TODO: 在此添加控件通知处理程序代码
	CString exchangeid = m_ctlExchange.GetCurSelKey();
	//if()
	m_ctlProduct.ResetContent();
	
	//if (exchangeid == "!")
	//	return;
	m_ctlProduct.FilterByExchangeid(exchangeid);
	m_ctlProduct.InsertString(0, CString("!-全部"));
	//for each (exchange_product_st ep in theApp.m_BaseInfo.exchange_product)
	//{
	//	if (exchangeid == ep.exchangeid) {
	//		m_ctlProduct.AddString(CString(ep.productid) + "-" + ep.product_name);
	//	}
	//}
	m_ctlProduct.SetCurSel(0);
	ResetList();
}


void CInstrumentDialog::OnCbnSelchangeComboProduct()
{
	// TODO: 在此添加控件通知处理程序代码
	ResetList();
}


void CInstrumentDialog::OnCbnSelchangeComboTradeStatus()
{
	// TODO: 在此添加控件通知处理程序代码
	ResetList();
}

void CInstrumentDialog::ResetList()
{
	CString exchangeid = m_ctlExchange.GetCurSelKey();
	//CString productid;
	//m_ctlProduct.GetLBText(m_ctlProduct.GetCurSel(), productid);
	//hyphenstr hp(productid);
	CString productid = m_ctlProduct.GetCurSelKey();

	CString status;
	m_ctlStatus.GetLBText(m_ctlStatus.GetCurSel(), status);
	hyphenstr hs(status);
	status = hs.left;

	m_listctrl.DeleteAllItems();
	for (int i = 0; i < theApp.m_BaseInfo.instruments.size(); i++) {
		const instrument_st &inst = theApp.m_BaseInfo.instruments[i];
		//exchange_st e = theApp.GetProudctExchange(e.);
		if (exchangeid != "!" && exchangeid != inst.exchangeid)
			continue;
		if (productid != "!" && productid != inst.productid)
			continue;
		if (status != "!" && status != inst.status)
			continue;
		
		int index = m_listctrl.GetItemCount();
		char buff[256];
		m_listctrl.InsertItem(index, "");
		m_listctrl.SetItemText(index, 0, inst.instrumentid);
		m_listctrl.SetItemText(index, 1, inst.product_name);
		m_listctrl.SetItemText(index, 2, inst.delivery_date);
		m_listctrl.SetItemText(index, 3, theApp.DictToStr(DICT_CLASS_CURRENCY, inst.currency_type));

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "%d", inst.multiple);
		m_listctrl.SetItemText(index, 4, buff);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "%f", inst.price_tick);
		m_listctrl.SetItemText(index, 5, buff);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "%d", (int)inst.min_lots);
		m_listctrl.SetItemText(index, 6, buff);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "%d", (int)inst.max_lots);
		m_listctrl.SetItemText(index, 7, buff);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "%d", inst.max_position);
		m_listctrl.SetItemText(index, 8, buff);

		m_listctrl.SetItemText(index, 9, theApp.DictToStr(DICT_CLASS_INST_STATUS, inst.status));

	}
	
}
