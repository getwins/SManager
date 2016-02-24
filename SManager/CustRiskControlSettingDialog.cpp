// CustRiskControlSettingDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "CustRiskControlSettingDialog.h"
#include "afxdialogex.h"
#include "struct_helper.h"
#include <algorithm>
#include "Format.h"
#include "CommBCRequest.h"
#include <sstream>

// CCustRiskControlSettingDialog �Ի���

IMPLEMENT_DYNAMIC(CCustRiskControlSettingDialog, CDialogEx)

CCustRiskControlSettingDialog::CCustRiskControlSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CUST_RISK_CONTROL_SETTING, pParent)
	, m_bClear(FALSE)
	, m_bForce(FALSE)
	//, m_bAllow(FALSE)
{

}

CCustRiskControlSettingDialog::~CCustRiskControlSettingDialog()
{
}

void CCustRiskControlSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CUST, m_ctlCust);
	//  DDX_Control(pDX, IDC_RADIO_ALLOW, m_btnAllow);
	DDX_Control(pDX, IDC_EDIT_CAPTIAL, m_ctlCapital);
	DDX_Check(pDX, IDC_CHECK_CLEAR, m_bClear);
	DDX_Check(pDX, IDC_CHECK_FORCE, m_bForce);
	//  DDX_Radio(pDX, IDC_RADIO_ALLOW, m_bAllow);
}


BEGIN_MESSAGE_MAP(CCustRiskControlSettingDialog, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_CLEAR, &CCustRiskControlSettingDialog::OnBnClickedCheckNight)
	ON_BN_CLICKED(IDC_CHECK_FORCE, &CCustRiskControlSettingDialog::OnBnClickedCheckForce)
	ON_CBN_SELCHANGE(IDC_COMBO_CUST, &CCustRiskControlSettingDialog::OnCbnSelchangeComboCust)
	ON_BN_CLICKED(ID_SET, &CCustRiskControlSettingDialog::OnBnClickedSet)
	ON_BN_CLICKED(ID_DELETE, &CCustRiskControlSettingDialog::OnBnClickedDelete)
END_MESSAGE_MAP()


// CCustRiskControlSettingDialog ��Ϣ�������


void CCustRiskControlSettingDialog::OnBnClickedCheckNight()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_bClear)
	{
		GetDlgItem(IDC_RADIO_ALLOW)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_FORBIDDEN)->EnableWindow(TRUE);
	}
		//m_btnAllow.EnableWindow(TRUE);
	else
	{
		GetDlgItem(IDC_RADIO_ALLOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_FORBIDDEN)->EnableWindow(FALSE);
	}
		//m_btnAllow.EnableWindow(FALSE);
}


void CCustRiskControlSettingDialog::OnBnClickedCheckForce()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_bForce)
		m_ctlCapital.EnableWindow(TRUE);
	else
		m_ctlCapital.EnableWindow(FALSE);
}


void CCustRiskControlSettingDialog::OnCbnSelchangeComboCust()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString cust_no = m_ctlCust.GetCurSelKey();
	if (cust_no.GetLength() == 0)
		return;
	cust_base_info_st cbi = theApp.GetCustBaseInfo(cust_no);
	if (cust_no != cbi.basic.scust_no)
		return;
	std::vector<rc_desc_st>::iterator it;
	it = find_if(cbi.rcs.begin(), cbi.rcs.end(),
		[](rc_desc_st &rc)->bool { return strcmp(rc.classflag, "0001") == 0; });
	if (it != cbi.rcs.end())
	{
		int p = atoi(it->paramlist);
		if (p == 1)
		{
			((CButton*)GetDlgItem(IDC_RADIO_ALLOW))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_RADIO_FORBIDDEN))->SetCheck(FALSE);
			//m_btnAllow.SetCheck(TRUE);
		}	
		else
		{
			((CButton*)GetDlgItem(IDC_RADIO_ALLOW))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_RADIO_FORBIDDEN))->SetCheck(TRUE);
		}
			//m_btnAllow.SetCheck(FALSE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_ALLOW))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_FORBIDDEN))->SetCheck(FALSE);
	}

	it = find_if(cbi.rcs.begin(), cbi.rcs.end(),
		[](rc_desc_st &rc)->bool { return strcmp(rc.classflag, "0002") == 0; });
	if (it != cbi.rcs.end())
	{
		//double p = atof(it->paramlist);
		//m_ctlCapital = p;
		//m_ctlCapital.SetWindowText(fundfmt(p));
		m_ctlCapital.SetWindowText(it->paramlist);
	}
	else
	{
		m_ctlCapital.SetWindowText("������");
	}
}


void CCustRiskControlSettingDialog::OnBnClickedSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString cust_no = m_ctlCust.GetCurSelKey();
	if (cust_no.GetLength() == 0)
		return;
	BCResult result;
	std::ostringstream oss;
	bool bchange = false;
	Scoped_BCHANDLE handle;
	std::vector<rc_desc_st>::iterator it;
	cust_base_info_st cbi = theApp.GetCustBaseInfo(cust_no);
	
	if (m_bClear)
	{
		it = find_if(cbi.rcs.begin(), cbi.rcs.end(),
			[](rc_desc_st &rc)->bool { return strcmp(rc.classflag, "0001") == 0; });
		char *paramlist = (((CButton*)GetDlgItem(IDC_RADIO_ALLOW))->GetCheck() == TRUE ? "1" : "0");
		if (it == cbi.rcs.end())
		{		
			result = BCRequestManageCustRC_860001(handle, "1", cust_no.GetBuffer(), "0001", "0000", paramlist);
			if (result)
			{
				rc_desc_st rc = { 0 };
				strcpy(rc.cust_no, cust_no.GetBuffer());
				strcpy(rc.classflag, "0001");
				strcpy(rc.type, "0000");
				strcpy(rc.paramlist, paramlist);
				cbi.rcs.push_back(rc);
				bchange = true;
				//theApp.SetCustBaseInfo(cbi);
				oss << "��ӿͻ�������ֹ���ɹ�.";
			}
			else
			{
				oss << "��ӿͻ�������ֹ���ʧ��," << result << ".";
			}
		}
		else
		{
			result = BCRequestManageCustRC_860001(handle, "2", cust_no.GetBuffer(), "0001", "0000", paramlist);
			if (result)
			{
				strcpy(it->paramlist , paramlist);
				//theApp.SetCustBaseInfo(cbi);
				bchange = true;
				oss << "�޸Ŀͻ�������ֹ���ɹ�.";
			}
			else
			{
				oss << "�޸Ŀͻ�������ֹ���ɹ�," << result;
			}
		}
	}

	if (m_bForce)
	{
		it = find_if(cbi.rcs.begin(), cbi.rcs.end(),
			[](rc_desc_st &rc)->bool { return strcmp(rc.classflag, "0002") == 0; });
		CString str;
		m_ctlCapital.GetWindowText(str);
		if (it == cbi.rcs.end())
		{
			result = BCRequestManageCustRC_860001(handle, "1", cust_no.GetBuffer(), "0002", "0000", str.GetBuffer());
			if (result)
			{
				rc_desc_st rc = { 0 };
				strcpy(rc.cust_no, cust_no.GetBuffer());
				strcpy(rc.classflag, "0002");
				strcpy(rc.type, "0000");
				strcpy(rc.paramlist, str.GetBuffer());
				cbi.rcs.push_back(rc);
				bchange = true;
				//theApp.SetCustBaseInfo(cbi);
				oss << "��ӿͻ�Ȩ��С��ǿƽ����ɹ�.";
			}
			else
			{
				oss << "��ӿͻ�Ȩ��С��ǿƽ����ʧ�ܣ�" << result << ".";
			}
		}
		else
		{
			result = BCRequestManageCustRC_860001(handle, "2", cust_no.GetBuffer(), "0002", "0000", str.GetBuffer());
			if(result)
			{
				memset(it->paramlist, 0, sizeof(it->paramlist));
				strcpy(it->paramlist, str.GetBuffer());
				bchange = true;
				oss << "�޸Ŀͻ�Ȩ��С��ǿƽ����ɹ�.";
			}
			else
			{
				oss << "�޸Ŀͻ�Ȩ��С��ǿƽ����ʧ�ܣ�" << result << ".";
			}
		}
	}

	if (bchange)
	{
		theApp.SetCustBaseInfo(cbi);
	}

	if (!oss.str().empty())
	{
		MessageBox(oss.str().c_str());
		PostOutputMsg(oss.str());
	}
}


void CCustRiskControlSettingDialog::OnBnClickedDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString cust_no = m_ctlCust.GetCurSelKey();
	if (cust_no.GetLength() == 0)
		return;
	BCResult result;
	std::ostringstream oss;
	bool bchange = false;
	Scoped_BCHANDLE handle;
	std::vector<rc_desc_st>::iterator it;
	cust_base_info_st cbi = theApp.GetCustBaseInfo(cust_no);

	if (m_bClear)
	{
		it = find_if(cbi.rcs.begin(), cbi.rcs.end(),
			[](rc_desc_st &rc)->bool { return strcmp(rc.classflag, "0001") == 0; });
		if (it == cbi.rcs.end())
		{
			oss << "�ͻ�������ֹ��򲻴���.";
		}
		else
		{
			result = BCRequestManageCustRC_860001(handle, "3", it->cust_no, "0001", "0000", "");
			if (result)
			{
				oss << "�ͻ�������ֹ���ɾ���ɹ�.";
				cbi.rcs.erase(it);
				bchange = true;
			}
			else
			{
				oss << "�ͻ�������ֹ���ɾ��ʧ��," << result << ".";
			}
		}
	}

	if (m_bForce)
	{
		it = find_if(cbi.rcs.begin(), cbi.rcs.end(),
			[](rc_desc_st &rc)->bool { return strcmp(rc.classflag, "0002") == 0; });
		if (it == cbi.rcs.end())
		{
			oss << "�ͻ�Ȩ��С��ǿƽ���򲻴���.";
		}
		else
		{
			result = BCRequestManageCustRC_860001(handle, "3", it->cust_no, "0002", "0000", "");
			if (result)
			{
				oss << "�ͻ�Ȩ��С��ǿƽɾ���ɹ�.";
				cbi.rcs.erase(it);
				bchange = true;
			}
			else
			{
				oss << "�ͻ�Ȩ��С��ǿƽɾ��ʧ�ܣ�" << result << ".";
			}
		}
	}

	if (bchange)
	{
		theApp.SetCustBaseInfo(cbi);
	}

	if (!oss.str().empty())
	{
		MessageBox(oss.str().c_str());
		PostOutputMsg(oss.str());
	}
}


BOOL CCustRiskControlSettingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	OnCbnSelchangeComboCust();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
