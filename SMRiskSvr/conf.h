#pragma once

#include <string>
#include <vector>
#include <boost/date_time/posix_time/posix_time.hpp>

struct cfg_exch_clearance_st
{
	std::string exchangeid;
	boost::posix_time::time_duration nigth_clear;
	boost::posix_time::time_duration day_clear;
};

struct cfg_drtp_st
{
	std::string host;
	int port;
	int funcno;
};

struct cfg_oper_st
{
	char code[16];
	char passwd[9];
};

struct cfg_market_st
{
	char brokerid[11];
	char userid[11];
	char passwd[9];
	char front[80];
};

struct config_st
{
	cfg_drtp_st drtp;
	cfg_oper_st oper;
	std::vector<cfg_exch_clearance_st> exch_clrs;
	cfg_market_st market;
};

extern config_st g_conf;

void read_conf();