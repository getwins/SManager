#pragma once
#include <thread>
//#include <boost/thread.hpp>
#include <string>
#include <map>
#include <condition_variable>
#include <shared_mutex>
#include <set>
#include "struct_helper.h"
#include "nbccclt.h"

class cust_dynamic_info_worker
{
public:
	
public:
	static cust_dynamic_info_worker *instance();

	void start();
	void stop();

	const cust_login_info_st& get_cust_login_info(const std::string &cust_no);
	const cust_capital_st& get_cust_capital(const std::string &cust_no);
	const PosiVec_t& get_cust_position(const std::string &cust_no);
	const PosiDetailVec_t& get_cust_posi_detail(const std::string &cust_no);
	const OrderVec_t& get_cust_order(const std::string &cust_no);
	const TradeVec_t& get_cust_trade(const std::string &cust_no);

	const system_info_st &get_sysinfo();
private:
	cust_dynamic_info_worker();
	~cust_dynamic_info_worker();

	void loop_request_cust_login_info();
	void loop_request_cust_capital();
	void loop_request_cust_basic();
	//void loop_request_cust_position();
	//void loop_request_cust_td_posi_detail();
	//void loop_request_cust_order();
	//void loop_request_cust_trade();

	void request_cust_position(BCHANDLE handle, const std::string &cust_no);
	void request_position_offset_volume(BCHANDLE handle, cust_position_st &p);
	void request_cust_td_posi_detail(BCHANDLE handle, const std::string &cust_no);
	void request_cust_order(BCHANDLE handle, const std::string &cust_no);
	void request_cust_trade(BCHANDLE handle, const std::string &cust_no);

	void request_sysinfo(BCHANDLE handle);

	//boost::shared_ptr<boost::thread> cust_login_info_thr_;
	//boost::shared_ptr<boost::thread> cust_captial_thr_;
	//boost::shared_ptr<boost::thread> cust_basic_thr_;

	std::shared_ptr<std::thread> cust_login_info_thr_;
	std::shared_ptr<std::thread> cust_captial_thr_;
	//std::shared_ptr<std::thread> cust_basic_thr_;

	//std::shared_ptr<std::thread> cust_list_thr_;

	//std::shared_ptr<std::thread> cust_position_thr_;
	//std::shared_ptr<std::thread> cust_td_posi_detail_thr_;
	//std::shared_ptr<std::thread> cust_order_thr_;
	//std::shared_ptr<std::thread> cust_trade_thr_;
	
	//system_info_st sysinfo_;
	//std::map<std::string, cust_dynamic_info_st> info_;
	//std::mutex mutex_;
	//std::mutex si_mutex_;
	//std::condition_variable cv_;
	//std::set<int> force_orderseqs_;
	//std::map
};

