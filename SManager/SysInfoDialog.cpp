// SysInfoDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "SysInfoDialog.h"
#include "afxdialogex.h"
#include "SMutility.h"

// CSysInfoDialog �Ի���

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


// CSysInfoDialog ��Ϣ�������


//void CSysInfoDialog::OnEnChangeEditSysDate()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


BOOL CSysInfoDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//system_info_st sysinfo = theApp.m_BaseInfo.sysinfo;
	RequestSysInfo_100319();

	SetDlgItemText(IDC_EDIT_SYS_DATE, m_SysInfo.system_date);
	SetDlgItemText(IDC_EDIT_CHECK_DATE, m_SysInfo.check_date);
	SetDlgItemText(IDC_EDIT_NEXT_TRADINGDAY, m_SysInfo.next_tradingday);
	SetDlgItemText(IDC_EDIT_HOST_DATE, m_SysInfo.host_date);
	SetDlgItemText(IDC_EDIT_HOST_TIME, m_SysInfo.system_time);
	SetDlgItemText(IDC_EDIT_SYS_STATUS, theApp.DictToStr(DICT_CLASS_SYSTEM_STATUS, m_SysInfo.system_status));


	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
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
