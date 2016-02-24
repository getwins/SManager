// CommissionTemplateSettingDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "CommissionTemplateSettingDialog.h"
#include "afxdialogex.h"
#include "SMutility.h"

// CCommissionTemplateSettingDialog �Ի���

IMPLEMENT_DYNAMIC(CCommissionTemplateSettingDialog, CDialogEx)

CCommissionTemplateSettingDialog::CCommissionTemplateSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMMISSION_SETTING, pParent)
	, m_date(_T(""))
	, m_TdByAmt(0)
	, m_ByAmt(0)
	, m_TdByVol(0)
	, m_ByVol(0)
{

}

CCommissionTemplateSettingDialog::~CCommissionTemplateSettingDialog()
{
}

void CCommissionTemplateSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COMMISSION, m_ctlCommission);
	DDX_Control(pDX, IDC_COMBO_EXCHANGE, m_ctlExchange);
	DDX_Control(pDX, IDC_COMBO_PRODUCT, m_ctlProduct);
	DDX_Text(pDX, IDC_EDIT_DELIVERY_DATE, m_date);
	DDX_Text(pDX, IDC_EDIT_TD_BYAMOUNT, m_TdByAmt);
	DDX_Text(pDX, IDC_EDIT_BYAMOUNT, m_ByAmt);
	DDX_Text(pDX, IDC_EDIT_TD_BYVOLUME, m_TdByVol);
	DDX_Text(pDX, IDC_EDIT_BYVOLUME, m_ByVol);
}


BEGIN_MESSAGE_MAP(CCommissionTemplateSettingDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCommissionTemplateSettingDialog::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_EXCHANGE, &CCommissionTemplateSettingDialog::OnCbnSelchangeComboExchange)
END_MESSAGE_MAP()


BOOL CCommissionTemplateSettingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//if (m_flag == "5" || m_flag == "3")
	//{
	//	m_ctlCommission.SelectKey(m_ctit->template_no);
	//	m_ctlExchange.SelectKey(m_ctit->exchangeid);
	//	m_ctlProduct.SelectKey(m_ctit->productid);
	//	m_date = m_ctit->delivery_date;
	//	m_ByAmt = m_ctit->commission_rate_by_money;
	//	m_ByVol = m_ctit->commission_rate_by_volume;
	//	m_TdByAmt = m_ctit->offset_td_commission_rate_by_money;
	//	m_TdByVol = m_ctit->offset_td_commission_rate_by_volume;
	//}

	//if (m_flag == "3")
	//{
	//	//this->GetDlgItem()
	//}

	UpdateData(FALSE);
	//if(m_flag)

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


// CCommissionTemplateSettingDialog ��Ϣ�������


void CCommissionTemplateSettingDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
	//CString ctno = m_ctlCommission.GetCurSelKey();
	UpdateData(TRUE);
	hyphenstr hct = m_ctlCommission.GetCurSelKV();
	if (hct.left.GetLength() == 0)
	{
		MessageBox("����ѡ��ģ��");
		return;
	}

	CString exchangeid = m_ctlExchange.GetCurSelKey();
	if (exchangeid.GetLength() == 0)
	{
		MessageBox("����ѡ������");
		return;
	}
	CString productid = m_ctlProduct.GetCurSelKey();
	if (productid.GetLength() == 0)
	{
		MessageBox("����ѡ��Ʒ��");
		return;
	}
	if (RequestCommissionSetting_851338(hct.left, exchangeid, productid))
	{
		std::vector<commission_template_st> &vc = theApp.m_BaseInfo.commissions[hct.left.GetBuffer()];
		std::vector<commission_template_st>::iterator ctit;
		if (m_flag == "4")
		{
			commission_template_st ct;
			strcpy(ct.template_no, hct.left.GetBuffer());
			strcpy(ct.template_name, hct.right.GetBuffer());
			strcpy(ct.exchangeid, exchangeid.GetBuffer());
			strcpy(ct.productid, productid.GetBuffer());
			strcpy(ct.delivery_date, m_date.GetBuffer());
			ct.commission_rate_by_money = m_ByAmt;
			ct.commission_rate_by_volume = m_ByVol;
			ct.offset_td_commission_rate_by_money = m_TdByAmt;
			ct.offset_td_commission_rate_by_volume = m_TdByVol;
			vc.push_back(ct);
			MessageBox("��ӳɹ�");
		}
			
		else if (m_flag == "5")
		{
			for (ctit = vc.begin(); ctit != vc.end(); ++ctit)
			{
				if (hct.left == ctit->template_no && exchangeid == ctit->exchangeid
					&& productid == ctit->productid && m_date == ctit->delivery_date)
				{
					ctit->commission_rate_by_money = m_ByAmt;
					ctit->commission_rate_by_volume = m_ByVol;
					ctit->offset_td_commission_rate_by_money = m_TdByAmt;
					ctit->offset_td_commission_rate_by_volume = m_TdByVol;
					break;
				}				
			}
			
			MessageBox("�޸ĳɹ�");
		}
		else if (m_flag == "3")
		{
			for (ctit = vc.begin(); ctit != vc.end(); ++ctit)
			{
				if (hct.left == ctit->template_no && exchangeid == ctit->exchangeid
					&& productid == ctit->productid && m_date == ctit->delivery_date)
					break;
			}
			if (ctit != vc.end())
				vc.erase(ctit);
			MessageBox("ɾ���ɹ�");
		}
			
	}
}
//
///* ���룺����Ա			input:scust_no
//������־�����ӡ��޸ġ�ɾ����			sstatus0
//ģ���			sholder_ac_no
//������			smarket_code
//Ʒ��			sstock_code
//������			sdate0
//��ƽ�������ʣ�����			damt2
//��ƽ�������ʣ���������			damt3
//��ƽ���ӷ���1������			damt4
//��ƽ���ӷ���1����������			damt5
//��ƽ���ӷ���2������			damt6
//��ƽ���ӷ���2����������			damt7
//ƽ���������ʣ�����			damt8
//ƽ���������ʣ���������			damt9
//ƽ�񸽼ӷ���1������			damt10
//ƽ�񸽼ӷ���1����������			damt11
//ƽ�񸽼ӷ���2������			damt12
//ƽ�񸽼ӷ���2����������			damt13
//�����������ʣ�����			damt14
//�����������ʣ���������			damt15
//��ֵ��ƽ�������ʣ�����			damt16
//��ֵ��ƽ�������ʣ���������			damt17
//��ֵ��ƽ���ӷ���1(�����)			damt18
//��ֵ��ƽ���ӷ���1����������			damt19
//��ֵ��ƽ���ӷ���2(�����)			damt20
//��ֵ��ƽ���ӷ���2����������			damt21
//��ֵƽ���������ʣ�����			damt22
//��ֵƽ���������ʣ���������			damt23
//��ֵƽ�񸽼ӷ���1(�����)			damt24
//��ֵƽ�񸽼ӷ���1����������			damt25
//��ֵƽ�񸽼ӷ���2(�����)			damt26
//��ֵƽ�񸽼ӷ���2����������			damt27
//��ֵ�����������ʣ�����			damt28
//��ֵ�����������ʣ���������			damt29
//������ƽ�������ʣ�����			damt30
//������ƽ�������ʣ���������			damt31
//����ƽ���������ʣ�����			damt32
//����ƽ���������ʣ���������			damt33
//���������������ʣ�����			damt0
//���������������ʣ��������� */			damt1

bool CCommissionTemplateSettingDialog::RequestCommissionSetting_851338(CString &ctno, CString &exchangeid, CString &productid)
{
	Scoped_BCHANDLE handle;
	BCSetRequestType(handle, 851338);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sstatus0", m_flag.GetBuffer());
	//CString ctno = m_ctlCommission.GetCurSelKey();
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", ctno.GetBuffer());
	//CString exchangeid = m_ctlExchange.GetCurSelKey();
	BCSetStringFieldByName(handle, 0, "smarket_code", exchangeid.GetBuffer());
	//CString productid = m_ctlProduct.GetCurSelKey();
	BCSetStringFieldByName(handle, 0, "sstock_code", productid.GetBuffer());
	BCSetStringFieldByName(handle, 0, "sdate0", m_date.GetBuffer());
	BCSetDoubleFieldByName(handle, 0, "damt2", m_ByAmt);
	BCSetDoubleFieldByName(handle, 0, "damt3", m_ByVol);
	BCSetDoubleFieldByName(handle, 0, "damt8", m_TdByAmt);
	BCSetDoubleFieldByName(handle, 0, "damt9", m_TdByVol);
	//BCSetStringFieldByName(handle, 0, "sstatus0", "4");


	return MyBCRequest(handle, FetchRowFunc_t(), BCMSG_NOR_NOMB);
}


void CCommissionTemplateSettingDialog::OnCbnSelchangeComboExchange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ctlProduct.FilterByExchangeid(m_ctlExchange.GetCurSelKey());
	m_ctlProduct.SetCurSel(0);
}


