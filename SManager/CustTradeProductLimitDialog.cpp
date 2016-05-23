// CustTradeProductLimitDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "CustTradeProductLimitDialog.h"
#include "afxdialogex.h"

#include "CommBCRequest.h"
#include "Format.h"
#include "CustTradeProductLimitDialog.h"


// CCustTradeProductLimitDialog �Ի���

IMPLEMENT_DYNAMIC(CCustTradeProductLimitDialog, CDialogEx)

CCustTradeProductLimitDialog::CCustTradeProductLimitDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CUST_TRADE_PRODUCT_LIMIT, pParent)
{

}

CCustTradeProductLimitDialog::~CCustTradeProductLimitDialog()
{
}

void CCustTradeProductLimitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CUST, m_ctlCust);
	DDX_Control(pDX, IDC_LIST_CUST_TRADE_PRODUCT_LIMIT, m_listctrl);
}


BEGIN_MESSAGE_MAP(CCustTradeProductLimitDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CCustTradeProductLimitDialog::OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CCustTradeProductLimitDialog::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CCustTradeProductLimitDialog::OnBnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CCustTradeProductLimitDialog::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CCustTradeProductLimitDialog ��Ϣ�������
BOOL CCustTradeProductLimitDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	LVCOLUMN cols2[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "�ͻ���" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "������" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "Ʒ��" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "������" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "��������" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "��֤����(����)" }
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 120, "ƽ���ַ�����(���)" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 120, "ƽ���ַ�����(����)" }
	};

	for (int i = 0; i < sizeof(cols2) / sizeof(cols2[0]); ++i)
		m_listctrl.InsertColumn(i, &cols2[i]);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CCustTradeProductLimitDialog::OnBnClickedButtonQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_listctrl.DeleteAllItems();
	CString cust_no = m_ctlCust.GetCurSelKey();
	if (cust_no.GetLength() == 0)
		return;

	Scoped_BCHANDLE handle;

	BCRequestQryTradeProductLimit_854152(handle, cust_no.GetBuffer(), m_tpls);
	for (auto it = m_tpls.begin(); it != m_tpls.end(); ++it)
	{
		int item = std::distance(m_tpls.begin(), it);
		m_listctrl.InsertItem(item, it->cust_no);
		m_listctrl.SetItemText(item, 1, it->exchangeid);
		m_listctrl.SetItemText(item, 2, it->productid);
		m_listctrl.SetItemText(item, 2, it->delivery_date);
		m_listctrl.SetItemText(item, 3, it->limit_type);
	}
}


void CCustTradeProductLimitDialog::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCustTradeProductLimitSettingDailog dlg;
	dlg.m_flag = "1";
	dlg.m_ctlCust.SelectKey(m_ctlCust.GetCurSelKey());
	//if (dlg.DoModal() != IDCANCEL)
	//{
	//	OnBnClickedButtonQuery();
	//}
	dlg.DoModal();
	OnBnClickedButtonQuery();

}


void CCustTradeProductLimitDialog::OnBnClickedButtonModify()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel = m_listctrl.GetSelectionMark();
	if (sel < 0 || sel >= m_tpls.size())
	{
		MessageBox("����ѡ��");
		return;
	}

	CCustTradeProductLimitSettingDailog dlg;
	cust_margin_rate_st cmr = m_tpls[sel];
	dlg.m_flag = "2";
	dlg.m_ctlCust.SelectKey(cmr.cust_no);
	dlg.m_ctlExchange.SelectKey(cmr.exchangeid);
	dlg.m_ctlProduct.SelectKey(cmr.productid);
	//dlg.m_ByAmt = cmr.margin_rate_by_money;
	//dlg.m_ByVol = cmr.margin_rate_by_volume;

	//if (dlg.DoModal() != IDCANCEL)
	//{
	//	OnBnClickedButtonQuery();
	//}

	dlg.DoModal();
	OnBnClickedButtonQuery();
}


void CCustTradeProductLimitDialog::OnBnClickedButtonDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel = m_listctrl.GetSelectionMark();
	if (sel < 0 || sel >= m_tpls.size())
	{
		MessageBox("����ѡ��");
		return;
	}

	CCustTradeProductLimitSettingDailog dlg;
	cust_margin_rate_st cmr = m_tpls[sel];
	dlg.m_flag = "3";
	dlg.m_ctlCust.SelectKey(cmr.cust_no);
	dlg.m_ctlExchange.SelectKey(cmr.exchangeid);
	dlg.m_ctlProduct.SelectKey(cmr.productid);
	dlg.m_ByAmt = cmr.margin_rate_by_money;
	dlg.m_ByVol = cmr.margin_rate_by_volume;

	//if (dlg.DoModal() != IDCANCEL)
	//{
	//	OnBnClickedButtonQuery();
	//}

	dlg.DoModal();
	OnBnClickedButtonQuery();
}

