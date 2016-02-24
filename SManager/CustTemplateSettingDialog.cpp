// CustTemplateSettingDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "CustTemplateSettingDialog.h"
#include "afxdialogex.h"
#include <cassert>
#include <sstream>
// CCustTemplateSettingDialog �Ի���

IMPLEMENT_DYNAMIC(CCustTemplateSettingDialog, CDialogEx)

CCustTemplateSettingDialog::CCustTemplateSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CUST_TEMPLATE_SETTING, pParent)
	, m_IsSettingCommission(FALSE)
	, m_IsSettingMargin(FALSE)
{

}

CCustTemplateSettingDialog::~CCustTemplateSettingDialog()
{
}

void CCustTemplateSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CUST, m_ctlCust);
	DDX_Control(pDX, IDC_COMBO_COMMISSION, m_ctlCommission);
	DDX_Control(pDX, IDC_COMBO_MARGIN, m_ctlMargin);
	DDX_Check(pDX, IDC_CHECK_COMMISSION, m_IsSettingCommission);
	DDX_Check(pDX, IDC_CHECK_MARGIN, m_IsSettingMargin);
}


BEGIN_MESSAGE_MAP(CCustTemplateSettingDialog, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_COMMISSION, &CCustTemplateSettingDialog::OnBnClickedCheckCommission)
	ON_BN_CLICKED(IDC_CHECK_MARGIN, &CCustTemplateSettingDialog::OnBnClickedCheckMargin)
	ON_CBN_SELCHANGE(IDC_COMBO_CUST, &CCustTemplateSettingDialog::OnCbnSelchangeComboCust)
	ON_BN_CLICKED(IDOK, &CCustTemplateSettingDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CCustTemplateSettingDialog ��Ϣ�������


void CCustTemplateSettingDialog::OnBnClickedCheckCommission()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_IsSettingCommission)
	{
		m_ctlCommission.EnableWindow(TRUE);
	}
	else
	{
		m_ctlCommission.EnableWindow(FALSE);
	}
}


void CCustTemplateSettingDialog::OnBnClickedCheckMargin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_IsSettingMargin)
	{
		m_ctlMargin.EnableWindow(TRUE);
	}
	else
	{
		m_ctlMargin.EnableWindow(FALSE);
	}
}


void CCustTemplateSettingDialog::OnCbnSelchangeComboCust()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString cust_no = m_ctlCust.GetCurSelKey();
	cust_base_info_st cbi = theApp.GetCustBaseInfo(cust_no);
	m_ctlCommission.SelectKey(cbi.commission_temp.template_no);
	m_ctlMargin.SelectKey(cbi.margin_temp.template_no); 
}


BOOL CCustTemplateSettingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	OnCbnSelchangeComboCust();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CCustTemplateSettingDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString cust_no = m_ctlCust.GetCurSelKey();
	if (!cust_no.GetLength())
		return;
	std::ostringstream oss;

	if (m_IsSettingCommission)
	{
		cust_base_info_st cbi = theApp.GetCustBaseInfo(cust_no);
		assert(cust_no == cbi.basic.scust_no);
		CString ctno = m_ctlCommission.GetCurSelKey();
		CString ctna = m_ctlCommission.GetCurSelVal();

		if (ctno != cbi.commission_temp.template_no)
		{
			char *flag;
			if (cbi.commission_temp.template_no[0] == 0)
				flag = "1";
			else
				flag = "2";
			if (SetCustCommissionTemplate_851342(cbi.basic.scust_no, ctno.GetBuffer(), flag))
			{
				strcpy(cbi.commission_temp.template_no, ctno);
				strcpy(cbi.commission_temp.template_name, ctna);
				theApp.SetCustBaseInfo(cbi);
				//CDialogEx::OnOK();
				oss << "�ͻ�������������ģ��ɹ�,";
			}
		}
	}

	if (m_IsSettingMargin)
	{
		cust_base_info_st cbi = theApp.GetCustBaseInfo(cust_no);
		assert(cust_no == cbi.basic.scust_no);
		CString mtno = m_ctlMargin.GetCurSelKey();
		CString mtna = m_ctlMargin.GetCurSelVal();
		if (mtno != cbi.margin_temp.template_no)
		{
			char *flag;
			if (cbi.margin_temp.template_no[0] == 0)
				flag = "1";
			else
				flag = "2";
			if (SetCustMarginTemplate_851348(cbi.basic.scust_no, mtno.GetBuffer(), flag))
			{
				strcpy(cbi.margin_temp.template_no, mtno);
				strcpy(cbi.margin_temp.template_name, mtna);
				theApp.SetCustBaseInfo(cbi);
				oss << "�ͻ����ñ�֤����ģ��ɹ�";
				//CDialogEx::OnOK();
			}
		}
	}

	if (!oss.str().empty()) {
		MessageBox(oss.str().c_str());
	}
	//CDialogEx::OnOK();
}

bool CCustTemplateSettingDialog::SetCustCommissionTemplate_851342(char * cust_no, char * template_no, char * flag)
{
	Scoped_BCHANDLE handle;
	BCSetRequestType(handle, 851342);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code); //����Ա
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no); //�ͻ���
	BCSetStringFieldByName(handle, 0, "sstatus0", flag); //������־������
	BCSetStringFieldByName(handle, 0, "sstation0", template_no); //��������ģ��
	BCSetStringFieldByName(handle, 0, "sstatus1", "0"); //�������ʶ������� ��
	BCSetStringFieldByName(handle, 0, "sstatus2", "0"); //��������ǿ���滻��־ ��
	return MyBCRequest(handle, FetchRowFunc_t(), BCMSG_NOR_NOMB);
}

bool CCustTemplateSettingDialog::SetCustMarginTemplate_851348(char * cust_no, char * template_no, char * flag)
{
	Scoped_BCHANDLE handle;
	BCSetRequestType(handle, 851348);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code); //����Ա
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no); //�ͻ���
	BCSetStringFieldByName(handle, 0, "sstatus0", flag); //������־������
	BCSetStringFieldByName(handle, 0, "sstation0", template_no); //��������ģ��
	BCSetStringFieldByName(handle, 0, "sstatus1", "0"); //�������ʶ������� ��
	BCSetStringFieldByName(handle, 0, "sstatus2", "0"); //��������ǿ���滻��־ ��
	return MyBCRequest(handle, FetchRowFunc_t(), BCMSG_NOR_NOMB);
}
