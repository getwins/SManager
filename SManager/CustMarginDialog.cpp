// CustMarginDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "CustMarginDialog.h"
#include "afxdialogex.h"
#include "CommBCRequest.h"
#include "Format.h"
#include "CustMarginRateSettingDialog.h"

// CCustMarginDialog �Ի���

IMPLEMENT_DYNAMIC(CCustMarginDialog, CDialogEx)

CCustMarginDialog::CCustMarginDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CUST_MARGIN, pParent)
{

}

CCustMarginDialog::~CCustMarginDialog()
{
}

void CCustMarginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CUST, m_ctlCust);
	DDX_Control(pDX, IDC_LIST_CUST_MARGIN_RATE, m_listctrl);
}


BEGIN_MESSAGE_MAP(CCustMarginDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CCustMarginDialog::OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CCustMarginDialog::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CCustMarginDialog::OnBnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CCustMarginDialog::OnBnClickedButtonDelete)
END_MESSAGE_MAP()

BOOL CCustMarginDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	LVCOLUMN cols2[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "�ͻ���" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "������" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "Ʒ��" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "������" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "��֤����(���)" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "��֤����(����)" }
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 120, "ƽ���ַ�����(���)" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 120, "ƽ���ַ�����(����)" }
	};

	for (int i = 0; i < sizeof(cols2) / sizeof(cols2[0]); ++i)
		m_listctrl.InsertColumn(i, &cols2[i]);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

// CCustMarginDialog ��Ϣ�������


void CCustMarginDialog::OnBnClickedButtonQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_listctrl.DeleteAllItems();
	CString cust_no = m_ctlCust.GetCurSelKey();
	if (cust_no.GetLength() == 0)
		return;

	Scoped_BCHANDLE handle;

	BCRequestQryCustMarginRate_851313(handle, cust_no.GetBuffer(), m_cmrs);
	for (auto it = m_cmrs.begin(); it != m_cmrs.end(); ++it)
	{
		int item = std::distance(m_cmrs.begin(), it);
		m_listctrl.InsertItem(item, it->cust_no);
		m_listctrl.SetItemText(item, 1, it->exchangeid);
		m_listctrl.SetItemText(item, 2, it->productid);
		m_listctrl.SetItemText(item, 3, Nfmt(it->margin_rate_by_money));
		m_listctrl.SetItemText(item, 4, Nfmt(it->margin_rate_by_volume));
	}
}


void CCustMarginDialog::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCustMarginRateSettingDialog dlg;
	dlg.m_flag = "4";
	dlg.m_ctlCust.SelectKey(m_ctlCust.GetCurSelKey());
	//if (dlg.DoModal() != IDCANCEL)
	//{
	//	OnBnClickedButtonQuery();
	//}
	dlg.DoModal();
	OnBnClickedButtonQuery();

}


void CCustMarginDialog::OnBnClickedButtonModify()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel = m_listctrl.GetSelectionMark();
	if (sel < 0 || sel >= m_cmrs.size())
	{
		MessageBox("����ѡ��");
		return;
	}
	
	CCustMarginRateSettingDialog dlg;
	cust_margin_rate_st cmr = m_cmrs[sel];
	dlg.m_flag = "5";
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


void CCustMarginDialog::OnBnClickedButtonDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel = m_listctrl.GetSelectionMark();
	if (sel < 0 || sel >= m_cmrs.size())
	{
		MessageBox("����ѡ��");
		return;
	}

	CCustMarginRateSettingDialog dlg;
	cust_margin_rate_st cmr = m_cmrs[sel];
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



