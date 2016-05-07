// QrySettlementDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "QrySettlementDialog.h"
#include "afxdialogex.h"
#include "Format.h"


static const char *tip_line = "-------------------------------------------------------------------------------------\r\n";

// CQrySettlementDialog 对话框

IMPLEMENT_DYNAMIC(CQrySettlementDialog, CDialogEx)

CQrySettlementDialog::CQrySettlementDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_QUERY_SETTLEMENT, pParent)
	, m_DateTime(COleDateTime::GetCurrentTime())
	, m_SettleStr(_T(""))
{

}

CQrySettlementDialog::~CQrySettlementDialog()
{
}

void CQrySettlementDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CUST, m_ctlCust);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER, m_DateTime);
	DDX_Text(pDX, IDC_EDIT_SETTLEMENT, m_SettleStr);
}


BEGIN_MESSAGE_MAP(CQrySettlementDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CQrySettlementDialog::OnBnClickedButtonQuery)
END_MESSAGE_MAP()


// CQrySettlementDialog 消息处理程序


BOOL CQrySettlementDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CQrySettlementDialog::OnBnClickedButtonQuery()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//UpdateData(FALSE);

	UpdateData(TRUE);
	CString cust_no = m_ctlCust.GetCurSelKey();
	if (cust_no.GetLength() == 0) {
		MessageBox("请选择客户");
		return;
	}
	
	Scoped_BCHANDLE handle;
	CString date = m_DateTime.Format("%Y%m%d");

	RequestSettleCustReady_853015(cust_no.GetBuffer(), date.GetBuffer(), handle);
	RequestSettleFundQuery_853000(cust_no.GetBuffer(), date.GetBuffer(), handle);
	RequestSettleTradeQuery_853002(cust_no.GetBuffer(), date.GetBuffer(), handle);
	RequestSettleOffsetDetail_853003(cust_no.GetBuffer(), date.GetBuffer(), handle);
	RequestSettlePosiDetail_853004(cust_no.GetBuffer(), date.GetBuffer(), handle);
	RequestSettlePosiSummary_853005(cust_no.GetBuffer(), date.GetBuffer(), handle);

	m_SettleStr = format_title() + format_fund() + format_trade() + format_od() + format_pd() + format_ps();

	UpdateData(FALSE);
}

BCResult CQrySettlementDialog::RequestSettleCustReady_853015(CString cust_no, char *date, BCHANDLE handle)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 853015);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "scusttypes", (cust_no + ",").GetBuffer());
	BCSetStringFieldByName(handle, 0, "ssectypes", "");
	BCSetStringFieldByName(handle, 0, "sstatus0", "1");
	//CString date = m_DateTime.Format("%Y%m%d");
	BCSetStringFieldByName(handle, 0, "sdate0", date);
	BCSetStringFieldByName(handle, 0, "sdate1", date);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", "111111");
	BCSetStringFieldByName(handle, 0, "sstatus1", "0");
	return MyBCRequest(handle, FetchRowFunc_t(), BCMSG_TOOP);
}
//
///* 输出：客户号		output:sholder_ac_no
//客户名称		sname
//币种		scurrency_type
//上日结存		damt0
//出金		damt1
//入金		damt2
//手续费		damt3
//上交手续费		damt4
//当日盈亏		damt5
//其它费用		damt6
//今日结存		damt7
//浮动盈亏		damt8
//在途资金		damt9
//未交割盈亏		damt10
//期末权益		damt11
//客户保证金		damt12
//基础保证金		damt13
//可用资金		damt14
//质押金额		damt15
//交易所保证金		damt16
//风险度		scust_auth
//追加保证金		damt18
//客户类号		sserial1
//客户类名称		sall_name
//风险级别		sstatus0
//交割盈亏		damt19
//平仓盈亏_盯		damt20
//平仓盈亏_浮		damt21
//持仓盈亏_盯		damt22
//平仓盈亏		damt23
//交割保证金		damt24
//交割手续费		damt25
//可提资金		damt26
//总盈亏 */		damt27
BCResult CQrySettlementDialog::RequestSettleFundQuery_853000(char * cust_no, char *date, BCHANDLE handle)
{
	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
		BCGetStringFieldByName(handle, row, "sholder_ac_no", m_fund.cust_no, sizeof(m_fund.cust_no));
		BCGetStringFieldByName(handle, row, "sname", m_fund.cust_name, sizeof(m_fund.cust_name));
		BCGetStringFieldByName(handle, row, "scurrency_type", m_fund.currency, sizeof(m_fund.currency));
		BCGetDoubleFieldByName(handle, row, "damt0", &m_fund.yd_balance);
		BCGetDoubleFieldByName(handle, row, "damt1", &m_fund.out_money);
		BCGetDoubleFieldByName(handle, row, "damt2", &m_fund.in_money);
		BCGetDoubleFieldByName(handle, row, "damt3", &m_fund.commission);
		BCGetDoubleFieldByName(handle, row, "damt4", &m_fund.sj_commission);
		BCGetDoubleFieldByName(handle, row, "damt5", &m_fund.td_profit);
		BCGetDoubleFieldByName(handle, row, "damt6", &m_fund.other_fee);
		BCGetDoubleFieldByName(handle, row, "damt7", &m_fund.td_balance);
		BCGetDoubleFieldByName(handle, row, "damt8", &m_fund.float_profit);
		BCGetDoubleFieldByName(handle, row, "damt9", &m_fund.roading_fund);
		BCGetDoubleFieldByName(handle, row, "damt10", &m_fund.undelivery_profit);
		//BCGetDoubleFieldByName(handle, row, "damt11", );
		BCGetDoubleFieldByName(handle, row, "damt12", &m_fund.margin);
		BCGetDoubleFieldByName(handle, row, "damt13", &m_fund.base_margin);
		BCGetDoubleFieldByName(handle, row, "damt14", &m_fund.available);
		BCGetDoubleFieldByName(handle, row, "damt15", &m_fund.impawn_amt);
		BCGetDoubleFieldByName(handle, row, "damt16", &m_fund.exchange_margin);
		//BCGetDoubleFieldByName(handle, row, "scust_auth", );
		BCGetDoubleFieldByName(handle, row, "damt18", &m_fund.add_margin);
		//BCGetDoubleFieldByName(handle, row, "sserial1", &m_fund);
		//BCGetDoubleFieldByName(handle, row, "sall_name", );
		BCGetStringFieldByName(handle, row, "sstatus0", m_fund.risk_level, sizeof(m_fund.risk_level));
		BCGetDoubleFieldByName(handle, row, "damt19", &m_fund.delivery_profit);
		//BCGetDoubleFieldByName(handle, row, "damt20", &);
		//BCGetDoubleFieldByName(handle, row, "damt21", );
		BCGetDoubleFieldByName(handle, row, "damt22", &m_fund.hold_profit);
		BCGetDoubleFieldByName(handle, row, "damt23", &m_fund.offset_profit);
		BCGetDoubleFieldByName(handle, row, "damt24", &m_fund.delivery_margin);
		BCGetDoubleFieldByName(handle, row, "damt25", &m_fund.delivery_commission);
		//BCGetDoubleFieldByName(handle, row, "damt26", &m_);
		BCGetDoubleFieldByName(handle, row, "damt27", &m_fund.total_profit);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 853000);
	BCSetStringFieldByName(handle, 0, "sucst_no", g_cfg.oper_code);
	//CString date = m_DateTime.Format("%Y%m%d");
	BCSetStringFieldByName(handle, 0, "sdate0", date);
	BCSetStringFieldByName(handle, 0, "sdate1", date);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);
	BCSetStringFieldByName(handle, 0, "sstatus0", "0"); //是否按类合计，
	BCSetStringFieldByName(handle, 0, "sstatus1", "2"); //帐单模式，
	BCSetStringFieldByName(handle, 0, "sstatus2", "1"); //强平标准，
	
	memset(&m_fund, 0, sizeof(m_fund));

	return MyBCRequest(handle, fetcher, BCMSG_TOOP);
}

//
///* 输出：日期		output:sdate0
//客户号		sholder_ac_no
//成交序号		lvol2
//交易所		smarket_code
//席位		sserial2
//品种		sstock_code
//交割期		sdate1
//币种		scurrency_type
//买卖		sstatus2
//开平		sstatus3
//投保		sstatus4
//成交手数		lvol1
//成交价		damt2
//手续费		damt3
//其它费用		damt6
//交易所成交序号		scust_auth
//成交额		damt1
//平仓盈亏 */		damt11


BCResult CQrySettlementDialog::RequestSettleTradeQuery_853002(char * cust_no, char *date, BCHANDLE handle)
{
	FetchRowFunc_t fetcher = [&](BCHANDLE handle, int row)
	{
		cust_trade_st t = { 0 };
		BCGetStringFieldByName(handle, row, "sdate0", t.date, sizeof(t.date));
		BCGetStringFieldByName(handle, row, "sholder_ac_no", t.cust_no, sizeof(t.cust_no));
		BCGetIntFieldByName(handle, row, "lvol2", &t.tradeseq);
		//BCGetIntFieldByName(handle, row, "lserial0", &t.orderseq);
		BCGetStringFieldByName(handle, row, "smarket_code", t.exchangeid, sizeof(t.exchangeid));
		BCGetStringFieldByName(handle, row, "sserial2", t.seatid, sizeof(t.seatid));
		BCGetStringFieldByName(handle, row, "sstock_code", t.productid, sizeof(t.productid));
		BCGetStringFieldByName(handle, row, "sdate1", t.delivery_date, sizeof(t.delivery_date));
		BCGetStringFieldByName(handle, row, "scurrency_type", t.currency, sizeof(t.currency));
		BCGetStringFieldByName(handle, row, "sstatus3", t.offset_flag, sizeof(t.offset_flag));
		BCGetStringFieldByName(handle, row, "sstatus2", t.direction, sizeof(t.direction));
		BCGetStringFieldByName(handle, row, "sstatus4", t.hedge_flag, sizeof(t.hedge_flag));
		BCGetDoubleFieldByName(handle, row, "damt2", &t.price);
		BCGetIntFieldByName(handle, row, "lvol1", &t.volume);
		BCGetDoubleFieldByName(handle, row, "damt3", &t.commission);

		//BCGetStringFieldByName(handle, row, "sbank_acc", t.instrumentid, sizeof(t.instrumentid));
		////BCGetStringFieldByName(handle, row, "sstatus0", t.order_status, sizeof(t.order_status));
		//BCGetStringFieldByName(handle, row, "sorder0", t.sysid, sizeof(t.sysid));
		//BCGetStringFieldByName(handle, row, "stime3", t.time, sizeof(t.time));
		//BCGetStringFieldByName(handle, row, "scust_no", t.oper_no, sizeof(t.oper_no));
		//BCGetStringFieldByName(handle, row, "sstatus0", t.force_close, sizeof(t.force_close));

		m_trades.push_back(t);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 853002);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sdate0", date);
	BCSetStringFieldByName(handle, 0, "sdate1", date);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);
	BCSetStringFieldByName(handle, 0, "sserial1", "");
	BCSetStringFieldByName(handle, 0, "sstatus1", "2"); //帐单模式，
	BCSetStringFieldByName(handle, 0, "scurrency_type", "1");
	BCSetStringFieldByName(handle, 0, "sstatus0", "0"); //是否按类合记
	BCSetStringFieldByName(handle, 0, "sstatus1", ""); //是否合并
	
	m_trades.clear();

	return MyBCRequest(handle, fetcher, BCMSG_NOR_NOMB);
}

BCResult CQrySettlementDialog::RequestSettleOffsetDetail_853003(char * cust_no, char * date, BCHANDLE handle)
{
	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row)
	{
		cust_offset_detail_st od = { 0 };
		BCGetStringFieldByName(handle, row, "sdate0", od.close_date, sizeof(od.close_date));
		BCGetStringFieldByName(handle, row, "sdate2", od.open_date, sizeof(od.open_date));
		BCGetIntFieldByName(handle, row, "lserial0", &od.close_serial);
		BCGetIntFieldByName(handle, row, "lserial1", &od.open_serial);
		BCGetStringFieldByName(handle, row, "smarket_code", od.exchangeid, sizeof(od.exchangeid));
		BCGetStringFieldByName(handle, row, "sserial2", od.seatid, sizeof(od.seatid));
		BCGetStringFieldByName(handle, row, "sstock_code", od.product, sizeof(od.product));
		BCGetStringFieldByName(handle, row, "sdate1", od.delivery_date, sizeof(od.delivery_date));
		BCGetStringFieldByName(handle, row, "sstatus2", od.direction, sizeof(od.direction));
		BCGetIntFieldByName(handle, row, "lvol1", &od.volume);
		BCGetDoubleFieldByName(handle, row, "damt0", &od.close_price);
		BCGetDoubleFieldByName(handle, row, "damt1", &od.open_price);
		BCGetDoubleFieldByName(handle, row, "damt2", &od.yd_settle_price);
		BCGetDoubleFieldByName(handle, row, "damt3", &od.float_profit);
		BCGetDoubleFieldByName(handle, row, "damt4", &od.hold_profit);
		
		m_OffDetails.push_back(od);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 853003);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sdate0", date);
	BCSetStringFieldByName(handle, 0, "sdate1", date);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);
	BCSetStringFieldByName(handle, 0, "sserial1", "");
	BCSetStringFieldByName(handle, 0, "sstatus1", "2"); //帐单模式，
	BCSetStringFieldByName(handle, 0, "scurrency_type", "1");
	BCSetStringFieldByName(handle, 0, "sstatus0", "0"); //是否按类合记
	BCSetStringFieldByName(handle, 0, "sstatus1", ""); //是否合并

	m_OffDetails.clear();

	return MyBCRequest(handle, fetcher, BCMSG_NOR_NOMB);
}

//
///* 输出：开仓日期		output:sdate0
//成交序号		lserial1
//客户号		sholder_ac_no
//交易所		smarket_code
//席位（不显示）		sserial2
//品种		sstock_code
//交割期		sdate1
//买卖		sstatus2
//手数		lvol1
//价格		damt0
//结算价		damt1
//昨结价		damt2
//保证金		damt3
//浮动盈亏		damt4
//持仓盈亏（盯）		damt5
//交易编码		sholder_ac_no2
//投保		sstatus3
//交易所成交号 */		scust_auth

BCResult CQrySettlementDialog::RequestSettlePosiDetail_853004(char * cust_no, char * date, BCHANDLE handle)
{
	FetchRowFunc_t fetcher = [&](BCHANDLE handle, int row)
	{
		cust_td_position_detail_st pd = { 0 };
		BCGetStringFieldByName(handle, row, "sdate0", pd.open_date, sizeof(pd.open_date));
		BCGetIntFieldByName(handle, row, "lserial1", &pd.trade_seq);
		BCGetStringFieldByName(handle, row, "sholder_ac_no", pd.cust_no, sizeof(pd.cust_no));
		BCGetStringFieldByName(handle, row, "smarket_code", pd.exchangeid, sizeof(pd.exchangeid));
		BCGetStringFieldByName(handle, row, "sstock_code", pd.product, sizeof(pd.product));
		BCGetStringFieldByName(handle, row, "sdate1", pd.delivery_date, sizeof(pd.delivery_date));
		BCGetStringFieldByName(handle, row, "sstatus2", pd.direction, sizeof(pd.direction));
		BCGetStringFieldByName(handle, row, "sstatus3", pd.hedge_flag, sizeof(pd.hedge_flag));
		BCGetIntFieldByName(handle, row, "lvol1", &pd.volume);
		BCGetDoubleFieldByName(handle, row, "damt0", &pd.hold_price);
		BCGetDoubleFieldByName(handle, row, "damt1", &pd.td_settle_price);
		BCGetDoubleFieldByName(handle, row, "damt2", &pd.yd_settle_price);
		BCGetDoubleFieldByName(handle, row, "damt3", &pd.hold_margin);
		BCGetDoubleFieldByName(handle, row, "damt4", &pd.float_profit);
		BCGetStringFieldByName(handle, row, "sholder_ac_no2", pd.tradeid, sizeof(pd.tradeid));
		//BCGetStringFieldByName(handle, row, "sdate2", pd.trade_date, sizeof(pd.trade_date));
		//BCGetStringFieldByName(handle, row, "sdate3", pd.open_date, sizeof(pd.open_date));
		//BCGetStringFieldByName(handle, row, "sstatus3", pd.scurrency_type, sizeof(pd.scurrency_type));
		//BCGetStringFieldByName(handle, row, "scert_addr", pd.instrumentid, sizeof(pd.instrumentid));
		//BCGetIntFieldByName(handle, row, "lvol10", &pd.multiple);
		//BCGetDoubleFieldByName(handle, row, "damt26", &pd.open_price);
		
		m_PosiDetails.push_back(pd);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 853004);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sdate0", date);
	BCSetStringFieldByName(handle, 0, "sdate1", date);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);
	BCSetStringFieldByName(handle, 0, "sserial1", "");
	BCSetStringFieldByName(handle, 0, "sstatus1", "2"); //帐单模式，
	BCSetStringFieldByName(handle, 0, "scurrency_type", "1");
	BCSetStringFieldByName(handle, 0, "sstatus0", "0"); //是否按类合记
	BCSetStringFieldByName(handle, 0, "sstatus1", ""); //是否合并

	m_PosiDetails.clear();

	return MyBCRequest(handle, fetcher);
}

//
///* 输出：客户号			output:sholder_ac_no
//交易所			smarket_code
//席位（不显示）			sserial2
//品种			sstock_code
//交割期			sdate1
//投保			sstatus3
//买持量			lvol2
//卖持量			lvol3
//买均价			damt6
//卖均价			damt7
//结算价			damt1
//昨结价			damt2
//保证金			damt3
//浮动盈亏			damt4
//持仓盈亏（盯） */			damt5

BCResult CQrySettlementDialog::RequestSettlePosiSummary_853005(char * cust_no, char * date, BCHANDLE handle)
{
	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row)
	{
		cust_posi_summary_st ps = { 0 };

		BCGetStringFieldByName(handle, row, "sholder_ac_no", ps.cust_no, sizeof(ps.cust_no));
		BCGetStringFieldByName(handle, row, "smarket_code", ps.exchangeid, sizeof(ps.exchangeid));
		BCGetStringFieldByName(handle, row, "sserial2", ps.seatid, sizeof(ps.seatid));
		BCGetStringFieldByName(handle, row, "sstock_code", ps.product, sizeof(ps.product));
		BCGetStringFieldByName(handle, row, "sdate1", ps.delivery_date, sizeof(ps.delivery_date));
		BCGetStringFieldByName(handle, row, "sstatus3", ps.hedge_flag, sizeof(ps.hedge_flag));
		BCGetIntFieldByName(handle, row, "lvol2", &ps.buy_hold_volume);
		BCGetIntFieldByName(handle, row, "lvol3", &ps.sell_hold_volume);
		BCGetDoubleFieldByName(handle, row, "damt6", &ps.buy_aver_price);
		BCGetDoubleFieldByName(handle, row, "damt7", &ps.sell_aver_price);
		BCGetDoubleFieldByName(handle, row, "damt1", &ps.settle_price);
		BCGetDoubleFieldByName(handle, row, "damt2", &ps.yd_settle_price);
		BCGetDoubleFieldByName(handle, row, "damt3", &ps.margin);
		BCGetDoubleFieldByName(handle, row, "damt4", &ps.float_profit);
		BCGetDoubleFieldByName(handle, row, "damt5", &ps.hold_profit);
		
		m_PosiSummary.push_back(ps);
	};
	BCResetHandle(handle);
	BCSetRequestType(handle, 853005);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sdate0", date);
	BCSetStringFieldByName(handle, 0, "sdate1", date);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);
	BCSetStringFieldByName(handle, 0, "sserial1", "");
	BCSetStringFieldByName(handle, 0, "sstatus1", "2"); //帐单模式，
	BCSetStringFieldByName(handle, 0, "scurrency_type", "1");
	BCSetStringFieldByName(handle, 0, "sstatus0", "0");

	m_PosiSummary.clear();

	return MyBCRequest(handle, fetcher, BCMSG_NOR_NOMB);
}

CString CQrySettlementDialog::format_title()
{
	CString str;
	str += "             交易结算单                     \r\n";
	str += CString("   客户号: ") + m_fund.cust_no + "   客户名称: " + m_fund.cust_name + "    日期: " + m_DateTime.Format("%Y%m%d") + "\r\n";
	str += "\r\n\r\n";
	return str;
}

CString CQrySettlementDialog::format_fund()
{
	//CString fund = "资金状况 币种:人民币\r\n";
	//fund += tip_line;
	//fund += "   上日结存:" + fundfmt(m_fund.yd_balance) + "     当日结存:" + fundfmt(m_fund.td_balance) + "     可用资金:" + fundfmt(m_fund.available) + "\r\n";
	//fund += "     出入金:" + fundfmt(m_fund.out_money) +  "     客户权益:" + fundfmt(m_fund.td_balance) + "   风险级别:" + theApp.DictToStr(DICT_CLASS_RISK_LEVEL, m_fund.risk_level) + "\r\n";
	//fund += "     手续费:" + fundfmt(m_fund.commission) + "     盈亏:" + fundfmt(m_fund.total_profit) + "\r\n";

	CString str = "资金状况 币种:人民币\r\n";
	str += tip_line;
	char buff[1024];
	sprintf(buff, "%-14s:%-14.2f %-14s:%14.2f  %-14s:%14.2f\r\n",
		"上日结存", m_fund.yd_balance, "当日结存", m_fund.td_balance, "客户权益", m_fund.td_balance);
	str += buff;

	sprintf(buff, "%-14s:%-14.2f %-14s:%14.2f  %-14s:%14.2f\r\n",
		"入金", m_fund.in_money, "出金", m_fund.out_money, "可用资金", m_fund.available);
	str += buff;

	sprintf(buff, "%-14s:%-14.2f %-14s:%14.2f  %-14s:%14.2f\r\n",
		"手续费", m_fund.commission, "上交手续费", m_fund.sj_commission, "交割手续费", m_fund.delivery_commission);
	str += buff;

	sprintf(buff, "%-14s:%-14.2f %-14s:%14.2f  %-14s:%14.2f %-14s:%14.2f\r\n",
		"保证金", m_fund.margin, "基础保证金", m_fund.base_margin, "追加保证金", m_fund.add_margin, "交割保证金", m_fund.delivery_margin);
	str += buff;

	sprintf(buff, "%-14s:%-14.2f %-14s:%14.2f  %-14s:%14.2f  %-14s:%14.2f\r\n",
		 "总盈亏", m_fund.total_profit, "持仓盈亏", m_fund.hold_profit, "平仓盈亏", m_fund.offset_profit, "交割盈亏", m_fund.delivery_profit);
	str += buff;

	str += "\r\n\r\n";
	return str;

	//m_SettleStr += fund;
}

CString CQrySettlementDialog::format_trade()
{
	CString str = "成交记录\r\n";
	str += tip_line;
	char buff[1024];
	sprintf(buff, "%-10s%-12s%-10s%-10s%-6s%-6s%-6s%-8s%-15s%-15s\r\n",
		"成交日期", "交易所","品种","合约","买卖", "开平", "投保", "手数", "成交价", "手续费");
	str += buff;
	for (TradeVec_t::iterator it = m_trades.begin(); it != m_trades.end(); ++it)
	{
		char inst[81] = { 0 };
		strcat(inst, it->productid);
		strcat(inst, it->delivery_date);
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "%-10s%-12s%-10s%-10s%-6s%-6s%-6s%-8d%-15.2f%-15.2f\r\n",
			it->date, it->exchangeid, it->productid, inst,
			theApp.DictToStr(DICT_CLASS_BUY_SELL, it->direction),
			theApp.DictToStr(DICT_CLASS_OFFSET_FLAG, it->offset_flag),
			theApp.DictToStr(DICT_CLASS_HEDGE_FLAG, it->hedge_flag),
			it->volume, it->price, it->commission);
		str += buff;
	}
	str += "\r\n\r\n";

	return str;
}

CString CQrySettlementDialog::format_od()
{
	CString str = "平仓明细\r\n";
	str += tip_line;
	char buff[1024];
	sprintf(buff, "%-10s%-12s%-10s%-10s%-6s%-8s%-15s%-10s%-15s%-15s%-15s\r\n",
		"平仓日期", "交易所", "品种", "合约", "买卖", "手数", "平仓价", "开仓日期", "开仓价格", "昨结算", "平仓盈亏(盯)");
	str += buff;

	for (std::vector<cust_offset_detail_st>::iterator it = m_OffDetails.begin(); it != m_OffDetails.end(); ++it)
	{
		char inst[81] = { 0 };
		strcat(inst, it->product);
		strcat(inst, it->delivery_date);
		sprintf(buff, "%-10s%-12s%-10s%-10s%-6s%-8d%-15.2f%-10s%-15.2f%-15.2f%-15.2f\r\n",
			it->close_date, it->exchangeid, it->product, inst,
			theApp.DictToStr(DICT_CLASS_BUY_SELL, it->direction),
			it->volume, it->close_price, it->open_date, it->open_price, it->yd_settle_price, it->hold_profit);
		str += buff;
	}
	str += "\r\n\r\n";

	return str;
}

CString CQrySettlementDialog::format_pd()
{
	CString str = "持仓明细\r\n";
	str += tip_line;
	char buff[1024];
	sprintf(buff, "%-10s%-12s%-10s%-10s%-6s%-6s%-8s%-15s%-15s%-15s%-15s%-15s\r\n",
		"开仓日期", "交易所", "品种", "合约", "买卖", "投保", "持仓量", "持仓价", "昨结算", "今结算", "浮动盈亏", "保证金");
	str += buff;
	for (PosiDetailVec_t::iterator it = m_PosiDetails.begin(); it != m_PosiDetails.end(); ++it)
	{
		char inst[81] = { 0 };
		strcat(inst, it->product);
		strcat(inst, it->delivery_date);
		sprintf(buff, "%-10s%-12s%-10s%-10s%-6s%-6s%-8d%-15.2f%-15.2f%-15.2f%-15.2f%-15.2f\r\n",
			it->open_date, it->exchangeid, it->product, inst,
			theApp.DictToStr(DICT_CLASS_BUY_SELL, it->direction),
			theApp.DictToStr(DICT_CLASS_HEDGE_FLAG, it->hedge_flag),
			it->volume, it->hold_price, it->yd_settle_price, it->td_settle_price, it->float_profit, it->hold_margin);
		str += buff;
	}

	str += "\r\n\r\n";

	return str;
}

CString CQrySettlementDialog::format_ps()
{
	CString str = "持仓汇总\r\n";
	str += tip_line;
	char buff[1024];
	sprintf(buff, "%-12s%-10s%-10s%-6s%-8s%-8s%-15s%-15s%-15s%-15s%-15s%-15s%-15s\r\n",
		 "交易所", "品种", "合约", "投保", "买持量", "卖持量","买均价","卖均价","今结算","昨结算","保证金","浮动盈亏","持仓盈亏(盯)");
	str += buff;
	
	for (std::vector<cust_posi_summary_st>::iterator it = m_PosiSummary.begin(); it != m_PosiSummary.end(); ++it)
	{
		char inst[81] = { 0 };
		strcat(inst, it->product);
		strcat(inst, it->delivery_date);
		sprintf(buff, "%-12s%-10s%-10s%-6s%-8d%-8d%-15.2f%-15.2f%-15.2f%-15.2f%-15.2f%-15.2f%-15.2f\r\n",
			it->exchangeid, it->product, inst,
			theApp.DictToStr(DICT_CLASS_HEDGE_FLAG, it->hedge_flag),
			it->buy_hold_volume, it->sell_hold_volume, it->buy_aver_price, it->sell_aver_price,
			it->settle_price, it->yd_settle_price, it->margin,
			it->float_profit, it->hold_profit);
		str += buff;
	}

	str += "\r\n\r\n";

	return str;
}

CString CQrySettlementDialog::format_tail()
{
	//CString str = tip_line;
	//str += "制表:系统管理员          ";
	//str += "制表日期:\r\n";
	//str += CString("客户号:" )+ m_fund.cust_no + "        ";

	//str += CString("   客户号: ") + m_fund.cust_no + "   客户名称: " + m_fund.cust_name + "    日期: " + m_DateTime.Format("%Y%m%d") + "\r\n";
	//str += "\r\n\r\n";
	//return str;
	return CString();
}
