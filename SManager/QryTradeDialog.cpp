// QryTradeDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "QryTradeDialog.h"
#include "afxdialogex.h"


// CQryTradeDialog �Ի���

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


// CQryTradeDialog ��Ϣ�������


BOOL CQryTradeDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	LVCOLUMN cols[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "ί�б��" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "�ɽ�ʱ��" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "��Լ" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "����" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "��ƽ" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "�ɽ��۸�" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "�ɽ�����" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "�ɽ����" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "������" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "ǿƽ��־" }
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "ӯ��" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "�����˻�" }
	};

	for (int i = 0; i < sizeof(cols) / sizeof(cols[0]); ++i)
		m_listctrl.InsertColumn(i, &cols[i]);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CQryTradeDialog::OnBnClickedButtonQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

//function 852103 description  "��ʷʵʱ�ɽ���ѯ"  reqlevel 3
///* ���룺����Ա����ʼ���ڣ��������ڣ���ѯ����Ա���ͻ��ţ���������Ʒ�֣������ڣ���������ƽ */
//input:scust_no, sdate0, sdate1, scust_no2, sholder_ac_no, smarket_code, sstock_code, sdate2, sstatus0, sstatus1
//	/* ��������ڣ�ί�кţ��ͻ��ţ��ͻ����ƣ����ױ��룬��������Ʒ�ִ��룬�����ڣ��������򣬳ɽ��ۣ��ɽ�������ƽ��ǣ�Ͷ����ǣ�ϵͳ�ţ��ɽ��ţ��������ɽ��ţ�ϯλ�ţ����֣��ɽ�ʱ�䣬ǿƽ��־ */
//	output : sdate0, lserial0, sholder_ac_no, sname, sholder_ac_no2, smarket_code, sstock_code, sdate1, sstatus3, damt1, lvol2, sstatus1, sstatus2, sorder0, lvol3, sorder2, sserial2, scurrency_type, stime0, sstatus0
//
//
//���������		output : sdate0
//ί�к�		 lserial0
//�ͻ���		 sholder_ac_no
//�ͻ�����		 sname
//���ױ���		 sholder_ac_no2
//������		 smarket_code
//Ʒ�ִ���		 sstock_code
//������		 sdate1
//��������		 sstatus3
//�ɽ���		 damt1
//�ɽ���		 lvol2
//��ƽ���		 sstatus1
//Ͷ�����		 sstatus2
//ϵͳ��		 sorder0
//�ɽ���		 lvol3
//�������ɽ���		 sorder2
//ϯλ��		 sserial2
//����		 scurrency_type
//�ɽ�ʱ��		 stime0
//ǿƽ��־ 		 sstatus0

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
