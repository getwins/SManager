// CustMarginRateSettingDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "CustMarginRateSettingDialog.h"
#include "afxdialogex.h"

#include "CommBCRequest.h"
// CCustMarginRateSettingDialog �Ի���

IMPLEMENT_DYNAMIC(CCustMarginRateSettingDialog, CDialogEx)

CCustMarginRateSettingDialog::CCustMarginRateSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CUST_MARGIN_RATE_SETTING, pParent)
	, m_ByAmt(0)
	, m_ByVol(0)
{

}

CCustMarginRateSettingDialog::~CCustMarginRateSettingDialog()
{
}

void CCustMarginRateSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CUST, m_ctlCust);
	DDX_Control(pDX, IDC_COMBO_EXCHANGE, m_ctlExchange);
	DDX_Control(pDX, IDC_COMBO_PRODUCT, m_ctlProduct);
	DDX_Text(pDX, IDC_EDIT_BYAMOUNT, m_ByAmt);
	DDX_Text(pDX, IDC_EDIT_BYVOLUME, m_ByVol);
}


BEGIN_MESSAGE_MAP(CCustMarginRateSettingDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCustMarginRateSettingDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCustMarginRateSettingDialog::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CCustMarginRateSettingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (strcmp(m_flag, "5") == 0)
	{
		GetDlgItem(IDC_COMBO_CUST)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_EXCHANGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PRODUCT)->EnableWindow(FALSE);
	}

	if (strcmp(m_flag, "3") == 0)
	{
		GetDlgItem(IDC_COMBO_CUST)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_EXCHANGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PRODUCT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BYAMOUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BYVOLUME)->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


// CCustMarginRateSettingDialog ��Ϣ�������


void CCustMarginRateSettingDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
	UpdateData(TRUE);
	Scoped_BCHANDLE handle;
	cust_margin_rate_st cmr = { 0 };
	
	strcpy(cmr.cust_no, m_ctlCust.GetCurSelKey());
	strcpy(cmr.exchangeid, m_ctlExchange.GetCurSelKey());
	strcpy(cmr.productid, m_ctlProduct.GetCurSelKey());
	cmr.margin_rate_by_money = m_ByAmt;
	cmr.margin_rate_by_volume = m_ByVol;

	BCResult result = BCRequestSetCustMarginRate_851312(handle, m_flag, cmr);
	if (result)
	{
		MessageBox("�����ɹ�");
	}
	else {
		MessageBox(result.errmsg);
	}
}


void CCustMarginRateSettingDialog::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


