// CustTradeProductLimitSettingDailog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "CustTradeProductLimitSettingDailog.h"
#include "afxdialogex.h"
#include "CommBCRequest.h"


// CCustTradeProductLimitSettingDailog 对话框

IMPLEMENT_DYNAMIC(CCustTradeProductLimitSettingDailog, CDialogEx)

CCustTradeProductLimitSettingDailog::CCustTradeProductLimitSettingDailog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CUST_TRADE_PRODUCT_LIMIT_SETTING, pParent)
{

}

CCustTradeProductLimitSettingDailog::~CCustTradeProductLimitSettingDailog()
{
}

void CCustTradeProductLimitSettingDailog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CUST, m_ctlCust);
	DDX_Control(pDX, IDC_COMBO_EXCHANGE, m_ctlExchange);
	DDX_Control(pDX, IDC_COMBO_PRODUCT, m_ctlProduct);
	DDX_Text(pDX, IDC_EDIT_DELIVERY_DATE, m_date);
	//DDX_Text(pDX, IDC_EDIT_TD_BYAMOUNT, m_ByAmt);
	//  DDX_Text(pDX, IDC_EDIT_TD_BYVOLUME, m_byVol);
	//DDX_Text(pDX, IDC_EDIT_BYAMOUNT, m_TdByAmt);
	//DDX_Text(pDX, IDC_EDIT_BYVOLUME, m_TdByVol);
	//DDX_Text(pDX, IDC_EDIT_TD_BYVOLUME, m_ByVol);
	DDV_MaxChars(pDX, m_date, 4);
	DDX_Control(pDX, IDC_COMBO_LIMIT_TYPE, m_LimitType);
}


BEGIN_MESSAGE_MAP(CCustTradeProductLimitSettingDailog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCustTradeProductLimitSettingDailog::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_EXCHANGE, &CCustTradeProductLimitSettingDailog::OnCbnSelchangeComboExchange)
END_MESSAGE_MAP()


// CCustTradeProductLimitSettingDailog 消息处理程序


BOOL CCustTradeProductLimitSettingDailog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (strcmp(m_flag, "2") == 0)
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
		GetDlgItem(IDC_COMBO_LIMIT_TYPE)->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


// CCustTradeProductLimitSettingDailog 消息处理程序


void CCustTradeProductLimitSettingDailog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	UpdateData(TRUE);
	Scoped_BCHANDLE handle;
	trade_product_limit_st tpl = { 0 };
	CString cust_no = m_ctlCust.GetCurSelKey();
	if (cust_no.GetLength() == 0)
	{
		MessageBox("请选择客户");
		return;
	}
	CString exchangeid = m_ctlExchange.GetCurSelKey();
	if (exchangeid.GetLength() == 0)
	{
		MessageBox("请选择交易所");
		return;
	}
	CString productid = m_ctlProduct.GetCurSelKey();
	if (productid.GetLength() == 0)
	{
		MessageBox("请选择品种");
		return;
	}
	CString limit_type = m_LimitType.GetCurSelKey();
	if (limit_type.GetLength() == 0)
	{
		MessageBox("请选择限制类型");
		return;
	}

	strcpy(tpl.cust_no, cust_no);
	strcpy(tpl.exchangeid, exchangeid);
	strcpy(tpl.productid, productid);
	strcpy(tpl.delivery_date, m_date);
	strcpy(tpl.limit_type, limit_type);

	BCResult result = BCRequestSetTradeProductLimit_854151(handle, m_flag.GetBuffer(), tpl);
	if (result)
	{
		MessageBox("操作成功");
	}
	else
	{
		MessageBox(result.errmsg);
	}
}




void CCustTradeProductLimitSettingDailog::OnCbnSelchangeComboExchange()
{
	// TODO: 在此添加控件通知处理程序代码
	CString exchangeid = m_ctlExchange.GetCurSelKey();
	m_ctlProduct.FilterByExchangeid(exchangeid);
}

