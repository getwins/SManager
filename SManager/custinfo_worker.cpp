#include "stdafx.h"
#include "request_thread_group.h"

#include "nbccclt.h"
#include "SManager.h"

#include <condition_variable>
#include <shared_mutex>


static request_thread_group *singleton = NULL;
static std::condition_variable cv;
static std::mutex mutex;

//bool g_exit = false;

void RequestCustInfoList()
{
	BCHANDLE handle = BCNewHandle(XpackDescribleFile);
	BCResetHandle(handle);
	BCSetRequestType(handle, 851002);

	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);//操作员
	BCSetStringFieldByName(handle, 0, "sstatus1", "0");
	BCSetStringFieldByName(handle, 0, "sstatus0", "1"); //客户性质

	MyBCCallRequest(handle);

	int record;
	BCGetRecordCount(handle, &record);

	for (int i = 0; i < record; ++i)
	{
		char cust_open_oper[11] = { 0 };
		BCGetStringFieldByName(handle, i, "scust_no", cust_open_oper, 11);
		if (strcmp(cust_open_oper, g_cfg.oper_code) == 0) {
			custinfo_st custinfo = { 0 };
			BCGetStringFieldByName(handle, i, "sholder_ac_no", custinfo.scust_no, sizeof(custinfo.scust_no));
			BCGetStringFieldByName(handle, i, "sname", custinfo.sname, sizeof(custinfo.sname));
			BCGetStringFieldByName(handle, i, "sstatus3", custinfo.sstatus0, sizeof(custinfo.sstatus0));
			g_map_custinfo.insert(std::make_pair(custinfo.scust_no, custinfo));
		}
	}

	while (BCHaveNextPack(handle)) {
		int errcode = 0;
		char errmsg[256] = { 0 };
		BCCallNext(handle, 10000, &errcode, errmsg);
		BCGetRecordCount(handle, &record);
		for (int i = 0; i < record; ++i)
		{
			char cust_open_oper[11] = { 0 };
			BCGetStringFieldByName(handle, i, "scust_no", cust_open_oper, 11);
			if (strcmp(cust_open_oper, g_cfg.oper_code) == 0) {
				custinfo_st custinfo = { 0 };
				BCGetStringFieldByName(handle, i, "sholder_ac_no", custinfo.scust_no, sizeof(custinfo.scust_no));
				BCGetStringFieldByName(handle, i, "sname", custinfo.sname, sizeof(custinfo.sname));
				BCGetStringFieldByName(handle, i, "sstatus3", custinfo.sstatus0, sizeof(custinfo.sstatus0));
				g_map_custinfo.insert(std::make_pair(custinfo.scust_no, custinfo));
			}
		}
	}
	BCDeleteHandle(handle);
}

void LoopRequestCustLoginInfo()
{

	BCHANDLE handle = BCNewHandle(XpackDescribleFile);
	
	for (;;)
	{
		for (iter_t it = g_map_custinfo.begin(); it != g_map_custinfo.end(); ++it)
		{
			BCResetHandle(handle);
			BCSetRequestType(handle, 854093);
			BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
			BCSetStringFieldByName(handle, 0, "sholder_ac_no", it->second.scust_no);
			MyBCCallRequest(handle);

			char sholder_ac_no[16] = { 0 };
			BCGetStringFieldByName(handle, 0, "sholder_ac_no", sholder_ac_no, sizeof(sholder_ac_no));
			assert(strcmp(sholder_ac_no, it->second.scust_no) == 0);

			BCGetStringFieldByName(handle, 0, "status0", it->second.login_info.login_status,
				sizeof(it->second.login_info.login_status));
			BCGetStringFieldByName(handle, 0, "sholder_type", it->second.login_info.cur_app_name,
				sizeof(it->second.login_info.cur_app_name));
			BCGetStringFieldByName(handle, 0, "sphone2", it->second.login_info.cur_login_mac,
				sizeof(it->second.login_info.cur_login_mac));
			BCGetStringFieldByName(handle, 0, "sphone3", it->second.login_info.cur_login_ip,
				sizeof(it->second.login_info.cur_login_ip));
			BCGetStringFieldByName(handle, 0, "stime2", it->second.login_info.cur_login_time,
				sizeof(it->second.login_info.cur_login_time));
			BCGetStringFieldByName(handle, 0, "sdate2", it->second.login_info.cur_login_time,
				sizeof(it->second.login_info.cur_login_date));

		}
		//std::this_thread::sleep_for(std::chrono::seconds(20));
		std::unique_lock<std::mutex> lck(mutex);
		if (cv.wait_for(lck, std::chrono::seconds(20)) != std::cv_status::timeout)
			return;
		//::PostMessage()
	}
	BCDeleteHandle(handle);
}


void LoopRequestCustCaptial()
{
	//g_thr_custinfo->join();
	//static int loop = 0;

	BCHANDLE handle = BCNewHandle(XpackDescribleFile);
	//if (!g_exit)
	for (int loop = 0; ;loop++)
	{
		for (iter_t it = g_map_custinfo.begin(); it != g_map_custinfo.end(); ++it)
		{
			if (loop != 0 && it->second.login_info.login_status[0] == '0')
				continue;

			BCResetHandle(handle);
			BCSetRequestType(handle, 854196);
			BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
			BCSetStringFieldByName(handle, 0, "sholder_ac_no", it->second.scust_no);
			BCSetStringFieldByName(handle, 0, "scurrency_type", ""); //
			MyBCCallRequest(handle);


			char sholder_ac_no[16] = { 0 };
			BCGetStringFieldByName(handle, 0, "sholder_ac_no", sholder_ac_no, sizeof(sholder_ac_no));
			assert(strcmp(sholder_ac_no, it->second.scust_no) == 0);

			BCGetStringFieldByName(handle, 0, "scurrency_type", it->second.captial.scurrency_type,
				sizeof(it->second.captial.scurrency_type));
			BCGetDoubleFieldByName(handle, 0, "damt3", &it->second.captial.yd_balance);
			BCGetDoubleFieldByName(handle, 0, "damt26", &it->second.captial.available);
			BCGetDoubleFieldByName(handle, 0, "damt23", &it->second.captial.margin);
			BCGetDoubleFieldByName(handle, 0, "damt29", &it->second.captial.buy_frzn_margin);
			BCGetDoubleFieldByName(handle, 0, "damt30", &it->second.captial.sell_frzn_margin);
			BCGetDoubleFieldByName(handle, 0, "damt6", &it->second.captial.commission);
			BCGetDoubleFieldByName(handle, 0, "damt33", &it->second.captial.frzn_commission);
			BCGetDoubleFieldByName(handle, 0, "damt11", &it->second.captial.offset_profit);
			BCGetDoubleFieldByName(handle, 0, "damt8", &it->second.captial.float_profit);
			BCGetDoubleFieldByName(handle, 0, "damt28", &it->second.captial.capital);
			BCGetDoubleFieldByName(handle, 0, "damt25", &it->second.captial.frzn_moeny);
			BCGetStringFieldByName(handle, 0, "sstatus3", it->second.captial.risk_level,
				sizeof(it->second.captial.risk_level));
			BCGetDoubleFieldByName(handle, 0, "damt22", &it->second.captial.ris_degree);

		}
		//std::this_thread::sleep_for(std::chrono::seconds(2));
		std::unique_lock<std::mutex> lck(mutex);
		if (cv.wait_for(lck, std::chrono::seconds(20)) != std::cv_status::timeout)
			return;
	}
	BCDeleteHandle(handle);
}

void LoopRequestTdPositionDetail()
{
	//g_thr_custinfo->join();
	//static int loop = 0;

	BCHANDLE handle = BCNewHandle(XpackDescribleFile);
	//if (!g_exit)
	for (int loop = 0; ; loop++)
	{
		for (iter_t it = g_map_custinfo.begin(); it != g_map_custinfo.end(); ++it)
		{
			if (loop != 0 && it->second.login_info.login_status[0] == '0')
				continue;

			it->second.posi_detail.clear();
			BCResetHandle(handle);
			BCSetRequestType(handle, 854186);
			BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
			BCSetStringFieldByName(handle, 0, "sholder_ac_no", it->second.scust_no);
			//BCSetStringFieldByName(handle, 0, "sbank_acc", ""); //合约代码
			MyBCCallRequest(handle);
			int record = 0;
			BCGetRecordCount(handle, &record);
			for (int i = 0; i < record; i++)
			{
				cust_td_position_detail_st pd = { 0 };
				char sholder_ac_no[16] = { 0 };
				BCGetStringFieldByName(handle, i, "sholder_ac_no", sholder_ac_no, sizeof(sholder_ac_no));
				assert(strcmp(sholder_ac_no, it->second.scust_no) == 0);

				int record = 0;
				BCGetRecordCount(handle, &record);
				for (int i = 0; i < record; i++)
				{
					BCGetIntFieldByName(handle, i, "lvol9", &pd.trade_seq);
					BCGetStringFieldByName(handle, i, "sholder_ac_no", pd.exchangeid,
						sizeof(pd.exchangeid));
					BCGetStringFieldByName(handle, i, "sstatus3", pd.scurrency_type,
						sizeof(pd.scurrency_type));
					BCGetStringFieldByName(handle, i, "sholder_ac_no2", pd.tradeid,
						sizeof(pd.tradeid));
					BCGetStringFieldByName(handle, i, "sstock_code", pd.product,
						sizeof(pd.product));
					BCGetStringFieldByName(handle, i, "sdate1", pd.delivery_date,
						sizeof(pd.delivery_date));
					BCGetStringFieldByName(handle, i, "sstatus2", pd.direction,
						sizeof(pd.direction));
					BCGetStringFieldByName(handle, i, "sstatus4", pd.hedge_flag,
						sizeof(pd.hedge_flag));
					BCGetIntFieldByName(handle, i, "lvol8", &pd.volume);
					BCGetIntFieldByName(handle, i, "lvol10", &pd.multiple);
					BCGetDoubleFieldByName(handle, i, "damt21", &pd.hold_price);
					BCGetDoubleFieldByName(handle, i, "damt26", &pd.open_price);
					BCGetDoubleFieldByName(handle, i, "damt1", &pd.hold_margin);
					BCGetDoubleFieldByName(handle, i, "damt8", &pd.td_settle_price);
					BCGetDoubleFieldByName(handle, i, "damt7", &pd.yd_settle_price);
					it->second.posi_detail.push_back(pd);
				}

				while (BCHaveNextPack(handle)) {
					int errcode = 0;
					char errmsg[256] = { 0 };
					BCCallNext(handle, 10000, &errcode, errmsg);
					BCGetRecordCount(handle, &record);
					for (int i = 0; i < record; ++i)
					{
						cust_td_position_detail_st pd = { 0 };
						BCGetIntFieldByName(handle, i, "lvol9", &pd.trade_seq);
						BCGetStringFieldByName(handle, i, "sholder_ac_no", pd.exchangeid,
							sizeof(pd.exchangeid));
						BCGetStringFieldByName(handle, i, "sstatus3", pd.scurrency_type,
							sizeof(pd.scurrency_type));
						BCGetStringFieldByName(handle, i, "sholder_ac_no2", pd.exchangeid,
							sizeof(pd.tradeid));
						BCGetStringFieldByName(handle, i, "sstock_code", pd.product,
							sizeof(pd.product));
						BCGetStringFieldByName(handle, i, "sdate1", pd.delivery_date,
							sizeof(pd.delivery_date));
						BCGetStringFieldByName(handle, i, "sstatus2", pd.direction,
							sizeof(pd.direction));
						BCGetStringFieldByName(handle, i, "sstatus4", pd.hedge_flag,
							sizeof(pd.hedge_flag));
						BCGetIntFieldByName(handle, i, "lvol8", &pd.volume);
						BCGetIntFieldByName(handle, i, "lvol10", &pd.multiple);
						BCGetDoubleFieldByName(handle, i, "damt21", &pd.hold_price);
						BCGetDoubleFieldByName(handle, i, "damt26", &pd.open_price);
						BCGetDoubleFieldByName(handle, i, "damt1", &pd.hold_margin);
						BCGetDoubleFieldByName(handle, i, "damt8", &pd.td_settle_price);
						BCGetDoubleFieldByName(handle, i, "damt7", &pd.yd_settle_price);
						it->second.posi_detail.push_back(pd);
					}
				}

			}

		}
		//std::this_thread::sleep_for(std::chrono::seconds(2));
		std::unique_lock<std::mutex> lck(mutex);
		if (cv.wait_for(lck, std::chrono::seconds(20)) != std::cv_status::timeout)
			return;
	}
	BCDeleteHandle(handle);
}

void LoopRequestCustPosition()
{
	//g_thr_custinfo->join();
	//static int loop = 0;

	BCHANDLE handle = BCNewHandle(XpackDescribleFile);
	//if (!g_exit)
	for (int loop = 0; ; loop++)
	{
		for (iter_t it = g_map_custinfo.begin(); it != g_map_custinfo.end(); ++it)
		{
			if (loop != 0 && it->second.login_info.login_status[0] == '0')
				continue;

			it->second.posi_detail.clear();
			BCResetHandle(handle);
			BCSetRequestType(handle, 854048);
			BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
			BCSetStringFieldByName(handle, 0, "sholder_ac_no", it->second.scust_no);
			BCSetStringFieldByName(handle, 0, "sstatus3", "1"); //是否不输出登记标记
			BCSetStringFieldByName(handle, 0, "sstatus0", "0"); //汇总标记
																//BCSetStringFieldByName(handle, 0, "sbank_acc", ""); //合约代码
			MyBCCallRequest(handle);
			int record = 0;
			BCGetRecordCount(handle, &record);
			for (int i = 0; i < record; i++)
			{
				cust_position_st position = { 0 };
				char sholder_ac_no[16] = { 0 };
				BCGetStringFieldByName(handle, i, "sholder_ac_no", sholder_ac_no, sizeof(sholder_ac_no));
				assert(strcmp(sholder_ac_no, it->second.scust_no) == 0);

				BCGetStringFieldByName(handle, i, "smarket_code", position.exchangeid,
					sizeof(position.exchangeid));
				BCGetStringFieldByName(handle, i, "sserial2", position.exchangeid,
					sizeof(position.seatid));
				BCGetStringFieldByName(handle, i, "sstock_code", position.product,
					sizeof(position.product));
				BCGetStringFieldByName(handle, i, "sdate1", position.delivery_date,
					sizeof(position.delivery_date));
				BCGetStringFieldByName(handle, i, "sstatus2", position.direction,
					sizeof(position.direction));
				BCGetStringFieldByName(handle, i, "sstatus4", position.hedge_flag,
					sizeof(position.hedge_flag));

				BCGetIntFieldByName(handle, i, "lvol5", &position.positions);
				BCGetIntFieldByName(handle, i, "lvol12", &position.td_positions);
				BCGetDoubleFieldByName(handle, i, "damt21", &position.hold_average_price);
				BCGetDoubleFieldByName(handle, i, "damt26", &position.open_average_price);
				BCGetDoubleFieldByName(handle, i, "damt8", &position.float_profit);
				BCGetDoubleFieldByName(handle, i, "damt23", &position.last_price);
				BCGetIntFieldByName(handle, i, "lvol10", &position.multiple);
				it->second.position.push_back(position);
			}

			while (BCHaveNextPack(handle)) {
				int errcode = 0;
				char errmsg[256] = { 0 };
				BCCallNext(handle, 10000, &errcode, errmsg);
				BCGetRecordCount(handle, &record);
				for (int i = 0; i < record; ++i)
				{
					cust_position_st position = { 0 };
					char sholder_ac_no[16] = { 0 };
					BCGetStringFieldByName(handle, i, "sholder_ac_no", sholder_ac_no, sizeof(sholder_ac_no));
					assert(strcmp(sholder_ac_no, it->second.scust_no) == 0);

					BCGetStringFieldByName(handle, i, "smarket_code", position.exchangeid,
						sizeof(position.exchangeid));
					BCGetStringFieldByName(handle, i, "sserial2", position.exchangeid,
						sizeof(position.seatid));
					BCGetStringFieldByName(handle, i, "sstock_code", position.product,
						sizeof(position.product));
					BCGetStringFieldByName(handle, i, "sdate1", position.delivery_date,
						sizeof(position.delivery_date));
					BCGetStringFieldByName(handle, i, "sstatus2", position.direction,
						sizeof(position.direction));
					BCGetStringFieldByName(handle, i, "sstatus4", position.hedge_flag,
						sizeof(position.hedge_flag));

					BCGetIntFieldByName(handle, i, "lvol5", &position.positions);
					BCGetIntFieldByName(handle, i, "lvol12", &position.td_positions);
					BCGetDoubleFieldByName(handle, i, "damt21", &position.hold_average_price);
					BCGetDoubleFieldByName(handle, i, "damt26", &position.open_average_price);
					BCGetDoubleFieldByName(handle, i, "damt8", &position.float_profit);
					BCGetDoubleFieldByName(handle, i, "damt23", &position.last_price);
					BCGetIntFieldByName(handle, i, "lvol10", &position.multiple);
					it->second.position.push_back(position);
				}
			}

		}

		//std::this_thread::sleep_for(std::chrono::seconds(2));
		std::unique_lock<std::mutex> lck(mutex);
		if (cv.wait_for(lck, std::chrono::seconds(20)) != std::cv_status::timeout)
			return;
	}
	BCDeleteHandle(handle);
}
request_thread_group::request_thread_group()
{
}


request_thread_group::~request_thread_group()
{
}

void request_thread_group::start()
{
	if (!singleton) {
		singleton = new request_thread_group();
		singleton->cust_list_thr_.reset(new std::thread(RequestCustInfoList));

		singleton->cust_list_thr_->join();

		singleton->cust_login_info_thr_.reset(new std::thread(LoopRequestCustLoginInfo));
		singleton->cust_captial_thr_.reset(new std::thread(LoopRequestCustCaptial));
		singleton->cust_td_posi_detail_thr_.reset(new std::thread(LoopRequestTdPositionDetail));
		singleton->cust_position_thr_.reset(new std::thread(LoopRequestCustPosition));
	}
}

void request_thread_group::stop()
{
	//g_exit = true;
	if (singleton) {
		cv.notify_all();
		singleton->cust_login_info_thr_->join();
		singleton->cust_captial_thr_->join();
		singleton->cust_td_posi_detail_thr_->join();
		singleton->cust_position_thr_->join();
		delete singleton;
		singleton = NULL;
	}
	
}
