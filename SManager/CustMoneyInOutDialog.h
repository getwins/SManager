#pragma once
#include "afxwin.h"
#include "struct.h"
#include "KVhyphenComboBox.h"
// CCustMoneyInOutDialog 对话框
#include <map>

struct cust_withdraw_st
{
	double withdraw_fund;
	double withdraw_cash;
	double trade_withdraw;
	double settle_withdraw;
};

class CCustMoneyInOutDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCustMoneyInOutDialog)

public:
	CCustMoneyInOutDialog(/*const system_info_st &sysinfo,*/ CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCustMoneyInOutDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUST_MOENY_IN_OUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CCustComboBox m_ctlCust;
private:
	bool RequestSysInfo_100319();
	bool RequestAddCustMoneyIn_851100(BCHANDLE handle, char *direction);
	bool RequestChkCustMoneyIn_851104(BCHANDLE handle, char *direction);
	bool RequestCustWithdraw_851101(char *cust_no);
	//CComboBox m_ctlCust;
	
	double m_Amount;
//	CString m_Amount;
	int m_serial;
	system_info_st m_SysInfo;
	std::map<std::string, cust_withdraw_st> m_mWithdraw;

	CString m_CustName;
	CString m_CustStatus;
	double m_TradeWithdraw;
//	CString m_SettleWithdraw;
	double m_cash;
	double m_SettleWithdraw;
public:
	afx_msg void OnBnClickedButtonMoneyIn();
	afx_msg void OnBnClickedButtonMoneyOut();
	afx_msg void OnBnClickedButtonFlush();
	void UpdateCustWithDraw(char *cust_no);
	afx_msg void OnCbnSelchangeComboCust();
};
