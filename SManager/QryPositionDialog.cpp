// QryPositionDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "QryPositionDialog.h"
#include "afxdialogex.h"


// CQryPositionDialog �Ի���

IMPLEMENT_DYNAMIC(CQryPositionDialog, CDialogEx)

CQryPositionDialog::CQryPositionDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_QUERY_POSITION, pParent)
	, m_DateTime(COleDateTime::GetCurrentTime())
{

}

CQryPositionDialog::~CQryPositionDialog()
{
}

void CQryPositionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_QUERY_CUST, m_ctlCust);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER, m_DateTime);
	DDX_Control(pDX, IDC_LIST_RECORD, m_listctrl);
}


BEGIN_MESSAGE_MAP(CQryPositionDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CQryPositionDialog::OnBnClickedButtonQuery)
END_MESSAGE_MAP()


// CQryPositionDialog ��Ϣ�������


void CQryPositionDialog::OnBnClickedButtonQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_vPosition.clear();
	m_listctrl.DeleteAllItems();
	CString str;
	if (RequestPosition_851401()) {
		

		for (int i = 0; i < m_vPosition.size(); i++) {
			cust_position_st &p = m_vPosition[i];
			m_listctrl.InsertItem(i, p.instrumentid);
			m_listctrl.SetItemText(i, 1, theApp.DictToStr(DICT_CLASS_BUY_SELL, p.direction));
			str.Format("%d", p.positions);
			m_listctrl.SetItemText(i, 2, str);
			str.Format("%f", p.hold_average_price);
			m_listctrl.SetItemText(i, 3, str);
			str.Format("%f", p.settle_price);
			m_listctrl.SetItemText(i, 4, str);
			str.Format("%f", p.float_profit);
			m_listctrl.SetItemText(i, 5, str);
			str.Format("%f", p.market_profit);
			m_listctrl.SetItemText(i, 6, str);
		}
	}
}


BOOL CQryPositionDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	LVCOLUMN cols[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "��Լ" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "����" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "�ֲ�" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "�ֲ־���" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "�����" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "����ӯ��" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "����ӯ��" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "�����˻�" }
	};

	for (int i = 0; i < sizeof(cols) / sizeof(cols[0]); ++i)
		m_listctrl.InsertColumn(i, &cols[i]);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//function 851401 description  "�ͻ��ֲֲ�ѯ"
///* ���룺����Ա�����ڣ��ͻ��ţ��ͻ��࣬��������Ʒ�ִ��룬������,ϯλ����Ʒ�ֽ����ںϼƣ����ױ��룬���ɺϼ�,�Ƿ������Ʋ�ѯ */
//input:scust_no, sdate0, sholder_ac_no, sserial1, smarket_code, sstock_code, sdate1, sserial2, lvol1, sholder_ac_no2, sstatus0, semp_no2
//	/* ������ͻ��ţ��ͻ����ƣ����֣�ϯλ����������Ʒ�ִ��룬�����ڣ��������������۸񣬽���ۣ���֤�𣬸���ӯ�������ױ��룬Ͷ���������ʽ𣬿�������,zryk */
//	output : sholder_ac_no, sname, scurrency_type, sserial2, smarket_code, sstock_code, sdate1, sstatus1, lvol10, damt0, damt1, damt2, damt3, sholder_ac_no2, sstatus2, damt4, sdate0, damt5

///* ������ͻ���		output:sholder_ac_no
//�ͻ�����		sname
//����		scurrency_type
//ϯλ		sserial2
//������		smarket_code
//Ʒ�ִ���		sstock_code
//������		sdate1
//����		sstatus1
//����		lvol10
//�۸�		damt0
//�����		damt1
//��֤��		damt2
//����ӯ��		damt3
//���ױ���		sholder_ac_no2
//Ͷ��		sstatus2
//�����ʽ�		damt4
//��������		sdate0
//zryk */		damt5
bool CQryPositionDialog::RequestPosition_851401()
{
	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
		cust_position_st p = { 0 };
		BCGetStringFieldByName(handle, row, "sdate0", p.open_date, sizeof(p.open_date));
		BCGetStringFieldByName(handle, row, "smarket_code", p.exchangeid, sizeof(p.exchangeid));
		BCGetStringFieldByName(handle, row, "sserial2", p.exchangeid, sizeof(p.seatid));
		BCGetStringFieldByName(handle, row, "sstock_code", p.product, sizeof(p.product));
		BCGetStringFieldByName(handle, row, "sdate1", p.delivery_date, sizeof(p.delivery_date));
		BCGetStringFieldByName(handle, row, "sphone", p.instrumentid, sizeof(p.instrumentid));
		BCGetStringFieldByName(handle, row, "sstatus1", p.direction, sizeof(p.direction));
		BCGetStringFieldByName(handle, row, "sstatus2", p.hedge_flag, sizeof(p.hedge_flag));
		BCGetIntFieldByName(handle, row, "lvol10", &p.positions);
		//BCGetIntFieldByName(handle, row, "lvol12", &p.td_positions);
		BCGetDoubleFieldByName(handle, row, "damt0", &p.hold_average_price);
		//BCGetDoubleFieldByName(handle, row, "damt0", &p.open_average_price);
		BCGetDoubleFieldByName(handle, row, "damt3", &p.float_profit);
		//BCGetDoubleFieldByName(handle, row, "damt23", &p.last_price);
		BCGetDoubleFieldByName(handle, row, "damt1", &p.settle_price);
		//BCGetIntFieldByName(handle, row, "lvol10", &p.multiple);
		BCGetDoubleFieldByName(handle, row, "damt5", &p.market_profit);
		BCGetDoubleFieldByName(handle, row, "damt2", &p.marign);
		this->m_vPosition.push_back(p);
	};

	//���룺����Ա			input : scust_no
	//	����			sdate0
	//	�ͻ���			sholder_ac_no
	//	�ͻ���			sserial1
	//	������			smarket_code
	//	Ʒ�ִ���			sstock_code
	//	������			sdate1
	//	ϯλ			sserial2
	//	��Ʒ�ֽ����ںϼ�			lvol1
	//	���ױ���			sholder_ac_no2
	//	���ɺϼ�			sstatus0
	//	�Ƿ������Ʋ�ѯ 			semp_no2

	Scoped_BCHANDLE handle;
	BCSetRequestType(handle, 851401);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	CString strDate = m_DateTime.Format("%Y%m%d");
	BCSetStringFieldByName(handle, 0, "sdate0", strDate.GetBuffer());
	CString strCust = m_ctlCust.GetCurSelKey();
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", strCust.GetBuffer());
	BCSetStringFieldByName(handle, 0, "lvol1", "1"); //��Ʒ�ֽ����ںϼ�
	BCSetStringFieldByName(handle, 0, "sstatus0", "1"); //���ɺϼ�
	
	return MyBCRequest(handle, fetcher, BCMSG_TOOP);
}
