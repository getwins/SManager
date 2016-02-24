// CustStatusMaintainDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "CustStatusMaintainDialog.h"
#include "afxdialogex.h"


// CCustStatusMaintainDialog 对话框

IMPLEMENT_DYNAMIC(CCustStatusMaintainDialog, CDialogEx)

CCustStatusMaintainDialog::CCustStatusMaintainDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CUST_STATUS_MAINTAIN, pParent)
	, m_CustName(_T(""))
	, m_CardType(_T(""))
	, m_CustStatus(_T(""))
	, m_CardNo(_T(""))
{

}

CCustStatusMaintainDialog::~CCustStatusMaintainDialog()
{
}

void CCustStatusMaintainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CUST, m_ctlCust);
	DDX_Control(pDX, IDC_COMBO_CUST_STATUS, m_ctlStatus);
	//  DDX_Control(pDX, IDC_EDIT_CUST_NAME, m_ctlCustName);
	//  DDX_Control(pDX, IDC_EDIT_CUST_STATUS, m_ctlCustStatus);
	//  DDX_Control(pDX, IDC_EDIT_CARD_TYPE, m_ctlCardType);
	DDX_Text(pDX, IDC_EDIT_CUST_NAME, m_CustName);
	//  DDX_Control(pDX, IDC_EDIT_CUST_STATUS, m_CustStatus);
	DDX_Text(pDX, IDC_EDIT_CARD_TYPE, m_CardType);
	DDX_Text(pDX, IDC_EDIT_CUST_STATUS, m_CustStatus);
	DDX_Text(pDX, IDC_EDIT_CARD_NO, m_CardNo);
}


BEGIN_MESSAGE_MAP(CCustStatusMaintainDialog, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_CUST, &CCustStatusMaintainDialog::OnCbnSelchangeComboCust)
	ON_BN_CLICKED(IDOK, &CCustStatusMaintainDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CCustStatusMaintainDialog 消息处理程序


BOOL CCustStatusMaintainDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//for each (cust_base_info_st cbi in theApp.GetCustBaseInfoList())
	//{
	//	m_ctlCust.AddString((CString(cbi.scust_no) + "-" + cbi.sname).GetBuffer());
	//}
	OnCbnSelchangeComboCust();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

bool CCustStatusMaintainDialog::RequestCustStatusMaintain_851005(char * cust_no, char * cust_status)
{
	Scoped_BCHANDLE handle;
	BCResetHandle(handle);
	BCSetRequestType(handle, 851005);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);
	BCSetStringFieldByName(handle, 0, "sstatus1", cust_status);
	return MyBCRequest(handle, FetchRowFunc_t(), BCMSG_NOR_NOMB);
}


void CCustStatusMaintainDialog::OnCbnSelchangeComboCust()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_ctlCust.GetCurSel();
	if (sel < 0 || sel >= m_ctlCust.GetCount())
		return;
	//cust_base_info_st cbi = theApp.GetCustBaseInfoList()[sel];
	CString cust_no = m_ctlCust.GetCurSelKey();
	cust_base_info_st cbi = theApp.GetCustBaseInfo(cust_no);
	m_CustName = cbi.basic.sname;
	//m_CardType = cbi.basic.
	m_CardType = theApp.DictToStr(DICT_CLASS_CARD_TYPE, cbi.basic.card_type);
	m_CardNo = cbi.basic.card_no;
	m_CustStatus = theApp.DictToStr(DICT_CLASS_CUST_STATUS, cbi.basic.sstatus0);
	if (cbi.basic.sstatus0[0] == '0')
		m_ctlStatus.SetCurSel(0);
	else if (cbi.basic.sstatus0[0] == '4')
		m_ctlStatus.SetCurSel(1);
	else if (cbi.basic.sstatus0[0] == '5')
		m_ctlStatus.SetCurSel(2);
	else if (cbi.basic.sstatus0[0] == '6')
		m_ctlStatus.SetCurSel(3);
	else if (cbi.basic.sstatus0[0] == '7')
		m_ctlStatus.SetCurSel(4);
	else
		m_ctlStatus.SetCurSel(-1);
	
	UpdateData(FALSE);
}


void CCustStatusMaintainDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel;
	sel = m_ctlCust.GetCurSel();
	if (sel < 0 || sel >= m_ctlCust.GetCount()) {
		MessageBox("请选择客户");
		return;
	}

	CString cust_no = m_ctlCust.GetCurSelKey();
	cust_base_info_st cbi = theApp.GetCustBaseInfo(cust_no);
	sel = m_ctlStatus.GetCurSel();
	if (sel < 0 || sel >= m_ctlStatus.GetCount())
	{
		MessageBox("请选择客户状态");
		return;
	}
	CString str;
	m_ctlStatus.GetLBText(sel, str);
	hyphenstr h(str);
	if (cbi.basic.sstatus0[0] == h.left[0])
		return;

	if (RequestCustStatusMaintain_851005(cust_no.GetBuffer(), h.left.GetBuffer()))
	{
		cbi.basic.sstatus0[0] = h.left[0];
		theApp.SetCustBaseInfo(cbi);
		//CDialogEx::OnOK();
		MessageBox("客户状态修改成功");
	}
}
