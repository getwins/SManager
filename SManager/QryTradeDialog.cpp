// QryTradeDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "QryTradeDialog.h"
#include "afxdialogex.h"


// CQryTradeDialog 对话框

IMPLEMENT_DYNAMIC(CQryTradeDialog, CDialogEx)

CQryTradeDialog::CQryTradeDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_QUERY_TRADE, pParent)
	, m_DateTime(COleDateTime::GetCurrentTime())
{

}

CQryTradeDialog::~CQryTradeDialog()
{
}

void CQryTradeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_QUERY_CUST, m_ctlCust);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER, m_DateTime);
	DDX_Control(pDX, IDC_LIST_RECORD, m_listctrl);
}


BEGIN_MESSAGE_MAP(CQryTradeDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CQryTradeDialog::OnBnClickedButtonQuery)
END_MESSAGE_MAP()


// CQryTradeDialog 消息处理程序


BOOL CQryTradeDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	LVCOLUMN cols[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "委托编号" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "成交时间" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "合约" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "买卖" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "开平" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "成交价格" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "成交手数" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "成交编号" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "交易所" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "强平标志" }
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "盈亏" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "交易账户" }
	};

	for (int i = 0; i < sizeof(cols) / sizeof(cols[0]); ++i)
		m_listctrl.InsertColumn(i, &cols[i]);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CQryTradeDialog::OnBnClickedButtonQuery()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_vTrade.clear();
	m_listctrl.DeleteAllItems();
	if (!RequestTrade_852103())
		return;

	
	CString str;
	for (int i = 0; i < m_vTrade.size(); i++) {
		cust_trade_st &t = m_vTrade[i];
		str.Format("%d", t.orderseq);
		m_listctrl.InsertItem(i, str);
		m_listctrl.SetItemText(i, 1, t.time);
		m_listctrl.SetItemText(i, 2, t.instrumentid);
		m_listctrl.SetItemText(i, 3, theApp.DictToStr(DICT_CLASS_BUY_SELL, t.direction));
		m_listctrl.SetItemText(i, 4, theApp.DictToStr(DICT_CLASS_OFFSET_FLAG, t.offset_flag));
		str.Format("%f", t.price);
		m_listctrl.SetItemText(i, 5, str);
		str.Format("%d", t.volume);
		m_listctrl.SetItemText(i, 6, str);
		str.Format("%d", t.tradeseq);
		m_listctrl.SetItemText(i, 7, str);
		m_listctrl.SetItemText(i, 8, t.exchangeid);
		//str.Format("%f", t.commission);
		m_listctrl.SetItemText(i, 9, theApp.DictToStr(DICT_CLASS_FORCE_CLOSE, t.force_close));
		

	}
}

//function 852103 description  "历史实时成交查询"  reqlevel 3
///* 输入：操作员，开始日期，结束日期，查询操作员，客户号，交易所，品种，交割期，买卖，开平 */
//input:scust_no, sdate0, sdate1, scust_no2, sholder_ac_no, smarket_code, sstock_code, sdate2, sstatus0, sstatus1
//	/* 输出：日期，委托号，客户号，客户名称，交易编码，交易所，品种代码，交割期，买卖方向，成交价，成交量，开平标记，投保标记，系统号，成交号，交易所成交号，席位号，币种，成交时间，强平标志 */
//	output : sdate0, lserial0, sholder_ac_no, sname, sholder_ac_no2, smarket_code, sstock_code, sdate1, sstatus3, damt1, lvol2, sstatus1, sstatus2, sorder0, lvol3, sorder2, sserial2, scurrency_type, stime0, sstatus0
//
//
//输出：日期		output : sdate0
//委托号		 lserial0
//客户号		 sholder_ac_no
//客户名称		 sname
//交易编码		 sholder_ac_no2
//交易所		 smarket_code
//品种代码		 sstock_code
//交割期		 sdate1
//买卖方向		 sstatus3
//成交价		 damt1
//成交量		 lvol2
//开平标记		 sstatus1
//投保标记		 sstatus2
//系统号		 sorder0
//成交号		 lvol3
//交易所成交号		 sorder2
//席位号		 sserial2
//币种		 scurrency_type
//成交时间		 stime0
//强平标志 		 sstatus0

bool CQryTradeDialog::RequestTrade_852103()
{
	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
		cust_trade_st t = { 0 };
		//char sholder_ac_no[16] = { 0 };
		//BCGetStringFieldByName(handle, row, "sholder_ac_no", sholder_ac_no, sizeof(sholder_ac_no));
		//assert(cust_no == sholder_ac_no);
		BCGetStringFieldByName(handle, row, "sdate0", t.date, sizeof(t.date));
		BCGetIntFieldByName(handle, row, "lserial0", &t.orderseq);
		BCGetStringFieldByName(handle, row, "sholder_ac_no2", t.trade_no, sizeof(t.trade_no));
		BCGetStringFieldByName(handle, row, "smarket_code", t.exchangeid, sizeof(t.exchangeid));
		BCGetStringFieldByName(handle, row, "sstock_code", t.productid, sizeof(t.productid));
		BCGetStringFieldByName(handle, row, "sdate1", t.delivery_date, sizeof(t.delivery_date));
		BCGetStringFieldByName(handle, row, "scert_addr", t.instrumentid, sizeof(t.instrumentid));
		//BCGetStringFieldByName(handle, row, "sstatus0", t.order_status, sizeof(t.order_status));
		BCGetStringFieldByName(handle, row, "sstatus1", t.offset_flag, sizeof(t.offset_flag));
		BCGetStringFieldByName(handle, row, "sstatus3", t.direction, sizeof(t.direction));
		BCGetStringFieldByName(handle, row, "sstatus2", t.hedge_flag, sizeof(t.hedge_flag));
		BCGetDoubleFieldByName(handle, row, "damt1", &t.price);
		BCGetIntFieldByName(handle, row, "lvol2", &t.volume);
		BCGetStringFieldByName(handle, row, "sorder0", t.sysid, sizeof(t.sysid));
		BCGetIntFieldByName(handle, row, "lvol3", &t.tradeseq);
		BCGetStringFieldByName(handle, row, "sserial2", t.seatid, sizeof(t.seatid));
		BCGetStringFieldByName(handle, row, "scurrency_type", t.currency, sizeof(t.currency));
		BCGetStringFieldByName(handle, row, "stime0", t.time, sizeof(t.time));
		//BCGetDoubleFieldByName(handle, row, "damt6", &t.commission);
		BCGetStringFieldByName(handle, row, "scust_no", t.oper_no, sizeof(t.oper_no));
		BCGetStringFieldByName(handle, row, "sstatus0", t.force_close, sizeof(t.force_close));
		
		this->m_vTrade.push_back(t);
	};

	Scoped_BCHANDLE handle;
	BCSetRequestType(handle, 852103);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	CString strDate = m_DateTime.Format("%Y%m%d");
	BCSetStringFieldByName(handle, 0, "sdate0", strDate.GetBuffer());
	BCSetStringFieldByName(handle, 0, "sdate1", strDate.GetBuffer());
	CString strCust = m_ctlCust.GetCurSelKey();
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", strCust.GetBuffer());
	return MyBCRequest(handle, fetcher, BCMSG_TOOP);
}
