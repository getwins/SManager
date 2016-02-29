
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "SManager.h"

#include "MainFrm.h"
//#include "ProductDataDialog.h"
#include "string_identifer.h"
#include "cust_dynamic_info_worker.h"
#include "MdApi.h"
#include "SMutility.h"
#include "struct_helper.h"

#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	//ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	//ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	//	ON_COMMAND(ID_PRODUCT_DATA, &CMainFrame::OnProductData)
	//ON_COMMAND(ID_SYSTEM_INFO, &CMainFrame::OnSystemInfo)
	//ON_MESSAGE(WM_USER_OUPUT, &CMainFrame::OnUserOuput)
	ON_MESSAGE(WM_USER_OUTPUT, &CMainFrame::OnUserOutput)
	ON_MESSAGE(WM_USER_CUST_DYNAMIC_INFO, &CMainFrame::OnUserCustDynamicInfo)
	//ON_COMMAND(ID_MONEY_IN_OUT, &CMainFrame::OnMoneyInOut)
	ON_MESSAGE(WM_USER_CUST_BASE_INFO, &CMainFrame::OnUserCustBaseInfo)
	ON_MESSAGE(WM_USER_CUST_SEL_CHANGED, &CMainFrame::OnUserCustSelChanged)
	ON_MESSAGE(WM_USER_INST_MARKET, &CMainFrame::OnUserInstMarket)
	//ON_MESSAGE(WM_USER_REQUEST_INST_DONE, &CMainFrame::OnUserRequestInstDone)
	ON_COMMAND(ID_SHOW_ALL_CUST_POSITION, &CMainFrame::OnShowAllCustPosition)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	//theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_XP);
	m_bAutoMenuEnable = FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("δ�ܴ����˵���\n");
		return -1;      // δ�ܴ���
	}

	//m_wndMenuBar.GetMenu()->GetSubMenu(2)->GetMenuItemCount
	//if (g_cfg.oper_type == 1)
	//{
	//	CMenu *menu = CMenu::FromHandle(m_wndMenuBar.GetDefaultMenu());
	//	//menu = menu->GetSubMenu(0);
	//	menu = menu->GetSubMenu(2);
	//	//menu->Ena
	//	//m_wndMenuBar.get
	//	//m_wndMenuBar.GetMenuItem(2)->EnableWindow(FALSE);
	//	////menu->get
	//	//menu = menu->GetSubMenu(0);
	//	//menu = menu->GetSubMenu(2);
	//	for (int i = 0; i < menu->GetMenuItemCount(); i++) {
	//		menu->EnableMenuItem(i, MF_BYPOSITION | MF_GRAYED);
	//		CString str;
	//		menu->GetMenuString(i, str, MF_BYPOSITION);
	//		TRACE0(str);
	//	}
	//		
	//}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// ��ֹ�˵����ڼ���ʱ��ý���
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	//{
	//	TRACE0("δ�ܴ���������\n");
	//	return -1;      // δ�ܴ���
	//}

	//CString strToolBarName;
	//bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	//ASSERT(bNameValid);
	//m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	//m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// �����û�����Ĺ���������: 
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: �������ϣ���������Ͳ˵�����ͣ������ɾ��������
	//m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndMenuBar.EnableDocking(CBRS_ALIGN_TOP);
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	//DockPane(&m_wndToolBar);


	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	//EnableAutoHidePanes(CBRS_ALIGN_ANY);
	//GetDockingManager()->DisableRestoreDockState(TRUE);
	//GetDockingManager()->EnableDockSiteMenu(FALSE);
	// ���ز˵���ͼ��(�����κα�׼��������): 
	//CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, 0);
	//CMFCToolBar::AddToolBarForImageCollection(IDR_POSITION_MENU_IMAGES, 0);

	// ����ͣ������
	if (!CreateDockingWindows())
	{
		TRACE0("δ�ܴ���ͣ������\n");
		return -1;
	}

	//CRect rect(0, 0, 300, 900);
	DockPane(&m_wndCustInfo);
	DockPane(&m_wndOutput);
	m_wndCustInfo.DockToWindow(&m_wndOutput, CBRS_TOP);
	//m_wndCustInfo.DockToWindow(&m_wndOutput, CBRS_TOP, rect);
	//m_wndOutput.MoveWindow(CRect(0, 0, 300, 400));
	
	m_wndTabbe1.AddTab(&m_wndPosition, TRUE, TRUE, FALSE);
	m_wndTabbe1.AddTab(&m_wndOrder, TRUE, FALSE, FALSE);
	m_wndTabbe1.AddTab(&m_wndTradeQry, TRUE, FALSE, FALSE);

	m_wndTabbe2.AddTab(&m_wndMarket, TRUE, TRUE, FALSE);
	m_wndTabbe2.AddTab(&m_wndTrade, TRUE, FALSE, FALSE);
	

	DockPane(&m_wndTabbe1);
	DockPane(&m_wndTabbe2);
	m_wndTabbe1.DockToWindow(&m_wndTabbe2, CBRS_LEFT);

	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	// ���ù�������ͣ�����ڲ˵��滻
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);


	//// ���ÿ���(��ס Alt �϶�)�������Զ���
	//CMFCToolBar::EnableQuickCustomization();

	//if (CMFCToolBar::GetUserImages() == NULL)
	//{
	//	// �����û�����Ĺ�����ͼ��
	//	if (m_UserImages.Load(_T(".\\UserImages.bmp")))
	//	{
	//		CMFCToolBar::SetUserImages(&m_UserImages);
	//	}
	//}

	//// ���ò˵����Ի�(���ʹ�õ�����)
	//// TODO: �������Լ��Ļ������ȷ��ÿ�������˵�������һ���������
	//CList<UINT, UINT> lstBasicCommands;

	//lstBasicCommands.AddTail(ID_FILE_NEW);
	//lstBasicCommands.AddTail(ID_FILE_OPEN);
	//lstBasicCommands.AddTail(ID_FILE_SAVE);
	//lstBasicCommands.AddTail(ID_FILE_PRINT);
	//lstBasicCommands.AddTail(ID_APP_EXIT);
	//lstBasicCommands.AddTail(ID_EDIT_CUT);
	//lstBasicCommands.AddTail(ID_EDIT_PASTE);
	//lstBasicCommands.AddTail(ID_EDIT_UNDO);
	//lstBasicCommands.AddTail(ID_APP_ABOUT);
	//lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	//lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
	//lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	//lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	//lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	//lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	//CMFCToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	
	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	//BOOL bNameValid;

	//// ��������ͼ
	//CString strClassView;
	//bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	//ASSERT(bNameValid);
	//if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("δ�ܴ���������ͼ������\n");
	//	return FALSE; // δ�ܴ���
	//}

	//// �����ļ���ͼ
	//CString strFileView;
	//bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	//ASSERT(bNameValid);
	//if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("δ�ܴ������ļ���ͼ������\n");
	//	return FALSE; // δ�ܴ���
	//}
	DWORD ControlBarStyle = AFX_CBRS_CLOSE | AFX_CBRS_RESIZE;
	if (g_cfg.wnd_float)
		ControlBarStyle |= AFX_CBRS_FLOAT;
	CRect bottomRect(0, 0, 400, 900);
	DWORD DS = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_FLOAT_MULTI;
	if (!m_wndTabbe1.CreateEx(0, "", this, bottomRect, TRUE, 1000, DS | CBRS_BOTTOM, AFX_CBRS_REGULAR_TABS, ControlBarStyle))
	{
		TRACE0("δ�ܴ�����TABBE1������\n");
		return FALSE;
	}
	
	if (!m_wndTabbe2.CreateEx(0, "", this, bottomRect, TRUE, 1001, DS | CBRS_BOTTOM, AFX_CBRS_REGULAR_TABS, ControlBarStyle))
	{
		TRACE0("δ�ܴ�����TABBE2������\n");
		return FALSE;
	}
	
	if (!m_wndPosition.Create("�ֲ�", this, bottomRect, FALSE, ID_VIEW_POSITIONVIEW, DS | CBRS_BOTTOM, AFX_CBRS_REGULAR_TABS, ControlBarStyle))
	{
		TRACE0("δ�ܴ������ֲ֡�����\n");
		return FALSE;
	}
	
	if (!m_wndOrder.Create("ί��", this, bottomRect, FALSE, ID_VIEW_ORDERVIEW, DS | CBRS_BOTTOM, AFX_CBRS_REGULAR_TABS, ControlBarStyle))
	{
		TRACE0("δ�ܴ�����ί�С�����\n");
		return FALSE;
	}

	if (!m_wndTradeQry.Create("�ɽ�", this, bottomRect, FALSE, ID_VIEW_TRADEVIEW, DS | CBRS_BOTTOM, AFX_CBRS_REGULAR_TABS, ControlBarStyle))
	{
		TRACE0("δ�ܴ������˻���Ϣ������\n");
		return FALSE;
	}

	if (!m_wndTrade.Create("�������", this, bottomRect, TRUE, ID_VIEW_TRADE, DS | CBRS_BOTTOM, AFX_CBRS_REGULAR_TABS, ControlBarStyle))
	{
		TRACE0("δ�ܴ�����������塱����\n");
		return FALSE;
	}

	if (!m_wndMarket.Create("��Լ����", this, bottomRect, TRUE, ID_VIEW_MARKET, DS | CBRS_BOTTOM, AFX_CBRS_REGULAR_TABS, ControlBarStyle))
	{
		TRACE0("δ�ܴ�������Լ���顱����\n");
		return FALSE;
	}

	if (!m_wndCustInfo.Create("�˻���Ϣ", this, CRect(0, 0, 300, 1000), TRUE, ID_VIEW_PROPERTIESWND, DS | CBRS_RIGHT, AFX_CBRS_REGULAR_TABS, ControlBarStyle))
	{
		TRACE0("δ�ܴ������˻���Ϣ������\n");
		return FALSE;
	}
	if (!m_wndOutput.Create("���", this, CRect(0, 0, 300, 200), TRUE, ID_VIEW_OUTPUTWND, DS | CBRS_RIGHT, AFX_CBRS_REGULAR_TABS, ControlBarStyle))
	{
		TRACE0("δ�ܴ����������\n");
		return FALSE; // δ�ܴ���
	}

	// �����������
	//CString strOutputWnd;
	//bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	//ASSERT(bNameValid);
	

	//// �������Դ���
	//CString strPropertiesWnd;
	//bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	//ASSERT(bNameValid);
	//if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("δ�ܴ��������ԡ�����\n");
	//	return FALSE; // δ�ܴ���
	//}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	//SetTitle(CString("SManager-") + g_cfg.oper_code);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	//HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	//HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	//HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	//HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

}

//void CMainFrame::calcCustFloatProfit(const std::string & inst)
//{
//	struct vol_profit
//	{
//		vol_profit()
//			: volume(0)
//			, profit(0.0){}
//		int volume;
//		double profit;
//	};
//
//	depth_market_data_st dmd = MdApi_GetMarketData(inst);
//	std::map<std::string, std::vector<cust_td_position_detail_st>>::iterator mit;
//	std::vector<cust_td_position_detail_st>::iterator vpdit;
//	for (mit = m_mCustPosiDetail.begin(); mit != m_mCustPosiDetail.end(); ++mit)
//	{
//		std::map<cust_position_calc_id, vol_profit> mPosiProfit;
//		for (vpdit = mit->second.begin(); vpdit != mit->second.end(); ++vpdit)
//		{
//			
//			if (vpdit->instrumentid == inst)
//			{
//				cust_position_calc_id calcid = { vpdit->cust_no , vpdit->instrumentid , vpdit->direction };
//				//strcpy(calcid.cust_no, vpdit->cust_no);
//				//strcpy(calcid.instrumentid, vpdit->instrumentid);
//				//strcpy(calcid.direction, vpdit->direction);
//				mPosiProfit[calcid].profit += calcProfit(*vpdit, dmd);
//				mPosiProfit[calcid].volume += vpdit->volume;
//			}
//		}
//
//		for (auto calc_it = mPosiProfit.begin(); calc_it != mPosiProfit.end(); ++calc_it)
//		{
//			cust_position_id_st posi_id = { calc_it->first.cust_no, 
//				calc_it->first.instrumentid,
//				calc_it->first.direction,
//				calc_it->second.volume };
//			std::ostringstream oss;
//			oss << posi_id;
//			
//
//			CViewHint hint;
//			hint.type = get_identifer(oss.str());
//			hint.identifer = int(calc_it->second.profit * 100);
//			::PostMessage(m_wndPosition.GetSafeHwnd(),
//				WM_USER_CUST_POSITION_FLOAT_PROFIT,
//				hint.type,
//				hint.identifer);
//			 
//			GetActiveDocument()->UpdateAllViews(NULL, WM_USER_CUST_POSITION_FLOAT_PROFIT, &hint);
//
//			if (m_ShowAllCustPosiDlg.GetSafeHwnd())
//			{
//				m_ShowAllCustPosiDlg.UpdateCustPositionProfit(hint.type, hint.identifer);
//			}
//		}
//	}
//}

//void CMainFrame::RequestSystemInfo()
//{
//	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
//		BCGetStringFieldByName(handle, 0, "sdate0", m_SysInfo.system_date, sizeof(m_SysInfo.system_time));
//		BCGetStringFieldByName(handle, 0, "sdate2", m_SysInfo.check_date, sizeof(m_SysInfo.check_date));
//		BCGetStringFieldByName(handle, 0, "sdate1", m_SysInfo.next_tradingday, sizeof(m_SysInfo.next_tradingday));
//		BCGetStringFieldByName(handle, 0, "sdate3", m_SysInfo.host_date, sizeof(m_SysInfo.host_date));
//		BCGetStringFieldByName(handle, 0, "sstatus0", m_SysInfo.system_status, sizeof(m_SysInfo.system_status));
//		BCGetStringFieldByName(handle, 0, "stime0", m_SysInfo.system_time, sizeof(m_SysInfo.system_time));
//	};
//	//system_info_st sysinfo;
//	//BCHANDLE handle = BCNewHandle(XpackDescribleFile);
//	Scoped_BCHANDLE handle;
//	BCResetHandle(handle);
//	BCSetRequestType(handle, 100319);
//	BCSetStringFieldByName(handle, 0, "sbranch_code0", "000");
//	MyBCRequest(handle, fetcher);
//	//MyBCCallRequest(handle);
//	//BCGetStringFieldByName(handle, 0, "sdate0", m_SysInfo.system_date, sizeof(m_SysInfo.system_time));
//	//BCGetStringFieldByName(handle, 0, "sdate2", m_SysInfo.check_date, sizeof(m_SysInfo.check_date));
//	//BCGetStringFieldByName(handle, 0, "sdate1", m_SysInfo.next_tradingday, sizeof(m_SysInfo.next_tradingday));
//	//BCGetStringFieldByName(handle, 0, "sdate3", m_SysInfo.host_date, sizeof(m_SysInfo.host_date));
//	//BCGetStringFieldByName(handle, 0, "sstatus0", m_SysInfo.system_status, sizeof(m_SysInfo.system_status));
//	//BCGetStringFieldByName(handle, 0, "stime0", m_SysInfo.system_time, sizeof(m_SysInfo.system_time));
//	//BCDeleteHandle(handle);
//
//	//m_BaseInfo.sysinfo = sysinfo;
//}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

//void CMainFrame::OnViewCustomize()
//{
//	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ɨ��˵�*/);
//	pDlgCust->EnableUserDefinedToolbars();
//	pDlgCust->Create();
//}
//
//LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
//{
//	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
//	if (lres == 0)
//	{
//		return 0;
//	}
//
//	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
//	ASSERT_VALID(pUserToolbar);
//
//	BOOL bNameValid;
//	CString strCustomize;
//	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
//	ASSERT(bNameValid);
//
//	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
//	return lres;
//}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	//m_wndOutput.UpdateFonts();
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// ���ཫִ�������Ĺ���

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// Ϊ�����û������������Զ��尴ť
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	//m_wndOutput.UpdateFonts();
}


//void CMainFrame::OnProductData()
//{
//	// TODO: �ڴ���������������
//	CProductDataDialog dlg;
//	dlg.DoModal();
//}


//void CMainFrame::OnSystemInfo()
//{
//	// TODO: �ڴ���������������
//	RequestSystemInfo();
//	CSysInfoDialog dlg(&m_SysInfo);
//	dlg.DoModal();
//}


//afx_msg LRESULT CMainFrame::OnUserOuput(WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}


afx_msg LRESULT CMainFrame::OnUserOutput(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(m_wndOutput.GetSafeHwnd(), WM_USER_OUTPUT, 0, 0);
	return 0;
}


afx_msg LRESULT CMainFrame::OnUserCustDynamicInfo(WPARAM wParam, LPARAM lParam)
{
	CViewHint hint;
	hint.type = wParam;
	hint.identifer = lParam;
	GetActiveDocument()->UpdateAllViews(NULL, WM_USER_CUST_DYNAMIC_INFO, &hint);
	
	if (wParam == CUST_DYNAMIC_POSITION_CHANGED || wParam == CUST_DYNAMIC_POSI_DETAIL_CHANGED) {
		::PostMessage(m_wndPosition.GetSafeHwnd(), WM_USER_CUST_DYNAMIC_INFO, wParam, lParam);
		::PostMessage(m_wndMarket.GetSafeHwnd(), WM_USER_CUST_DYNAMIC_INFO, wParam, lParam);
		::PostMessage(m_wndCustInfo.GetSafeHwnd(), WM_USER_CUST_DYNAMIC_INFO, wParam, lParam);
		//if (wParam == CUST_DYNAMIC_POSI_DETAIL_CHANGED) 
		//{
		//	std::string cust_no = fetch_string(lParam);
		//	m_mCustPosiDetail[cust_no] = cust_dynamic_info_worker::instance()->get_cust_posi_detail(cust_no);
		//}
		if (wParam == CUST_DYNAMIC_POSITION_CHANGED && m_ShowAllCustPosiDlg.GetSafeHwnd())
		{
			m_ShowAllCustPosiDlg.ShowPosition();
		}
			
	}
	else if (wParam == CUST_DYNAMIC_ORDER_CHANGED) {
		::PostMessage(m_wndOrder.GetSafeHwnd(), WM_USER_CUST_DYNAMIC_INFO, wParam, lParam);
		::PostMessage(m_wndMarket.GetSafeHwnd(), WM_USER_CUST_DYNAMIC_INFO, wParam, lParam);
	}
	else if (wParam == CUST_DYNAMIC_TRADE_CHANGED || wParam == CUST_DYNAMIC_TRADE_DETAIL_CHANGED) {
		::PostMessage(m_wndTradeQry.GetSafeHwnd(), WM_USER_CUST_DYNAMIC_INFO, wParam, lParam);
		::PostMessage(m_wndMarket.GetSafeHwnd(), WM_USER_CUST_DYNAMIC_INFO, wParam, lParam);
	}

	
	return 0;
}


//void CMainFrame::OnMoneyInOut()
//{
//	// TODO: �ڴ���������������
//	theApp.WaitCustInfoReady();
//	CCustMoneyInOutDialog dlg(m_SysInfo);
//	dlg.DoModal();
//}


afx_msg LRESULT CMainFrame::OnUserCustBaseInfo(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(m_wndCustInfo.GetSafeHwnd(), WM_USER_CUST_BASE_INFO, wParam, lParam);
	CViewHint hint;
	hint.type = wParam;
	hint.identifer = lParam;
	GetActiveDocument()->UpdateAllViews(NULL, WM_USER_CUST_BASE_INFO, &hint);
	return 0;
}


afx_msg LRESULT CMainFrame::OnUserCustSelChanged(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(m_wndPosition.GetSafeHwnd(), WM_USER_CUST_SEL_CHANGED, wParam, lParam);
	::PostMessage(m_wndOrder.GetSafeHwnd(), WM_USER_CUST_SEL_CHANGED, wParam, lParam);
	::PostMessage(m_wndTradeQry.GetSafeHwnd(), WM_USER_CUST_SEL_CHANGED, wParam, lParam);
	::PostMessage(m_wndTrade.GetSafeHwnd(), WM_USER_CUST_SEL_CHANGED, wParam, lParam);
	::PostMessage(m_wndCustInfo.GetSafeHwnd(), WM_USER_CUST_SEL_CHANGED, wParam, lParam);
	return 0;
}


afx_msg LRESULT CMainFrame::OnUserInstMarket(WPARAM wParam, LPARAM lParam)
{
	::PostMessage(m_wndMarket.GetSafeHwnd(), WM_USER_INST_MARKET, wParam, lParam);
	::PostMessage(m_wndPosition.GetSafeHwnd(), WM_USER_INST_MARKET, wParam, lParam);
	
	std::string inst = fetch_string(wParam);
	CViewHint hint;
	//hint.type = wParam;
	hint.identifer = wParam;
	GetActiveDocument()->UpdateAllViews(NULL, WM_USER_INST_MARKET, &hint);

	if (m_ShowAllCustPosiDlg.GetSafeHwnd())
		m_ShowAllCustPosiDlg.UpdateInstMarket(inst);

	return 0;
}


//afx_msg LRESULT CMainFrame::OnUserRequestInstDone(WPARAM wParam, LPARAM lParam)
//{
//	::PostMessage(m_wndTrade.GetSafeHwnd(), WM_USER_REQUEST_INST_DONE, wParam, lParam);
//	return 0;
//}


void CMainFrame::OnShowAllCustPosition()
{
	// TODO: �ڴ���������������
	m_ShowAllCustPosiDlg.DoModal();
}
