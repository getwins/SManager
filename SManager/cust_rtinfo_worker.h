#pragma once

#include <thread>
class request_thread_group
{
public:
	static void start();
	static void stop();


private:
	request_thread_group();
	~request_thread_group();


private:
	std::shared_ptr<std::thread> cust_list_thr_;
	std::shared_ptr<std::thread> cust_login_info_thr_;
	std::shared_ptr<std::thread> cust_captial_thr_;
	std::shared_ptr<std::thread> cust_td_posi_detail_thr_;
	std::shared_ptr<std::thread> cust_position_thr_;
	//static bool stop_;
};

