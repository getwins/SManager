#include "stdafx.h"
#include "CommBCRequest.h"
#include "SManager.h"
#include <algorithm>

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
	BCSetStringFieldByName(handle, 0, "sorder2", g_cfg.oper_code);
	std::string::iterator it = std::find_if(inst.begin(), inst.end(), ::isdigit);
	std::string product(inst.begin(), it);
	std::string delivery(it, inst.end());
	BCSetStringFieldByName(handle, 0, "sstock_code", (char*)product.c_str());
	BCSetStringFieldByName(handle, 0, "sdate0", (char*)delivery.c_str());

	memset(&o_md, 0, sizeof(o_md));

	return MyBCRequest(handle, fetcher, BCMSG_NOR_NOMB);
}

//
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

BCResult BCRequestOrderInsert_854002(BCHANDLE handle, char *cust_no, char *inst, char *exchangeid, 
	char *offset_flag, char *direction,
	double price, int vol)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 854002);

	BCSetStringFieldByName(handle, 0, "smarket_code", exchangeid);
	BCSetStringFieldByName(handle, 0, "scust_auth", inst);
	BCSetIntFieldByName(handle, 0, "lvol0", vol);
	BCSetStringFieldByName(handle, 0, "sstatus2", direction);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);
	BCSetDoubleFieldByName(handle, 0, "damt0", price);
	BCSetStringFieldByName(handle, 0, "sstatus3", offset_flag);//开平
	BCSetStringFieldByName(handle, 0, "sstatus4", "0");//投保
	BCSetStringFieldByName(handle, 0, "smain_flag2", "1");//下单人类型
	BCSetStringFieldByName(handle, 0, "sholder_type", "1");//委托方式 柜台委托
	BCSetStringFieldByName(handle, 0, "scurrency_type", "1");//scurrency_type = '', 币种
	BCSetStringFieldByName(handle, 0, "sstatus0", "0");//强平
	BCSetStringFieldByName(handle, 0, "scust_type", "0");//定单类型 限价
														 //BCSetStringFieldByName(handle, 0, "scust_type", PriceType);//定单类型 限价
	BCSetStringFieldByName(handle, 0, "scust_type2", "0");//成交属性 //'0', 'GFD当日有效'
	BCSetStringFieldByName(handle, 0, "sstatus1", "A");	//sstatus1 = '', 委托单类型
	BCSetIntFieldByName(handle, 0, "lvol8", vol);//lvol8 = '', 最小成交数量

	return MyBCRequest(handle, FetchRowFunc_t(), BCMSG_NOR_NOMB);
}

BCResult BCRequestOffsetOrderInsert(BCHANDLE handle, cust_position_st posi, const inst_market_st &m)
{
	return BCRequestOrderInsert_854002(handle, posi.cust_no, posi.instrumentid, posi.exchangeid, "1",
		strcmp(posi.direction, "0") ? "0" : "1",
		strcmp(posi.direction, "0") ? m.upperlimit_price : m.lowerlimit_price ,
		posi.can_offset_vol);
}

BCResult BCRequestOffsetOrderInsert(BCHANDLE handle, cust_td_position_detail_st posi_detail, const inst_market_st &m)
{
	return BCRequestOrderInsert_854002(handle, posi_detail.cust_no, posi_detail.instrumentid,
		posi_detail.exchangeid, "1",
		strcmp(posi_detail.direction, "0") ? "0" : "1",
		strcmp(posi_detail.direction, "0") ? m.upperlimit_price : m.lowerlimit_price,
		posi_detail.volume);
}

BCResult BCRequestCustClassInfo_851028(BCHANDLE handle, int level, std::vector<cust_class_st> &o_cust_classes)
{
	FetchRowFunc_t fetcher = [&](BCHANDLE handle, int row)
	{
		cust_class_st cc = { 0 };
		BCGetIntFieldByName(handle, row, "lvol0", &cc.level);
		BCGetStringFieldByName(handle, row, "sserial1", cc.subclass_no, sizeof(cc.subclass_no));
		BCGetStringFieldByName(handle, row, "sbank_acc", cc.subclass_name, sizeof(cc.subclass_name));
		BCGetStringFieldByName(handle, row, "sbank_acc2", cc.level_name, sizeof(cc.level_name));
		BCGetIntFieldByName(handle, row, "lvol1", &cc.parent_level);
		BCGetStringFieldByName(handle, row, "sorder0", cc.parent_subclass_no, sizeof(cc.parent_subclass_no));
		BCGetStringFieldByName(handle, row, "scert_no", cc.parent_subclass_name, sizeof(cc.parent_subclass_name));
		BCGetIntFieldByName(handle, row, "lvol2", &cc.id);
		o_cust_classes.push_back(cc);
	};

	o_cust_classes.clear();
	BCResetHandle(handle);
	BCSetRequestType(handle, 851028);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetIntFieldByName(handle, 0, "lvol0", level);
	BCSetStringFieldByName(handle, 0, "sstatus0", "1");
	return MyBCRequest(handle, fetcher, BCMSG_ERR_TO_OP);
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
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);

	return MyBCRequest(handle, fetcher, BCMSG_ERR_TO_OP);
}

BCResult BCRequestRC_860000(BCHANDLE handle, char *cust_no, char *classflag, char *type, std::vector<rc_desc_st> &o_rcs)
{
	FetchRowFunc_t fetcher = [&](BCHANDLE handle, int row)
	{
		rc_desc_st rc = { 0 };
		BCGetStringFieldByName(handle, row, "sholder_ac_no", rc.cust_no, sizeof(rc.cust_no));
		BCGetStringFieldByName(handle, row, "sserial0", rc.classflag, sizeof(rc.classflag));
		BCGetStringFieldByName(handle, row, "sserial1", rc.type, sizeof(rc.type));
		BCGetStringFieldByName(handle, row, "ssectypes", rc.paramlist, sizeof(rc.paramlist));
		o_rcs.push_back(rc);
	};
	BCResetHandle(handle);
	BCSetRequestType(handle, 860000);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);
	BCSetStringFieldByName(handle, 0, "sserial0", classflag);
	BCSetStringFieldByName(handle, 0, "sserial1", type);
	return MyBCRequest(handle, fetcher, BCMSG_ERR_TO_OP);
}

BCResult BCRequestManageCustRC_860001(BCHANDLE handle, char *manage_flag, char *cust_no, char *classflag, char *type, char *param_list)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 860001);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sstatus0", manage_flag);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);
	BCSetStringFieldByName(handle, 0, "sserial0", classflag);
	BCSetStringFieldByName(handle, 0, "sserial1", type);
	BCSetStringFieldByName(handle, 0, "ssectypes", param_list);
	
	return MyBCRequest(handle);
}

BCResult BCRequestQryOperPerm_860003(BCHANDLE handle, char *oper_code, char *type, std::vector<oper_sm_perm_st> &o_perms)
{
	FetchRowFunc_t fetcher = [&](BCHANDLE handle, int row)
	{
		oper_sm_perm_st perm = { 0 };
		BCGetStringFieldByName(handle, row, "scust_no", perm.cust_no, sizeof(perm.cust_no));
		BCGetStringFieldByName(handle, row, "sserial0", perm.type, sizeof(perm.type));
		BCGetStringFieldByName(handle, row, "ssectypes", perm.param, sizeof(perm.param));
		o_perms.push_back(perm);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 860003);
	BCSetStringFieldByName(handle, 0, "scust_no2", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "scust_no", oper_code);
	BCSetStringFieldByName(handle, 0, "sserial0", type);
	BCSetStringFieldByName(handle, 0, "ssectypes", "");

	return MyBCRequest(handle, fetcher);
}

BCResult BCRequestSetCustCommissionRate_851304(BCHANDLE handle, char *flag, cust_commission_rate_st &ccr)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 851304);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sstatus0", flag);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", ccr.cust_no);
	BCSetStringFieldByName(handle, 0, "sstatus2", "0");
	BCSetStringFieldByName(handle, 0, "smarket_code", ccr.exchangeid);
	BCSetStringFieldByName(handle, 0, "sstock_code", ccr.productid);
	BCSetStringFieldByName(handle, 0, "sdate0", ccr.delivery_date);
	BCSetDoubleFieldByName(handle, 0, "damt2", ccr.commission_rate_by_money);
	BCSetDoubleFieldByName(handle, 0, "damt3", ccr.commission_rate_by_volume);
	BCSetDoubleFieldByName(handle, 0, "damt8", ccr.offset_td_commission_rate_by_money);
	BCSetDoubleFieldByName(handle, 0, "damt9", ccr.offset_td_commission_rate_by_volume);
	BCSetDoubleFieldByName(handle, 0, "damt14", ccr.commission_rate_by_money);
	BCSetDoubleFieldByName(handle, 0, "damt15", ccr.commission_rate_by_volume);

	BCSetDoubleFieldByName(handle, 0, "damt16", ccr.commission_rate_by_money);
	BCSetDoubleFieldByName(handle, 0, "damt17", ccr.commission_rate_by_volume);
	BCSetDoubleFieldByName(handle, 0, "damt22", ccr.commission_rate_by_money);
	BCSetDoubleFieldByName(handle, 0, "damt23", ccr.commission_rate_by_volume);
	BCSetDoubleFieldByName(handle, 0, "damt28", ccr.commission_rate_by_money);
	BCSetDoubleFieldByName(handle, 0, "damt29", ccr.commission_rate_by_volume);

	BCSetDoubleFieldByName(handle, 0, "damt30", ccr.commission_rate_by_money);
	BCSetDoubleFieldByName(handle, 0, "damt31", ccr.commission_rate_by_volume);
	BCSetDoubleFieldByName(handle, 0, "damt32", ccr.commission_rate_by_money);
	BCSetDoubleFieldByName(handle, 0, "damt33", ccr.commission_rate_by_volume);
	BCSetDoubleFieldByName(handle, 0, "damt0", ccr.commission_rate_by_money);
	BCSetDoubleFieldByName(handle, 0, "damt1", ccr.commission_rate_by_volume);

	return MyBCRequest(handle);
}

//
///* 输出：客户号				output:sholder_ac_no
//客户名称				sname
//交易所				smarket_code
//品种				sstock_code
//交割期				sdate0
//开平手续费率（按金额）				damt2
//开平手续费率（按手数）				damt3
//开平附加费率1（按金额）				damt4
//开平附加费率1（按手数）				damt5
//开平附加费率2（按金额）				damt6
//开平附加费率2（按手数）				damt7
//平今手续费率（按金额）				damt8
//平今手续费率（按手数）				damt9
//平今附加费率1（按金额）				damt10
//平今附加费率1（按手数）				damt11
//平今附加费率2（按金额）				damt12
//平今附加费率2（按手数）				damt13
//交割手续费率（按金额）				damt14
//交割手续费率（按手数）				damt15
//保值开平手续费率（按金额）				damt16
//保值开平手续费率（按手数）				damt17
//保值开平附加费率1(按金额)				damt18
//保值开平附加费率1（按手数）				damt19
//保值开平附加费率2(按金额)				damt20
//保值开平附加费率2（按手数）				damt21
//保值平今手续费率（按金额）				damt22
//保值平今手续费率（按手数）				damt23
//保值平今附加费率1(按金额)				damt24
//保值平今附加费率1（按手数）				damt25
//保值平今附加费率2(按金额)				damt26
//保值平今附加费率2（按手数）				damt27
//保值交割手续费率（按金额）				damt28
//保值交割手续费率（按手数）				damt29
//套利开平手续费率（按金额）				damt30
//套利开平手续费率（按手数）				damt31
//套利平今手续费率（按金额）				damt32
//套利平今手续费率（按手数）				damt33
//套利交割手续费率（按金额）				damt0
//套利交割手续费率（按手数） */				damt1

BCResult BCRequestQryCustCommissionRate_851305(BCHANDLE handle, char *cust_no, std::vector<cust_commission_rate_st> &o_ccrs)
{
	FetchRowFunc_t fetcher = [&](BCHANDLE handle, int row)
	{
		cust_commission_rate_st ccr = { 0 };
		BCGetStringFieldByName(handle, row, "sholder_ac_no", ccr.cust_no, sizeof(ccr.cust_no));
		BCGetStringFieldByName(handle, row, "smarket_code", ccr.exchangeid, sizeof(ccr.exchangeid));
		BCGetStringFieldByName(handle, row, "sstock_code", ccr.productid, sizeof(ccr.productid));
		BCGetStringFieldByName(handle, row, "sdate0", ccr.delivery_date, sizeof(ccr.delivery_date));
		BCGetDoubleFieldByName(handle, row, "damt2", &ccr.commission_rate_by_money);
		BCGetDoubleFieldByName(handle, row, "damt3", &ccr.commission_rate_by_volume);
		BCGetDoubleFieldByName(handle, row, "damt8", &ccr.offset_td_commission_rate_by_money);
		BCGetDoubleFieldByName(handle, row, "damt9", &ccr.offset_td_commission_rate_by_volume);

		o_ccrs.push_back(ccr);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 851305);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);

	o_ccrs.clear();

	return MyBCRequest(handle, fetcher, BCMSG_TOOP);
}


///* 输入：操作员			input:scust_no
//操作标志（增加、修改、删除）			sstatus0
//客户号			sholder_ac_no
//客户类			sserial1
//交易所			smarket_code
//品种			sstock_code
//投机保证金按金额收取			damt2
//投机保证金率按手数收取			damt3
//保值保证金按金额收取			damt4
//保值保证金率按手数收取			damt5
//公司标准			sstatus1
//套利保证金按金额收取			damt6
//套利保证金率按手数收取 */			damt7

BCResult BCRequestSetCustMarginRate_851312(BCHANDLE handle, char *flag, cust_margin_rate_st &cmr)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 851312);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cmr.cust_no);
	BCSetStringFieldByName(handle, 0, "sstatus0", flag);
	BCSetStringFieldByName(handle, 0, "smarket_code", cmr.exchangeid);
	BCSetStringFieldByName(handle, 0, "sstock_code", cmr.productid);
	BCSetDoubleFieldByName(handle, 0, "damt2", cmr.margin_rate_by_money);
	BCSetDoubleFieldByName(handle, 0, "damt3", cmr.margin_rate_by_volume);
	BCSetDoubleFieldByName(handle, 0, "damt4", cmr.margin_rate_by_money);
	BCSetDoubleFieldByName(handle, 0, "damt5", cmr.margin_rate_by_volume);
	BCSetDoubleFieldByName(handle, 0, "damt6", cmr.margin_rate_by_money);
	BCSetDoubleFieldByName(handle, 0, "damt7", cmr.margin_rate_by_volume);
	return MyBCRequest(handle);
}

BCResult BCRequestQryCustMarginRate_851313(BCHANDLE handle, char *cust_no, std::vector<cust_margin_rate_st> &o_cmrs)
{
	FetchRowFunc_t fetcher = [&](BCHANDLE handle, int row)
	{
		cust_margin_rate_st cmr = { 0 };
		BCGetStringFieldByName(handle, row, "sholder_ac_no", cmr.cust_no, sizeof(cmr.cust_no));
		BCGetStringFieldByName(handle, row, "smarket_code", cmr.exchangeid, sizeof(cmr.exchangeid));
		BCGetStringFieldByName(handle, row, "sstock_code", cmr.productid, sizeof(cmr.productid));
		//BCGetStringFieldByName(handle, row, "sdate0", cmr.delivery_date, sizeof(cmr.delivery_date));
		BCGetDoubleFieldByName(handle, row, "damt2", &cmr.margin_rate_by_money);
		BCGetDoubleFieldByName(handle, row, "damt3", &cmr.margin_rate_by_volume);
		//BCGetDoubleFieldByName(handle, row, "damt8", &cmr.offset_td_commission_rate_by_money);
		//BCGetDoubleFieldByName(handle, row, "damt9", &cmr.offset_td_commission_rate_by_volume);
		
		o_cmrs.push_back(cmr);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 851313);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);

	o_cmrs.clear();

	return MyBCRequest(handle, fetcher, BCMSG_TOOP);
}


BCResult BCRequestSetOperator_851200(BCHANDLE handle, char *flag, oper_basic_st ob)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 851200);
	BCSetStringFieldByName(handle, 0, "scust_no2", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sname", ob.oper_name);
	BCSetStringFieldByName(handle, 0, "sstatus0", flag);
	BCSetStringFieldByName(handle, 0, "scust_no",ob.oper_code);
	BCSetStringFieldByName(handle, 0, "sstatus1", ob.node_limit);
	BCSetStringFieldByName(handle, 0, "sstatus2", ob.access_limit);
	BCSetStringFieldByName(handle, 0, "sstatus3", ob.oper_status);
	BCSetStringFieldByName(handle, 0, "usset0", ob.menu_perm);
	BCSetStringFieldByName(handle, 0, "usset1", ob.func_perm);
	
	return MyBCRequest(handle);
}

//输出：操作员代码		output : scust_no
//操作员姓名		sname
//电话		sphone
//手机		sphone3
//地址		saddr
//邮编		spost_code
//电子邮件		semail
//是否限制访问站点		sstatus1
//是否限制访问		sstatus2
//操作员状态		sstatus3
//菜单权限		usset0
//功能权限		usset1
//程序标识		scusttypes
//权限模板代码		sbank_code
//菜单权限2		usset2
//功能权限3 		usset3

BCResult BCRequestQryOperator_851201(BCHANDLE handle, char *oper_code, oper_basic_st &o_oper)
{
	FetchRowFunc_t fetcher = [&](BCHANDLE handle, int row)
	{
		BCGetStringFieldByName(handle, row, "scust_no", o_oper.oper_code, sizeof(o_oper.oper_code));
		BCGetStringFieldByName(handle, row, "sname", o_oper.oper_name, sizeof(o_oper.oper_name));
		BCGetStringFieldByName(handle, row, "sstatus1", o_oper.node_limit, sizeof(o_oper.node_limit));
		BCGetStringFieldByName(handle, row, "sstatus2", o_oper.access_limit, sizeof(o_oper.access_limit));
		BCGetStringFieldByName(handle, row, "sstatus3", o_oper.oper_status, sizeof(o_oper.oper_status));
		BCGetStringFieldByName(handle, row, "usset0", o_oper.menu_perm, sizeof(o_oper.menu_perm));
		BCGetStringFieldByName(handle, row, "usset1", o_oper.func_perm, sizeof(o_oper.func_perm));
		BCGetStringFieldByName(handle, row, "sbank_code", o_oper.app_flag, sizeof(o_oper.app_flag));
		BCGetStringFieldByName(handle, row, "usset2", o_oper.menu_perm2, sizeof(o_oper.menu_perm2));
		BCGetStringFieldByName(handle, row, "usset3", o_oper.func_perm3, sizeof(o_oper.func_perm3));
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 851201);
	BCSetStringFieldByName(handle, 0, "scust_no2", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "scust_no", oper_code);

	return MyBCRequest(handle, fetcher);
}

BCResult BCRequestSetOperCustCorrespond_851206(BCHANDLE handle, char *flag, char *oper_code, char *cust_no)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 851206);
	BCSetStringFieldByName(handle, 0, "scust_no2", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sstatus0", flag);
	BCSetStringFieldByName(handle, 0, "scust_no", oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", cust_no);
	
	return MyBCRequest(handle);
}

/* 输出：操作员，客户号，客户名称，客户类别，客户状态 */
//output:sholder_ac_no2, sholder_ac_no, sname, sserial1, sstatus0
BCResult BCRequestQryOperCustCorrespond_851243(BCHANDLE handle, char *oper_code, std::vector<oper_cust_correspond_st> &o_occs)
{
	FetchRowFunc_t fetcher = [&](BCHANDLE handle, int row)
	{
		oper_cust_correspond_st occ = { 0 };
		BCGetStringFieldByName(handle, row, "sholder_ac_no2", occ.oper_code, sizeof(occ.oper_code));
		BCGetStringFieldByName(handle, row, "sholder_ac_no", occ.cust_no, sizeof(occ.cust_no));
		BCGetStringFieldByName(handle, row, "sname", occ.cust_name, sizeof(occ.cust_name));
		BCGetStringFieldByName(handle, row, "sserial1", occ.cust_class, sizeof(occ.cust_class));
		BCGetStringFieldByName(handle, row, "sstatus0", occ.cust_status, sizeof(occ.cust_status));
		o_occs.push_back(occ);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 851243);
	BCSetStringFieldByName(handle, 0, "scust_no2", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "scust_no", oper_code);
	BCSetStringFieldByName(handle, 0, "sstatus0", "1");

	return MyBCRequest(handle, fetcher);
}