// QrySettlementDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "QrySettlementDialog.h"
#include "afxdialogex.h"
#include "Format.h"


static const char *tip_line = "-------------------------------------------------------------------------------------\r\n";

// CQrySettlementDialog �Ի���

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


// CQrySettlementDialog ��Ϣ�������


BOOL CQrySettlementDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CQrySettlementDialog::OnBnClickedButtonQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	//UpdateData(FALSE);

	UpdateData(TRUE);
	CString cust_no = m_ctlCust.GetCurSelKey();
	if (cust_no.GetLength() == 0) {
		MessageBox("��ѡ��ͻ�");
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
///* ������ͻ���		output:sholder_ac_no
//�ͻ�����		sname
//����		scurrency_type
//���ս��		damt0
//����		damt1
//���		damt2
//������		damt3
//�Ͻ�������		damt4
//����ӯ��		damt5
//��������		damt6
//���ս��		damt7
//����ӯ��		damt8
//��;�ʽ�		damt9
//δ����ӯ��		damt10
//��ĩȨ��		damt11
//�ͻ���֤��		damt12
//������֤��		damt13
//�����ʽ�		damt14
//��Ѻ���		damt15
//��������֤��		damt16
//���ն�		scust_auth
//׷�ӱ�֤��		damt18
//�ͻ����		sserial1
//�ͻ�������		sall_name
//���ռ���		sstatus0
//����ӯ��		damt19
//ƽ��ӯ��_��		damt20
//ƽ��ӯ��_��		damt21
//�ֲ�ӯ��_��		damt22
//ƽ��ӯ��		damt23
//���֤��		damt24
//����������		damt25
//�����ʽ�		damt26
//��ӯ�� */		damt27
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
	BCSetStringFieldByName(handle, 0, "sstatus0", "0"); //�Ƿ���ϼƣ�
	BCSetStringFieldByName(handle, 0, "sstatus1", "2"); //�ʵ�ģʽ��
	BCSetStringFieldByName(handle, 0, "sstatus2", "1"); //ǿƽ��׼��
	
	memset(&m_fund, 0, sizeof(m_fund));

	return MyBCRequest(handle, fetcher, BCMSG_TOOP);
}

//
///* ���������		output:sdate0
//�ͻ���		sholder_ac_no
//�ɽ����		lvol2
//������		smarket_code
//ϯλ		sserial2
//Ʒ��		sstock_code
//������		sdate1
//����		scurrency_type
//����		sstatus2
//��ƽ		sstatus3
//Ͷ��		sstatus4
//�ɽ�����		lvol1
//�ɽ���		damt2
//������		damt3
//��������		damt6
//�������ɽ����		scust_auth
//�ɽ���		damt1
//ƽ��ӯ�� */		damt11


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
	BCSetStringFieldByName(handle, 0, "sstatus1", "2"); //�ʵ�ģʽ��
	BCSetStringFieldByName(handle, 0, "scurrency_type", "1");
	BCSetStringFieldByName(handle, 0, "sstatus0", "0"); //�Ƿ���ϼ�
	BCSetStringFieldByName(handle, 0, "sstatus1", ""); //�Ƿ�ϲ�
	
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
	BCSetStringFieldByName(handle, 0, "sstatus1", "2"); //�ʵ�ģʽ��
	BCSetStringFieldByName(handle, 0, "scurrency_type", "1");
	BCSetStringFieldByName(handle, 0, "sstatus0", "0"); //�Ƿ���ϼ�
	BCSetStringFieldByName(handle, 0, "sstatus1", ""); //�Ƿ�ϲ�

	m_OffDetails.clear();

	return MyBCRequest(handle, fetcher, BCMSG_NOR_NOMB);
}

//
///* �������������		output:sdate0
//�ɽ����		lserial1
//�ͻ���		sholder_ac_no
//������		smarket_code
//ϯλ������ʾ��		sserial2
//Ʒ��		sstock_code
//������		sdate1
//����		sstatus2
//����		lvol1
//�۸�		damt0
//�����		damt1
//����		damt2
//��֤��		damt3
//����ӯ��		damt4
//�ֲ�ӯ��������		damt5
//���ױ���		sholder_ac_no2
//Ͷ��		sstatus3
//�������ɽ��� */		scust_auth

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
	BCSetStringFieldByName(handle, 0, "sstatus1", "2"); //�ʵ�ģʽ��
	BCSetStringFieldByName(handle, 0, "scurrency_type", "1");
	BCSetStringFieldByName(handle, 0, "sstatus0", "0"); //�Ƿ���ϼ�
	BCSetStringFieldByName(handle, 0, "sstatus1", ""); //�Ƿ�ϲ�

	m_PosiDetails.clear();

	return MyBCRequest(handle, fetcher);
}

//
///* ������ͻ���			output:sholder_ac_no
//������			smarket_code
//ϯλ������ʾ��			sserial2
//Ʒ��			sstock_code
//������			sdate1
//Ͷ��			sstatus3
//�����			lvol2
//������			lvol3
//�����			damt6
//������			damt7
//�����			damt1
//����			damt2
//��֤��			damt3
//����ӯ��			damt4
//�ֲ�ӯ�������� */			damt5

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
	BCSetStringFieldByName(handle, 0, "sstatus1", "2"); //�ʵ�ģʽ��
	BCSetStringFieldByName(handle, 0, "scurrency_type", "1");
	BCSetStringFieldByName(handle, 0, "sstatus0", "0");

	m_PosiSummary.clear();

	return MyBCRequest(handle, fetcher, BCMSG_NOR_NOMB);
}

CString CQrySettlementDialog::format_title()
{
	CString str;
	str += "             ���׽��㵥                     \r\n";
	str += CString("   �ͻ���: ") + m_fund.cust_no + "   �ͻ�����: " + m_fund.cust_name + "    ����: " + m_DateTime.Format("%Y%m%d") + "\r\n";
	str += "\r\n\r\n";
	return str;
}

CString CQrySettlementDialog::format_fund()
{
	//CString fund = "�ʽ�״�� ����:�����\r\n";
	//fund += tip_line;
	//fund += "   ���ս��:" + fundfmt(m_fund.yd_balance) + "     ���ս��:" + fundfmt(m_fund.td_balance) + "     �����ʽ�:" + fundfmt(m_fund.available) + "\r\n";
	//fund += "     �����:" + fundfmt(m_fund.out_money) +  "     �ͻ�Ȩ��:" + fundfmt(m_fund.td_balance) + "   ���ռ���:" + theApp.DictToStr(DICT_CLASS_RISK_LEVEL, m_fund.risk_level) + "\r\n";
	//fund += "     ������:" + fundfmt(m_fund.commission) + "     ӯ��:" + fundfmt(m_fund.total_profit) + "\r\n";

	CString str = "�ʽ�״�� ����:�����\r\n";
	str += tip_line;
	char buff[1024];
	sprintf(buff, "%-14s:%-14.2f %-14s:%14.2f  %-14s:%14.2f\r\n",
		"���ս��", m_fund.yd_balance, "���ս��", m_fund.td_balance, "�ͻ�Ȩ��", m_fund.td_balance);
	str += buff;

	sprintf(buff, "%-14s:%-14.2f %-14s:%14.2f  %-14s:%14.2f\r\n",
		"���", m_fund.in_money, "����", m_fund.out_money, "�����ʽ�", m_fund.available);
	str += buff;

	sprintf(buff, "%-14s:%-14.2f %-14s:%14.2f  %-14s:%14.2f\r\n",
		"������", m_fund.commission, "�Ͻ�������", m_fund.sj_commission, "����������", m_fund.delivery_commission);
	str += buff;

	sprintf(buff, "%-14s:%-14.2f %-14s:%14.2f  %-14s:%14.2f %-14s:%14.2f\r\n",
		"��֤��", m_fund.margin, "������֤��", m_fund.base_margin, "׷�ӱ�֤��", m_fund.add_margin, "���֤��", m_fund.delivery_margin);
	str += buff;

	sprintf(buff, "%-14s:%-14.2f %-14s:%14.2f  %-14s:%14.2f  %-14s:%14.2f\r\n",
		 "��ӯ��", m_fund.total_profit, "�ֲ�ӯ��", m_fund.hold_profit, "ƽ��ӯ��", m_fund.offset_profit, "����ӯ��", m_fund.delivery_profit);
	str += buff;

	str += "\r\n\r\n";
	return str;

	//m_SettleStr += fund;
}

CString CQrySettlementDialog::format_trade()
{
	CString str = "�ɽ���¼\r\n";
	str += tip_line;
	char buff[1024];
	sprintf(buff, "%-10s%-12s%-10s%-10s%-6s%-6s%-6s%-8s%-15s%-15s\r\n",
		"�ɽ�����", "������","Ʒ��","��Լ","����", "��ƽ", "Ͷ��", "����", "�ɽ���", "������");
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
	CString str = "ƽ����ϸ\r\n";
	str += tip_line;
	char buff[1024];
	sprintf(buff, "%-10s%-12s%-10s%-10s%-6s%-8s%-15s%-10s%-15s%-15s%-15s\r\n",
		"ƽ������", "������", "Ʒ��", "��Լ", "����", "����", "ƽ�ּ�", "��������", "���ּ۸�", "�����", "ƽ��ӯ��(��)");
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
	CString str = "�ֲ���ϸ\r\n";
	str += tip_line;
	char buff[1024];
	sprintf(buff, "%-10s%-12s%-10s%-10s%-6s%-6s%-8s%-15s%-15s%-15s%-15s%-15s\r\n",
		"��������", "������", "Ʒ��", "��Լ", "����", "Ͷ��", "�ֲ���", "�ֲּ�", "�����", "�����", "����ӯ��", "��֤��");
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
	CString str = "�ֲֻ���\r\n";
	str += tip_line;
	char buff[1024];
	sprintf(buff, "%-12s%-10s%-10s%-6s%-8s%-8s%-15s%-15s%-15s%-15s%-15s%-15s%-15s\r\n",
		 "������", "Ʒ��", "��Լ", "Ͷ��", "�����", "������","�����","������","�����","�����","��֤��","����ӯ��","�ֲ�ӯ��(��)");
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
	//str += "�Ʊ�:ϵͳ����Ա          ";
	//str += "�Ʊ�����:\r\n";
	//str += CString("�ͻ���:" )+ m_fund.cust_no + "        ";

	//str += CString("   �ͻ���: ") + m_fund.cust_no + "   �ͻ�����: " + m_fund.cust_name + "    ����: " + m_DateTime.Format("%Y%m%d") + "\r\n";
	//str += "\r\n\r\n";
	//return str;
	return CString();
}
