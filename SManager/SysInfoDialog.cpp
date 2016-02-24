// SysInfoDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "SysInfoDialog.h"
#include "afxdialogex.h"
#include "SMutility.h"

// CSysInfoDialog 对话框

IMPLEMENT_DYNAMIC(CSysInfoDialog, CDialogEx)

CSysInfoDialog::CSysInfoDialog(/*system_info_st *pSysInfo,*/ CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SYSTEM_INFO, pParent)
	//, m_pSysInfo(pSysInfo)
{

}

CSysInfoDialog::~CSysInfoDialog()
{
}

void CSysInfoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSysInfoDialog, CDialogEx)
//	ON_EN_CHANGE(IDC_EDIT_SYS_DATE, &CSysInfoDialog::OnEnChangeEditSysDate)
END_MESSAGE_MAP()


// CSysInfoDialog 消息处理程序


//void CSysInfoDialog::OnEnChangeEditSysDate()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，它将不
//	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// TODO:  在此添加控件通知处理程序代码
//}


BOOL CSysInfoDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//system_info_st sysinfo = theApp.m_BaseInfo.sysinfo;
	RequestSysInfo_100319();

	SetDlgItemText(IDC_EDIT_SYS_DATE, m_SysInfo.system_date);
	SetDlgItemText(IDC_EDIT_CHECK_DATE, m_SysInfo.check_date);
	SetDlgItemText(IDC_EDIT_NEXT_TRADINGDAY, m_SysInfo.next_tradingday);
	SetDlgItemText(IDC_EDIT_HOST_DATE, m_SysInfo.host_date);
	SetDlgItemText(IDC_EDIT_HOST_TIME, m_SysInfo.system_time);
	SetDlgItemText(IDC_EDIT_SYS_STATUS, theApp.DictToStr(DICT_CLASS_SYSTEM_STATUS, m_SysInfo.system_status));


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

bool CSysInfoDialog::RequestSysInfo_100319()
{
	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
		BCGetStringFieldByName(handle, 0, "sdate0", m_SysInfo.system_date, sizeof(m_SysInfo.system_time));
		BCGetStringFieldByName(handle, 0, "sdate2", m_SysInfo.check_date, sizeof(m_SysInfo.check_date));
		BCGetStringFieldByName(handle, 0, "sdate1", m_SysInfo.next_tradingday, sizeof(m_SysInfo.next_tradingday));
		BCGetStringFieldByName(handle, 0, "sdate3", m_SysInfo.host_date, sizeof(m_SysInfo.host_date));
		BCGetStringFieldByName(handle, 0, "sstatus0", m_SysInfo.system_status, sizeof(m_SysInfo.system_status));
		BCGetStringFieldByName(handle, 0, "stime0", m_SysInfo.system_time, sizeof(m_SysInfo.system_time));
	};

	Scoped_BCHANDLE handle;
	BCResetHandle(handle);
	BCSetRequestType(handle, 100319);
	BCSetStringFieldByName(handle, 0, "sbranch_code0", "000");
	return MyBCRequest(handle, fetcher);

}
