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
///* ���룺����Ա		input:scust_no
//�ͻ���		sholder_ac_no
//���ױ���		sholder_ac_no2
//����������		smarket_code
//��������		sstatus2
//��Լ��		scust_auth
//�걨����		lvol0
//�걨�۸�		damt0
//��ƽ���		sstatus3
//Ͷ�����		sstatus4
//ϯλ��		sserial2
//�µ�������		smain_flag2
//ί�з�ʽ		sholder_type
//����		scurrency_type
//ǿƽ��־		sstatus0
//��������		scust_type
//�ɽ�����		scust_type2
//ֹ��ֹӯ��		damt3
//˽����Ϣ		sstation1
//˽�б�ʶ		semp_no
//˽����ˮ��		scust_limit
//ί�е�����		sstatus1
//���ƽ�񵥱��		smarket_code2
//��С�ɽ����� */		lvol8

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
	BCSetStringFieldByName(handle, 0, "sstatus3", offset_flag);//��ƽ
	BCSetStringFieldByName(handle, 0, "sstatus4", "0");//Ͷ��
	BCSetStringFieldByName(handle, 0, "smain_flag2", "1");//�µ�������
	BCSetStringFieldByName(handle, 0, "sholder_type", "1");//ί�з�ʽ ��̨ί��
	BCSetStringFieldByName(handle, 0, "scurrency_type", "1");//scurrency_type = '', ����
	BCSetStringFieldByName(handle, 0, "sstatus0", "0");//ǿƽ
	BCSetStringFieldByName(handle, 0, "scust_type", "0");//�������� �޼�
														 //BCSetStringFieldByName(handle, 0, "scust_type", PriceType);//�������� �޼�
	BCSetStringFieldByName(handle, 0, "scust_type2", "0");//�ɽ����� //'0', 'GFD������Ч'
	BCSetStringFieldByName(handle, 0, "sstatus1", "A");	//sstatus1 = '', ί�е�����
	BCSetIntFieldByName(handle, 0, "lvol8", vol);//lvol8 = '', ��С�ɽ�����

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
///* ������ͻ���				output:sholder_ac_no
//�ͻ�����				sname
//������				smarket_code
//Ʒ��				sstock_code
//������				sdate0
//��ƽ�������ʣ�����				damt2
//��ƽ�������ʣ���������				damt3
//��ƽ���ӷ���1������				damt4
//��ƽ���ӷ���1����������				damt5
//��ƽ���ӷ���2������				damt6
//��ƽ���ӷ���2����������				damt7
//ƽ���������ʣ�����				damt8
//ƽ���������ʣ���������				damt9
//ƽ�񸽼ӷ���1������				damt10
//ƽ�񸽼ӷ���1����������				damt11
//ƽ�񸽼ӷ���2������				damt12
//ƽ�񸽼ӷ���2����������				damt13
//�����������ʣ�����				damt14
//�����������ʣ���������				damt15
//��ֵ��ƽ�������ʣ�����				damt16
//��ֵ��ƽ�������ʣ���������				damt17
//��ֵ��ƽ���ӷ���1(�����)				damt18
//��ֵ��ƽ���ӷ���1����������				damt19
//��ֵ��ƽ���ӷ���2(�����)				damt20
//��ֵ��ƽ���ӷ���2����������				damt21
//��ֵƽ���������ʣ�����				damt22
//��ֵƽ���������ʣ���������				damt23
//��ֵƽ�񸽼ӷ���1(�����)				damt24
//��ֵƽ�񸽼ӷ���1����������				damt25
//��ֵƽ�񸽼ӷ���2(�����)				damt26
//��ֵƽ�񸽼ӷ���2����������				damt27
//��ֵ�����������ʣ�����				damt28
//��ֵ�����������ʣ���������				damt29
//������ƽ�������ʣ�����				damt30
//������ƽ�������ʣ���������				damt31
//����ƽ���������ʣ�����				damt32
//����ƽ���������ʣ���������				damt33
//���������������ʣ�����				damt0
//���������������ʣ��������� */				damt1

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


///* ���룺����Ա			input:scust_no
//������־�����ӡ��޸ġ�ɾ����			sstatus0
//�ͻ���			sholder_ac_no
//�ͻ���			sserial1
//������			smarket_code
//Ʒ��			sstock_code
//Ͷ����֤�𰴽����ȡ			damt2
//Ͷ����֤���ʰ�������ȡ			damt3
//��ֵ��֤�𰴽����ȡ			damt4
//��ֵ��֤���ʰ�������ȡ			damt5
//��˾��׼			sstatus1
//������֤�𰴽����ȡ			damt6
//������֤���ʰ�������ȡ */			damt7

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