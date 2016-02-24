#pragma once

#include <vector>
#include <iostream>
#include <map>

struct oper_sm_perm_st
{
	char cust_no[16];
	char type[5];
	char param[201];
};

//function 851345 description  "手续费率模板总表查询"  reqlevel 2
struct commission_template_no_st
{
	char template_no[16];
	char template_name[201];
};

//function 851347 description  "保证金率模板总表查询"  reqlevel 2
struct margin_template_no_st
{
	char template_no[16];
	char template_name[201];
};

struct cust_class_st
{
	int level;
	char subclass_no[13];
	char subclass_name[41];
	char level_name[41];
	int parent_level;
	char parent_subclass_no[13];
	char parent_subclass_name[41];
	int id;
};

struct cust_basic_st
{
	char	scust_no[11];		//0.1 客户号
	char	sname[21];		//6.姓名
	char	cust_class[13]; //客户类
	char	sstatus0[2];	//108.状态，客户状态
	char	card_type[3];
	char	card_no[51];
};

struct rc_desc_st
{
	char cust_no[16];
	char classflag[5];
	char type[5];
	char paramlist[201];
};

//
///* 输出：客户号			output:sholder_ac_no
//客户名称			sname
//客户全称			sall_name
//证件类型			scust_type2
//证件号码			scert_no
//邮编			spost_code
//地址			saddr
//电话			sphone
//手机			sphone3
//传真			sphone2
//电子邮件			semail
//法人			scust_auth2
//法人证件			scust_limit2
//合同编号			sbankname2
//客户类			sserial1
//类内资金共享			sstatus2
//开户日期			sdate0
//销户日期			sdate1
//客户性质			sstatus1
//指令下达人1			scust_auth
//指令下达人身份证号1			scust_limit
//指令下达人2			spager
//指令下达人身份证号2			scard0
//指令下达人3			sstation1
//指令下达人身份证号3			scard1
//资金调拨人1			scert_no2
//资金调拨人身份证号1			sbank_acc
//资金调拨人2			scert_addr
//资金调拨人身份证号2			sbank_acc2
//备注			snote
//客户状态			sstatus3
//设置情况			semp_no
//开户操作员			scust_no
//销户操作员			scust_no2
//组织机构代码			sname2
//组织机构简称			scusttypes
//合作机构代码			sbankname
//合作机构简称			ssectypes
//经纪人 */			vsvarstr1

struct cust_base_info_st
{
	//char	scust_no[11];		//0.1 客户号
	//char	sname[21];		//6.姓名
	//char	cust_class[13]; //客户类
	//char	sstatus0[2];	//108.状态，客户状态
	//char	card_type[3];
	//char	card_no[51];
	cust_basic_st basic;
	commission_template_no_st commission_temp;
	margin_template_no_st margin_temp;
	std::vector<rc_desc_st> rcs;
};

typedef std::vector<cust_base_info_st>::iterator cbi_iter_t;
struct cust_login_info_st
{
	char cur_login_date[9];
	char cur_login_time[9];
	char cur_login_ip[31];
	char cur_login_mac[31];
	char cur_app_name[2];
	char login_status[2];
};
//854196
//资金帐号	sholder_ac_no
//币种	scurrency_type
//客户简称	sall_name
//交易日期	sdate0
//上日结存	damt3
//可用资金	damt26
//买冻结	damt29
//卖冻结	damt30
//冻结手续费	damt33
//手续费	damt6
//买保证金	damt13
//卖保证金	damt1
//平仓盈亏	damt11
//浮动盈亏	damt8
//在途资金	damt10
//未交割盈亏	damt18
//今日入金	damt4
//今日出金	damt5
//动态权益	damt28
//冻结资金	damt25
//总保证金	damt23
//总交易所保证金	damt24
//质押金额	damt19
//信用金额	damt31
//风险级别	sstatus3
//风险度1	damt22
//风险度2	damt21
struct cust_capital_st
{
	char cust_no[16];
	char scurrency_type[3];
	double yd_balance; //上日结存damt3
	double available; //可用资金damt26
	double margin; //总保证金
	double buy_frzn_margin; //买冻结保证金
	double sell_frzn_margin; //买冻结保证金
	double commission; //手续费
	double frzn_commission;//冻结手续费
	double offset_profit; //平仓盈亏
	double float_profit;// 浮动盈亏
	double dynamic_capital; //动态权益
	double frzn_moeny; //冻结资金
	double td_money_in;
	double td_money_out;
	char risk_level[2];
	double risk_degree;
};

inline bool operator == (const cust_capital_st &lhs, const cust_capital_st &rhs)
{
	return lhs.yd_balance == rhs.yd_balance
		&& lhs.available == rhs.available
		&& lhs.margin == rhs.margin
		&& lhs.buy_frzn_margin == rhs.buy_frzn_margin
		&& lhs.sell_frzn_margin == rhs.sell_frzn_margin
		&& lhs.commission == rhs.commission
		&& lhs.frzn_commission == rhs.frzn_commission
		&& lhs.offset_profit == rhs.offset_profit
		&& lhs.float_profit == rhs.float_profit
		&& lhs.dynamic_capital == rhs.dynamic_capital
		&& lhs.frzn_moeny == rhs.frzn_moeny
		&& lhs.td_money_in == rhs.td_money_in
		&& lhs.td_money_out == rhs.td_money_out
		&& strcmp(lhs.risk_level, rhs.risk_level) == 0
		&& lhs.risk_degree == rhs.risk_degree;
}

inline bool operator != (const cust_capital_st &lhs, const cust_capital_st &rhs)
{
	return !(lhs == rhs);
}
//function 854186 description  "实时持仓明细查询"  reqlevel 3
//交易日期		sdate2
//开仓日期		sdate3
//成交序号		lvol9
//席位号		sserial2
//客户号		sholder_ac_no
//客户简称		sname
//交易所		smarket_code
//币种		sstatus3
//交易编码		sholder_ac_no2
//品种代码		sstock_code
//交割期		sdate1
//买卖标志		sstatus2
//投保标志		sstatus4
//持仓手数		lvol8
//持仓价格		damt21
//开仓价格		damt26
//持仓保证金		damt1
//交易所保证金		damt2
//每手数量		lvol10
//持仓类型		sorder0
//组合优先级		sorder1
//交易所成交号		sserial0
//交易类别		spost_code2
//组合合约代码		sbankname
//今结算价		damt8
//昨结算价		damt7

struct cust_td_position_detail_st
{
	char cust_no[16];
	char trade_date[9];
	char open_date[9];
	int trade_seq;
	char exchangeid[2];
	char scurrency_type[2];
	char tradeid[16];
	char product[9];
	char delivery_date[9];
	char instrumentid[41];
	char direction[2];
	char hedge_flag[2];
	int volume;
	int multiple;
	double hold_price;
	double open_price;
	double hold_margin;
	double yd_settle_price;
	double td_settle_price;
	double float_profit;
	double last_price;
};

//function 854048 description  "查询客户持仓"  reqlevel 1
//日期		sdate2
//交易所代码		smarket_code
//客户号		sholder_ac_no
//客户名称		sname
//品种代码		sstock_code
//交割期		sdate1
//投保标记		sstatus4
//买卖方向		sstatus2
//持仓手数		lvol5
//今仓手数		lvol12
//持仓均价		damt21
//浮动盈亏		damt8
//交易编码		sholder_ac_no2
//席位号		sserial2
//开仓均价		damt26
//每手数量		lvol10
//最新价		damt23
//damt0
//
//客户号		sholder_ac_no
//客户名称		sname
//组合策略代码		spost_code2
//组合策略名称		scust_limit
//组合合约		sbankname
//
//买保证金		damt1
//卖保证金		damt2
//买持仓均价		damt21
//卖持仓均价		damt26
//交易编码		sholder_ac_no2
//交易所		smarket_code
//席位号		sserial2

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
	int can_offset_vol; //可平量
	int td_can_offset_vol; //可平今量
};

//struct cust_position_id_st
//{
//	char cust_no[16];
//	char instrumentid[41];
//	char direction[2];
//	int volume;
//};
//
//std::ostream& operator << (std::ostream &os, const cust_position_id_st &posi_id)
//{
//	return os << posi_id.cust_no << " "
//		<< posi_id.instrumentid << " "
//		<< posi_id.direction << " "
//		<< posi_id.volume;
//}
//
//std::istream& operator >> (std::ostream &is, cust_position_id_st &posi_id)
//{
//	return is >> posi_id.cust_no >> posi_id.instrumentid >> posi_id.direction >> posi_id.volume;
//}
//
//inline bool operator < (const cust_position_id_st &lhs, const cust_position_id_st &rhs)
//{
//	if (strcmp(lhs.cust_no, rhs.cust_no) >= 0) return false;
//	if (strcmp(lhs.instrumentid, rhs.instrumentid) >= 0) return false;
//	if (strcmp(lhs.direction, rhs.direction) >= 0) return false;
//	if (lhs.volume >= rhs.volume) return false;
//	return true;
//}
//
//inline bool operator == (const cust_position_st &p, const cust_position_id_st &id)
//{
//	return strcmp(p.cust_no, id.cust_no) == 0
//		&& strcmp(p.instrumentid, id.instrumentid) == 0
//		&& strcmp(p.direction, id.direction) == 0
//		&& p.positions == id.volume;
//}
//function 852102 description  "历史实时委托查询"  reqlevel 3
//日期		sdate0
//委托号		lserial0
//操作员号		scust_no
//客户号		sholder_ac_no
//客户名称		sname
//交易所		smarket_code
//品种代码		sstock_code
//交割期		sdate1
//委托单状态		sstatus0
//开平标记		sstatus1
//投保标记		sstatus2
//买卖方向		sstatus3
//委托价		damt0
//委托量		lvol0
//剩余量		lvol1
//成交价		damt1
//成交量		lvol2
//系统号		sorder0
//委托时间		stime0
//申报时间		stime1
//交易编码		sholder_ac_no2
//委托方式		sholder_type
//撤单用户		scust_no2
//撤单时间		stime2
//委托单类型		sstatus4
//席位号		sserial2
//币种		scurrency_type
//备注		snote
//定单类型		scust_type
//成交属性		scust_type2
//止损（盈）价		damt3
//触发时间		stime3
//组合类型		sstat_type
//策略代码		spost_code2
//合约1		sphone
//合约2		sphone2
//合约3		sphone3
//合约4		spager
//私有信息		sstation1
//私有标识		semp_no
//私有流水号		scust_limit
//最小成交数量		lvol8

//function 854094 description  "柜台查询当日委托单"  reqlevel 3
//委托号		lserial0
//操作员		scust_no
//客户号		sholder_ac_no
//客户名称		sname
//交易所代码		smarket_code
//品种代码		sstock_code
//交割期		sdate1
//委托单状态		sstatus0
//开平标记		sstatus3
//投保标记		sstatus4
//买卖方向		sstatus2
//委托价		damt0
//委托量		lvol0
//剩余量		lvol2
//成交价		damt2
//成交量		lvol1
//系统号		sorder0
//委托时间		stime0
//申报时间		stime1
//交易编码		sholder_ac_no2
//委托方式		sholder_type
//撤单用户		scust_no2
//撤单时间		stime2
//委托单类型		sstat_type2
//席位号		sserial2
//币种		scurrency_type
//备注		snote
//修改批次		lwithdraw_flag
//强平标志		sstatus1
//下单席位		vsvarstr1
//本地标志		smain_flag
//定单类型		scust_type
//成交属性		scust_type2
//止损（盈）价		damt3
//触发时间		stime3
//组合类型		sstat_type
//策略代码		spost_code2
//合约1		sphone
//合约2		sphone2
//合约3		sphone3
//合约4		spager
//冻结保证金		damt32
//冻结手续费		damt33
//私有信息		sstation1
//私有标识		semp_no
//私有流水号		scust_limit
//最小成交数量 		lvol8

struct cust_order_st
{
	char cust_no[16];
	char date[9];
	int orderseq;
	char oper_no[11];
	char exchangeid[2];
	char productid[9];
	char delivery_date[9];
	char instrumentid[31];
	char order_status[2];
	char offset_flag[2];
	char hedge_flag[2];
	char direction[2];
	double order_price;
	int origin_total_volume;
	int total_volume;
	double traded_price;
	int traded_volume;
	char sysid[11];
	char order_time[9];
	char insert_time[9];
	char trade_no[16];
	char order_way[3];
	char cancel_cust[11];
	char cancel_time[9];
	char order_type[2];
	char seatid[13];
	char currency[3];
	char force_close[2];
};

//function 852103 description  "历史实时成交查询"  reqlevel 3
//日期		sdate0
//委托号		lserial0
//客户号		sholder_ac_no
//客户名称		sname
//交易编码		sholder_ac_no2
//交易所		smarket_code
//品种代码		sstock_code
//交割期		sdate1
//买卖方向		sstatus3
//成交价		damt1
//成交量		lvol2
//开平标记		sstatus1
//投保标记		sstatus2
//系统号		sorder0
//成交号		lvol3
//交易所成交号		sorder2
//席位号		sserial2
//币种		scurrency_type
//成交时间		stime0
//强平标志		sstatus0

//function 854095 description  "柜台查询当日成交"  reqlevel 3
///* 输入：操作员，席位号，客户号，客户类，合约号，合并标志，所有成交单标记，币种，是否不输出合计标记，下单操作员，汇总标记 */
//input:scust_no, sserial2, sholder_ac_no, sserial1, scust_auth, sstatus1, smain_flag, scurrency_type, sstatus3, scust_no2, sstatus0
///* 输出：委托号，客户号，客户名称，交易编码，交易所代码，品种代码，交割期，成交量，成交价，开平标记，投保标记，买卖方向，系统号，成交号，交易所成交号，席位号，币种，成交时间，手续费，操作员，强平标志 */
//output : lserial0, sholder_ac_no, sname, sholder_ac_no2, smarket_code, sstock_code, sdate1, lvol1, damt2, sstatus3, sstatus4, sstatus2, sorder0, lvol11, sserial0, sserial2, scurrency_type, stime3, damt6, scust_no, sstatus0

//
//输出：委托号			output : lserial0
//客户号			sholder_ac_no
//客户名称			sname
//交易编码			sholder_ac_no2
//交易所代码			smarket_code
//品种代码			sstock_code
//交割期			sdate1
//成交量			lvol1
//成交价			damt2
//开平标记			sstatus3
//投保标记			sstatus4
//买卖方向			sstatus2
//系统号			sorder0
//成交号			lvol11
//交易所成交号			sserial0
//席位号			sserial2
//币种			scurrency_type
//成交时间			stime3
//手续费			damt6
//操作员			scust_no
//强平标志			sstatus0


struct cust_trade_st
{
	char cust_no[16];
	char date[9];
	int orderseq;
	char trade_no[16];
	char exchangeid[2];
	char productid[9];
	char delivery_date[9];
	char instrumentid[31];
	char direction[2];
	double price;
	int volume;
	char offset_flag[2];
	char hedge_flag[2];
	char sysid[11];
	int tradeseq;
	char seatid[13];
	char currency[3];
	char time[9];
	double commission;
	char oper_no[31];
	char force_close[2];
};


struct cust_dynamic_info_st
{
	cust_login_info_st login_info; //登录信息
	cust_capital_st captial; //资金信息
	std::vector<cust_position_st> positions;
	std::vector<cust_td_position_detail_st> posi_detail;
	std::vector<cust_order_st> orders;
	std::vector<cust_trade_st> trades;
};

//struct custinfo_st
//{
//	//char	scust_no[11];		//0.1 客户号
//	//char	sname[21];		//6.姓名
//	//char	sstatus0[2];	//108.状态，客户状态
//	cust_base_info_st base_info;
//	cust_login_info_st login_info; //登录信息
//	cust_capital_st captial; //资金信息
//	std::vector<cust_position_st> position;
//	std::vector<cust_td_position_detail_st> posi_detail;
//};

//function 851230 description  "交易所查询"  reqlevel 2
struct exchange_st
{
	char exchangeid[2];
	char chinese_name[41];
	char chinese_all_name[41];
	char english_name[51];
	char status[2];
};

struct seat_st
{
	char seat_no[15];
	char seat_name[21];
	char member_no[21];
	char seat_status[2];

};

//function 851238 description  "品种查询"  reqlevel 1
//品种代码			sstock_code
//每手数量			damt13
//交割规则（交易规则）			sstatus1
//最小手数			lvol0
//最大手数			lvol1
//最大持仓			lvol2
//涨停板率			damt0
//跌停板率			damt1
//开平手续费率（按金额）			damt2
//开平手续费率（按手数）			damt3
//平今手续费率（按金额）			damt4
//平今手续费率（按手数）			damt5
//交割手续费率（按金额）			damt6
//交割手续费率（按手数）			damt7
//投机保证金按金额收取			damt8
//投机保证金率按手数收取			damt9
//保值保证金按金额收取			damt10
//保值保证金率按手数收取			damt11
//币种			scurrency_type
//品种类型			sstatus2
//状态			sstatus3
//最小变动价位			damt12
//最后交易日后几日交割			lvol3
//品种名称			sname
//分母			lvol4
//指数			lvol5
//交割规则			sstatus4
//保值开平手续费率（按金额）			damt14
//保值开平手续费率（按手数）			damt15
//保值平今手续费率（按金额）			damt16
//保值平今手续费率（按手数）			damt17
//保值交割手续费率（按金额）			damt18
//保值交割手续费率（按手数）			damt19
//套利开平手续费率（按金额）			damt20
//套利开平手续费率（按手数）			damt21
//套利平今手续费率（按金额）			damt22
//套利平今手续费率（按手数）			damt23
//套利交割手续费率（按金额）			damt24
//套利交割手续费率（按手数）			damt25
//套利保证金按金额收取			damt26
//套利保证金率按手数收取			damt27
//是否使用外汇质押			scust_type2
//合约规则			sholder_ac_no2
//合约跨度			lvol6
//交割方式			smain_flag2

struct product_st
{
	char productid[9];
	char name[21];
	char inst_rule[16];
	char trade_rule[16];
	char currency_type[3];
	char type[2];
	char status[2];
	double multiple;
	double price_tick;
	int min_lots;
	int max_lots;
	int max_position;
	double upper_limit_rate;
	double lower_limit_rate;
	double commission_rate_by_volume;
	double commission_rate_by_money;
	double margin_rate_by_volume;
	double margin_rate_by_money;
};

//function 851240 description  "交易所对应品种查询"  reqlevel 1
///* 输入：操作员，交易所代码 */
//input:scust_no, smarket_code
//	/* 输出：交易所代码，品种代码，品种名称 */
//	output : smarket_code, sstock_code, sname
struct exchange_product_st
{
	char exchangeid[2];
	char productid[9];
	char product_name[21];
};


//function 854088 description  "交易合约基本信息查询"  reqlevel 2
//
//交易所代码		smarket_code
//品种代码		sstock_code
//品种名称		sname2
//交割期		sdate1
//合约号		scust_auth
//最大持仓量		lvol5
//每手数量		lvol10
//交割规则		scurrency_type2
//最小手数		damt15
//最大手数		damt14
//币种 		scurrency_type
//最小变动价		damt6
//合约状态		sstatus1
//function 854044 description  "查询可交易合约"  reqlevel 1
///* 输入：交易所代码，席位号 */
//input:smarket_code, sserial2
//	/* 输出：（输出接口不要使用sstatus0，已被原油占用了）交易所代码，品种代码，交割期，合约号，合约名称，每手数量，币种，最小变动价位，合约状态,昨日收盘价, 昨日结算价, 当日涨停价, 当日跌停价，合约是否参与上海单向大边优惠 */
//	output : smarket_code, sstock_code, sdate1, scust_auth, scust_auth2, lvol10, scurrency_type, damt6, sstatus1, damt31, damt32, damt27, damt28, sstatus3
//
struct instrument_st
{
	char exchangeid[2];
	char productid[9];
	char product_name[21];
	char delivery_date[9];
	char delivery_rule[3];
	char instrumentid[21];
	char currency_type[3];
	int max_position;
	int multiple;
	double min_lots;
	double max_lots;
	double price_tick;
	char status[2];
};

inline bool operator < (const instrument_st &lhs, const instrument_st &rhs)
{
	return strcmp(lhs.instrumentid, rhs.instrumentid) < 0;
}


//function 851339 description  "手续费率模板查询"
//模板号			sholder_ac_no
//模板名称			scusttypes
//交易所			smarket_code
//品种			sstock_code
//交割期			sdate0
//开平手续费率（按金额）			damt2
//开平手续费率（按手数）			damt3
//开平附加费率1（按金额）			damt4
//开平附加费率1（按手数）			damt5
//开平附加费率2（按金额）			damt6
//开平附加费率2（按手数）			damt7
//平今手续费率（按金额）			damt8
//平今手续费率（按手数）			damt9
//平今附加费率1（按金额）			damt10
//平今附加费率1（按手数）			damt11
//平今附加费率2（按金额）			damt12
//平今附加费率2（按手数）			damt13
//交割手续费率（按金额）			damt14
//交割手续费率（按手数）			damt15
//保值开平手续费率（按金额）			damt16
//保值开平手续费率（按手数）			damt17
//保值开平附加费率1(按金额)			damt18
//保值开平附加费率1（按手数）			damt19
//保值开平附加费率2(按金额)			damt20
//保值开平附加费率2（按手数）			damt21
//保值平今手续费率（按金额）			damt22
//保值平今手续费率（按手数）			damt23
//保值平今附加费率1(按金额)			damt24
//保值平今附加费率1（按手数）			damt25
//保值平今附加费率2(按金额)			damt26
//保值平今附加费率2（按手数）			damt27
//保值交割手续费率（按金额）			damt28
//保值交割手续费率（按手数）			damt29
//套利开平手续费率（按金额）			damt30
//套利开平手续费率（按手数）			damt31
//套利平今手续费率（按金额）			damt32
//套利平今手续费率（按手数）			damt33
//套利交割手续费率（按金额）			damt0
//套利交割手续费率（按手数）			damt1
struct commission_template_st
{
	char template_no[16];
	char template_name[201];
	char exchangeid[2];
	char productid[9];
	char delivery_date[9];
	double commission_rate_by_money;
	double commission_rate_by_volume;
	double offset_td_commission_rate_by_money;
	double offset_td_commission_rate_by_volume;
};

//function 851341 description  "保证金率模板查询"
struct margin_template_st
{
	char template_no[16];
	char template_name[201];
	char exchangeid[2];
	char productid[9];
	//char delivery_date[9];
	double margin_rate_by_money;
	double margin_rate_by_volume;
};

struct cust_commission_rate_st
{
	char cust_no[16];
	char exchangeid[2];
	char productid[9];
	char delivery_date[9];
	double commission_rate_by_money;
	double commission_rate_by_volume;
	double offset_td_commission_rate_by_money;
	double offset_td_commission_rate_by_volume;
};

struct cust_margin_rate_st
{
	char cust_no[16];
	char exchangeid[2];
	char productid[9];
	char delivery_date[9];
	double margin_rate_by_money;
	double margin_rate_by_volume;
};

struct system_info_st
{
	char system_date[9];
	char check_date[9];
	char next_tradingday[9];
	char host_date[9];
	char system_status[2];
	char system_time[9];
	long request_time;
};

struct operator_info_st
{

};

struct dict_st
{
	int dict_class;
	char dict_code[21];
	char dict_note[81];
};

//inline std::ostream operator << (std::ostream &os, dict_st &dict)
//{
//	return os << dict.dict_class << dict.dict_code << dict.dict_note
//}

//struct his_order_st
//{
//
//};

struct baseinfo_st
{
	//system_info_st sysinfo;
	std::vector<dict_st> dicts;
	std::vector<exchange_st> exchanges;
	std::vector<product_st> products;
	std::vector<seat_st> seats;
	std::vector<exchange_product_st> exchange_product;
	std::vector<instrument_st> instruments;
	std::vector<commission_template_no_st> commission_no;
	//std::vector<commission_template_st> commissions;
	std::map<std::string, std::vector<commission_template_st>> commissions;
	std::vector<margin_template_no_st> margin_no;
	//StrMtMap_t margins;
	std::map<std::string, std::vector<margin_template_st>> margins;
	std::vector<cust_class_st> cust_classes;
};

struct inst_market_st
{
	double last_price;
	double bid_price;
	double ask_price;
	double range; //涨跌
	double highest_price;
	double lowest_price;
	double open_price;
	double	upperlimit_price;
	///跌停板价
	double	lowerlimit_price;
	int bid_vol;
	int ask_vol;
	int volume;
	int delta;
	double pre_close_price;
	///涨停板价
	double settlement_price;
	double his_highest_price;
	double his_lowest_price;
};