#pragma once
#include "afxwin.h"
#include "ATLComTime.h"
#include "KVhyphenComboBox.h"
#include "MyBCRequest.h"
// CQrySettlementDialog �Ի���
#include "struct_helper.h"


struct cust_settle_fund_st
{
	char cust_no[16];
	char cust_name[21];
	char currency[3];
	double yd_balance;
	double out_money;
	double in_money;
	double commission;
	double sj_commission;
	double td_profit;
	double other_fee;
	double td_balance;
	double float_profit;
	double roading_fund;
	double undelivery_profit;
	double margin;
	double base_margin;
	double available;
	double impawn_amt;
	double exchange_margin;
	char risk_degree[21];
	double add_margin;
	char risk_level[2];
	double delivery_profit;
	double offset_profit;
	double hold_profit;
	double delivery_margin;
	double delivery_commission;
	double total_profit;
};

struct cust_posi_summary_st
{
	char cust_no[16];
	char exchangeid[2];
	char seatid[13];	
	char product[9];
	char delivery_date[9];
	char hedge_flag[2];
	int buy_hold_volume;
	int sell_hold_volume;
	double buy_aver_price;
	double sell_aver_price;
	double settle_price;
	double yd_settle_price;
	double margin;
	double float_profit;
	double hold_profit;
};
//
//
///* �����ƽ������			output:sdate0
//ƽ����ˮ��			lserial0
//������			smarket_code
//ϯλ������ʾ��			sserial2
//�ͻ��ţ�����ʾ��			sholder_ac_no
//Ʒ��			sstock_code
//������			sdate1
//����			sstatus2
//����			lvol1
//ƽ�ּ۸�			damt0
//���ּ۸�			damt1
//���ս����			damt2
//ƽ��ӯ��������			damt3
//ƽ��ӯ��������			damt4
//��������			sdate2
//������ˮ��			lserial1
//���ױ��루����ʾ��			sholder_ac_no2
//Ͷ��������ʾ��			sstatus3
//ƽ������			sstatus1
//ƽ�ֵ��������ɽ���			scust_auth
//���ֵ��������ɽ��� */			scust_auth2

struct cust_offset_detail_st
{
	char close_date[9];
	char open_date[9];
	int close_serial;
	int open_serial;
	char exchangeid[2];
	char seatid[13];
	char cust_no[16];
	char product[9];
	char delivery_date[9];
	char direction[2];
	int volume;
	double close_price;
	double open_price;
	double yd_settle_price;
	double float_profit;
	double hold_profit;
};


class CQrySettlementDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CQrySettlementDialog)

public:
	CQrySettlementDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQrySettlementDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QUERY_SETTLEMENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	//CComboBox m_ctlCust;
	
	COleDateTime m_DateTime;
	CString m_SettleStr;

	cust_settle_fund_st m_fund;
	//cust_position_st m_posi;
	TradeVec_t m_trades;
	PosiDetailVec_t m_PosiDetails;
	std::vector<cust_posi_summary_st> m_PosiSummary;
	std::vector<cust_offset_detail_st> m_OffDetails;
public:
	CCustComboBox m_ctlCust;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonQuery();

	BCResult RequestSettleCustReady_853015(CString cust_no, char *date, BCHANDLE handle);
	BCResult RequestSettleFundQuery_853000(char *cust_no, char *date,BCHANDLE handle);
	BCResult RequestSettleTradeQuery_853002(char *cust_no, char *date, BCHANDLE handle);
	BCResult RequestSettleOffsetDetail_853003(char *cust_no, char *date, BCHANDLE handle);
	BCResult RequestSettlePosiDetail_853004(char *cust_no, char *date, BCHANDLE handle);
	BCResult RequestSettlePosiSummary_853005(char *cust_no, char *date, BCHANDLE handle);
	
	

	CString format_title();
	CString format_fund();
	CString format_trade();
	CString format_od();
	CString format_pd();
	CString format_ps();
	CString format_tail();
};
