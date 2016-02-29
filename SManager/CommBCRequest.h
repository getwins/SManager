#pragma once
#include "struct.h"
#include "MyBCRequest.h"
#include <string>

BCResult BCRequestInstMarketData_851503(BCHANDLE handle, std::string inst, inst_market_st &o_md);
BCResult BCRequestOrderInsert_854002(BCHANDLE handle, char *cust_no, char *inst, char *exchangeid, 
	char *offset_flag, char *direction,
	double price, int vol);

BCResult BCRequestOffsetOrderInsert(BCHANDLE handle, cust_position_st posi, const inst_market_st &m);
BCResult BCRequestOffsetOrderInsert(BCHANDLE handle, cust_td_position_detail_st posi_detail, const inst_market_st &m);

BCResult BCRequestCustClassInfo_851028(BCHANDLE handle, int level, std::vector<cust_class_st> &o_cust_classes);
BCResult BCRequestCustBasic_850003(BCHANDLE handle, char *cust_no, cust_basic_st &o_cb);
BCResult BCRequestRC_860000(BCHANDLE handle, char *cust_no, char *classflag, char *type, std::vector<rc_desc_st> &o_rcs);
BCResult BCRequestManageCustRC_860001(BCHANDLE handle, char *manage_flag, char *cust_no, char *classflag, char *type, char *param_list);

BCResult BCRequestQryOperPerm_860003(BCHANDLE handle, char *oper_code, char *type, std::vector<oper_sm_perm_st> &o_perms);

BCResult BCRequestSetCustCommissionRate_851304(BCHANDLE handle, char *flag, cust_commission_rate_st &ccr);
BCResult BCRequestQryCustCommissionRate_851305(BCHANDLE handle, char *cust_no, std::vector<cust_commission_rate_st> &o_ccrs);
BCResult BCRequestSetCustMarginRate_851312(BCHANDLE handle, char *flag, cust_margin_rate_st &cmr);
BCResult BCRequestQryCustMarginRate_851313(BCHANDLE handle, char *cust_no, std::vector<cust_margin_rate_st> &o_cmrs);

BCResult BCRequestSetOperator_851200(BCHANDLE handle, char *flag, oper_basic_st ob);
BCResult BCRequestQryOperator_851201(BCHANDLE handle, char *oper_code, oper_basic_st &o_oper_basic);
BCResult BCRequestSetOperCustCorrespond_851206(BCHANDLE handle, char *flag, char *oper_code, char *cust_no);
BCResult BCRequestQryOperCustCorrespond_851243(BCHANDLE handle, char *oper_code, std::vector<oper_cust_correspond_st> &o_occs);