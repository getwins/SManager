
// SMPerm.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "SMPerm.h"
#include "SMPermDlg.h"
#include "MyBCRequest.h"
#include "OperLoginDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char g_drtp_host[32];
int g_drtp_port;
int g_funcno;
char g_oper[16];

void ReadConf()
{
	char path[MAX_PATH] = { 0 };
	GetCurrentDirectory(1000, path);
	strcat(path, "/conf.ini");

	GetPrivateProfileString("drtp", "host", "", g_drtp_host, sizeof(g_drtp_host), path);
	g_drtp_port = GetPrivateProfileInt("drtp", "port", 3000, path);
	g_funcno = GetPrivateProfileInt("drtp", "funcno", 19900, path);
}

// CSMPermApp

BEGIN_MESSAGE_MAP(CSMPermApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()



// CSMPermApp ����

CSMPermApp::CSMPermApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSMPermApp ����

CSMPermApp theApp;


// CSMPermApp ��ʼ��

BOOL CSMPermApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("SMPerm"));

	ReadConf();
	if (!MyBCCLTInit(g_drtp_host, g_drtp_port))
	{
		MessageBox(NULL, "BCCLT��ʼ��ʧ��", "�쳣", MB_ICONERROR);
		return FALSE;
	}

	COperLoginDialog loginDlg;
	if (loginDlg.DoModal() != IDOK)
	{
		return FALSE;
	}

	strcpy(g_oper, loginDlg.m_OperCode);

	CSMPermDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�

	BCResult result = BCRequestOperLogout_854001(loginDlg.m_OperCode.GetBuffer(), loginDlg.m_OperPasswd.GetBuffer());
	if (!result)
	{
		MessageBox(NULL, result.errmsg, "����Ա�ǳ�ʧ��", MB_ICONERROR);
	}
	return FALSE;
}

