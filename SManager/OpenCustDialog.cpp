// OpenCustDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "OpenCustDialog.h"
#include "afxdialogex.h"

#include <sstream>
// COpenCustDialog �Ի���

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


// COpenCustDialog ��Ϣ�������

///* ���룺����Ա			input:scust_no
//�ͻ���			sholder_ac_no
//�ͻ�����			sname
//�ͻ�ȫ��			sall_name
//֤������			scust_type2
//֤������			scert_no
//�ʱ�			spost_code
//��ַ			saddr
//�绰			sphone
//�ֻ�			sphone3
//����			sphone2
//�����ʼ�			semail
//����			scust_auth2
//��ͬ���			sbankname2
//��ע			snote
//��������			stx_pwd
//�ʽ�����			swithdraw_pwd
//�ͻ���			sserial1
//�����ʽ���			sstatus2
//�ͻ�����			sstatus1
//ָ���´���1			scust_auth
//ָ���´������֤��1			scust_limit
//ָ���´���2			spager
//ָ���´������֤��2			scard0
//ָ���´���3			sstation1
//ָ���´������֤��3			scard1
//�ʽ������1			scert_no2
//�ʽ���������֤��1			sbank_acc
//�ʽ������2			scert_addr
//�ʽ���������֤��2			sbank_acc2
//����֤��			scust_limit2
//��֯��������			sname2
//������������			sbankname
//ί������ */			semp_pwd

bool COpenCustDialog::CallOpenCust_851000(BCHANDLE handle)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 851000);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", m_CustNo.GetBuffer());
	BCSetStringFieldByName(handle, 0, "sname", m_CustName.GetBuffer());
	BCSetStringFieldByName(handle, 0, "sall_name", m_CustName.GetBuffer());
	BCSetStringFieldByName(handle, 0, "sstatus1", "1"); //�ͻ����� ��Ȼ��
	BCSetStringFieldByName(handle, 0, "scust_type2", "9"); //֤������ ����֤��
	BCSetStringFieldByName(handle, 0, "scert_no", m_CustName.GetBuffer());//֤������
	BCSetStringFieldByName(handle, 0, "stx_pwd", m_TradePasswd.GetBuffer()); //��������
	BCSetStringFieldByName(handle, 0, "swithdraw_pwd", m_MoneyPasswd.GetBuffer()); //�ʽ�����

	char cust_class[13] = { 0 };
	memset(cust_class, '_', sizeof(cust_class) - 1);
	strcpy(cust_class, m_ctlCustClass.GetCurSelKey());
	BCSetStringFieldByName(handle, 0, "sserial1", cust_class); //�ͻ���
	
	return MyBCRequest(handle, FetchRowFunc_t(), BCMSG_ERR_TOALL);
}

bool COpenCustDialog::SetCustCommissionTemplate_851342(BCHANDLE handle, char *template_no)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 851342);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code); //����Ա
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", m_CustNo.GetBuffer()); //�ͻ���
	BCSetStringFieldByName(handle, 0, "sstatus0", "1"); //������־������
	BCSetStringFieldByName(handle, 0, "sstation0", template_no); //��������ģ��
	BCSetStringFieldByName(handle, 0, "sstatus1", "0"); //�������ʶ������� ��
	BCSetStringFieldByName(handle, 0, "sstatus2", "0"); //��������ǿ���滻��־ ��
	return  MyBCRequest(handle, FetchRowFunc_t(), BCMSG_ERR_TOALL);
}

bool COpenCustDialog::SetCustMarginTemplate_851348(BCHANDLE handle, char *template_no)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 851348);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code); //����Ա
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", m_CustNo.GetBuffer()); //�ͻ���
	BCSetStringFieldByName(handle, 0, "sstatus0", "1"); //������־������
	BCSetStringFieldByName(handle, 0, "sstation0", template_no); //��������ģ��
	BCSetStringFieldByName(handle, 0, "sstatus1", "0"); //�������ʶ������� ��
	BCSetStringFieldByName(handle, 0, "sstatus2", "0"); //��������ǿ���滻��־ ��
	return  MyBCRequest(handle, FetchRowFunc_t(), BCMSG_ERR_TOALL);
}

bool COpenCustDialog::SetCustTradeCode_851010(BCHANDLE handle)
{
	for each (seat_st s in theApp.m_BaseInfo.seats)
	{
		BCResetHandle(handle);
		BCSetRequestType(handle, 851010);
		BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code); //����Ա
		BCSetStringFieldByName(handle, 0, "sstatus0", "4"); //������־������ //����������
		BCSetStringFieldByName(handle, 0, "sholder_ac_no", m_CustNo.GetBuffer()); //�ͻ���
		BCSetStringFieldByName(handle, 0, "sserial2", s.seat_no); //ϯλ��
		BCSetStringFieldByName(handle, 0, "sholder_ac_no2", (CString("SM") + m_CustNo).GetBuffer());//���ױ���
		BCSetStringFieldByName(handle, 0, "sstatus1", "1");//���ױ�����Դ
		if(strcmp(s.seat_no, "cffex") == 0)
			BCSetStringFieldByName(handle, 0, "sstatus2", "3");//���ױ������� Ͷ��

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
	//BCSetStringFieldByName(handle, 0, "usset0", "");//Ȩ���б�
	BCSetStringFieldByName(handle, 0, "sbank_acc", "1100011000111001100111100001100000000000");//�˵��б�
	//BCSetStringFieldByName(handle, 0, "sbank_code", "");//��Ȩ��ģ�����
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//function 851000 description  "�ͻ�����"  reqlevel 1
//����Ա				scust_no
//�ͻ���				sholder_ac_no
//�ͻ�����				sname
//�ͻ�ȫ��				sall_name
//֤������				scust_type2
//֤������				scert_no
//�ʱ�				spost_code
//��ַ				saddr
//�绰				sphone
//�ֻ�				sphone3
//����				sphone2
//�����ʼ�				semail
//����				scust_auth2
//��ͬ���				sbankname2
//��ע				snote
//��������				stx_pwd
//�ʽ�����				swithdraw_pwd
//�ͻ���				sserial1
//�����ʽ���				sstatus2
//�ͻ�����				sstatus1
//ָ���´���1				scust_auth
//ָ���´������֤��1				scust_limit
//ָ���´���2				spager
//ָ���´������֤��2				scard0
//ָ���´���3				sstation1
//ָ���´������֤��3				scard1
//�ʽ������1				scert_no2
//�ʽ���������֤��1				sbank_acc
//�ʽ������2				scert_addr
//�ʽ���������֤��2				sbank_acc2
//����֤��				scust_limit2
//��֯��������				sname2
//������������				sbankname
//ί������				semp_pwd


void COpenCustDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(theApp.m_CustNum > 0 && theApp.GetCustnoList().size() >= theApp.m_CustNum)
	{
		MessageBox("����Ա���������Ѵ����ޣ����ܿ���");
		return;
	}

	Scoped_BCHANDLE handle;
	if (!CallOpenCust_851000(handle))
		return;

	std::ostringstream oss;
	oss << "�ͻ������ɹ�";

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
			oss << "��������������ģ��ʧ��";


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
			oss << ",���ñ�֤����ģ��ʧ��";

	}

	if (!SetCustTradeCode_851010(handle))
		oss << ",���ÿͻ����ױ���ʧ��";

	if (!SetCustAuthority_851012(handle))
		oss << ",���ÿͻ�Ȩ��ʧ��";

	if (!SetCustMultiLogin_851278(handle))
		oss << ",���ÿͻ�����¼ʧ��";

	oss << ".";
	theApp.AddCust(cbi);
	MessageBox(oss.str().c_str());

	//CDialogEx::OnOK();
}
