//#include "stdafx.h"
#include "conf.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

config_st g_conf;

void read_conf()
{
	boost::property_tree::ptree ptree;
	boost::property_tree::read_xml("conf.xml", ptree);
	g_conf.drtp.host = ptree.get<std::string>("configuration.drtp.host");
	g_conf.drtp.port = ptree.get<int>("configuration.drtp.port", 3000);
	g_conf.drtp.funcno = ptree.get<int>("configuration.drtp.funcno", 19900);

	std::string oper_code = ptree.get<std::string>("configuration.oper.code");
	std::string oper_pass = ptree.get<std::string>("configuration.oper.passwd");
	oper_code.copy(g_conf.oper.code, sizeof(g_conf.oper.code));
	oper_pass.copy(g_conf.oper.passwd, sizeof(g_conf.oper.passwd));

	for each (boost::property_tree::ptree::value_type var in ptree.get_child("configuration.clearances"))
	{
		cfg_exch_clearance_st exch_clr;
		std::string tmp;
		//std::cout << var.first << std::endl;
		exch_clr.exchangeid = var.second.get<std::string>("exchangeid");
		tmp = var.second.get<std::string>("night");
		exch_clr.nigth_clear = boost::posix_time::duration_from_string(tmp);
		tmp = var.second.get<std::string>("day");
		exch_clr.day_clear = boost::posix_time::duration_from_string(tmp);
		g_conf.exch_clrs.push_back(exch_clr);
	}

	//std::string mar
	ptree.get<std::string>("configuration.market.brokerid").copy(g_conf.market.brokerid, sizeof(g_conf.market.brokerid));
	ptree.get<std::string>("configuration.market.userid").copy(g_conf.market.userid, sizeof(g_conf.market.userid));
	ptree.get<std::string>("configuration.market.passwd").copy(g_conf.market.passwd, sizeof(g_conf.market.passwd));
	ptree.get<std::string>("configuration.market.front").copy(g_conf.market.front, sizeof(g_conf.market.front));
}