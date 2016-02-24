// CustCommissionRateSettingDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "CustCommissionRateSettingDialog.h"
#include "afxdialogex.h"
#include "CommBCRequest.h"

// CCustCommissionRateSettingDialog 对话框

IMPLEMENT_DYNAMIC(CCustCommissionRateSettingDialog, CDialogEx)

CCustCommissionRateSettingDialog::CCustCommissionRateSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CUST_COMMISSION_RATE_SETTING, pParent)
	, m_Date(_T(""))
	, m_ByAmt(0)
	//, m_byVol(_T(""))
	, m_TdByAmt(0)
	, m_TdByVol(0)
	, m_ByVol(0)
{

}

CCustCommissionRateSettingDialog::~CCustCommissionRateSettingDialog()
{
}

void CCustCommissionRateSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CUST, m_ctlCust);
	DDX_Control(pDX, IDC_COMBO_EXCHANGE, m_ctlExchange);
	DDX_Control(pDX, IDC_COMBO_PRODUCT, m_ctlProduct);
	DDX_Text(pDX, IDC_EDIT_DELIVERY_DATE, m_Date);
	DDX_Text(pDX, IDC_EDIT_TD_BYAMOUNT, m_ByAmt);
	//  DDX_Text(pDX, IDC_EDIT_TD_BYVOLUME, m_byVol);
	DDX_Text(pDX, IDC_EDIT_BYAMOUNT, m_TdByAmt);
	DDX_Text(pDX, IDC_EDIT_BYVOLUME, m_TdByVol);
	DDX_Text(pDX, IDC_EDIT_TD_BYVOLUME, m_ByVol);
	DDV_MaxChars(pDX, m_Date, 4);
}


BEGIN_MESSAGE_MAP(CCustCommissionRateSettingDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCustCommissionRateSettingDialog::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CCustCommissionRateSettingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (strcmp(m_flag, "5") == 0)
	{
		GetDlgItem(IDC_COMBO_CUST)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_EXCHANGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PRODUCT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CUST)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DELIVERY_DATE)->EnableWindow(FALSE);
	}

	if (strcmp(m_flag, "3") == 0)
	{
		GetDlgItem(IDC_COMBO_CUST)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_EXCHANGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PRODUCT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CUST)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DELIVERY_DATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BYAMOUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BYVOLUME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TD_BYAMOUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TD_BYVOLUME)->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


// CCustCommissionRateSettingDialog 消息处理程序


void CCustCommissionRateSettingDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	UpdateData(TRUE);
	Scoped_BCHANDLE handle;
	cust_commission_rate_st ccr = { 0 };
	CString cust_no = m_ctlCust.GetCurSelKey();
	CString exchangeid = m_ctlExchange.GetCurSelKey();
	CString productid = m_ctlProduct.GetCurSelKey();

	strcpy(ccr.cust_no, cust_no);
	strcpy(ccr.exchangeid, exchangeid);
	strcpy(ccr.productid, productid);
	strcpy(ccr.delivery_date, m_Date);
	ccr.commission_rate_by_money = m_ByAmt;
	ccr.commission_rate_by_volume = m_ByVol;
	ccr.offset_td_commission_rate_by_money = m_TdByAmt;
	ccr.offset_td_commission_rate_by_volume = m_TdByVol;

	BCResult result = BCRequestSetCustCommissionRate_851304(handle, m_flag, ccr);
	if (result)
	{
		MessageBox("操作成功");
	}
	else
	{
		MessageBox(result.errmsg);
	}
}


