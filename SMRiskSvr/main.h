#pragma once
#include <iostream>
#include <mutex>
#include <boost/date_time/posix_time/posix_time.hpp>

extern std::mutex g_cout_mutex;

#define SAFE_COUT_BEGINE  \
{ \
	std::unique_lock<std::mutex> lkc(g_cout_mutex);\
	std::cout << boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time()) << ","  << std::this_thread::get_id() << ","


#define SAFE_COUT_END   std::endl;}