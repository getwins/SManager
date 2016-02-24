// ProductDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "ProductDialog.h"
#include "afxdialogex.h"
//#include <boost/lexical_cast.hpp>

// CProductDialog �Ի���
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


// CProductDialog ��Ϣ�������


BOOL CProductDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "��ͷ" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "����" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "���׹���" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 200, "��Լ����" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "����" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "ÿ������" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "��С�䶯��" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "��С����" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "�������" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "���ֲ�" }
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "��ͣ����" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "��ͣ����" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "������(����)" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "������(���)" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "��֤��(������" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "��֤��(���)" }
	};

	for (int i = 0; i < sizeof(cols) / sizeof(cols[0]); ++i)
		m_listctrl.InsertColumn(i, &cols[i]);
	m_ComboExchange.InsertString(0, (CString("!-") + "ȫ��"));
	m_ComboExchange.SetCurSel(0);
	//ResetList();
	m_ctlStatus.SetCurSel(0);
	ResetList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CProductDialog::OnCbnSelchangeComboExchange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		//	strcpy(buff, "ÿ�µ�15��");
		//else if (strcmp("2", rule) == 0)
		//	strcpy(buff, "��10������");
		//else if (strcmp("3", rule) == 0)
		//	strcpy(buff, "�������߽�����");
		//else if (strcmp("4", rule) == 0)
		//	strcpy(buff, "������");
		//else if (strcmp("5", rule) == 0)
		//	strcpy(buff, "����");
		//else if (strcmp("6", rule) == 0)
		//	strcpy(buff, "������ǰһ������");
		//else if (strcmp("7", rule) == 0)
		//	strcpy(buff, "������������");
		//else if (strcmp("8", rule) == 0)
		//	strcpy(buff, "�ڶ���������");
		//else if (strcmp("9", rule) == 0)
		//	strcpy(buff, "��5��������");
		//else if (strcmp("A", rule) == 0)
		//	strcpy(buff, "��ĺ�Լ������ǰһ���µĵ�15��������");
		//else if (strcmp("B", rule) == 0)
		//	strcpy(buff, "����ڻ�������ǰ2���µ����1��������");


		//m_listctrl.SetItemText(index, 2, buff);
		const char *trade_rule = theApp.DictToStr(DICT_CLASS_LAST_TRADEDAY_RULE, p.trade_rule);
		m_listctrl.SetItemText(index, 2, trade_rule);

		//memset(buff, 0, sizeof(buff));
		//const char *rule = p.inst_rule;
		//if (strcmp("01", rule) == 0)
		//	strcpy(buff, "������");
		//else if (strcmp("02", rule) == 0)
		//	strcpy(buff, "˫��");
		//else if (strcmp("03", rule) == 0)
		//	strcpy(buff, "����");
		//else if (strcmp("04", rule) == 0)
		//	strcpy(buff, "����");
		//else if (strcmp("05", rule) == 0)
		//	strcpy(buff, "���������¼�������������");
		//else if (strcmp("06", rule) == 0)
		//	strcpy(buff, "���������¼�������������");
		//else if (strcmp("07", rule) == 0)
		//	strcpy(buff, "ʮ���������¼�ʮ������������");
		//else if (strcmp("08", rule) == 0)
		//	strcpy(buff, "���������¼����ʮһ�����ڵ�˫�º�Լ");
		//else if (strcmp("99", rule) == 0)
		//	strcpy(buff, "�����Լ");

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ResetList();
}
