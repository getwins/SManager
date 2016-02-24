
// SMPermDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMPerm.h"
#include "SMPermDlg.h"
#include "afxdialogex.h"
#include <algorithm>
#include <sstream>
#include <regex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::regex reg("(\\d{4})(\\d{2})(\\d{2})-(\\d{4})(\\d{2})(\\d{2})");
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSMPermDlg �Ի���



CSMPermDlg::CSMPermDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SMPERM_DIALOG, pParent)
	, m_OperCode(_T(""))
	//, m_CustNum(0)
	//, m_DateFrom(COleDateTime::GetCurrentTime())
	//, m_DateTo(COleDateTime::GetCurrentTime())
	, m_CustNum(_T(""))
	, m_DateFrom(_T(""))
	, m_DateTo(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_DateFrom.SetStatus(COleDateTime::null);
	//m_DateTo.SetStatus(COleDateTime::null);
}

void CSMPermDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OPER, m_OperCode);
	//  DDX_Text(pDX, IDC_EDIT_OPEN_CUST_VOLUME, m_CustNum);
	//  DDX_Text(pDX, IDC_EDIT_DATE_FROM, m_DateFrom);
	//  DDX_Text(pDX, IDC_EDIT_DATE_TO, m_DateTo);
	DDX_Text(pDX, IDC_EDIT_OPEN_CUST_VOLUME, m_CustNum);
	DDX_Text(pDX, IDC_EDIT_DATE_FROM, m_DateFrom);
	DDX_Text(pDX, IDC_EDIT_DATE_TO, m_DateTo);
}

BEGIN_MESSAGE_MAP(CSMPermDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_QUERY, &CSMPermDlg::OnBnClickedBtnQuery)
	ON_BN_CLICKED(ID_SET, &CSMPermDlg::OnBnClickedSet)
	ON_BN_CLICKED(ID_DEL, &CSMPermDlg::OnBnClickedDel)
END_MESSAGE_MAP()


// CSMPermDlg ��Ϣ�������

BOOL CSMPermDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSMPermDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSMPermDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSMPermDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSMPermDlg::OnBnClickedBtnQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BCResult result;
	std::ostringstream oss;
	UpdateData(TRUE);
	result = BCRequestQryOper_851201(m_handle, m_OperCode.GetBuffer());
	if (!result)
	{
		MessageBox(result.errmsg);
		return;
	}

	std::vector<oper_sm_perm_st> perms;
	result = BCRequestQryOperPerm_860003(m_handle, m_OperCode.GetBuffer(), "", perms);
	if (!result)
	{
		MessageBox(result.errmsg);
		return;
	}

	std::vector<oper_sm_perm_st>::iterator it;
	it = std::find_if(perms.begin(), perms.end(), [](oper_sm_perm_st &perm)->bool { return strcmp(perm.type, "001") == 0; });
	if (it != perms.end())
	{
		m_CustNum = it->param;
		UpdateData(FALSE);
	}

	it = std::find_if(perms.begin(), perms.end(), [](oper_sm_perm_st &perm)->bool { return strcmp(perm.type, "002") == 0; });
	if (it != perms.end())
	{
		std::cmatch cm;
		if (std::regex_match(it->param, cm, reg))
		{
			//m_DateFrom.SetDate(atoi(cm[1].str().c_str()), atoi(cm[2].str().c_str()), atoi(cm[3].str().c_str()));
			//m_DateFrom.SetDate(atoi(cm[4].str().c_str()), atoi(cm[5].str().c_str()), atoi(cm[6].str().c_str()));
			m_DateFrom = (cm[1].str() + cm[2].str() + cm[3].str()).c_str();
			m_DateTo = (cm[4].str() + cm[5].str() + cm[6].str()).c_str();
			UpdateData(FALSE);
		}
		else
		{
			MessageBox("���ڸ�ʽ����");
		}
		//m_CustNum = it->param;
		//UpdateData(FALSE);
		//m_DateFrom.SetDate
	}

}


void CSMPermDlg::OnBnClickedSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	BCResult result;
	std::ostringstream oss;

	CString dateParam = m_DateFrom + "-" + m_DateTo;
	if (!std::regex_match(dateParam.GetBuffer(), reg))
	{
		MessageBox("���ڸ�ʽ����");
		return;
	}

	result = BCRequestQryOper_851201(m_handle, m_OperCode.GetBuffer());
	if (!result)
	{
		MessageBox(result.errmsg);
		return;
	}

	std::vector<oper_sm_perm_st> perms;
	result = BCRequestQryOperPerm_860003(m_handle, m_OperCode.GetBuffer(), "", perms);
	//if (!result)
	//{
	//	MessageBox(result.errmsg);
	//	return;
	//}
	std::vector<oper_sm_perm_st>::iterator it;
	it = std::find_if(perms.begin(), perms.end(), [](oper_sm_perm_st &perm)->bool { return strcmp(perm.type, "001") == 0; });
	if (it == perms.end())
	{
		result = BCRequestSetOperPerm_860004(m_handle, "1", m_OperCode.GetBuffer(), "001", m_CustNum.GetBuffer());
		if (result)
			oss << "��ӿ����������Ƴɹ�|";
		else
			oss << result << "|";
	}
	else
	{
		result = BCRequestSetOperPerm_860004(m_handle, "2", m_OperCode.GetBuffer(), "001", m_CustNum.GetBuffer());
		if (result)
			oss << "�޸Ŀ����������Ƴɹ�|";
		else
			oss << result << "|";
	}

	it = std::find_if(perms.begin(), perms.end(), [](oper_sm_perm_st &perm)->bool { return strcmp(perm.type, "002") == 0; });
	//CString dateParam = m_DateFrom.Format("%Y%m%d") + "-" + m_DateTo.Format("%Y%m%d");
	

	if (it == perms.end())
	{
		
		result = BCRequestSetOperPerm_860004(m_handle, "1", m_OperCode.GetBuffer(), "002", dateParam.GetBuffer());
		if (result)
			oss << "��Ӳ���ԱSManagerʹ�����޳ɹ�|";
		else
			oss << result << "|";
	}
	else
	{
		result = BCRequestSetOperPerm_860004(m_handle, "2", m_OperCode.GetBuffer(), "002", dateParam.GetBuffer());
		if (result)
			oss << "�޸Ĳ���ԱSManagerʹ�����޳ɹ�|";
		else
			oss << result << "|";
	}

	MessageBox(oss.str().c_str());
}


void CSMPermDlg::OnBnClickedDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//UpdateData(TRUE);
	BCResult result;
	std::ostringstream oss;
	result = BCRequestQryOper_851201(m_handle, m_OperCode.GetBuffer());
	if (!result)
	{
		MessageBox(result.errmsg);
		return;
	}

	std::vector<oper_sm_perm_st> perms;
	result = BCRequestQryOperPerm_860003(m_handle, m_OperCode.GetBuffer(), "", perms);
	if (!result)
	{
		MessageBox(result.errmsg);
		return;
	}
	std::vector<oper_sm_perm_st>::iterator it;
	it = std::find_if(perms.begin(), perms.end(), [](oper_sm_perm_st &perm)->bool { return strcmp(perm.type, "001") == 0; });
	if (it == perms.end())
	{
		oss << "δ��Ӳ���ԱSManager������������|";
	}
	else
	{
		result = BCRequestSetOperPerm_860004(m_handle, "3", m_OperCode.GetBuffer(), "001", "");
		if (result)
			oss << "ɾ������ԱSManager�����������Ƴɹ�|";
		else
			oss << result << "|";
	}

	it = std::find_if(perms.begin(), perms.end(), [](oper_sm_perm_st &perm)->bool { return strcmp(perm.type, "002") == 0; });
	//CString dateParam = m_DateFrom.Format("%Y%m%d") + "-" + m_DateTo.Format("%Y%m%d");
	if (it == perms.end())
	{
		oss << "δ��Ӳ���ԱSManagerʹ����������|";
	}
	else
	{
		result = BCRequestSetOperPerm_860004(m_handle, "3", m_OperCode.GetBuffer(), "002", "");
		if (result)
			oss << "ɾ������ԱSManagerʹ�����޳ɹ�|";
		else
			oss << result << "|";
	}

	MessageBox(oss.str().c_str());
}

