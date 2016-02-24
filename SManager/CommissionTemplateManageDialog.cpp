// CommissionTemplateManageDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "CommissionTemplateManageDialog.h"
#include "afxdialogex.h"
#include "SMutility.h"
#include "AddTemplateDialog.h"
#include "CommissionTemplateSettingDialog.h"
// CCommissionTemplateManageDialog 对话框

IMPLEMENT_DYNAMIC(CCommissionTemplateManageDialog, CDialogEx)

CCommissionTemplateManageDialog::CCommissionTemplateManageDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_COMMISSION_TEMPLATE, pParent)
{

}

CCommissionTemplateManageDialog::~CCommissionTemplateManageDialog()
{
}

void CCommissionTemplateManageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COMMISSION_TEMPLATE, m_listctrl1);
	DDX_Control(pDX, IDC_LIST_COMMISSION, m_listctrl2);
}


BEGIN_MESSAGE_MAP(CCommissionTemplateManageDialog, CDialogEx)
//	ON_NOTIFY(NM_CLICK, IDC_LIST_COMMISSION_TEMPLATE, &CCommissionTemplateManageDialog::OnNMClickListCommissionTemplate)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_COMMISSION_TEMPLATE, &CCommissionTemplateManageDialog::OnLvnItemchangedListCommissionTemplate)
	ON_BN_CLICKED(IDC_BUTTON_ADD_TEMPLATE, &CCommissionTemplateManageDialog::OnBnClickedButtonAddTemplate)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_TEMPLATE, &CCommissionTemplateManageDialog::OnBnClickedButtonDeleteTemplate)
	ON_BN_CLICKED(IDC_BUTTON_ADD_COMMISSION, &CCommissionTemplateManageDialog::OnBnClickedButtonAddCommission)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_COMMISSION, &CCommissionTemplateManageDialog::OnBnClickedButtonModifyCommission)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_COMMISSION, &CCommissionTemplateManageDialog::OnBnClickedButtonDeleteCommission)
END_MESSAGE_MAP()


// CCommissionTemplateManageDialog 消息处理程序


BOOL CCommissionTemplateManageDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//m_listctrl
	LVCOLUMN cols1[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "模板代码" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "模板名称" }
	};

	for (int i = 0; i < sizeof(cols1) / sizeof(cols1[0]); ++i)
		m_listctrl1.InsertColumn(i, &cols1[i]);

	LVCOLUMN cols2[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "模板代码" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "模板名称" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "品种" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "交易所" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "交割期" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "手费续率(金额)" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "手费续率(手数)" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 120, "平今手费续率(金额)" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 120, "平今手费续率(手数)" }
	};

	for (int i = 0; i < sizeof(cols2) / sizeof(cols2[0]); ++i)
		m_listctrl2.InsertColumn(i, &cols2[i]);

	ResetList1();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CCommissionTemplateManageDialog::ResetList1()
{
	m_listctrl1.DeleteAllItems();
	m_listctrl2.DeleteAllItems();
	std::vector<commission_template_no_st> &v_ctn = theApp.m_BaseInfo.commission_no;
	for (int i = 0; i < v_ctn.size(); i++)
	{
		m_listctrl1.InsertItem(i, v_ctn[i].template_no);
		m_listctrl1.SetItemText(i, 1, v_ctn[i].template_name);
	}
}

void CCommissionTemplateManageDialog::ResetList2(CString ctno)
{
	m_listctrl2.DeleteAllItems();
	//int nItem = m_listctrl1.GetSelectionMark();
	//CString no = m_listctrl1.GetItemText(nItem, 0);

	std::vector<commission_template_st> &ct = theApp.m_BaseInfo.commissions[ctno.GetBuffer()];
	m_listctrl2.DeleteAllItems();
	for (int i = 0; i < ct.size(); i++)
	{
		m_listctrl2.InsertItem(i, ct[i].template_no);
		m_listctrl2.SetItemText(i, 1, ct[i].template_name);
		m_listctrl2.SetItemText(i, 2, ct[i].productid);
		m_listctrl2.SetItemText(i, 3, ct[i].exchangeid);
		m_listctrl2.SetItemText(i, 4, ct[i].delivery_date);
		CString str;
		str.Format("%.7f", ct[i].commission_rate_by_money);
		m_listctrl2.SetItemText(i, 5, str);
		str.Format("%f", ct[i].commission_rate_by_volume);
		m_listctrl2.SetItemText(i, 6, str);
		str.Format("%.7f", ct[i].offset_td_commission_rate_by_money);
		m_listctrl2.SetItemText(i, 7, str);
		str.Format("%f", ct[i].offset_td_commission_rate_by_volume);
	}
}

bool CCommissionTemplateManageDialog::RequestAddTemplate_851344(CString temp_no, CString temp_name)
{
	Scoped_BCHANDLE handle;
	BCSetRequestType(handle, 851344);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sstatus0", "1");
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", temp_no.GetBuffer());
	BCSetStringFieldByName(handle, 0, "scusttypes", temp_name.GetBuffer());

	return MyBCRequest(handle, FetchRowFunc_t(), true);
}

bool CCommissionTemplateManageDialog::RequestDelTemplate_851344(CString temp_no)
{

	Scoped_BCHANDLE handle;
	BCSetRequestType(handle, 851344);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sstatus0", "3");
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", temp_no.GetBuffer());
	//BCSetStringFieldByName(handle, 0, "scusttypes", dlg.m_TempName.GetBuffer());

	return MyBCRequest(handle, FetchRowFunc_t(), true);
}


//void CCommissionTemplateManageDialog::OnNMClickListCommissionTemplate(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	int sel = m_listctrl1.GetSelectionMark();
//	if (sel >= 0)
//	{
//		CString no = m_listctrl1.GetItemText(sel, 0);
//
//	}
//	*pResult = 0;
//}


void CCommissionTemplateManageDialog::OnLvnItemchangedListCommissionTemplate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (LVIF_STATE == pNMLV->uChanged && (pNMLV->uNewState & LVIS_SELECTED))
	{
		int nItem = pNMLV->iItem;
		if (-1 != nItem)
		{
			CString ctno = m_listctrl1.GetItemText(nItem, 0);
			ResetList2(ctno);
		}
	}

	*pResult = 0;
}


void CCommissionTemplateManageDialog::OnBnClickedButtonAddTemplate()
{
	// TODO: 在此添加控件通知处理程序代码
	CAddTemplateDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;
	if (RequestAddTemplate_851344(dlg.m_TempNo, dlg.m_TempName)) {
		commission_template_no_st ctn;
		strcpy(ctn.template_no, dlg.m_TempNo);
		strcpy(ctn.template_name, dlg.m_TempName);
		theApp.m_BaseInfo.commission_no.push_back(ctn);
		ResetList1();
	}
}


void CCommissionTemplateManageDialog::OnBnClickedButtonDeleteTemplate()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_listctrl1.GetSelectionMark();
	if (sel < 0 || sel >= m_listctrl1.GetItemCount())
	{
		MessageBox("请选中要删除的手续费率模板");
		return;
	}
	CString temp_no = m_listctrl1.GetItemText(sel, 0);
	CString temp_name = m_listctrl1.GetItemText(sel, 1);

	CString text;
	text.Format("确认要删除%s-%s手续费率模板吗？", temp_no, temp_name);
	if (MessageBox(text, "提示", MB_YESNO) != IDYES)
		return;
	if (RequestDelTemplate_851344(temp_no))
	{
		std::vector<commission_template_no_st>::iterator it;
		for (it = theApp.m_BaseInfo.commission_no.begin(); it != theApp.m_BaseInfo.commission_no.end(); ++it)
		{
			if (temp_no == it->template_no) break;
		}
		if (it != theApp.m_BaseInfo.commission_no.end())
			theApp.m_BaseInfo.commission_no.erase(it);
		ResetList1();
	}
	//MessageBox("确认要")
}


void CCommissionTemplateManageDialog::OnBnClickedButtonAddCommission()
{
	// TODO: 在此添加控件通知处理程序代码
	CCommissionTemplateSettingDialog dlg;
	int sel = m_listctrl1.GetSelectionMark();
	CString ctno = m_listctrl1.GetItemText(sel, 0);
	dlg.m_ctlCommission.SelectKey(ctno);
	dlg.m_flag = "4";
	dlg.DoModal();
	ResetList2(ctno);
}


void CCommissionTemplateManageDialog::OnBnClickedButtonModifyCommission()
{
	// TODO: 在此添加控件通知处理程序代码
	//if()
	CCommissionTemplateSettingDialog dlg;
	CString ctno = m_listctrl1.GetItemText(m_listctrl1.GetSelectionMark(), 0);
	dlg.m_ctlCommission.SelectKey(ctno);
	int sel = m_listctrl2.GetSelectionMark();
	if (sel >= 0 && sel < m_listctrl2.GetItemCount())
	{
		//std::vector<commission_template_st> &ct = theApp.m_BaseInfo.commissions[no.GetBuffer()];
		if (ctno.GetLength() != 0) {
			commission_template_st &ct = theApp.m_BaseInfo.commissions[ctno.GetBuffer()][sel];
			dlg.m_ctlExchange.SelectKey(ct.exchangeid);
			dlg.m_ctlProduct.SelectKey(ct.productid);
			dlg.m_date = ct.delivery_date;
			dlg.m_ByAmt = ct.commission_rate_by_money;
			dlg.m_ByVol = ct.commission_rate_by_volume;
			dlg.m_TdByAmt = ct.offset_td_commission_rate_by_money;
			dlg.m_TdByVol = ct.offset_td_commission_rate_by_volume;
		}
	}
	dlg.m_flag = "5";
	dlg.DoModal();
	ResetList2(ctno);
}


void CCommissionTemplateManageDialog::OnBnClickedButtonDeleteCommission()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_listctrl2.GetSelectionMark();
	if (sel < 0 || sel >= m_listctrl2.GetItemCount()) {
		MessageBox("请选中要删除的手续费率模板设置条目");
		return;
	}
	CString ctno = m_listctrl2.GetItemText(sel, 0);
	commission_template_st &ct = theApp.m_BaseInfo.commissions[ctno.GetBuffer()][sel];
	CCommissionTemplateSettingDialog dlg;
	dlg.m_ctlCommission.SelectKey(ctno);
	dlg.m_ctlExchange.SelectKey(ct.exchangeid);
	dlg.m_ctlProduct.SelectKey(ct.productid);
	dlg.m_date = ct.delivery_date;
	dlg.m_ByAmt = ct.commission_rate_by_money;
	dlg.m_ByVol = ct.commission_rate_by_volume;
	dlg.m_TdByAmt = ct.offset_td_commission_rate_by_money;
	dlg.m_TdByVol = ct.offset_td_commission_rate_by_volume;
	dlg.m_flag = "3";
	dlg.DoModal();
	ResetList2(ctno);
}
