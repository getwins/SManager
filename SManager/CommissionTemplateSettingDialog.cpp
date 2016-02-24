// CommissionTemplateSettingDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "CommissionTemplateSettingDialog.h"
#include "afxdialogex.h"
#include "SMutility.h"

// CCommissionTemplateSettingDialog 对话框

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


	// TODO:  在此添加额外的初始化
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
				  // 异常: OCX 属性页应返回 FALSE
}


// CCommissionTemplateSettingDialog 消息处理程序


void CCommissionTemplateSettingDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	//CString ctno = m_ctlCommission.GetCurSelKey();
	UpdateData(TRUE);
	hyphenstr hct = m_ctlCommission.GetCurSelKV();
	if (hct.left.GetLength() == 0)
	{
		MessageBox("请先选择模板");
		return;
	}

	CString exchangeid = m_ctlExchange.GetCurSelKey();
	if (exchangeid.GetLength() == 0)
	{
		MessageBox("请先选择交易所");
		return;
	}
	CString productid = m_ctlProduct.GetCurSelKey();
	if (productid.GetLength() == 0)
	{
		MessageBox("请先选择品种");
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
			MessageBox("添加成功");
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
			
			MessageBox("修改成功");
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
			MessageBox("删除成功");
		}
			
	}
}
//
///* 输入：操作员			input:scust_no
//操作标志（增加、修改、删除）			sstatus0
//模板号			sholder_ac_no
//交易所			smarket_code
//品种			sstock_code
//交割期			sdate0
//开平手续费率（按金额）			damt2
//开平手续费率（按手数）			damt3
//开平附加费率1（按金额）			damt4
//开平附加费率1（按手数）			damt5
//开平附加费率2（按金额）			damt6
//开平附加费率2（按手数）			damt7
//平今手续费率（按金额）			damt8
//平今手续费率（按手数）			damt9
//平今附加费率1（按金额）			damt10
//平今附加费率1（按手数）			damt11
//平今附加费率2（按金额）			damt12
//平今附加费率2（按手数）			damt13
//交割手续费率（按金额）			damt14
//交割手续费率（按手数）			damt15
//保值开平手续费率（按金额）			damt16
//保值开平手续费率（按手数）			damt17
//保值开平附加费率1(按金额)			damt18
//保值开平附加费率1（按手数）			damt19
//保值开平附加费率2(按金额)			damt20
//保值开平附加费率2（按手数）			damt21
//保值平今手续费率（按金额）			damt22
//保值平今手续费率（按手数）			damt23
//保值平今附加费率1(按金额)			damt24
//保值平今附加费率1（按手数）			damt25
//保值平今附加费率2(按金额)			damt26
//保值平今附加费率2（按手数）			damt27
//保值交割手续费率（按金额）			damt28
//保值交割手续费率（按手数）			damt29
//套利开平手续费率（按金额）			damt30
//套利开平手续费率（按手数）			damt31
//套利平今手续费率（按金额）			damt32
//套利平今手续费率（按手数）			damt33
//套利交割手续费率（按金额）			damt0
//套利交割手续费率（按手数） */			damt1

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
	// TODO: 在此添加控件通知处理程序代码
	m_ctlProduct.FilterByExchangeid(m_ctlExchange.GetCurSelKey());
	m_ctlProduct.SetCurSel(0);
}


