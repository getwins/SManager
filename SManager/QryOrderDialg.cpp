// QryOrderDialg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "QryOrderDialg.h"
#include "afxdialogex.h"


// CQryOrderDialg �Ի���

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


// CQryOrderDialg ��Ϣ�������


void CQryOrderDialg::OnBnClickedButtonQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
//	// TODO: �ڴ����ר�ô����/����û���
//
//	return CDialogEx::OnNotify(wParam, lParam, pResult);
//}


BOOL CQryOrderDialg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//std::vector<std::string> cust_nos = theApp.GetCustnoList();
	//for each (std::string cust in theApp.GetCustnoList())
	//{
	//	m_ctlCust.AddString(cust.c_str());
	//}

	LVCOLUMN cols[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "ί�б��" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "ί��ʱ��" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "��Լ" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "����" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "��ƽ" },		
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "ί�м۸�" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "ί����" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "δ�ɽ�" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "�ѳɽ�" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "��ϸ״̬" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 120, "ί��״̬" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "ǿƽ" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "�����Ϣ" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "������" }
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "�����˻�" },
	};

	for (int i = 0; i < sizeof(cols) / sizeof(cols[0]); ++i)
		m_listctrl.InsertColumn(i, &cols[i]);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//function 852102 description  "��ʷʵʱί�в�ѯ"  reqlevel 3
///* ���룺����Ա,��ʼ����,��������,��ѯ����Ա,�ͻ���,������,Ʒ��,������,����,��ƽ,��Ϸ���ϲ�ѯ��־,���Դ��� */
//input:scust_no, sdate0, sdate1, scust_no2, sholder_ac_no, smarket_code, sstock_code, sdate2, sstatus0, sstatus1, sstatus2, spost_code2
//	/* ���������,ί�к�,����Ա��,�ͻ���,�ͻ�����,������,Ʒ�ִ���,������,ί�е�״̬,��ƽ���,Ͷ�����,��������,ί�м�,ί����,ʣ����,�ɽ���,�ɽ���,ϵͳ��,ί��ʱ��,�걨ʱ��,���ױ���,ί�з�ʽ,�����û�,����ʱ��,ί�е�����,ϯλ��,����,��ע,��������,�ɽ�����,ֹ��ӯ����,����ʱ��,�������,���Դ���,��Լ1,��Լ2,��Լ3,��Լ4,˽����Ϣ,˽�б�ʶ,˽����ˮ��,��С�ɽ�����*/
//	output : sdate0, lserial0, scust_no, sholder_ac_no, sname, smarket_code, sstock_code, sdate1, sstatus0, sstatus1, sstatus2, sstatus3, damt0, lvol0, lvol1, damt1, lvol2, sorder0, stime0, stime1, sholder_ac_no2, sholder_type, scust_no2, stime2, sstatus4, sserial2, scurrency_type, snote, scust_type, scust_type2, damt3, stime3, sstat_type, spost_code2, sphone, sphone2, sphone3, spager, sstation1, semp_no, scust_limit, lvol8
//

//����		sdate0
//ί�к�		 lserial0
//����Ա��		 scust_no
//�ͻ���		 sholder_ac_no
//�ͻ�����		 sname
//������		 smarket_code
//Ʒ�ִ���		 sstock_code
//������		 sdate1
//ί�е�״̬		 sstatus0
//��ƽ���		 sstatus1
//Ͷ�����		 sstatus2
//��������		 sstatus3
//ί�м�		 damt0
//ί����		 lvol0
//ʣ����		 lvol1
//�ɽ���		 damt1
//�ɽ���		 lvol2
//ϵͳ��		 sorder0
//ί��ʱ��		 stime0
//�걨ʱ��		 stime1
//���ױ���		 sholder_ac_no2
//ί�з�ʽ		 sholder_type
//�����û�		 scust_no2
//����ʱ��		 stime2
//ί�е�����		 sstatus4
//ϯλ��		 sserial2
//����		 scurrency_type
//��ע		 snote
//��������		 scust_type
//�ɽ�����		 scust_type2
//ֹ��ӯ����		 damt3
//����ʱ��		 stime3
//�������		 sstat_type
//���Դ���		 spost_code2
//��Լ1		 sphone
//��Լ2		 sphone2
//��Լ3		 sphone3
//��Լ4		 spager
//˽����Ϣ		 sstation1
//˽�б�ʶ		 semp_no
//˽����ˮ��		 scust_limit
//��С�ɽ�����		 lvol8


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
