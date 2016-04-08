// CustCommissionDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "CustCommissionDialog.h"
#include "afxdialogex.h"

#include "CommBCRequest.h"
#include "Format.h"
#include "CustCommissionRateSettingDialog.h"
#include <boost/algorithm/string.hpp>
// CCustCommissionDialog �Ի���

IMPLEMENT_DYNAMIC(CCustCommissionDialog, CDialogEx)

CCustCommissionDialog::CCustCommissionDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CUST_COMMISSION, pParent)
{

}

CCustCommissionDialog::~CCustCommissionDialog()
{
}

void CCustCommissionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CUST, m_ctlCust);
	DDX_Control(pDX, IDC_LIST_CUST_COMMISSION, m_listctrl);
}


BEGIN_MESSAGE_MAP(CCustCommissionDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CCustCommissionDialog::OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CCustCommissionDialog::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CCustCommissionDialog::OnBnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CCustCommissionDialog::OnBnClickedButtonDelete)
END_MESSAGE_MAP()

BOOL CCustCommissionDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	LVCOLUMN cols2[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "�ͻ���" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "������" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "Ʒ��" },	
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "������" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "�ַ�����(���)" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "�ַ�����(����)" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 120, "ƽ���ַ�����(���)" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 120, "ƽ���ַ�����(����)" }
	};

	for (int i = 0; i < sizeof(cols2) / sizeof(cols2[0]); ++i)
		m_listctrl.InsertColumn(i, &cols2[i]);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CCustCommissionDialog::ResetList()
{
	m_listctrl.DeleteAllItems();
	for (auto it = m_ccrs.begin(); it != m_ccrs.end(); ++it)
	{
		int item = std::distance(m_ccrs.begin(), it);
		m_listctrl.InsertItem(item, it->cust_no);
		m_listctrl.SetItemText(item, 1, it->exchangeid);
		m_listctrl.SetItemText(item, 2, it->productid);
		m_listctrl.SetItemText(item, 3, it->delivery_date);
		m_listctrl.SetItemText(item, 4, Nfmt(it->commission_rate_by_money));
		m_listctrl.SetItemText(item, 5, Nfmt(it->commission_rate_by_volume));
		m_listctrl.SetItemText(item, 6, Nfmt(it->offset_td_commission_rate_by_money));
		m_listctrl.SetItemText(item, 7, Nfmt(it->offset_td_commission_rate_by_volume));
	}
}

// CCustCommissionDialog ��Ϣ�������


void CCustCommissionDialog::OnBnClickedButtonQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	CString cust_no = m_ctlCust.GetCurSelKey();
	if (cust_no.GetLength() == 0)
		return;

	BCRequestQryCustCommissionRate_851305(m_handle, cust_no.GetBuffer(), m_ccrs);
	ResetList();
}


void CCustCommissionDialog::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCustCommissionRateSettingDialog dlg;
	dlg.m_flag = "4";
	dlg.m_ctlCust.SelectKey(m_ctlCust.GetCurSelKey());
	//if (dlg.DoModal() != IDCANCEL)
	//{
	//	OnBnClickedButtonQuery();
	//}

	dlg.DoModal();
	OnBnClickedButtonQuery();
}


void CCustCommissionDialog::OnBnClickedButtonModify()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel = m_listctrl.GetSelectionMark();
	if (sel < 0 || sel >= m_ccrs.size())
	{
		MessageBox("����ѡ��");
		return;
	}
	cust_commission_rate_st ccr = m_ccrs[sel];
	CCustCommissionRateSettingDialog dlg;
	dlg.m_flag = "5";
	dlg.m_ctlCust.SelectKey(ccr.cust_no);
	dlg.m_ctlExchange.SelectKey(ccr.exchangeid);
	dlg.m_ctlProduct.SelectKey(ccr.productid);
	//boost::trim(ccr.delivery_date);
	dlg.m_Date = boost::trim_copy(std::string(ccr.delivery_date)).c_str();
	dlg.m_ByAmt = ccr.commission_rate_by_money;
	dlg.m_ByVol = ccr.commission_rate_by_volume;
	dlg.m_TdByAmt = ccr.offset_td_commission_rate_by_money;
	dlg.m_TdByVol = ccr.offset_td_commission_rate_by_volume;
	//if (dlg.DoModal() != IDCANCEL)
	//{
	//	OnBnClickedButtonQuery();
	//}

	dlg.DoModal();
	OnBnClickedButtonQuery();
}


void CCustCommissionDialog::OnBnClickedButtonDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel = m_listctrl.GetSelectionMark();
	if (sel < 0 || sel >= m_ccrs.size())
	{
		MessageBox("����ѡ��");
		return;
	}
	cust_commission_rate_st ccr = m_ccrs[sel];
	CCustCommissionRateSettingDialog dlg;
	dlg.m_flag = "3";
	dlg.m_ctlCust.SelectKey(ccr.cust_no);
	dlg.m_ctlExchange.SelectKey(ccr.exchangeid);
	dlg.m_ctlProduct.SelectKey(ccr.productid);
	//boost::trim(ccr.delivery_date);
	dlg.m_Date = boost::trim_copy(std::string(ccr.delivery_date)).c_str();
	dlg.m_ByAmt = ccr.commission_rate_by_money;
	dlg.m_ByVol = ccr.commission_rate_by_volume;
	dlg.m_TdByAmt = ccr.offset_td_commission_rate_by_money;
	dlg.m_TdByVol = ccr.offset_td_commission_rate_by_volume;
	//if (dlg.DoModal() != IDCANCEL)
	//{
	//	OnBnClickedButtonQuery();
	//}

	dlg.DoModal();
	OnBnClickedButtonQuery();
}



