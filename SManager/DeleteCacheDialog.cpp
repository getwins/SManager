// DeleteCacheDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "DeleteCacheDialog.h"
#include "afxdialogex.h"

#include <io.h>
// CDeleteCacheDialog 对话框

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


// CDeleteCacheDialog 消息处理程序


void CDeleteCacheDialog::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	//if(::remove())
	if (::access(cache_file_dict, 00) == 0) {
		if (::remove(cache_file_dict)) {
			m_ctlOutput.AddString("删除字典缓存文件失败");
			return;
		}
		else {
			m_ctlOutput.AddString("删除字典缓存文件成功");
		}

	}
	else {
		m_ctlOutput.AddString("字典缓存文件不存在");
	}

	if (::access(cache_file_exchange, 00) == 0) {
		if (::remove(cache_file_exchange)) {
			m_ctlOutput.AddString("删除交易所缓存文件失败");
			return;
		}
		else {
			m_ctlOutput.AddString("删除交易所缓存文件成功");
		}

	}
	else {
		m_ctlOutput.AddString("交易所缓存文件不存在");
	}

	if (::access(cache_file_seat, 00) == 0) {
		if (::remove(cache_file_seat)) {
			m_ctlOutput.AddString("删除席位缓存文件失败");
			return;
		}
		else {
			m_ctlOutput.AddString("删除席位缓存文件成功");
		}

	}
	else {
		m_ctlOutput.AddString("席位缓存文件不存在");
	}

	if (::access(cache_file_product, 00) == 0) {
		if (::remove(cache_file_product)) {
			m_ctlOutput.AddString("删除品种缓存文件失败");
			return;
		}
		else {
			m_ctlOutput.AddString("删除品种缓存文件成功");
		}

	}
	else {
		m_ctlOutput.AddString("席位品种文件不存在");
	}

	if (::access(cache_file_exchange_product, 00) == 0) {
		if (::remove(cache_file_exchange_product)) {
			m_ctlOutput.AddString("删除交易所品种对应关系缓存文件失败");
			return;
		}
		else {
			m_ctlOutput.AddString("删除交易所品种对应关系缓存文件成功");
		}

	}
	else {
		m_ctlOutput.AddString("席位交易所品种对应关系文件不存在");
	}

	m_ctlOutput.AddString("缓存文件清除成功");
	m_ctlOutput.AddString("请重启应用");
}
