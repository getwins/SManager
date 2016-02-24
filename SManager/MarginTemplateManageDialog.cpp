// MarginTemplateManageDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "MarginTemplateManageDialog.h"
#include "afxdialogex.h"

#include "SMutility.h"
#include "AddTemplateDialog.h"
#include "MarginTemplateSettingDialog.h"
// CMarginTemplateManageDialog �Ի���

IMPLEMENT_DYNAMIC(CMarginTemplateManageDialog, CDialogEx)

CMarginTemplateManageDialog::CMarginTemplateManageDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MARGIN_TEMPLATE, pParent)
{

}

CMarginTemplateManageDialog::~CMarginTemplateManageDialog()
{
}

void CMarginTemplateManageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MARGIN_TEMPLATE, m_listctrl1);
	DDX_Control(pDX, IDC_LIST_MARGIN, m_listctrl2);
}


BEGIN_MESSAGE_MAP(CMarginTemplateManageDialog, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_MARGIN_TEMPLATE, &CMarginTemplateManageDialog::OnLvnItemchangedListMarginTemplate)
	ON_BN_CLICKED(IDC_BUTTON_ADD_TEMPLATE, &CMarginTemplateManageDialog::OnBnClickedButtonAddTemplate)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_TEMPLATE, &CMarginTemplateManageDialog::OnBnClickedButtonDeleteTemplate)
	ON_BN_CLICKED(IDC_BUTTON_ADD_MARGIN, &CMarginTemplateManageDialog::OnBnClickedButtonAddMargin)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_MARGIN, &CMarginTemplateManageDialog::OnBnClickedButtonModifyMargin)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_MARGIN, &CMarginTemplateManageDialog::OnBnClickedButtonDeleteMargin)
END_MESSAGE_MAP()


// CMarginTemplateManageDialog ��Ϣ�������


bool CMarginTemplateManageDialog::RequestTemplate_851346(char *flag, char *temp_no, char *temp_name)
{
	Scoped_BCHANDLE handle;
	BCSetRequestType(handle, 851346);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sstatus0", flag);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", temp_no);
	BCSetStringFieldByName(handle, 0, "scusttypes", temp_name);
	
	return MyBCRequest(handle, FetchRowFunc_t(), BCMSG_NOR_NOMB);
}

void CMarginTemplateManageDialog::ResetList1()
{
	m_listctrl1.DeleteAllItems();
	m_listctrl2.DeleteAllItems();
	std::vector<margin_template_no_st> &v_mtn = theApp.m_BaseInfo.margin_no;
	for (int i = 0; i < v_mtn.size(); i++)
	{
		m_listctrl1.InsertItem(i, v_mtn[i].template_no);
		m_listctrl1.SetItemText(i, 1, v_mtn[i].template_name);
	}
}

void CMarginTemplateManageDialog::ResetList2(CString mtno)
{
	m_listctrl2.DeleteAllItems();
	//int nItem = m_listctrl1.GetSelectionMark();
	//CString no = m_listctrl1.GetItemText(nItem, 0);

	std::vector<margin_template_st> &mt = theApp.m_BaseInfo.margins[mtno.GetBuffer()];
	m_listctrl2.DeleteAllItems();
	for (int i = 0; i < mt.size(); i++)
	{
		m_listctrl2.InsertItem(i, mt[i].template_no);
		m_listctrl2.SetItemText(i, 1, mt[i].template_name);
		m_listctrl2.SetItemText(i, 2, mt[i].productid);
		m_listctrl2.SetItemText(i, 3, mt[i].exchangeid);
		//m_listctrl2.SetItemText(i, 4, mt[i].delivery_date);
		CString str;
		str.Format("%.7f", mt[i].margin_rate_by_money);
		m_listctrl2.SetItemText(i, 4, str);
		str.Format("%f", mt[i].margin_rate_by_volume);
		m_listctrl2.SetItemText(i, 5, str);
	}
}

BOOL CMarginTemplateManageDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	LVCOLUMN cols1[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "ģ�����" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "ģ������" }
	};

	for (int i = 0; i < sizeof(cols1) / sizeof(cols1[0]); ++i)
		m_listctrl1.InsertColumn(i, &cols1[i]);

	LVCOLUMN cols2[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "ģ�����" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "ģ������" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "Ʒ��" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "������" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "������" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "��֤����(���)" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 100, "��֤����(����)" }
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 120, "ƽ���ַ�����(���)" },
		//{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 120, "ƽ���ַ�����(����)" }
	};

	for (int i = 0; i < sizeof(cols2) / sizeof(cols2[0]); ++i)
		m_listctrl2.InsertColumn(i, &cols2[i]);

	////std::vector<commission_template_no_st> &v_ctn = theApp.m_BaseInfo.commission_no;
	//std::vector<margin_template_no_st> &v_mtn = theApp.m_BaseInfo.margin_no;
	//for (int i = 0; i < v_mtn.size(); i++)
	//{
	//	m_listctrl1.InsertItem(i, v_mtn[i].template_no);
	//	m_listctrl1.SetItemText(i, 1, v_mtn[i].template_name);
	//}

	ResetList1();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CMarginTemplateManageDialog::OnLvnItemchangedListMarginTemplate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (LVIF_STATE == pNMLV->uChanged && (pNMLV->uNewState & LVIS_SELECTED))
	{
		int nItem = pNMLV->iItem;
		if (-1 != nItem)
		{
			CString no = m_listctrl1.GetItemText(nItem, 0);

			//std::vector<margin_template_st> &vmt = theApp.m_BaseInfo.margins[no.GetBuffer()];
			//m_listctrl2.DeleteAllItems();
			//for (int i = 0; i < vmt.size(); i++)
			//{
			//	m_listctrl2.InsertItem(i, vmt[i].template_no);
			//	m_listctrl2.SetItemText(i, 1, vmt[i].template_name);
			//	m_listctrl2.SetItemText(i, 2, vmt[i].productid);
			//	m_listctrl2.SetItemText(i, 3, vmt[i].exchangeid);
			//	//m_listctrl2.SetItemText(i, 4, vmt[i].delivery_date);
			//	CString str;
			//	str.Format("%f", vmt[i].margin_rate_by_money);
			//	m_listctrl2.SetItemText(i, 4, str);
			//	str.Format("%f", vmt[i].margin_rate_by_volume);
			//	m_listctrl2.SetItemText(i, 5, str);
			//}
			ResetList2(no);
		}
	}

	*pResult = 0;
}


void CMarginTemplateManageDialog::OnBnClickedButtonAddTemplate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAddTemplateDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;
	if (RequestTemplate_851346("1", dlg.m_TempNo.GetBuffer(), dlg.m_TempName.GetBuffer())) {
		margin_template_no_st mtn = { 0 };
		strcpy(mtn.template_no, dlg.m_TempNo);
		strcpy(mtn.template_name, dlg.m_TempName);
		theApp.m_BaseInfo.margin_no.push_back(mtn);
		ResetList1();
	}
}


void CMarginTemplateManageDialog::OnBnClickedButtonDeleteTemplate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int sel = m_listctrl1.GetSelectionMark();
	if (sel < 0 || sel >= m_listctrl1.GetItemCount())
	{
		MessageBox("��ѡ��Ҫɾ���ı�֤����ģ��");
		return;
	}
	CString temp_no = m_listctrl1.GetItemText(sel, 0);
	CString temp_name = m_listctrl1.GetItemText(sel, 1);

	CString text;
	text.Format("ȷ��Ҫɾ��%s-%s��֤����ģ����", temp_no, temp_name);
	if (MessageBox(text, "��ʾ", MB_YESNO) != IDYES)
		return;
	if (RequestTemplate_851346("3", temp_no.GetBuffer(), temp_name.GetBuffer()))
	{
		std::vector<margin_template_no_st>::iterator it;
		for (it = theApp.m_BaseInfo.margin_no.begin(); it != theApp.m_BaseInfo.margin_no.end(); ++it)
		{
			if (temp_no == it->template_no) break;
		}
		if (it != theApp.m_BaseInfo.margin_no.end())
			theApp.m_BaseInfo.margin_no.erase(it);
		ResetList1();
	}
}


void CMarginTemplateManageDialog::OnBnClickedButtonAddMargin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMarginTemplateSettingDialog dlg;
	CString mtno;
	int sel = m_listctrl1.GetSelectionMark();
	if (sel >= 0 && sel < m_listctrl1.GetItemCount()) {
		mtno = m_listctrl1.GetItemText(sel, 0);
		dlg.m_ctlMargin.SelectKey(mtno);
	}
	dlg.m_flag = "4";
	dlg.DoModal();
	if(mtno.GetLength())
		ResetList2(mtno);
}


void CMarginTemplateManageDialog::OnBnClickedButtonModifyMargin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMarginTemplateSettingDialog dlg;
	int sel = m_listctrl2.GetSelectionMark();
	if (sel < 0 || sel >= m_listctrl2.GetItemCount())
	{
		MessageBox("��ѡ��Ҫ�޸ĵı�֤����ģ��������");
		return;
	}
	CString mtno = m_listctrl2.GetItemText(sel, 0);
	dlg.m_ctlMargin.SelectKey(mtno);
	dlg.m_flag = "5";
	margin_template_st mt = theApp.m_BaseInfo.margins[mtno.GetBuffer()][sel];
	dlg.m_ctlExchange.SelectKey(mt.exchangeid);
	dlg.m_ctlProduct.SelectKey(mt.productid);
	dlg.m_ByAmt = mt.margin_rate_by_money;
	dlg.m_ByVol = mt.margin_rate_by_volume;
	dlg.DoModal();
	ResetList2(mtno);
}


void CMarginTemplateManageDialog::OnBnClickedButtonDeleteMargin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMarginTemplateSettingDialog dlg;
	int sel = m_listctrl2.GetSelectionMark();
	if (sel < 0 || sel >= m_listctrl2.GetItemCount())
	{
		MessageBox("��ѡ��Ҫɾ���ı�֤����ģ��������");
		return;
	}
	CString mtno = m_listctrl2.GetItemText(sel, 0);
	dlg.m_ctlMargin.SelectKey(mtno);
	dlg.m_flag = "3";
	margin_template_st mt = theApp.m_BaseInfo.margins[mtno.GetBuffer()][sel];
	dlg.m_ctlExchange.SelectKey(mt.exchangeid);
	dlg.m_ctlProduct.SelectKey(mt.productid);
	dlg.m_ByAmt = mt.margin_rate_by_money;
	dlg.m_ByVol = mt.margin_rate_by_volume;
	dlg.DoModal();
	ResetList2(mtno);
}
