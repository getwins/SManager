#pragma once

#include <functional>
#include <string>
#include <vector>
#include "nbccclt.h"

class Scoped_BCHANDLE
{
public:
	Scoped_BCHANDLE() {
		handle_ = BCNewHandle("cpack.dat");
	}
	~Scoped_BCHANDLE() {
		BCDeleteHandle(handle_);
	}
	operator BCHANDLE () {
		return handle_;
	}
private:
	BCHANDLE handle_;
};


typedef std::function<void(BCHANDLE, int)> FetchRowFunc_t;

struct BCResult
{
	BCResult()
	{
		errcode = 0;
		memset(errmsg, 0, sizeof(errmsg));
	}

	int errcode;
	char errmsg[256];

	operator bool() { return errcode == 0; };
	operator std::string() { return errmsg; };

	friend std::ostream& operator << (std::ostream &os, BCResult &result)
	{
		os << "errcode=" << result.errcode << ",";
		if (!result.errcode && strlen(result.errmsg) == 0)
			os << "errmsg=�ɹ�";
		else
			os << "errmsg=" << result.errmsg;
		return os;
	}
};


BCResult MyBCRequest(BCHANDLE handle, FetchRowFunc_t fetcher = FetchRowFunc_t());


BCResult BCRequestRCMaxSeq_860002(BCHANDLE handle, int &o_seq, char*cust_no, char *rc_class, char *rc_type);

struct rc_desc
{
	char cust_no[16];
	char classflag[5];
	char type[5];
	char paramlist[201];
};


BCResult BCRequestRC_860000(BCHANDLE handle, std::vector<rc_desc> &o_desc, char *rc_class = "", char *rc_type = "", char *cust_no = "");
//struct rc_force
//{
//	char cust_no[16];
//	char type[5];
//	char paramlist[201];
//};
// std::vector<rc_force> BCRequestRC_force(BCHANDLE handle);
//
//
//struct rc_forbidden
//{
//	char cust_no[16];
//	char paramlist[201];
//};
//
//std::vector<rc_forbidden> BCRequestRC_forbidden(BCHANDLE handle);

struct cust_capital_st
{
	char cust_no[16];
	char scurrency_type[3];
	double yd_balance; //���ս��damt3
	double available; //�����ʽ�damt26
	double margin; //�ܱ�֤��
	double buy_frzn_margin; //�򶳽ᱣ֤��
	double sell_frzn_margin; //�򶳽ᱣ֤��
	double commission; //������
	double frzn_commission;//����������
	double offset_profit; //ƽ��ӯ��
	double float_profit;// ����ӯ��
	double dynamic_capital; //��̬Ȩ��
	double frzn_moeny; //�����ʽ�
	double td_money_in;
	double td_money_out;
	char risk_level[2];
	double risk_degree;
};

BCResult BCRequestCustCaptial_854196(BCHANDLE handle, char *cust_no, cust_capital_st &o_sc);


struct cust_position_st
{
	char cust_no[16];
	char open_date[9];
	char exchangeid[2];
	//char scurrency_type[2];
	char seatid[13];
	char trade_no[16];
	char product[9];
	char delivery_date[9];
	char instrumentid[41];
	char direction[2];
	char hedge_flag[2];
	int positions;
	int td_positions;
	double hold_average_price;
	double float_profit;
	double open_average_price;
	double last_price;
	int multiple;
	double settle_price;
	double market_profit;
	double marign;
	int can_offset_vol; //��ƽ��
	int td_can_offset_vol; //��ƽ����
};

typedef std::vector<cust_position_st> posi_vec_t;

std::string FmtPosiID(const cust_position_st &p);

BCResult BCRequestCustPosition_854048(BCHANDLE handle, char *cust_no, posi_vec_t &o_p);

BCResult BCRequestPosiOffsetVolume_854058(BCHANDLE handle, cust_position_st &p);

struct inst_market_st
{
	double last_price;
	double bid_price;
	double ask_price;
	double range; //�ǵ�
	double highest_price;
	double lowest_price;
	double open_price;
	double	upperlimit_price;
	///��ͣ���
	double	lowerlimit_price;
	int bid_vol;
	int ask_vol;
	int volume;
	int delta;
	double pre_close_price;
	///��ͣ���
	double settlement_price;
	double his_highest_price;
	double his_lowest_price;
};

BCResult BCRequestInstMarketData_851503(BCHANDLE handle, std::string inst, inst_market_st &o_md);

BCResult BCRequestOffsetOrderInsert(BCHANDLE handle, cust_position_st posi, const inst_market_st &m);

BCResult BCRequestCustStatusSet_851005(BCHANDLE handle, char *cust_no, char *status);

struct cust_basic_st
{
	char	scust_no[11];		//0.1 �ͻ���
	char	sname[21];		//6.����
	char	cust_class[13]; //�ͻ���
	char	sstatus0[2];	//108.״̬���ͻ�״̬
	char	card_type[3];
	char	card_no[51];
};

BCResult BCRequestCustBasic_850003(BCHANDLE handle, char *cust_no, cust_basic_st &o_cb);

struct ts_param_st
{
	char seatid[13];
	char para_id[13];
	char para_val[13];
	char note[81];
};

BCResult BCRequestQryTradeParam_851217(BCHANDLE handle, char *seatid, char *paraid, ts_param_st &o_ts_param);