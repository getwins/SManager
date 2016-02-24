// OpenCustDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "OpenCustDialog.h"
#include "afxdialogex.h"

#include <sstream>
// COpenCustDialog 对话框

IMPLEMENT_DYNAMIC(COpenCustDialog, CDialogEx)

COpenCustDialog::COpenCustDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPEN_CUST, pParent)
	, m_CustNo(_T(""))
	, m_CustName(_T(""))
	, m_TradePasswd(_T(""))
	, m_MoneyPasswd(_T(""))
	, m_ctlCustClass(1)
{

}

COpenCustDialog::~COpenCustDialog()
{
}

void COpenCustDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COMMISSION, m_ctlCommission);
	DDX_Control(pDX, IDC_COMBO_MARGIN, m_ctlMargin);
	DDX_Text(pDX, IDC_EDIT_CUST_NO, m_CustNo);
	DDX_Text(pDX, IDC_EDIT_CUST_NAME, m_CustName);
	DDX_Text(pDX, IDC_EDIT_TRADE_PASSWD, m_TradePasswd);
	DDX_Text(pDX, IDC_EDIT_MONEY_PASSWD, m_MoneyPasswd);
	//  DDX_Control(pDX, IDC_COMBO1, m_ctlCustClass);
	DDX_Control(pDX, IDC_COMBO_CUST_CLASS, m_ctlCustClass);
}


BEGIN_MESSAGE_MAP(COpenCustDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &COpenCustDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// COpenCustDialog 消息处理程序

///* 输入：操作员			input:scust_no
//客户号			sholder_ac_no
//客户名称			sname
//客户全称			sall_name
//证件类型			scust_type2
//证件号码			scert_no
//邮编			spost_code
//地址			saddr
//电话			sphone
//手机			sphone3
//传真			sphone2
//电子邮件			semail
//法人			scust_auth2
//合同编号			sbankname2
//备注			snote
//交易密码			stx_pwd
//资金密码			swithdraw_pwd
//客户类			sserial1
//类内资金共享			sstatus2
//客户性质			sstatus1
//指令下达人1			scust_auth
//指令下达人身份证号1			scust_limit
//指令下达人2			spager
//指令下达人身份证号2			scard0
//指令下达人3			sstation1
//指令下达人身份证号3			scard1
//资金调拨人1			scert_no2
//资金调拨人身份证号1			sbank_acc
//资金调拨人2			scert_addr
//资金调拨人身份证号2			sbank_acc2
//法人证件			scust_limit2
//组织机构代码			sname2
//合作机构代码			sbankname
//委托密码 */			semp_pwd

bool COpenCustDialog::CallOpenCust_851000(BCHANDLE handle)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 851000);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", m_CustNo.GetBuffer());
	BCSetStringFieldByName(handle, 0, "sname", m_CustName.GetBuffer());
	BCSetStringFieldByName(handle, 0, "sall_name", m_CustName.GetBuffer());
	BCSetStringFieldByName(handle, 0, "sstatus1", "1"); //客户性质 自然人
	BCSetStringFieldByName(handle, 0, "scust_type2", "9"); //证件类型 其它证件
	BCSetStringFieldByName(handle, 0, "scert_no", m_CustName.GetBuffer());//证件号码
	BCSetStringFieldByName(handle, 0, "stx_pwd", m_TradePasswd.GetBuffer()); //交易密码
	BCSetStringFieldByName(handle, 0, "swithdraw_pwd", m_MoneyPasswd.GetBuffer()); //资金密码

	char cust_class[13] = { 0 };
	memset(cust_class, '_', sizeof(cust_class) - 1);
	strcpy(cust_class, m_ctlCustClass.GetCurSelKey());
	BCSetStringFieldByName(handle, 0, "sserial1", cust_class); //客户类
	
	return MyBCRequest(handle, FetchRowFunc_t(), BCMSG_ERR_TOALL);
}

bool COpenCustDialog::SetCustCommissionTemplate_851342(BCHANDLE handle, char *template_no)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 851342);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code); //操作员
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", m_CustNo.GetBuffer()); //客户号
	BCSetStringFieldByName(handle, 0, "sstatus0", "1"); //操作标志（增加
	BCSetStringFieldByName(handle, 0, "sstation0", template_no); //手续费率模板
	BCSetStringFieldByName(handle, 0, "sstatus1", "0"); //手续费率独立设置 否
	BCSetStringFieldByName(handle, 0, "sstatus2", "0"); //手续费率强制替换标志 否
	return  MyBCRequest(handle, FetchRowFunc_t(), BCMSG_ERR_TOALL);
}

bool COpenCustDialog::SetCustMarginTemplate_851348(BCHANDLE handle, char *template_no)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 851348);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code); //操作员
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", m_CustNo.GetBuffer()); //客户号
	BCSetStringFieldByName(handle, 0, "sstatus0", "1"); //操作标志（增加
	BCSetStringFieldByName(handle, 0, "sstation0", template_no); //手续费率模板
	BCSetStringFieldByName(handle, 0, "sstatus1", "0"); //手续费率独立设置 否
	BCSetStringFieldByName(handle, 0, "sstatus2", "0"); //手续费率强制替换标志 否
	return  MyBCRequest(handle, FetchRowFunc_t(), BCMSG_ERR_TOALL);
}

bool COpenCustDialog::SetCustTradeCode_851010(BCHANDLE handle)
{
	for each (seat_st s in theApp.m_BaseInfo.seats)
	{
		BCResetHandle(handle);
		BCSetRequestType(handle, 851010);
		BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code); //操作员
		BCSetStringFieldByName(handle, 0, "sstatus0", "4"); //操作标志（增加 //存在仍增加
		BCSetStringFieldByName(handle, 0, "sholder_ac_no", m_CustNo.GetBuffer()); //客户号
		BCSetStringFieldByName(handle, 0, "sserial2", s.seat_no); //席位号
		BCSetStringFieldByName(handle, 0, "sholder_ac_no2", (CString("SM") + m_CustNo).GetBuffer());//交易编码
		BCSetStringFieldByName(handle, 0, "sstatus1", "1");//交易编码来源
		if(strcmp(s.seat_no, "cffex") == 0)
			BCSetStringFieldByName(handle, 0, "sstatus2", "3");//交易编码属性 投机

		if (!MyBCRequest(handle, FetchRowFunc_t(), BCMSG_ERR_TOALL))
			return false;
	}
	
	return true;
}

bool COpenCustDialog::SetCustAuthority_851012(BCHANDLE handle)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 851012);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", m_CustNo.GetBuffer());
	//BCSetStringFieldByName(handle, 0, "usset0", "");//权限列表
	BCSetStringFieldByName(handle, 0, "sbank_acc", "1100011000111001100111100001100000000000");//菜单列表
	//BCSetStringFieldByName(handle, 0, "sbank_code", "");//，权限模板代码
	unsigned char usset0[100] = { 0 };
	usset0[0] = 192;
	usset0[2] = 8;
	usset0[5] = 202;
	usset0[6] = 131;
	usset0[7] = 240;
	usset0[13] = 2;
	usset0[14] = 128;
	usset0[16] = 34;
	usset0[17] = 12;
	usset0[18] = 128;
	usset0[23] = 3;
	usset0[24] = 252;
	usset0[25] = 4;
	BCSetStringFieldByName(handle, 0, "usset0", (char*)usset0);

	return MyBCRequest(handle, FetchRowFunc_t(), BCMSG_ERR_TOALL);
}

BCResult COpenCustDialog::SetCustMultiLogin_851278(BCHANDLE handle)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 851278);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sstatus2", "0");
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", m_CustNo.GetBuffer());
	BCSetStringFieldByName(handle, 0, "sserial1", "");
	BCSetStringFieldByName(handle, 0, "sstatus1", "1");
	return MyBCRequest(handle);
}

BOOL COpenCustDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//theApp.m_BaseInfo.commission_no
	//std::for_each(theApp.m_BaseInfo.commission_no.begin(),
	//	theApp.m_BaseInfo.commission_no.end(),
	//	[this]()
	//for each (commission_template_no_st ctn in theApp.m_BaseInfo.commission_no)
	//{
	//	m_ctlCommission.AddString(CString(ctn.template_no) + "-" + CString(ctn.template_name));
	//}
	//m_ctlCommission.SetCurSel(0);

	//for each(margin_template_no_st mtn in theApp.m_BaseInfo.margin_no)
	//{
	//	m_ctlMargin.AddString(CString(mtn.template_no) + "-" + CString(mtn.template_name));
	//}
	//m_ctlCommission.SetCurSel(0);
	//std::for_each
	m_ctlCustClass.InsertString(0, CString("_-_"));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//function 851000 description  "客户开户"  reqlevel 1
//操作员				scust_no
//客户号				sholder_ac_no
//客户名称				sname
//客户全称				sall_name
//证件类型				scust_type2
//证件号码				scert_no
//邮编				spost_code
//地址				saddr
//电话				sphone
//手机				sphone3
//传真				sphone2
//电子邮件				semail
//法人				scust_auth2
//合同编号				sbankname2
//备注				snote
//交易密码				stx_pwd
//资金密码				swithdraw_pwd
//客户类				sserial1
//类内资金共享				sstatus2
//客户性质				sstatus1
//指令下达人1				scust_auth
//指令下达人身份证号1				scust_limit
//指令下达人2				spager
//指令下达人身份证号2				scard0
//指令下达人3				sstation1
//指令下达人身份证号3				scard1
//资金调拨人1				scert_no2
//资金调拨人身份证号1				sbank_acc
//资金调拨人2				scert_addr
//资金调拨人身份证号2				sbank_acc2
//法人证件				scust_limit2
//组织机构代码				sname2
//合作机构代码				sbankname
//委托密码				semp_pwd


void COpenCustDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(theApp.m_CustNum > 0 && theApp.GetCustnoList().size() >= theApp.m_CustNum)
	{
		MessageBox("操作员开户数量已达上限，不能开户");
		return;
	}

	Scoped_BCHANDLE handle;
	if (!CallOpenCust_851000(handle))
		return;

	std::ostringstream oss;
	oss << "客户开户成功";

	cust_base_info_st cbi;
	strcpy(cbi.basic.scust_no, m_CustNo.GetBuffer());
	strcpy(cbi.basic.sname, m_CustName.GetBuffer());
	strcpy(cbi.basic.card_type, "1");
	strcpy(cbi.basic.sstatus0, "0");
	//int sel = m_ctlCommission.GetCurSel();
	//CString str;
	//if (sel >= 0 && sel < m_ctlCommission.GetCount()) {
	//	m_ctlCommission.GetLBText(sel, str);
	//	hyphenstr h(str);
	//	SetCustCommissionTemplate_851342(handle, h.left.GetBuffer());
	//}
	hyphenstr ctKV = m_ctlCommission.GetCurSelKV();
	if (ctKV.left.GetLength() > 0)
	{
		if (SetCustCommissionTemplate_851342(handle, ctKV.left.GetBuffer()))
		{
			strcpy(cbi.commission_temp.template_no, ctKV.left.GetBuffer());
			strcpy(cbi.commission_temp.template_name, ctKV.right.GetBuffer());
		}
		else
			oss << "，设置手续费率模板失败";


	}
	//sel = m_ctlMargin.GetCurSel();
	//if (sel >= 0 && sel < m_ctlMargin.GetCount()) {
	//	m_ctlMargin.GetLBText(sel, str);
	//	hyphenstr h(str);
	//	SetCustMarginTemplate_851348(handle, h.left.GetBuffer());
	//}
	hyphenstr mtKV = m_ctlMargin.GetCurSelKV();
	if (mtKV.left.GetLength() > 0)
	{
		if (SetCustMarginTemplate_851348(handle, mtKV.left.GetBuffer()))
		{
			strcpy(cbi.margin_temp.template_no, mtKV.left.GetBuffer());
			strcpy(cbi.margin_temp.template_name, mtKV.right.GetBuffer());
		}
		else
			oss << ",设置保证金率模板失败";

	}

	if (!SetCustTradeCode_851010(handle))
		oss << ",设置客户交易编码失败";

	if (!SetCustAuthority_851012(handle))
		oss << ",设置客户权限失败";

	if (!SetCustMultiLogin_851278(handle))
		oss << ",设置客户多点登录失败";

	oss << ".";
	theApp.AddCust(cbi);
	MessageBox(oss.str().c_str());

	//CDialogEx::OnOK();
}
