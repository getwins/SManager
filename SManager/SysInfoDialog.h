#pragma once


// CSysInfoDialog �Ի���
struct system_info_st;

class CSysInfoDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSysInfoDialog)

public:
	CSysInfoDialog(/*system_info_st *pSysInfo,*/ CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSysInfoDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SYSTEM_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	system_info_st m_SysInfo;

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnEnChangeEditSysDate();
	virtual BOOL OnInitDialog();
	bool RequestSysInfo_100319();
};
