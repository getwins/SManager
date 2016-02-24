// CustMarginDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "CustMarginDialog.h"
#include "afxdialogex.h"
#include "CommBCRequest.h"
#include "Format.h"
#include "CustMarginRateSettingDialog.h"

// CCustMarginDialog 对话框

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

	// TODO:  在此添加额外的初始化
	LVCOLUMN cols2[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "客户号" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "交易所" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "品种" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "交割期" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "保证金率(金额)" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "保证金率(手数)" }
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 120, "平今手费续率(金额)" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 120, "平今手费续率(手数)" }
	};

	for (int i = 0; i < sizeof(cols2) / sizeof(cols2[0]); ++i)
		m_listctrl.InsertColumn(i, &cols2[i]);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// CCustMarginDialog 消息处理程序


void CCustMarginDialog::OnBnClickedButtonQuery()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_listctrl.GetSelectionMark();
	if (sel < 0 || sel >= m_cmrs.size())
	{
		MessageBox("请先选中");
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
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_listctrl.GetSelectionMark();
	if (sel < 0 || sel >= m_cmrs.size())
	{
		MessageBox("请先选中");
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



