// DeleteCacheDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SManager.h"
#include "DeleteCacheDialog.h"
#include "afxdialogex.h"

#include <io.h>
// CDeleteCacheDialog �Ի���

IMPLEMENT_DYNAMIC(CDeleteCacheDialog, CDialogEx)

CDeleteCacheDialog::CDeleteCacheDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEL_CACHE_FILE, pParent)
{

}

CDeleteCacheDialog::~CDeleteCacheDialog()
{
}

void CDeleteCacheDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OUPUT, m_ctlOutput);
}


BEGIN_MESSAGE_MAP(CDeleteCacheDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDeleteCacheDialog::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CDeleteCacheDialog ��Ϣ�������


void CDeleteCacheDialog::OnBnClickedButtonDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//if(::remove())
	if (::access(cache_file_dict, 00) == 0) {
		if (::remove(cache_file_dict)) {
			m_ctlOutput.AddString("ɾ���ֵ仺���ļ�ʧ��");
			return;
		}
		else {
			m_ctlOutput.AddString("ɾ���ֵ仺���ļ��ɹ�");
		}

	}
	else {
		m_ctlOutput.AddString("�ֵ仺���ļ�������");
	}

	if (::access(cache_file_exchange, 00) == 0) {
		if (::remove(cache_file_exchange)) {
			m_ctlOutput.AddString("ɾ�������������ļ�ʧ��");
			return;
		}
		else {
			m_ctlOutput.AddString("ɾ�������������ļ��ɹ�");
		}

	}
	else {
		m_ctlOutput.AddString("�����������ļ�������");
	}

	if (::access(cache_file_seat, 00) == 0) {
		if (::remove(cache_file_seat)) {
			m_ctlOutput.AddString("ɾ��ϯλ�����ļ�ʧ��");
			return;
		}
		else {
			m_ctlOutput.AddString("ɾ��ϯλ�����ļ��ɹ�");
		}

	}
	else {
		m_ctlOutput.AddString("ϯλ�����ļ�������");
	}

	if (::access(cache_file_product, 00) == 0) {
		if (::remove(cache_file_product)) {
			m_ctlOutput.AddString("ɾ��Ʒ�ֻ����ļ�ʧ��");
			return;
		}
		else {
			m_ctlOutput.AddString("ɾ��Ʒ�ֻ����ļ��ɹ�");
		}

	}
	else {
		m_ctlOutput.AddString("ϯλƷ���ļ�������");
	}

	if (::access(cache_file_exchange_product, 00) == 0) {
		if (::remove(cache_file_exchange_product)) {
			m_ctlOutput.AddString("ɾ��������Ʒ�ֶ�Ӧ��ϵ�����ļ�ʧ��");
			return;
		}
		else {
			m_ctlOutput.AddString("ɾ��������Ʒ�ֶ�Ӧ��ϵ�����ļ��ɹ�");
		}

	}
	else {
		m_ctlOutput.AddString("ϯλ������Ʒ�ֶ�Ӧ��ϵ�ļ�������");
	}

	m_ctlOutput.AddString("�����ļ�����ɹ�");
	m_ctlOutput.AddString("������Ӧ��");
}
