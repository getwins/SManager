#include "stdafx.h"
#include "string_identifer.h"

#include <mutex>
#include <boost/bimap.hpp>

static std::mutex g_mutex;

typedef boost::bimap<int, std::string> bm_t;
static bm_t g_bm;

int get_identifer(const std::string &str)
{
	std::unique_lock<std::mutex> lck(g_mutex);
	bm_t::right_iterator rit = g_bm.right.find(str);
	if (rit != g_bm.right.end())
		return rit->second;
	else {
		size_t identifer = g_bm.left.size();
		g_bm.insert(bm_t::value_type(identifer, str));
		return identifer;
	}
}

std::string fetch_string(int identifer)
{
	std::unique_lock<std::mutex> lck(g_mutex);
	bm_t::left_iterator lit = g_bm.left.find(identifer);
	if (lit != g_bm.left.end())
		return lit->second;

	return std::string();
}