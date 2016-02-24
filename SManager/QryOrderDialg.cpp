// QryOrderDialg.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "QryOrderDialg.h"
#include "afxdialogex.h"


// CQryOrderDialg 对话框

IMPLEMENT_DYNAMIC(CQryOrderDialg, CDialogEx)

CQryOrderDialg::CQryOrderDialg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_QUERY_ORDER, pParent)
	, m_DateTime(COleDateTime::GetCurrentTime())
{

}

CQryOrderDialg::~CQryOrderDialg()
{
}

void CQryOrderDialg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_QUERY_CUST, m_ctlCust);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER, m_DateTime);
	DDX_Control(pDX, IDC_LIST_RECORD, m_listctrl);
}


BEGIN_MESSAGE_MAP(CQryOrderDialg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CQryOrderDialg::OnBnClickedButtonQuery)
END_MESSAGE_MAP()


// CQryOrderDialg 消息处理程序


void CQryOrderDialg::OnBnClickedButtonQuery()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_vOrder.clear();
	m_listctrl.DeleteAllItems();
	if (!RequestOrder_852102())
		return;
	
	CString str;
	for (int i = 0; i < m_vOrder.size(); i++) {
		cust_order_st &o = m_vOrder[i];
		str.Format("%d", o.orderseq);
		m_listctrl.InsertItem(i, str);
		m_listctrl.SetItemText(i, 1, o.order_time);
		m_listctrl.SetItemText(i, 2, o.instrumentid);
		m_listctrl.SetItemText(i, 3, theApp.DictToStr(DICT_CALSS_BUY_SELL, o.direction));
		m_listctrl.SetItemText(i, 4, theApp.DictToStr(DICT_CLASS_OFFSET_FLAG, o.offset_flag));
		str.Format("%f", o.order_price);
		m_listctrl.SetItemText(i, 5, str);
		str.Format("%d", o.origin_total_volume);
		m_listctrl.SetItemText(i, 6, str);
		str.Format("%d", o.traded_volume);
		m_listctrl.SetItemText(i, 7, str);
		m_listctrl.SetItemText(i, 8, theApp.DictToStr(DICT_CLASS_ORDER_STATUS, o.order_status));
		m_listctrl.SetItemText(i, 9, "");
		m_listctrl.SetItemText(i, 10, o.exchangeid);
	}
}


//BOOL CQryOrderDialg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	return CDialogEx::OnNotify(wParam, lParam, pResult);
//}


BOOL CQryOrderDialg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//std::vector<std::string> cust_nos = theApp.GetCustnoList();
	//for each (std::string cust in theApp.GetCustnoList())
	//{
	//	m_ctlCust.AddString(cust.c_str());
	//}

	LVCOLUMN cols[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "委托编号" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "委托时间" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "合约" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "买卖" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "开平" },		
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "委托价格" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "委托量" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "未成交" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "已成交" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "详细状态" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 120, "委托状态" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "强平" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "风控信息" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "交易所" }
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "交易账户" },
	};

	for (int i = 0; i < sizeof(cols) / sizeof(cols[0]); ++i)
		m_listctrl.InsertColumn(i, &cols[i]);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//function 852102 description  "历史实时委托查询"  reqlevel 3
///* 输入：操作员,开始日期,结束日期,查询操作员,客户号,交易所,品种,交割期,买卖,开平,组合非组合查询标志,策略代码 */
//input:scust_no, sdate0, sdate1, scust_no2, sholder_ac_no, smarket_code, sstock_code, sdate2, sstatus0, sstatus1, sstatus2, spost_code2
//	/* 输出：日期,委托号,操作员号,客户号,客户名称,交易所,品种代码,交割期,委托单状态,开平标记,投保标记,买卖方向,委托价,委托量,剩余量,成交价,成交量,系统号,委托时间,申报时间,交易编码,委托方式,撤单用户,撤单时间,委托单类型,席位号,币种,备注,定单类型,成交属性,止损（盈）价,触发时间,组合类型,策略代码,合约1,合约2,合约3,合约4,私有信息,私有标识,私有流水号,最小成交数量*/
//	output : sdate0, lserial0, scust_no, sholder_ac_no, sname, smarket_code, sstock_code, sdate1, sstatus0, sstatus1, sstatus2, sstatus3, damt0, lvol0, lvol1, damt1, lvol2, sorder0, stime0, stime1, sholder_ac_no2, sholder_type, scust_no2, stime2, sstatus4, sserial2, scurrency_type, snote, scust_type, scust_type2, damt3, stime3, sstat_type, spost_code2, sphone, sphone2, sphone3, spager, sstation1, semp_no, scust_limit, lvol8
//

//日期		sdate0
//委托号		 lserial0
//操作员号		 scust_no
//客户号		 sholder_ac_no
//客户名称		 sname
//交易所		 smarket_code
//品种代码		 sstock_code
//交割期		 sdate1
//委托单状态		 sstatus0
//开平标记		 sstatus1
//投保标记		 sstatus2
//买卖方向		 sstatus3
//委托价		 damt0
//委托量		 lvol0
//剩余量		 lvol1
//成交价		 damt1
//成交量		 lvol2
//系统号		 sorder0
//委托时间		 stime0
//申报时间		 stime1
//交易编码		 sholder_ac_no2
//委托方式		 sholder_type
//撤单用户		 scust_no2
//撤单时间		 stime2
//委托单类型		 sstatus4
//席位号		 sserial2
//币种		 scurrency_type
//备注		 snote
//定单类型		 scust_type
//成交属性		 scust_type2
//止损（盈）价		 damt3
//触发时间		 stime3
//组合类型		 sstat_type
//策略代码		 spost_code2
//合约1		 sphone
//合约2		 sphone2
//合约3		 sphone3
//合约4		 spager
//私有信息		 sstation1
//私有标识		 semp_no
//私有流水号		 scust_limit
//最小成交数量		 lvol8


bool CQryOrderDialg::RequestOrder_852102()
{
	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
		cust_order_st o = { 0 };
		//char sholder_ac_no[16] = { 0 };
		//BCGetStringFieldByName(handle, row, "sholder_ac_no", sholder_ac_no, sizeof(sholder_ac_no));
		//assert(cust_no == sholder_ac_no);
		BCGetStringFieldByName(handle, row, "sdate0", o.date, sizeof(o.date));
		BCGetIntFieldByName(handle, row, "lserial0", &o.orderseq);
		BCGetStringFieldByName(handle, row, "scust_no", o.oper_no, sizeof(o.oper_no));
		BCGetStringFieldByName(handle, row, "smarket_code", o.exchangeid, sizeof(o.exchangeid));
		BCGetStringFieldByName(handle, row, "sstock_code", o.productid, sizeof(o.productid));
		BCGetStringFieldByName(handle, row, "sdate1", o.delivery_date, sizeof(o.delivery_date));
		BCGetStringFieldByName(handle, row, "sphone", o.instrumentid, sizeof(o.instrumentid));
		BCGetStringFieldByName(handle, row, "sstatus0", o.order_status, sizeof(o.order_status));
		BCGetStringFieldByName(handle, row, "sstatus1", o.offset_flag, sizeof(o.offset_flag));
		BCGetStringFieldByName(handle, row, "sstatus3", o.direction, sizeof(o.direction));
		BCGetStringFieldByName(handle, row, "sstatus2", o.hedge_flag, sizeof(o.hedge_flag));
		BCGetDoubleFieldByName(handle, row, "damt0", &o.order_price);
		BCGetIntFieldByName(handle, row, "lvol0", &o.origin_total_volume);
		BCGetIntFieldByName(handle, row, "lvol1", &o.total_volume);
		BCGetDoubleFieldByName(handle, row, "damt1", &o.traded_price);
		BCGetIntFieldByName(handle, row, "lvol2", &o.traded_volume);
		BCGetStringFieldByName(handle, row, "sorder0", o.sysid, sizeof(o.sysid));
		BCGetStringFieldByName(handle, row, "stime0", o.order_time, sizeof(o.order_time));
		BCGetStringFieldByName(handle, row, "stime1", o.insert_time, sizeof(o.insert_time));
		BCGetStringFieldByName(handle, row, "sholder_ac_no2", o.trade_no, sizeof(o.trade_no));
		BCGetStringFieldByName(handle, row, "sholder_type", o.order_way, sizeof(o.order_way));
		BCGetStringFieldByName(handle, row, "scust_no2", o.cancel_cust, sizeof(o.cancel_cust));
		BCGetStringFieldByName(handle, row, "stime2", o.cancel_time, sizeof(o.cancel_time));
		BCGetStringFieldByName(handle, row, "scust_type", o.order_type, sizeof(o.order_type));
		BCGetStringFieldByName(handle, row, "sserial2", o.seatid, sizeof(o.seatid));
		BCGetStringFieldByName(handle, row, "scurrency_type", o.currency, sizeof(o.currency));
		//BCGetStringFieldByName(handle, row, "sstatus1", o.force_close, sizeof(o.force_close));
		m_vOrder.push_back(o);
	};
	Scoped_BCHANDLE handle;
	BCSetRequestType(handle, 852102);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	CString strDate = m_DateTime.Format("%Y%m%d");
	BCSetStringFieldByName(handle, 0, "sdate0", strDate.GetBuffer());
	BCSetStringFieldByName(handle, 0, "sdate1", strDate.GetBuffer());
	CString str = m_ctlCust.GetCurSelKey();
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", str.GetBuffer());
	
	return MyBCRequest(handle, fetcher, BCMSG_TOOP);
}
