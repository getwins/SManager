#pragma once

#include "struct.h"
#include <iostream>
#include <string>

typedef std::vector<cust_position_st> PosiVec_t;
typedef std::map<std::string, PosiVec_t> CustPosi_t;

typedef std::vector<cust_td_position_detail_st> PosiDetailVec_t;
typedef std::map<std::string, PosiDetailVec_t> CustPosiDetail_t;

typedef std::map<std::string, std::vector<commission_template_st>> StrCtMap_t;
typedef std::map<std::string, std::vector<margin_template_st>> StrMtMap_t;

typedef std::vector<cust_trade_st> TradeVec_t;
typedef std::vector<cust_order_st> OrderVec_t;

struct cust_position_id_st
{
	cust_position_id_st(){}
	cust_position_id_st(const std::string custno, const std::string &inst, const std::string &dir, int vol)
		: cust_no(custno)
		, instrumentid(inst)
		, direction(dir)
		, volume(vol){}
	std::string cust_no;
	std::string instrumentid;
	std::string direction;
	int volume;
};

inline cust_position_id_st get_posiid(const cust_position_st &p)
{
	return cust_position_id_st(p.cust_no, p.instrumentid, p.direction, p.positions);
}

inline std::ostream& operator << (std::ostream &os, const cust_position_id_st &posi_id)
{
	return os << posi_id.cust_no << " "
		<< posi_id.instrumentid << " "
		<< posi_id.direction << " "
		<< posi_id.volume;
}

inline std::istream& operator >> (std::istream &is, cust_position_id_st &posi_id)
{
	return is >> posi_id.cust_no >> posi_id.instrumentid >> posi_id.direction >> posi_id.volume;
}

//inline bool operator < (const cust_position_id_st &lhs, const cust_position_id_st &rhs)
//{
//	if (strcmp(lhs.cust_no, rhs.cust_no) >= 0) return false;
//	if (strcmp(lhs.instrumentid, rhs.instrumentid) >= 0) return false;
//	if (strcmp(lhs.direction, rhs.direction) >= 0) return false;
//	if (lhs.volume >= rhs.volume) return false;
//	return true;
//}

inline bool operator == (const cust_position_st &p, const cust_position_id_st &id)
{
	return id.cust_no == p.cust_no && id.instrumentid == p.instrumentid
		&& id.direction == p.direction &&  id.volume == p.positions;
}

struct cust_position_calc_id
{
	std::string cust_no;
	std::string instrumentid;
	std::string direction;
};

inline bool operator < (const cust_position_calc_id &lhs, const cust_position_calc_id &rhs)
{
	if (lhs.cust_no >= rhs.cust_no) return false;
	if (lhs.instrumentid >= rhs.instrumentid) return false;
	if (lhs.direction >= rhs.direction) return false;
	return true;
}

inline double calcProfit(const cust_td_position_detail_st &pd, double LastPrice)
{
	double profit = 0.0;
	if (strcmp(pd.open_date, pd.trade_date) == 0) //½ñ²Ö
	{
		if (strcmp(pd.direction, "0") == 0) //Âò
		{
			profit = (LastPrice - pd.hold_price) * pd.volume * pd.multiple;
		}
		else if (strcmp(pd.direction, "1") == 0) //Âô
		{
			profit = (pd.hold_price - LastPrice) * pd.volume * pd.multiple;
		}
	}
	else //×ò²Ö
	{
		if (strcmp(pd.direction, "0") == 0) //Âò
		{
			profit = (LastPrice - pd.yd_settle_price) * pd.volume * pd.multiple;
		}
		else if (strcmp(pd.direction, "1") == 0) //Âô
		{
			profit = (pd.yd_settle_price - LastPrice) * pd.volume * pd.multiple;
		}
	}
	return profit;
}

inline double calcProfit(const cust_position_st &p, double LastPrice)
{
	double profit = 0.0;
	if (strcmp(p.direction, "0") == 0)
	{
		profit = (LastPrice - p.hold_average_price) * p.positions * p.multiple;
	}
	else if (strcmp(p.direction, "1") == 0)
	{
		profit = (p.hold_average_price - LastPrice) * p.positions * p.multiple;
	}
	return profit;
}

inline double getLastPrice(const PosiVec_t &p, const char *inst, const char *direction)
{
	for (PosiVec_t::const_iterator it = p.begin(); it != p.end(); ++it)
	{
		if (strcmp(it->instrumentid, inst) == 0 && strcmp(it->direction, direction) == 0)
			return it->last_price;
	}
	return 0.0;
}

inline bool operator < (const dict_st &lhs, const dict_st &rhs)
{
	//return lhs.dict_class < rhs.dict_class && strcmp(lhs.dict_code, rhs.dict_code) < 0;
	if (lhs.dict_class < rhs.dict_class)
		return true;
	else if (lhs.dict_class == rhs.dict_class)
		return strcmp(lhs.dict_code, rhs.dict_code) < 0;
	return false;
}

inline bool canCancel(const cust_order_st &o)
{
	return (o.order_status[0] == 'n' || o.order_status[0] == 's' || o.order_status[0] == 'a' || o.order_status[0] == 'p');
}

inline bool operator == (const cust_basic_st &lhs, const cust_basic_st &rhs)
{
	return strcmp(lhs.scust_no, rhs.scust_no) == 0
		&& strcmp(lhs.sname, rhs.sname) == 0
		&& strcmp(lhs.cust_class, rhs.cust_class) == 0
		&& strcmp(lhs.sstatus0, rhs.sstatus0) == 0
		&& strcmp(lhs.card_no, rhs.card_no) == 0
		&& strcmp(lhs.card_type, rhs.card_type) == 0;
}
