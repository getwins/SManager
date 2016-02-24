// MarginTemplateSettingDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "MarginTemplateSettingDialog.h"
#include "afxdialogex.h"


// CMarginTemplateSettingDialog 对话框

IMPLEMENT_DYNAMIC(CMarginTemplateSettingDialog, CDialogEx)

CMarginTemplateSettingDialog::CMarginTemplateSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MARGIN_SETTING, pParent)
	, m_ByAmt(0)
	, m_ByVol(0)
{

}

CMarginTemplateSettingDialog::~CMarginTemplateSettingDialog()
{
}

void CMarginTemplateSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MARGIN, m_ctlMargin);
	DDX_Control(pDX, IDC_COMBO_EXCHANGE, m_ctlExchange);
	DDX_Control(pDX, IDC_COMBO_PRODUCT, m_ctlProduct);
	DDX_Text(pDX, IDC_EDIT_TD_BYAMOUNT, m_ByAmt);
	//  DDX_Control(pDX, IDC_EDIT_TD_BYVOLUME, m_ByVol);
	DDX_Text(pDX, IDC_EDIT_TD_BYVOLUME, m_ByVol);
}


BEGIN_MESSAGE_MAP(CMarginTemplateSettingDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMarginTemplateSettingDialog::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_EXCHANGE, &CMarginTemplateSettingDialog::OnCbnSelchangeComboExchange)
END_MESSAGE_MAP()


// CMarginTemplateSettingDialog 消息处理程序


void CMarginTemplateSettingDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	UpdateData(TRUE);
	hyphenstr hmt = m_ctlMargin.GetCurSelKV();
	CString exchangeid = m_ctlExchange.GetCurSelKey();
	CString productid = m_ctlProduct.GetCurSelKey();
	if (RequestMargin_851340(hmt.left.GetBuffer(), exchangeid.GetBuffer(), productid.GetBuffer()))
	{
		std::vector<margin_template_st> &vm = theApp.m_BaseInfo.margins[hmt.left.GetBuffer()];
		if (m_flag == "4")
		{
			margin_template_st mt = { 0 };
			strcpy(mt.template_no, hmt.left.GetBuffer());
			strcpy(mt.template_name, hmt.right.GetBuffer());
			strcpy(mt.exchangeid, exchangeid.GetBuffer());
			strcpy(mt.productid, productid.GetBuffer());
			mt.margin_rate_by_money = m_ByAmt;
			mt.margin_rate_by_volume = m_ByVol;
			vm.push_back(mt);
			MessageBox("添加成功");
		}
		else if (m_flag == "5")
		{
			std::vector<margin_template_st>::iterator mtit;
			for (mtit = vm.begin(); mtit != vm.end(); ++mtit)
			{
				if (hmt.left == mtit->template_no && exchangeid == mtit->exchangeid && productid == mtit->productid)
				{
					mtit->margin_rate_by_money = m_ByAmt;
					mtit->margin_rate_by_volume = m_ByVol;
					break;
				}
			}
			MessageBox("修改成功");
		}
		else if (m_flag == "3")
		{
			std::vector<margin_template_st>::iterator mtit;
			for (mtit = vm.begin(); mtit != vm.end(); ++mtit)
			{
				if (hmt.left == mtit->template_no && exchangeid == mtit->exchangeid && productid == mtit->productid)
					break;
			}
			if (mtit != vm.end())
				vm.erase(mtit);
			MessageBox("删除成功");
		}
	}
}


BOOL CMarginTemplateSettingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//if (m_flag == "5" || m_flag == "3")
	//{
	//	m_ctlExchange.SelectKey(m_mtit->exchangeid);
	//	m_ctlProduct.SelectKey(m_mtit->productid);
	//	m_ByAmt = m_mtit->margin_rate_by_money;
	//	m_ByVol = m_mtit->margin_rate_by_volume;
	//}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

bool CMarginTemplateSettingDialog::RequestMargin_851340(char *mtno, char *exchangeid, char *productid)
{
	Scoped_BCHANDLE handle;
	BCSetRequestType(handle, 851340);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sstatus0", m_flag.GetBuffer());
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", mtno);
	BCSetStringFieldByName(handle, 0, "smarket_code", exchangeid);
	BCSetStringFieldByName(handle, 0, "sstock_code", productid);
	BCSetDoubleFieldByName(handle, 0, "damt2", m_ByAmt);
	BCSetDoubleFieldByName(handle, 0, "damt3", m_ByVol);
	BCSetDoubleFieldByName(handle, 0, "damt4", 0.0);
	BCSetDoubleFieldByName(handle, 0, "damt5", 0.0);
	BCSetDoubleFieldByName(handle, 0, "damt6", 0.0);
	BCSetDoubleFieldByName(handle, 0, "damt7", 0.0);
	BCSetStringFieldByName(handle, 0, "smain_flag2", "0");
	return MyBCRequest(handle, FetchRowFunc_t(), BCMSG_NOR_NOMB);
}


void CMarginTemplateSettingDialog::OnCbnSelchangeComboExchange()
{
	// TODO: 在此添加控件通知处理程序代码
	CString exchangeid = m_ctlExchange.GetCurSelKey();
	m_ctlProduct.FilterByExchangeid(exchangeid);
}
