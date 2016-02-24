//#include "stdafx.h"
#include "MyBCRequest.h"
#include "conf.h"

#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>


extern int g_drtpno;

//extern int g_funcno;
//extern std::string g_oper_code;

BCResult MyBCRequest(BCHANDLE handle, FetchRowFunc_t fetcher)
{
	BCResult ret;
	int record;
	//int retcode;

	if (!BCCallRequest(handle, g_drtpno, 0, g_conf.drtp.funcno, 10000, &ret.errcode, ret.errmsg))
		goto failed;

	BCGetRetCode(handle, &ret.errcode);
	BCGetStringFieldByName(handle, 0, "vsmess", ret.errmsg, sizeof(ret.errmsg));

	if (ret.errcode)
		goto failed;

	BCGetRecordCount(handle, &record);
	for (int i = 0; i < record; i++)
	{
		if (fetcher)
			fetcher(handle, i);
	}

	while (BCHaveNextPack(handle))
	{
		if (!BCCallNext(handle, 10000, &ret.errcode, ret.errmsg))
			goto failed;

		BCGetRetCode(handle, &ret.errcode);
		BCGetStringFieldByName(handle, 0, "vsmess", ret.errmsg, sizeof(ret.errmsg));
		if (ret.errcode)
			goto failed;

		BCGetRecordCount(handle, &record);
		for (int i = 0; i < record; i++)
		{
			if (fetcher)
				fetcher(handle, i);
		}
	}

failed:
	//if(MsgToMessageBox)
	//	if(ret.errcode)
	//		::AfxMessageBox(ret.errmsg, MB_ICONSTOP | MB_OK);
	//	else
	//		::AfxMessageBox(ret.errmsg, MB_ICONINFORMATION | MB_OK);
	//if (MsgToOutput)
	//	PostOutputMsg(ret.errmsg);

	//if (ret.errcode) 
	//{
	//	if(whichMsgTowhere & BCMSG_ERR_TO_MB)
	//		::AfxMessageBox(ret.errmsg, MB_ICONWARNING | MB_OK);
	//	if(whichMsgTowhere & BCMSG_ERR_TO_OP)
	//		PostOutputMsg(ret.errmsg);
	//}
	//else
	//{
	//	if(whichMsgTowhere & BCMSG_NOR_TO_MB)
	//		::AfxMessageBox(ret.errmsg, MB_ICONINFORMATION | MB_OK);
	//	if((whichMsgTowhere & BCMSG_NOR_TO_OP) && strlen(ret.errmsg) > 0)
	//		PostOutputMsg(ret.errmsg);
	//}

	return ret;
}

BCResult BCRequestRCMaxSeq_860002(BCHANDLE handle, int &o_seq, char*cust_no, char *rc_class, char *rc_type)
{
	//int o_seq = 0;
	o_seq = 0;
	FetchRowFunc_t fetcher = [&o_seq](BCHANDLE handle, int row) {
		BCGetIntFieldByName(handle, row, "lserial0", &o_seq);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 860002);
	BCSetStringFieldByName(handle, 0, "scust_no", g_conf.oper.code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);
	BCSetStringFieldByName(handle, 0, "sserial0", rc_class);
	BCSetStringFieldByName(handle, 0, "sserial1", rc_type);
	return MyBCRequest(handle, fetcher);
}

BCResult BCRequestRC_860000(BCHANDLE handle, std::vector<rc_desc> &o_desc, char *rc_class, char *rc_type, char *cust_no)
{
	//std::vector<rc_desc> o_desc;
	o_desc.clear();
	FetchRowFunc_t fetcher = [&](BCHANDLE handle, int row) {
		//BCGetIntFieldByName(handle, row, "lserial0", &o_seq);
		rc_desc f;
		BCGetStringFieldByName(handle, row, "sholder_ac_no", f.cust_no, sizeof(f.cust_no));
		BCGetStringFieldByName(handle, row, "sserial1", f.classflag, sizeof(f.classflag));
		BCGetStringFieldByName(handle, row, "sserial1", f.type, sizeof(f.type));
		BCGetStringFieldByName(handle, row, "ssectypes", f.paramlist, sizeof(f.paramlist));
		o_desc.push_back(f);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 860000);
	BCSetStringFieldByName(handle, 0, "scust_no", g_conf.oper.code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);
	BCSetStringFieldByName(handle, 0, "sserial0", rc_class);
	BCSetStringFieldByName(handle, 0, "sserial1", rc_type);
	return MyBCRequest(handle, fetcher);
}

//std::vector<rc_force> BCRequestRC_force(BCHANDLE handle)
//{
//	std::vector<rc_force> o_forces;
//	FetchRowFunc_t fetcher = [&o_forces](BCHANDLE handle, int row) {
//		//BCGetIntFieldByName(handle, row, "lserial0", &o_seq);
//		rc_force f;
//		BCGetStringFieldByName(handle, row, "sholder_ac_no", f.cust_no, sizeof(f.cust_no));
//		BCGetStringFieldByName(handle, row, "sserial1", f.type, sizeof(f.type));
//		BCGetStringFieldByName(handle, row, "ssectypes", f.paramlist, sizeof(f.paramlist));
//		o_forces.push_back(f);
//	};
//
//	BCResetHandle(handle);
//	BCSetRequestType(handle, 860002);
//	BCSetStringFieldByName(handle, 0, "scust_no", g_conf.oper.code);
//	BCSetStringFieldByName(handle, 0, "sholder_ac_no", "");
//	BCSetStringFieldByName(handle, 0, "sserial0", "0002");
//	BCSetStringFieldByName(handle, 0, "sserial1", "");
//	BCResult bcret = MyBCRequest(handle, fetcher);
//	std::cout << bcret << std::endl;
//	return o_forces;
//}
//
//std::vector<rc_forbidden> BCRequestRC_forbidden(BCHANDLE handle)
//{
//	std::vector<rc_forbidden> o_forbidden;
//	FetchRowFunc_t fetcher = [&o_forbidden](BCHANDLE handle, int row) {
//		//BCGetIntFieldByName(handle, row, "lserial0", &o_seq);
//		rc_forbidden f;
//		BCGetStringFieldByName(handle, row, "sholder_ac_no", f.cust_no, sizeof(f.cust_no));
//		//BCGetStringFieldByName(handle, row, "sserial1", f.type, sizeof(f.type));
//		BCGetStringFieldByName(handle, row, "ssectypes", f.paramlist, sizeof(f.paramlist));
//		o_forbidden.push_back(f);
//	};
//
//	BCResetHandle(handle);
//	BCSetRequestType(handle, 860002);
//	BCSetStringFieldByName(handle, 0, "scust_no", g_conf.oper.code);
//	BCSetStringFieldByName(handle, 0, "sholder_ac_no", "");
//	BCSetStringFieldByName(handle, 0, "sserial0", "0001");
//	BCSetStringFieldByName(handle, 0, "sserial1", "");
//	BCResult bcret = MyBCRequest(handle, fetcher);
//	std::cout << bcret << std::endl;
//	return o_forbidden;
//}

BCResult BCRequestCustCaptial_854196(BCHANDLE handle, char *cust_no, cust_capital_st &o_sc)
{
	FetchRowFunc_t fetcher = [&](BCHANDLE handle, int row) {
		//char sholder_ac_no[16] = { 0 };
		//BCGetStringFieldByName(handle, 0, "sholder_ac_no", sholder_ac_no, sizeof(sholder_ac_no));
		//assert(strcmp(cust_no, sholder_ac_no) == 0);
		BCGetStringFieldByName(handle, 0, "sholder_ac_no", o_sc.cust_no, sizeof(o_sc.cust_no));
		BCGetStringFieldByName(handle, 0, "scurrency_type", o_sc.scurrency_type, sizeof(o_sc.scurrency_type));
		BCGetDoubleFieldByName(handle, 0, "damt3", &o_sc.yd_balance);
		BCGetDoubleFieldByName(handle, 0, "damt26", &o_sc.available);
		BCGetDoubleFieldByName(handle, 0, "damt23", &o_sc.margin);
		BCGetDoubleFieldByName(handle, 0, "damt29", &o_sc.buy_frzn_margin);
		BCGetDoubleFieldByName(handle, 0, "damt30", &o_sc.sell_frzn_margin);
		BCGetDoubleFieldByName(handle, 0, "damt6", &o_sc.commission);
		BCGetDoubleFieldByName(handle, 0, "damt33", &o_sc.frzn_commission);
		BCGetDoubleFieldByName(handle, 0, "damt11", &o_sc.offset_profit);
		BCGetDoubleFieldByName(handle, 0, "damt8", &o_sc.float_profit);
		BCGetDoubleFieldByName(handle, 0, "damt28", &o_sc.dynamic_capital);
		BCGetDoubleFieldByName(handle, 0, "damt25", &o_sc.frzn_moeny);
		BCGetStringFieldByName(handle, 0, "sstatus3", o_sc.risk_level, sizeof(o_sc.risk_level));
		BCGetDoubleFieldByName(handle, 0, "damt22", &o_sc.risk_degree);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 854196);
	BCSetStringFieldByName(handle, 0, "scust_no", g_conf.oper.code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);
	//BCSetStringFieldByName(handle, 0, "scurrency_type", ""); //
	return MyBCRequest(handle, fetcher);
}

std::string FmtPosiID(const cust_position_st &p)
{
	std::ostringstream oss;
	oss << p.cust_no << "|" << p.instrumentid << "|" << p.direction << "|" << p.hedge_flag << "|" << p.positions;
	return oss.str();
}


BCResult BCRequestCustPosition_854048(BCHANDLE handle, char *cust_no, posi_vec_t &o_p)
{
	FetchRowFunc_t fetcher = [&](BCHANDLE handle, int row) {
		cust_position_st p = { 0 };
		BCGetStringFieldByName(handle, row, "sholder_ac_no", p.cust_no, sizeof(p.cust_no));
		BCGetStringFieldByName(handle, row, "smarket_code", p.exchangeid, sizeof(p.exchangeid));
		BCGetStringFieldByName(handle, row, "sserial2", p.seatid, sizeof(p.seatid));
		BCGetStringFieldByName(handle, row, "sstock_code", p.product, sizeof(p.product));
		BCGetStringFieldByName(handle, row, "sdate1", p.delivery_date, sizeof(p.delivery_date));
		BCGetStringFieldByName(handle, row, "sbank_acc", p.instrumentid, sizeof(p.instrumentid));
		BCGetStringFieldByName(handle, row, "sstatus2", p.direction, sizeof(p.direction));
		BCGetStringFieldByName(handle, row, "sstatus4", p.hedge_flag, sizeof(p.hedge_flag));
		BCGetIntFieldByName(handle, row, "lvol5", &p.positions);
		BCGetIntFieldByName(handle, row, "lvol12", &p.td_positions);
		BCGetDoubleFieldByName(handle, row, "damt21", &p.hold_average_price);
		BCGetDoubleFieldByName(handle, row, "damt26", &p.open_average_price);
		BCGetDoubleFieldByName(handle, row, "damt8", &p.float_profit);
		BCGetDoubleFieldByName(handle, row, "damt23", &p.last_price);
		BCGetIntFieldByName(handle, row, "lvol10", &p.multiple);
		o_p.push_back(p);
	};

	o_p.clear();
	BCResetHandle(handle);
	BCSetRequestType(handle, 854048);
	BCSetStringFieldByName(handle, 0, "scust_no", g_conf.oper.code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);
	BCSetStringFieldByName(handle, 0, "sstatus3", "1"); //是否不输出登记标记
	BCSetStringFieldByName(handle, 0, "sstatus0", "0"); //汇总标记
	
	BCResult result = MyBCRequest(handle, fetcher);

	//for (posi_vec_t::iterator it = o_p.begin(); it != o_p.end(); ++it)
	//{
	//	BCRequestPosiOffsetVolume_854058(handle, *it);
	//}

	return result;
}

BCResult BCRequestPosiOffsetVolume_854058(BCHANDLE handle, cust_position_st &p)
{
	FetchRowFunc_t fetcher = [&p](BCHANDLE handle, int row)
	{
		BCGetIntFieldByName(handle, row, "lvol4", &p.can_offset_vol);
		BCGetIntFieldByName(handle, row, "lvol7", &p.td_can_offset_vol);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 854058);
	BCSetStringFieldByName(handle, 0, "scust_no", g_conf.oper.code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", p.cust_no);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no2", p.trade_no);
	BCSetStringFieldByName(handle, 0, "sserial2", p.seatid);
	BCSetStringFieldByName(handle, 0, "smarket_code", p.exchangeid);
	BCSetStringFieldByName(handle, 0, "scust_auth", p.instrumentid);
	BCSetStringFieldByName(handle, 0, "sstatus2", strcmp(p.direction, "0") ? "0" : "1");
	BCSetStringFieldByName(handle, 0, "sstatus4", p.hedge_flag);

	return MyBCRequest(handle, fetcher);
}


BCResult BCRequestInstMarketData_851503(BCHANDLE handle, std::string inst, inst_market_st &o_md)
{
	FetchRowFunc_t fetcher = [&](BCHANDLE handle, int row) {
		BCGetDoubleFieldByName(handle, 0, "damt1", &o_md.last_price);
		BCGetDoubleFieldByName(handle, 0, "damt2", &o_md.bid_price);
		BCGetDoubleFieldByName(handle, 0, "damt3", &o_md.ask_price);
		BCGetDoubleFieldByName(handle, 0, "damt4", &o_md.range);
		BCGetDoubleFieldByName(handle, 0, "damt5", &o_md.highest_price);
		BCGetDoubleFieldByName(handle, 0, "damt6", &o_md.lowest_price);
		BCGetDoubleFieldByName(handle, 0, "damt7", &o_md.open_price);
		BCGetDoubleFieldByName(handle, 0, "damt8", &o_md.upperlimit_price);
		BCGetDoubleFieldByName(handle, 0, "damt9", &o_md.lowerlimit_price);
		BCGetIntFieldByName(handle, 0, "lvol1", &o_md.bid_vol);
		BCGetIntFieldByName(handle, 0, "lvol2", &o_md.ask_vol);
		BCGetIntFieldByName(handle, 0, "lvol3", &o_md.volume);
		BCGetIntFieldByName(handle, 0, "lvol4", &o_md.delta);
		BCGetDoubleFieldByName(handle, 0, "damt10", &o_md.pre_close_price);
		BCGetDoubleFieldByName(handle, 0, "damt11", &o_md.settlement_price);
		BCGetDoubleFieldByName(handle, 0, "damt12", &o_md.his_highest_price);
		BCGetDoubleFieldByName(handle, 0, "damt13", &o_md.his_lowest_price);
	};
	BCResetHandle(handle);
	BCSetRequestType(handle, 851503);
	BCSetStringFieldByName(handle, 0, "sorder2", g_conf.oper.code);
	std::string::iterator it = std::find_if(inst.begin(), inst.end(), ::isdigit);
	std::string product(inst.begin(), it);
	std::string delivery(it, inst.end());
	BCSetStringFieldByName(handle, 0, "sstock_code", (char*)product.c_str());
	BCSetStringFieldByName(handle, 0, "sdate0", (char*)delivery.c_str());

	memset(&o_md, 0, sizeof(o_md));

	return MyBCRequest(handle, fetcher);
}

///* 输入：操作员		input:scust_no
//客户号		sholder_ac_no
//交易编码		sholder_ac_no2
//交易所代码		smarket_code
//买卖方向		sstatus2
//合约号		scust_auth
//申报手数		lvol0
//申报价格		damt0
//开平标记		sstatus3
//投保标记		sstatus4
//席位号		sserial2
//下单人类型		smain_flag2
//委托方式		sholder_type
//币种		scurrency_type
//强平标志		sstatus0
//定单类型		scust_type
//成交属性		scust_type2
//止损止盈价		damt3
//私有信息		sstation1
//私有标识		semp_no
//私有流水号		scust_limit
//委托单类型		sstatus1
//拆分平今单标记		smarket_code2
//最小成交数量 */		lvol8


BCResult BCRequestOffsetOrderInsert(BCHANDLE handle, cust_position_st posi, const inst_market_st &m)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 854002);

	BCSetStringFieldByName(handle, 0, "smarket_code", posi.exchangeid);
	BCSetStringFieldByName(handle, 0, "scust_auth", posi.instrumentid);
	BCSetIntFieldByName(handle, 0, "lvol0", posi.can_offset_vol);
	BCSetStringFieldByName(handle, 0, "sstatus2", strcmp(posi.direction, "0") ? "0" : "1");
	BCSetStringFieldByName(handle, 0, "scust_no", g_conf.oper.code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", posi.cust_no);
	BCSetDoubleFieldByName(handle, 0, "damt0", (strcmp(posi.direction, "0") ? m.upperlimit_price : m.lowerlimit_price));
	BCSetStringFieldByName(handle, 0, "sstatus3", "1");//开平
	BCSetStringFieldByName(handle, 0, "sstatus4", posi.hedge_flag);//投保
	BCSetStringFieldByName(handle, 0, "sserial2", posi.seatid);//席位号
	BCSetStringFieldByName(handle, 0, "smain_flag2", "1");//下单人类型
	BCSetStringFieldByName(handle, 0, "sholder_type", "1");//委托方式 柜台委托
	BCSetStringFieldByName(handle, 0, "scurrency_type", "1");//scurrency_type = '', 币种
	BCSetStringFieldByName(handle, 0, "sstatus0", "1");//强平
	BCSetStringFieldByName(handle, 0, "scust_type", "0");//定单类型 限价
														 //BCSetStringFieldByName(handle, 0, "scust_type", PriceType);//定单类型 限价
	BCSetStringFieldByName(handle, 0, "scust_type2", "0");//成交属性 //'0', 'GFD当日有效'
	BCSetStringFieldByName(handle, 0, "sstatus1", "A");	//sstatus1 = '', 委托单类型
	BCSetIntFieldByName(handle, 0, "lvol8", posi.can_offset_vol);//lvol8 = '', 最小成交数量

	return MyBCRequest(handle, FetchRowFunc_t());
}


BCResult BCRequestCustStatusSet_851005(BCHANDLE handle, char *cust_no, char *status)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 851005);

	BCSetStringFieldByName(handle, 0, "scust_no", g_conf.oper.code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);
	BCSetStringFieldByName(handle, 0, "sstatus1", status);

	return MyBCRequest(handle);
}

BCResult BCRequestCustBasic_850003(BCHANDLE handle, char *cust_no, cust_basic_st &o_cb)
{
	FetchRowFunc_t fetcher = [&](BCHANDLE handle, int row)
	{
		BCGetStringFieldByName(handle, row, "sholder_ac_no", o_cb.scust_no, sizeof(o_cb.scust_no));
		BCGetStringFieldByName(handle, row, "sserial1", o_cb.cust_class, sizeof(o_cb.cust_class));
		BCGetStringFieldByName(handle, row, "sstatus1", o_cb.sstatus0, sizeof(o_cb.sstatus0));
		BCGetStringFieldByName(handle, row, "sname", o_cb.sname, sizeof(o_cb.sname));
		BCGetStringFieldByName(handle, row, "lcert_code", o_cb.card_type, sizeof(o_cb.card_type));
		BCGetStringFieldByName(handle, row, "scert_no", o_cb.card_no, sizeof(o_cb.card_no));
	};
	memset(&o_cb, 0, sizeof(o_cb));
	BCResetHandle(handle);
	BCSetRequestType(handle, 850003);
	BCSetStringFieldByName(handle, 0, "scust_no", g_conf.oper.code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);

	return MyBCRequest(handle, fetcher);
}

BCResult BCRequestQryTradeParam_851217(BCHANDLE handle, char *seatid, char *paraid, ts_param_st &o_ts_param)
{
	FetchRowFunc_t fetcher = [&](BCHANDLE handle, int row)
	{
		BCGetStringFieldByName(handle, row, "sserial2", o_ts_param.seatid, sizeof(o_ts_param.seatid));
		BCGetStringFieldByName(handle, row, "sserial1", o_ts_param.para_id, sizeof(o_ts_param.para_id));
		BCGetStringFieldByName(handle, row, "sserial0", o_ts_param.para_val, sizeof(o_ts_param.para_val));
		BCGetStringFieldByName(handle, row, "snote", o_ts_param.note, sizeof(o_ts_param.note));
	};

	memset(&o_ts_param, 0, sizeof(o_ts_param));
	BCResetHandle(handle);
	BCSetRequestType(handle, 851217);
	BCSetStringFieldByName(handle, 0, "scust_no", g_conf.oper.code);
	BCSetStringFieldByName(handle, 0, "sstatus0", "2");
	BCSetStringFieldByName(handle, 0, "sserial2", seatid);
	BCSetStringFieldByName(handle, 0, "sserial1", paraid);
	BCSetStringFieldByName(handle, 0, "sstatus1", "0");

	return MyBCRequest(handle, fetcher);
}