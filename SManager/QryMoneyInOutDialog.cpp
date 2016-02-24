// QryMoneyInOutDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "QryMoneyInOutDialog.h"
#include "afxdialogex.h"


// CQryMoneyInOutDialog 对话框

IMPLEMENT_DYNAMIC(CQryMoneyInOutDialog, CDialogEx)

CQryMoneyInOutDialog::CQryMoneyInOutDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MONEY_QUERY, pParent)
	, m_DateEnd(COleDateTime::GetCurrentTime())
	, m_DateBegin(COleDateTime::GetCurrentTime())
{

}

CQryMoneyInOutDialog::~CQryMoneyInOutDialog()
{
}

void CQryMoneyInOutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ctlCust);
	//  DDX_Control(pDX, IDC_DATETIMEPICKER_BEGIN, m_DateBegin);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END, m_DateEnd);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_BEGIN, m_DateBegin);
	DDX_Control(pDX, IDC_LIST_RECORD, m_listctrl);
}


BEGIN_MESSAGE_MAP(CQryMoneyInOutDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CQryMoneyInOutDialog::OnBnClickedButtonQuery)
END_MESSAGE_MAP()


// CQryMoneyInOutDialog 消息处理程序


void CQryMoneyInOutDialog::OnBnClickedButtonQuery()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (RequestMoneyInOut_851403()) {
		m_listctrl.DeleteAllItems();
		CString str;
		for (int i = 0; i < m_vMoney.size(); i++) {
			cust_money_in_out_st &m = m_vMoney[i];
			m_listctrl.InsertItem(i, m.cust_no);
			m_listctrl.SetItemText(i, 1, m.date);
			m_listctrl.SetItemText(i, 2, m.time);
			str.Format("%d", m.serial);
			m_listctrl.SetItemText(i, 3, str);
			m_listctrl.SetItemText(i, 4, theApp.DictToStr(DICT_CLASS_CURRENCY, m.currency));
			m_listctrl.SetItemText(i, 5, theApp.DictToStr(DICT_CLASS_MONEYINOUT_TYPE, m.type));
			m_listctrl.SetItemText(i, 6, theApp.DictToStr(DICT_CLASS_FUND_TYPE, m.money_type));
			str.Format("%f", m.in_amt);
			m_listctrl.SetItemText(i, 7, str);
			str.Format("%f", m.out_amt);
			m_listctrl.SetItemText(i, 8, str);
		}
	}
}


BOOL CQryMoneyInOutDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LVCOLUMN cols[] = {
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "客户号" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "日期" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "时间" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "流水" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 50, "币种" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "出入金类型" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 80, "资金类型" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "入金" },
		{ LVCF_TEXT | LVCF_WIDTH , LVCFMT_LEFT, 70, "出金" },
	};

	for (int i = 0; i < sizeof(cols) / sizeof(cols[0]); ++i)
		m_listctrl.InsertColumn(i, &cols[i]);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

bool CQryMoneyInOutDialog::RequestMoneyInOut_851403()
{
	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
		cust_money_in_out_st m = { 0 };
		BCGetStringFieldByName(handle, row, "sdate0", m.date, sizeof(m.date));
		BCGetIntFieldByName(handle, row, "lserial0", &m.serial);
		BCGetStringFieldByName(handle, row, "sholder_ac_no", m.cust_no, sizeof(m.cust_no));
		BCGetStringFieldByName(handle, row, "scurrency_type", m.currency, sizeof(m.currency));
		BCGetStringFieldByName(handle, row, "sstatus1", m.type, sizeof(m.type));
		BCGetStringFieldByName(handle, row, "sstatus0", m.money_type, sizeof(m.money_type));
		BCGetDoubleFieldByName(handle, row, "damt1", &m.out_amt);
		BCGetDoubleFieldByName(handle, row, "damt2", &m.in_amt);

		this->m_vMoney.push_back(m);
	};

	Scoped_BCHANDLE handle;
	BCSetRequestType(handle, 851403);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	CString strDate = m_DateBegin.Format("%Y%m%d");
	BCSetStringFieldByName(handle, 0, "sdate0", strDate.GetBuffer());
	strDate = m_DateEnd.Format("%Y%m%d");
	BCSetStringFieldByName(handle, 0, "sdate1", strDate.GetBuffer());
	CString strCust = m_ctlCust.GetCurSelKey();
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", strCust.GetBuffer());

	return MyBCRequest(handle, fetcher, true);
}
