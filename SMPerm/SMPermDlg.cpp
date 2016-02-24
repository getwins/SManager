
// SMPermDlg.cpp : 实现文件
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
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CSMPermDlg 对话框



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


// CSMPermDlg 消息处理程序

BOOL CSMPermDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSMPermDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSMPermDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSMPermDlg::OnBnClickedBtnQuery()
{
	// TODO: 在此添加控件通知处理程序代码
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
			MessageBox("日期格式错误");
		}
		//m_CustNum = it->param;
		//UpdateData(FALSE);
		//m_DateFrom.SetDate
	}

}


void CSMPermDlg::OnBnClickedSet()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	BCResult result;
	std::ostringstream oss;

	CString dateParam = m_DateFrom + "-" + m_DateTo;
	if (!std::regex_match(dateParam.GetBuffer(), reg))
	{
		MessageBox("日期格式错误！");
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
			oss << "添加开户数据量制成功|";
		else
			oss << result << "|";
	}
	else
	{
		result = BCRequestSetOperPerm_860004(m_handle, "2", m_OperCode.GetBuffer(), "001", m_CustNum.GetBuffer());
		if (result)
			oss << "修改开户数量限制成功|";
		else
			oss << result << "|";
	}

	it = std::find_if(perms.begin(), perms.end(), [](oper_sm_perm_st &perm)->bool { return strcmp(perm.type, "002") == 0; });
	//CString dateParam = m_DateFrom.Format("%Y%m%d") + "-" + m_DateTo.Format("%Y%m%d");
	

	if (it == perms.end())
	{
		
		result = BCRequestSetOperPerm_860004(m_handle, "1", m_OperCode.GetBuffer(), "002", dateParam.GetBuffer());
		if (result)
			oss << "添加操作员SManager使用期限成功|";
		else
			oss << result << "|";
	}
	else
	{
		result = BCRequestSetOperPerm_860004(m_handle, "2", m_OperCode.GetBuffer(), "002", dateParam.GetBuffer());
		if (result)
			oss << "修改操作员SManager使用期限成功|";
		else
			oss << result << "|";
	}

	MessageBox(oss.str().c_str());
}


void CSMPermDlg::OnBnClickedDel()
{
	// TODO: 在此添加控件通知处理程序代码
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
		oss << "未添加操作员SManager开户数量限制|";
	}
	else
	{
		result = BCRequestSetOperPerm_860004(m_handle, "3", m_OperCode.GetBuffer(), "001", "");
		if (result)
			oss << "删除操作员SManager开户数量限制成功|";
		else
			oss << result << "|";
	}

	it = std::find_if(perms.begin(), perms.end(), [](oper_sm_perm_st &perm)->bool { return strcmp(perm.type, "002") == 0; });
	//CString dateParam = m_DateFrom.Format("%Y%m%d") + "-" + m_DateTo.Format("%Y%m%d");
	if (it == perms.end())
	{
		oss << "未添加操作员SManager使用期限限制|";
	}
	else
	{
		result = BCRequestSetOperPerm_860004(m_handle, "3", m_OperCode.GetBuffer(), "002", "");
		if (result)
			oss << "删除操作员SManager使用期限成功|";
		else
			oss << result << "|";
	}

	MessageBox(oss.str().c_str());
}

