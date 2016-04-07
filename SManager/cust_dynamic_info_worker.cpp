#include "stdafx.h"
#include <cassert>
#include <sstream>
//#include <bind>
#include "cust_dynamic_info_worker.h"
#include "string_identifer.h"
#include "CommBCRequest.h"
#include "SMutility.h"
#include "SManager.h"
#include "struct_helper.h"

static cust_dynamic_info_worker *singleton = NULL;

system_info_st sysinfo_;
std::map<std::string, cust_dynamic_info_st> info_;
std::mutex mutex_;
//std::mutex si_mutex_;
std::condition_variable cv_;
std::set<int> force_orderseqs_;

uintptr_t login_thread = 0;
uintptr_t capital_thread = 0;

unsigned _stdcall request_cust_login_proc(void* param)
{
	std::ostringstream oss;
	oss << "客户登录信息查询线程thread_id=" << std::showbase << std::hex << std::this_thread::get_id();
	PostOutputMsg(oss.str());

	Scoped_BCHANDLE handle;
	BCResult result;
	for (;;)
	{
		std::vector<std::string> custs = theApp.GetCustnoList();
		for each (std::string cust_no in custs)
		{
			result = BCRequestCustLoginInfo_854093(handle, cust_no, info_[cust_no].login_info);

			::PostMessage(theApp.GetMainWnd()->GetSafeHwnd(),
				WM_USER_CUST_DYNAMIC_INFO,
				CUST_DYNAMIC_LOGIN_INFO_CHANGED,
				get_identifer(cust_no));
		}

		time_t n = time(NULL);
		double diff = difftime(n, sysinfo_.request_time);
		if (sysinfo_.system_status[0] == '1') {
			//开盘状态5分钟请求一次
			if (diff > 300) {
				result = BCRequestSysInfo_100319(handle, sysinfo_);
			}
		}
		else {
			//非开盘状态1分钟请求一次
			if (diff > 60) {
				result = BCRequestSysInfo_100319(handle, sysinfo_);
			}
		}

		std::this_thread::sleep_for(std::chrono::seconds(g_cfg.login_interval));
	}
	return 0;
}

void update_cust_position_order_trade(BCHANDLE handle, cust_base_info_st &cbi)
{
	BCResult result;
	std::string cust_no = cbi.basic.scust_no;

	PosiVec_t &positions = info_[cust_no].positions;
	result = BCRequestCustPosition_854048(handle, cust_no, positions);
	for (PosiVec_t::iterator it = positions.begin(); it != positions.end(); ++it)
	{
		BCRequestPosiOffsetVolume_854058(handle, *it);
	}

	result = BCRequestCustPosiDetail_854186(handle, cust_no, info_[cust_no].posi_detail);

	OrderVec_t &orders = info_[cust_no].orders;
	result = BCRequestCustOrder_854094(handle, cust_no, orders);
	bool new_force = false;
	for (OrderVec_t::iterator it = orders.begin(); it != orders.end(); ++it)
	{
		if (it->force_close[0] != '0' && force_orderseqs_.find(it->orderseq) == force_orderseqs_.end())
		{
			if (theApp.m_bMsgBee)
				for (int i = 0; i < 5; i++) MessageBeep(MB_ICONINFORMATION);
			force_orderseqs_.insert(it->orderseq);
			new_force = true;
		}
	}

	//if (new_force)
	//{
	//	cust_basic_st cb = { 0 };
	//	result = BCRequestCustBasic_850003(handle, (char*)cust_no.c_str(), cb);
	//	if (!(cbi.basic == cb))
	//	{
	//		cbi.basic = cb;
	//		theApp.SetCustBaseInfo(cbi);
	//	}
	//		
	//}

	result = BCRequestCustTrade_854095(handle, cust_no, info_[cust_no].trades);

	::PostMessage(
		theApp.GetMainWnd()->GetSafeHwnd(),
		WM_USER_CUST_DYNAMIC_INFO,
		CUST_DYNAMIC_POSITION_CHANGED,
		get_identifer(cust_no));

	::PostMessage(
		theApp.GetMainWnd()->GetSafeHwnd(),
		WM_USER_CUST_DYNAMIC_INFO,
		CUST_DYNAMIC_POSI_DETAIL_CHANGED,
		get_identifer(cust_no));

	::PostMessage(
		theApp.GetMainWnd()->GetSafeHwnd(),
		WM_USER_CUST_DYNAMIC_INFO,
		CUST_DYNAMIC_ORDER_CHANGED,
		get_identifer(cust_no));

	::PostMessage(
		theApp.GetMainWnd()->GetSafeHwnd(),
		WM_USER_CUST_DYNAMIC_INFO,
		CUST_DYNAMIC_TRADE_CHANGED,
		get_identifer(cust_no));
}

unsigned _stdcall request_cust_capital_proc(void* param)
{
	cust_dynamic_info_worker *ptr = (cust_dynamic_info_worker *)(param);
	std::ostringstream oss;
	oss << "客户权益查询线程thread_id=" << std::showbase << std::hex << std::this_thread::get_id();
	PostOutputMsg(oss.str());

	Scoped_BCHANDLE handle;
	BCResult result;

	for (int i = 0;; i++)
	{
		if (i == 0 || sysinfo_.request_time == 0 || sysinfo_.system_status[0] == '0' || sysinfo_.system_status[0] == '1') 
		{
			for each(cust_base_info_st cbi in theApp.GetCustBaseInfoList())
			{
				std::string cust_no = cbi.basic.scust_no;

				/**实时查询客户状态
				*小林要求同一操作员登录不同的SManager，客户状态同步
				*/
				cust_basic_st cb = { 0 };
				result = BCRequestCustBasic_850003(handle, (char*)cust_no.c_str(), cb);
				if (!(cbi.basic == cb))
				{
					cbi.basic = cb;
					theApp.SetCustBaseInfo(cbi);
				}
		
				cust_capital_st rc = { 0 };
				cust_capital_st &lc = info_[cust_no].captial;
				result = BCRequestCustCapital_854196(handle, cust_no, rc);
				std::unique_lock<std::mutex> lck(mutex_);
				if (lc == rc)
					continue;

				if (lc.commission != rc.commission
					|| lc.frzn_commission != rc.frzn_commission
					|| lc.margin != rc.margin
					|| lc.buy_frzn_margin != rc.buy_frzn_margin
					|| lc.sell_frzn_margin != rc.sell_frzn_margin)
				{
					update_cust_position_order_trade(handle, cbi);
				}

				lc = rc;
				::PostMessage(theApp.GetMainWnd()->GetSafeHwnd(),
					WM_USER_CUST_DYNAMIC_INFO,
					CUST_DYNAMIC_CAPITAL,
					get_identifer(cust_no));

			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(g_cfg.capital_interval));
	}
	return 0;
}

cust_dynamic_info_worker * cust_dynamic_info_worker::instance()
{
	if (!singleton)
	{
		singleton = new cust_dynamic_info_worker();
	}
	return singleton;
}

void cust_dynamic_info_worker::start()
{
	if (login_thread == 0)
	{
		login_thread = _beginthreadex(NULL, 0, request_cust_login_proc, NULL, 0, NULL);
	}

	if (capital_thread == 0)
	{
		capital_thread = _beginthreadex(NULL, 0, request_cust_capital_proc, NULL, 0, NULL);
	}
	
	
	//cust_login_info_thr_.reset(
	//	new std::thread(std::bind(&cust_dynamic_info_worker::loop_request_cust_login_info, this)));
	//cust_captial_thr_.reset(
	//	new std::thread(std::bind(&cust_dynamic_info_worker::loop_request_cust_capital, this)));

	//cust_login_info_thr_.reset(
	//	new boost::thread(boost::bind(&cust_dynamic_info_worker::loop_request_cust_login_info, this)));
	//cust_captial_thr_.reset(
	//	new boost::thread(boost::bind(&cust_dynamic_info_worker::loop_request_cust_capital, this)));
	//cust_basic_thr_.reset(
	//	new std::thread(std::bind(&cust_dynamic_info_worker::loop_request_cust_basic, this)));
	//cust_position_thr_.reset(
	//	new std::thread(std::bind(&cust_dynamic_info_worker::loop_request_cust_position, this)));
	//cust_td_posi_detail_thr_.reset(
	//	new std::thread(std::bind(&cust_dynamic_info_worker::loop_request_cust_td_posi_detail, this)));
	//cust_order_thr_.reset(
	//	new std::thread(std::bind(&cust_dynamic_info_worker::loop_request_cust_order, this)));
	//cust_trade_thr_.reset(
	//	new std::thread(std::bind(&cust_dynamic_info_worker::loop_request_cust_trade, this)));

}

void cust_dynamic_info_worker::stop()
{
	//if (login_thread)
	//	_endthreadex(login_thread);

	//if(capital_thread)
	//	_endthreadex(capital_thread);
	//cv_.notify_all();

	//if(cust_login_info_thr_)
	//	if(cust_login_info_thr_->joinable())
	//		cust_login_info_thr_->join();

	//if(cust_captial_thr_)
	//	if(cust_captial_thr_->joinable())
	//		cust_captial_thr_->join();

	//if (cust_basic_thr_)
	//	if (cust_basic_thr_->joinable())
	//		cust_basic_thr_->join();

	//if(cust_position_thr_)
	//	if(cust_position_thr_->joinable())
	//		cust_position_thr_->join();

	//if(cust_td_posi_detail_thr_)
	//	if(cust_td_posi_detail_thr_->joinable())
	//		cust_td_posi_detail_thr_->join();

	//if(cust_order_thr_)
	//	if(cust_order_thr_->joinable())
	//		cust_order_thr_->join();

	//if(cust_trade_thr_)
	//	if(cust_trade_thr_->joinable())
	//		cust_trade_thr_->join();
}

const cust_login_info_st & cust_dynamic_info_worker::get_cust_login_info(const std::string & cust_no)
{
	// TODO: 在此处插入 return 语句
	std::unique_lock<std::mutex> lkc(mutex_);
	return info_[cust_no].login_info;

}

const cust_capital_st & cust_dynamic_info_worker::get_cust_capital(const std::string & cust_no)
{
	// TODO: 在此处插入 return 语句
	std::unique_lock<std::mutex> lkc(mutex_);
	return info_[cust_no].captial;
}

const PosiVec_t& cust_dynamic_info_worker::get_cust_position(const std::string & cust_no)
{
	// TODO: 在此处插入 return 语句
	std::unique_lock<std::mutex> lkc(mutex_);
	return info_[cust_no].positions;
}

const PosiDetailVec_t& cust_dynamic_info_worker::get_cust_posi_detail(const std::string & cust_no)
{
	// TODO: 在此处插入 return 语句
	std::unique_lock<std::mutex> lkc(mutex_);
	return info_[cust_no].posi_detail;
}

const OrderVec_t& cust_dynamic_info_worker::get_cust_order(const std::string & cust_no)
{
	// TODO: 在此处插入 return 语句
	std::unique_lock<std::mutex> lkc(mutex_);
	return info_[cust_no].orders;
}

const TradeVec_t& cust_dynamic_info_worker::get_cust_trade(const std::string & cust_no)
{
	// TODO: 在此处插入 return 语句
	std::unique_lock<std::mutex> lkc(mutex_);
	return info_[cust_no].trades;
}

const system_info_st & cust_dynamic_info_worker::get_sysinfo()
{
	// TODO: 在此处插入 return 语句
	return sysinfo_;
}

cust_dynamic_info_worker::cust_dynamic_info_worker()
{
	memset(&sysinfo_, 0, sizeof(sysinfo_));
}


cust_dynamic_info_worker::~cust_dynamic_info_worker()
{
}

void cust_dynamic_info_worker::loop_request_cust_login_info()
try
{
	std::ostringstream oss;
	oss << "客户登录信息查询线程thread_id=" << std::showbase << std::hex << std::this_thread::get_id();
	PostOutputMsg(oss.str());

	auto fetcher = [this](const std::string &cust_no, BCHANDLE handle, int row) {
		cust_login_info_st li = {0};
		char sholder_ac_no[16] = { 0 };
		BCGetStringFieldByName(handle, 0, "sholder_ac_no", sholder_ac_no, sizeof(sholder_ac_no));
		//assert(cust_no == sholder_ac_no);
		if (cust_no != sholder_ac_no)
			return;
		BCGetStringFieldByName(handle, 0, "sstatus0", li.login_status,sizeof(li.login_status));
		BCGetStringFieldByName(handle, 0, "sholder_type", li.cur_app_name, sizeof(li.cur_app_name));
		BCGetStringFieldByName(handle, 0, "sphone2", li.cur_login_mac, sizeof(li.cur_login_mac));
		BCGetStringFieldByName(handle, 0, "sphone3", li.cur_login_ip, sizeof(li.cur_login_ip));
		BCGetStringFieldByName(handle, 0, "stime2", li.cur_login_time, sizeof(li.cur_login_time));
		BCGetStringFieldByName(handle, 0, "sdate2", li.cur_login_date, sizeof(li.cur_login_date));
		if (li.login_status[0] != '1')
			li.login_status[0] = '0';
		std::unique_lock<std::mutex> lck(mutex_);
		info_[cust_no].login_info = li;
	};
	//BCHANDLE handle = BCNewHandle(XpackDescribleFile);
	Scoped_BCHANDLE handle;
	for (;;)
	{
		//theApp
		std::vector<std::string> custs = theApp.GetCustnoList();
		for each (std::string cust_no in custs)
		{
			BCResetHandle(handle);
			BCSetRequestType(handle, 854093);
			BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
			BCSetStringFieldByName(handle, 0, "sholder_ac_no", (char*)cust_no.c_str());
			
			MyBCRequest(handle, std::bind(fetcher, cust_no, std::placeholders::_1, std::placeholders::_2), BCMSG_ERR_TO_OP);
			::PostMessage(theApp.GetMainWnd()->GetSafeHwnd(),
				WM_USER_CUST_DYNAMIC_INFO,
				CUST_DYNAMIC_LOGIN_INFO_CHANGED, 
				get_identifer(cust_no));
		}
		
		time_t n = time(NULL);
		double diff = difftime(n, sysinfo_.request_time);
		if (sysinfo_.system_status[0] == '1') {
			//开盘状态5分钟请求一次
			if (diff > 300) {
				request_sysinfo(handle);
			}
		}
		else {
			//非开盘状态1分钟请求一次
			if (diff > 60) {
				request_sysinfo(handle);
			}
		}

		std::mutex mutex;
		std::unique_lock<std::mutex> lck(mutex);
		if (cv_.wait_for(lck, std::chrono::seconds(g_cfg.login_interval)) != std::cv_status::timeout)
			return;
		//::PostMessage()
	}
	//BCDeleteHandle(handle);
}
catch (std::exception &err)
{
	std::ostringstream oss;
	oss << "客户登录信息查询线程异常退出，" << err.what() << ",请重启程序.";
	PostOutputMsg(oss.str());
}


void cust_dynamic_info_worker::loop_request_cust_capital()
try
{
	std::ostringstream oss;
	oss << "客户权益查询线程thread_id=" << std::showbase << std::hex << std::this_thread::get_id();
	PostOutputMsg(oss.str());

	Scoped_BCHANDLE handle;

	auto fetcher = [&](const std::string &cust_no, BCHANDLE handle, int row) {
		//char sholder_ac_no[16] = { 0 };
		//BCGetStringFieldByName(handle, 0, "sholder_ac_no", sholder_ac_no, sizeof(sholder_ac_no));
		//assert(cust_no == sholder_ac_no);
		cust_capital_st c = { 0 };
		BCGetStringFieldByName(handle, 0, "sholder_ac_no", c.cust_no, sizeof(c.cust_no));
		assert(cust_no == c.cust_no);
		BCGetStringFieldByName(handle, 0, "scurrency_type", c.scurrency_type, sizeof(c.scurrency_type));
		BCGetDoubleFieldByName(handle, 0, "damt3", &c.yd_balance);
		BCGetDoubleFieldByName(handle, 0, "damt26", &c.available);
		BCGetDoubleFieldByName(handle, 0, "damt23", &c.margin);
		BCGetDoubleFieldByName(handle, 0, "damt29", &c.buy_frzn_margin);
		BCGetDoubleFieldByName(handle, 0, "damt30", &c.sell_frzn_margin);
		BCGetDoubleFieldByName(handle, 0, "damt6", &c.commission);
		BCGetDoubleFieldByName(handle, 0, "damt33", &c.frzn_commission);
		BCGetDoubleFieldByName(handle, 0, "damt11", &c.offset_profit);
		BCGetDoubleFieldByName(handle, 0, "damt8", &c.float_profit);
		BCGetDoubleFieldByName(handle, 0, "damt28", &c.dynamic_capital);
		BCGetDoubleFieldByName(handle, 0, "damt25", &c.frzn_moeny);
		BCGetStringFieldByName(handle, 0, "sstatus3", c.risk_level,sizeof(c.risk_level));
		BCGetDoubleFieldByName(handle, 0, "damt22", &c.risk_degree);

		//{
		//	std::unique_lock<std::mutex> lck(mutex_);
		//	this->info_[cust_no].captial = c;
		//}
		cust_capital_st org;
		{
			std::unique_lock<std::mutex> lck(mutex_);
			org = info_[cust_no].captial;
			if (org != c)
			{	
				info_[cust_no].captial = c;
				::PostMessage(theApp.GetMainWnd()->GetSafeHwnd(),
					WM_USER_CUST_DYNAMIC_INFO,
					CUST_DYNAMIC_CAPITAL,
					get_identifer(cust_no));
			}
		}
		
		if (org.commission != c.commission || org.frzn_commission != c.frzn_commission
			|| org.margin != c.margin || org.buy_frzn_margin != c.buy_frzn_margin || org.sell_frzn_margin != c.sell_frzn_margin)
		{
			request_cust_position(handle, cust_no);
			request_cust_td_posi_detail(handle, cust_no);
			request_cust_order(handle, cust_no);
			request_cust_trade(handle, cust_no);
		}
	};

	
	for (int i = 0;;i++)
	{
		if (i == 0 || sysinfo_.request_time == 0 || sysinfo_.system_status[0] == '0'|| sysinfo_.system_status[0] == '1') {
			for each(std::string cust_no in theApp.GetCustnoList())
			{
				BCResetHandle(handle);
				BCSetRequestType(handle, 854196);
				BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
				BCSetStringFieldByName(handle, 0, "sholder_ac_no", (char*)cust_no.c_str());
				//BCSetStringFieldByName(handle, 0, "scurrency_type", ""); //
				MyBCRequest(handle, std::bind(fetcher, cust_no, std::placeholders::_1, std::placeholders::_2), BCMSG_ERR_TO_OP);
			}
		}

		std::mutex mutex;
		std::unique_lock<std::mutex> lck(mutex);
		if (cv_.wait_for(lck, std::chrono::seconds(g_cfg.capital_interval)) != std::cv_status::timeout)
			return;
		//std::this_thread::sleep_for(std::chrono::seconds(g_cfg.capital_interval));
		
	}
}
catch (std::exception &err)
{
	std::ostringstream oss;
	oss << "客户权益查询线程异常退出," << err.what() << ",请重启程序.";
	PostOutputMsg(oss.str());
}

void cust_dynamic_info_worker::loop_request_cust_basic()
try
{
	std::ostringstream oss;
	oss << "客户基本资料查询线程thread_id=" << std::this_thread::get_id();
	PostOutputMsg(oss.str());

	Scoped_BCHANDLE handle;
	for (;;)
	{
		std::unique_lock<std::mutex> lck(mutex_);
		if (cv_.wait_for(lck, std::chrono::seconds(15)) != std::cv_status::timeout)
			return;

		for each(cust_base_info_st cbi in theApp.GetCustBaseInfoList())
		{
			cust_basic_st cb = { 0 };
			if (BCRequestCustBasic_850003(handle, cbi.basic.scust_no, cb))
			{
				if (cbi.basic == cb)
					continue;
				cbi.basic = cb;
				theApp.SetCustBaseInfo(cbi);
			}
		}

	}
}
catch (std::exception &err)
{
	std::ostringstream oss;
	oss << "客户基本资料查询线程异常退出，" << err.what() << ",请重启程序.";
	PostOutputMsg(oss.str());
}

//
//void cust_dynamic_info_worker::loop_request_cust_position()
//{
//	Scoped_BCHANDLE handle;
//	//BCHANDLE handle = BCNewHandle(XpackDescribleFile);
//	/*auto fetcher = [this](const std::string &cust_no, HANDLE handle, int row) {
//		cust_position_st p = { 0 };
//		char sholder_ac_no[16] = { 0 };
//		BCGetStringFieldByName(handle, row, "sholder_ac_no", sholder_ac_no, sizeof(sholder_ac_no));
//		assert(cust_no == sholder_ac_no);
//
//		BCGetStringFieldByName(handle, row, "smarket_code", p.exchangeid, sizeof(p.exchangeid));
//		BCGetStringFieldByName(handle, row, "sserial2", p.exchangeid, sizeof(p.seatid));
//		BCGetStringFieldByName(handle, row, "sstock_code", p.product, sizeof(p.product));
//		BCGetStringFieldByName(handle, row, "sdate1", p.delivery_date, sizeof(p.delivery_date));
//		BCGetStringFieldByName(handle, row, "sbank_acc", p.instrumentid, sizeof(p.instrumentid));
//		BCGetStringFieldByName(handle, row, "sstatus2", p.direction, sizeof(p.direction));
//		BCGetStringFieldByName(handle, row, "sstatus4", p.hedge_flag, sizeof(p.hedge_flag));
//		BCGetIntFieldByName(handle, row, "lvol5", &p.positions);
//		BCGetIntFieldByName(handle, row, "lvol12", &p.td_positions);
//		BCGetDoubleFieldByName(handle, row, "damt21", &p.hold_average_price);
//		BCGetDoubleFieldByName(handle, row, "damt26", &p.open_average_price);
//		BCGetDoubleFieldByName(handle, row, "damt8", &p.float_profit);
//		BCGetDoubleFieldByName(handle, row, "damt23", &p.last_price);
//		BCGetIntFieldByName(handle, row, "lvol10", &p.multiple);
//		
//		std::unique_lock<std::mutex> lck(mutex_);
//		this->info_[cust_no].positions.push_back(p);
//		::PostMessage(
//			theApp.GetMainWnd()->GetSafeHwnd(),
//			WM_USER_CUST_DYNAMIC_INFO,
//			CUST_DYNAMIC_POSITION_CHANGED,
//			get_identifer(cust_no));
//	};*/
//
//	//if (!g_exit)
//	for (int loop = 0; ; loop++)
//	{
//		std::vector<std::string> custs = theApp.GetCustnoList();
//		//for (iter_t it = g_map_custinfo.begin(); it != g_map_custinfo.end(); ++it)
//		for each(std::string cust_no in custs)
//		{
//			char login_status = '0';
//
//			{
//				std::unique_lock<std::mutex> lck(mutex_);
//				login_status = info_[cust_no].login_info.login_status[0];
//			}
//
//			if (loop != 0 && login_status  != '1')
//				continue;
//
//			{
//				std::unique_lock<std::mutex> lck(mutex_);
//				info_[cust_no].positions.clear();
//			}
//			//it->second.posi_detail.clear();
//			//BCResetHandle(handle);
//			//BCSetRequestType(handle, 854048);
//			//BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
//			//BCSetStringFieldByName(handle, 0, "sholder_ac_no", (char*)cust_no.c_str());
//			//BCSetStringFieldByName(handle, 0, "sstatus3", "1"); //是否不输出登记标记
//			//BCSetStringFieldByName(handle, 0, "sstatus0", "0"); //汇总标记
//			//													//BCSetStringFieldByName(handle, 0, "sbank_acc", ""); //合约代码
//			//MyBCRequest(handle, std::bind(fetcher, cust_no, std::placeholders::_1, std::placeholders::_2));
//			request_cust_position(handle, cust_no);
//		}
//		std::unique_lock<std::mutex> lck(mutex_);
//		if (cv_.wait_for(lck, std::chrono::seconds(20)) != std::cv_status::timeout)
//			return;
//	}
//	//BCDeleteHandle(handle);
//}
//
//void cust_dynamic_info_worker::loop_request_cust_td_posi_detail()
//{
//	Scoped_BCHANDLE handle;
//	//BCHANDLE handle = BCNewHandle(XpackDescribleFile);
//	//auto fetcher = [this](const std::string &cust_no, HANDLE handle, int row) {
//	//	cust_td_position_detail_st pd = { 0 };
//	//	char sholder_ac_no[16] = { 0 };
//	//	BCGetStringFieldByName(handle, row, "sholder_ac_no", sholder_ac_no, sizeof(sholder_ac_no));
//	//	assert(cust_no == sholder_ac_no);
//
//	//	BCGetIntFieldByName(handle, row, "lvol9", &pd.trade_seq);
//	//	BCGetStringFieldByName(handle, row, "sholder_ac_no", pd.exchangeid, sizeof(pd.exchangeid));
//	//	BCGetStringFieldByName(handle, row, "sstatus3", pd.scurrency_type, sizeof(pd.scurrency_type));
//	//	BCGetStringFieldByName(handle, row, "sholder_ac_no2", pd.tradeid, sizeof(pd.tradeid));
//	//	BCGetStringFieldByName(handle, row, "sstock_code", pd.product, sizeof(pd.product));
//	//	BCGetStringFieldByName(handle, row, "sdate1", pd.delivery_date, sizeof(pd.delivery_date));
//	//	BCGetStringFieldByName(handle, row, "sstatus2", pd.direction, sizeof(pd.direction));
//	//	BCGetStringFieldByName(handle, row, "sstatus4", pd.hedge_flag, sizeof(pd.hedge_flag));
//	//	BCGetIntFieldByName(handle, row, "lvol8", &pd.volume);
//	//	BCGetIntFieldByName(handle, row, "lvol10", &pd.multiple);
//	//	BCGetDoubleFieldByName(handle, row, "damt21", &pd.hold_price);
//	//	BCGetDoubleFieldByName(handle, row, "damt26", &pd.open_price);
//	//	BCGetDoubleFieldByName(handle, row, "damt1", &pd.hold_margin);
//	//	BCGetDoubleFieldByName(handle, row, "damt8", &pd.td_settle_price);
//	//	BCGetDoubleFieldByName(handle, row, "damt7", &pd.yd_settle_price);
//
//	//	std::unique_lock<std::mutex> lck(mutex_);
//	//	this->info_[cust_no].posi_detail.push_back(pd);
//	//	::PostMessage(
//	//		theApp.GetMainWnd()->GetSafeHwnd(),
//	//		WM_USER_CUST_DYNAMIC_INFO,
//	//		CUST_DYNAMIC_POSI_DETAIL_CHANGED,
//	//		get_identifer(cust_no));
//	//};
//
//	//if (!g_exit)
//	for (int loop = 0; ; loop++)
//	{
//		std::vector<std::string> custs = theApp.GetCustnoList();
//		//for (iter_t it = g_map_custinfo.begin(); it != g_map_custinfo.end(); ++it)
//		for each(std::string cust_no in custs)
//		{
//			char login_status = '0';
//
//			{
//				std::unique_lock<std::mutex> lck(mutex_);
//				login_status = info_[cust_no].login_info.login_status[0];
//			}
//
//			if (loop != 0 && login_status != '1')
//				continue;
//
//			{
//				std::unique_lock<std::mutex> lck(mutex_);
//				info_[cust_no].posi_detail.clear();
//			}
//			//it->second.posi_detail.clear();
//			//BCResetHandle(handle);
//			//BCSetRequestType(handle, 854186);
//			//BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
//			//BCSetStringFieldByName(handle, 0, "sholder_ac_no", (char*)cust_no.c_str());
//			//MyBCRequest(handle, std::bind(fetcher, cust_no, std::placeholders::_1, std::placeholders::_2));
//			request_cust_td_posi_detail(handle, cust_no);
//		}
//		std::unique_lock<std::mutex> lck(mutex_);
//		if (cv_.wait_for(lck, std::chrono::seconds(20)) != std::cv_status::timeout)
//			return;
//	}
//	//BCDeleteHandle(handle);
//}

//
//void cust_dynamic_info_worker::loop_request_cust_order()
//{
//	Scoped_BCHANDLE handle;
//	/*auto fetcher = [this](const std::string &cust_no, HANDLE handle, int row) {
//		cust_order_st o = { 0 };
//		char sholder_ac_no[16] = { 0 };
//		BCGetStringFieldByName(handle, row, "sholder_ac_no", sholder_ac_no, sizeof(sholder_ac_no));
//		assert(cust_no == sholder_ac_no);
//
//		BCGetIntFieldByName(handle, row, "lserial0", &o.orderseq);
//		BCGetStringFieldByName(handle, row, "scust_no", o.oper_no, sizeof(o.oper_no));
//		BCGetStringFieldByName(handle, row, "smarket_code", o.exchangeid, sizeof(o.exchangeid));
//		BCGetStringFieldByName(handle, row, "sstock_code", o.productid, sizeof(o.productid));
//		BCGetStringFieldByName(handle, row, "sdate1", o.delivery_date, sizeof(o.delivery_date));
//		BCGetStringFieldByName(handle, row, "sphone", o.instrumentid, sizeof(o.instrumentid));
//		BCGetStringFieldByName(handle, row, "sstatus0", o.order_status, sizeof(o.order_status));
//		BCGetStringFieldByName(handle, row, "sstatus3", o.offset_flag, sizeof(o.offset_flag));
//		BCGetStringFieldByName(handle, row, "sstatus2", o.direction, sizeof(o.direction));
//		BCGetStringFieldByName(handle, row, "sstatus4", o.hedge_flag, sizeof(o.hedge_flag));
//		BCGetDoubleFieldByName(handle, row, "damt0", &o.order_price);
//		BCGetIntFieldByName(handle, row, "lvol0", &o.origin_total_volume);
//		BCGetIntFieldByName(handle, row, "lvol2", &o.total_volume);
//		BCGetDoubleFieldByName(handle, row, "damt2", &o.traded_price);
//		BCGetIntFieldByName(handle, row, "lvol1", &o.traded_volume);
//		BCGetStringFieldByName(handle, row, "sorder0", o.sysid, sizeof(o.sysid));
//		BCGetStringFieldByName(handle, row, "stime0", o.order_time, sizeof(o.order_time));
//		BCGetStringFieldByName(handle, row, "stime1", o.insert_time, sizeof(o.insert_time));
//		BCGetStringFieldByName(handle, row, "sholder_ac_no2", o.trade_no, sizeof(o.trade_no));
//		BCGetStringFieldByName(handle, row, "sholder_type", o.order_way, sizeof(o.order_way));
//		BCGetStringFieldByName(handle, row, "scust_no2", o.cancel_cust, sizeof(o.cancel_cust));
//		BCGetStringFieldByName(handle, row, "stime2", o.cancel_time, sizeof(o.cancel_time));
//		BCGetStringFieldByName(handle, row, "sstat_type2", o.order_type, sizeof(o.order_type));
//		BCGetStringFieldByName(handle, row, "sserial2", o.seatid, sizeof(o.seatid));
//		BCGetStringFieldByName(handle, row, "scurrency_type", o.currency, sizeof(o.currency));
//		BCGetStringFieldByName(handle, row, "sstatus1", o.force_close, sizeof(o.force_close));
//
//		std::unique_lock<std::mutex> lck(mutex_);
//		this->info_[cust_no].orders.push_back(o);
//		::PostMessage(
//			theApp.GetMainWnd()->GetSafeHwnd(),
//			WM_USER_CUST_DYNAMIC_INFO,
//			CUST_DYNAMIC_ORDER_CHANGED,
//			get_identifer(cust_no));
//	};*/
//
//	//if (!g_exit)
//	for (int loop = 0; ; loop++)
//	{
//		std::vector<std::string> custs = theApp.GetCustnoList();
//		//for (iter_t it = g_map_custinfo.begin(); it != g_map_custinfo.end(); ++it)
//		for each(std::string cust_no in custs)
//		{
//			char login_status = '0';
//
//			{
//				std::unique_lock<std::mutex> lck(mutex_);
//				login_status = info_[cust_no].login_info.login_status[0];
//			}
//
//			if (loop != 0 && login_status != '1')
//				continue;
//
//			{
//				std::unique_lock<std::mutex> lck(mutex_);
//				info_[cust_no].orders.clear();
//			}
//			////it->second.posi_detail.clear();
//			//BCResetHandle(handle);
//			//BCSetRequestType(handle, 854094);
//			//BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
//			//BCSetStringFieldByName(handle, 0, "sholder_ac_no", (char*)cust_no.c_str());
//			//BCSetStringFieldByName(handle, 0, "smain_flag", "1");//所有委托单标记
//			//BCSetStringFieldByName(handle, 0, "sstatus3", "1");//是否不输出合计标记
//			//
//			//MyBCRequest(handle, std::bind(fetcher, cust_no, std::placeholders::_1, std::placeholders::_2));
//			request_cust_order(handle, cust_no);
//		}
//		std::unique_lock<std::mutex> lck(mutex_);
//		if (cv_.wait_for(lck, std::chrono::seconds(20)) != std::cv_status::timeout)
//			return;
//	}
//
//}

//void cust_dynamic_info_worker::loop_request_cust_trade()
//{
//	Scoped_BCHANDLE handle;
//	//auto fetcher = [this](const std::string &cust_no, HANDLE handle, int row) {
//	//	cust_trade_st t = { 0 };
//	//	char sholder_ac_no[16] = { 0 };
//	//	BCGetStringFieldByName(handle, row, "sholder_ac_no", sholder_ac_no, sizeof(sholder_ac_no));
//	//	assert(cust_no == sholder_ac_no);
//
//	//	BCGetIntFieldByName(handle, row, "lserial0", &t.orderseq);
//	//	//BCGetStringFieldByName(handle, row, "scust_no", o.oper_no, sizeof(o.oper_no));
//	//	BCGetStringFieldByName(handle, row, "smarket_code", t.exchangeid, sizeof(t.exchangeid));
//	//	BCGetStringFieldByName(handle, row, "sstock_code", t.productid, sizeof(t.productid));
//	//	BCGetStringFieldByName(handle, row, "sdate1", t.delivery_date, sizeof(t.delivery_date));
//	//	BCGetStringFieldByName(handle, row, "sbank_acc", t.instrumentid, sizeof(t.instrumentid));
//	//	//BCGetStringFieldByName(handle, row, "sstatus0", t.order_status, sizeof(t.order_status));
//	//	BCGetStringFieldByName(handle, row, "sstatus3", t.offset_flag, sizeof(t.offset_flag));
//	//	BCGetStringFieldByName(handle, row, "sstatus2", t.direction, sizeof(t.direction));
//	//	BCGetStringFieldByName(handle, row, "sstatus4", t.hedge_flag, sizeof(t.hedge_flag));
//	//	BCGetDoubleFieldByName(handle, row, "damt2", &t.price);
//	//	BCGetIntFieldByName(handle, row, "lvol1", &t.volume);
//	//	BCGetStringFieldByName(handle, row, "sorder0", t.sysid, sizeof(t.sysid));
//	//	BCGetIntFieldByName(handle, row, "lvol11", &t.tradeseq);
//	//	BCGetStringFieldByName(handle, row, "sserial2", t.seatid, sizeof(t.seatid));
//	//	BCGetStringFieldByName(handle, row, "scurrency_type", t.currency, sizeof(t.currency));
//	//	BCGetStringFieldByName(handle, row, "stime3", t.time, sizeof(t.time));
//	//	BCGetDoubleFieldByName(handle, row, "damt6", &t.commission);
//	//	BCGetStringFieldByName(handle, row, "scust_no", t.oper_no, sizeof(t.oper_no));
//	//	BCGetStringFieldByName(handle, row, "sstatus0", t.force_close, sizeof(t.force_close));
//
//	//	std::unique_lock<std::mutex> lck(mutex_);
//	//	this->info_[cust_no].trades.push_back(t);
//	//	::PostMessage(
//	//		theApp.GetMainWnd()->GetSafeHwnd(),
//	//		WM_USER_CUST_DYNAMIC_INFO,
//	//		CUST_DYNAMIC_TRADE_CHANGED,
//	//		get_identifer(cust_no));
//	//};
//
//	//if (!g_exit)
//	for (int loop = 0; ; loop++)
//	{
//		std::vector<std::string> custs = theApp.GetCustnoList();
//		//for (iter_t it = g_map_custinfo.begin(); it != g_map_custinfo.end(); ++it)
//		for each(std::string cust_no in custs)
//		{
//			char login_status = '0';
//
//			{
//				std::unique_lock<std::mutex> lck(mutex_);
//				login_status = info_[cust_no].login_info.login_status[0];
//			}
//
//			if (loop != 0 && login_status != '1')
//				continue;
//
//			{
//				std::unique_lock<std::mutex> lck(mutex_);
//				info_[cust_no].trades.clear();
//			}
//			//it->second.posi_detail.clear();
//			//BCResetHandle(handle);
//			//BCSetRequestType(handle, 854095);
//			//BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
//			//BCSetStringFieldByName(handle, 0, "sholder_ac_no", (char*)cust_no.c_str());
//			//BCSetStringFieldByName(handle, 0, "sstatus1", "0");//合并标志
//			//BCSetStringFieldByName(handle, 0, "smain_flag", "1");//所有委托单标记
//			//BCSetStringFieldByName(handle, 0, "sstatus3", "1");//是否不输出合计标记
//			//BCSetStringFieldByName(handle, 0, "sstatus0", "0");//汇总标记
//
//			//MyBCRequest(handle, std::bind(fetcher, cust_no, std::placeholders::_1, std::placeholders::_2));
//			request_cust_order(handle, cust_no);
//		}
//		std::unique_lock<std::mutex> lck(mutex_);
//		if (cv_.wait_for(lck, std::chrono::seconds(20)) != std::cv_status::timeout)
//			return;
//	}
//}


void cust_dynamic_info_worker::request_cust_position(BCHANDLE handle, const std::string &cust_no)
{
	auto fetcher = [this](const std::string &cust_no, HANDLE handle, int row) {
		cust_position_st p = { 0 };
		//char sholder_ac_no[16] = { 0 };
		BCGetStringFieldByName(handle, row, "sholder_ac_no", p.cust_no, sizeof(p.cust_no));
		assert(cust_no == p.cust_no);

		BCGetStringFieldByName(handle, row, "smarket_code", p.exchangeid, sizeof(p.exchangeid));
		BCGetStringFieldByName(handle, row, "sserial2", p.seatid, sizeof(p.seatid));
		BCGetStringFieldByName(handle, row, "sstock_code", p.product, sizeof(p.product));
		BCGetStringFieldByName(handle, row, "sdate1", p.delivery_date, sizeof(p.delivery_date));
		BCGetStringFieldByName(handle, row, "sbank_acc", p.instrumentid, sizeof(p.instrumentid));
		BCGetStringFieldByName(handle, row, "sstatus2", p.direction, sizeof(p.direction));
		BCGetStringFieldByName(handle, row, "sstatus4", p.hedge_flag, sizeof(p.hedge_flag));
		BCGetIntFieldByName(handle, row, "lvol5", &p.positions);
		BCGetIntFieldByName(handle, row, "lvol12", &p.td_positions);
		BCGetDoubleFieldByName(handle, row, "damt21", &p.hold_average_price);
		BCGetDoubleFieldByName(handle, row, "damt26", &p.open_average_price);
		BCGetDoubleFieldByName(handle, row, "damt8", &p.float_profit);
		BCGetDoubleFieldByName(handle, row, "damt23", &p.last_price);
		BCGetIntFieldByName(handle, row, "lvol10", &p.multiple);

		std::unique_lock<std::mutex> lck(mutex_);
		info_[cust_no].positions.push_back(p);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 854048);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", (char*)cust_no.c_str());
	BCSetStringFieldByName(handle, 0, "sstatus3", "1"); //是否不输出登记标记
	BCSetStringFieldByName(handle, 0, "sstatus0", "0"); //汇总标记
	//BCSetStringFieldByName(handle, 0, "sbank_acc", ""); //合约代码

	{
		std::unique_lock<std::mutex> lck(mutex_);
		info_[cust_no].positions.clear();
	}

	MyBCRequest(handle, std::bind(fetcher, cust_no, std::placeholders::_1, std::placeholders::_2), BCMSG_ERR_TO_OP);
	
	{
		std::unique_lock<std::mutex> lck(mutex_);
		for (PosiVec_t::iterator it = info_[cust_no].positions.begin(); it != info_[cust_no].positions.end(); ++it)
			request_position_offset_volume(handle, *it);
	}

	::PostMessage(
		theApp.GetMainWnd()->GetSafeHwnd(),
		WM_USER_CUST_DYNAMIC_INFO,
		CUST_DYNAMIC_POSITION_CHANGED,
		get_identifer(cust_no));
}

void cust_dynamic_info_worker::request_position_offset_volume(BCHANDLE handle, cust_position_st &p)
{
	FetchRowFunc_t fetcher = [&p](BCHANDLE handle, int row)
	{
		BCGetIntFieldByName(handle, row, "lvol4", &p.can_offset_vol);
		BCGetIntFieldByName(handle, row, "lvol7", &p.td_can_offset_vol);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 854058);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", p.cust_no);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no2", p.trade_no);
	BCSetStringFieldByName(handle, 0, "sserial2", p.seatid);
	BCSetStringFieldByName(handle, 0, "smarket_code", p.exchangeid);
	BCSetStringFieldByName(handle, 0, "scust_auth", p.instrumentid);
	BCSetStringFieldByName(handle, 0, "sstatus2", strcmp(p.direction, "0") ? "0" : "1");
	BCSetStringFieldByName(handle, 0, "sstatus4", p.hedge_flag);
	MyBCRequest(handle, fetcher, BCMSG_ERR_TO_OP);

}

void cust_dynamic_info_worker::request_cust_td_posi_detail(BCHANDLE handle, const std::string & cust_no)
{
	auto fetcher = [this](const std::string &cust_no, HANDLE handle, int row) {
		cust_td_position_detail_st pd = { 0 };
		//char sholder_ac_no[16] = { 0 };
		BCGetStringFieldByName(handle, row, "sholder_ac_no", pd.cust_no, sizeof(pd.cust_no));
		assert(cust_no == pd.cust_no);

		BCGetStringFieldByName(handle, row, "sdate2", pd.trade_date, sizeof(pd.trade_date));
		BCGetStringFieldByName(handle, row, "sdate3", pd.open_date, sizeof(pd.open_date));
		BCGetIntFieldByName(handle, row, "lvol9", &pd.trade_seq);
		BCGetStringFieldByName(handle, row, "smarket_code", pd.exchangeid, sizeof(pd.exchangeid));
		BCGetStringFieldByName(handle, row, "sstatus3", pd.scurrency_type, sizeof(pd.scurrency_type));
		BCGetStringFieldByName(handle, row, "sholder_ac_no2", pd.tradeid, sizeof(pd.tradeid));
		BCGetStringFieldByName(handle, row, "sstock_code", pd.product, sizeof(pd.product));
		BCGetStringFieldByName(handle, row, "sdate1", pd.delivery_date, sizeof(pd.delivery_date));
		BCGetStringFieldByName(handle, row, "scert_addr", pd.instrumentid, sizeof(pd.instrumentid));
		BCGetStringFieldByName(handle, row, "sstatus2", pd.direction, sizeof(pd.direction));
		BCGetStringFieldByName(handle, row, "sstatus4", pd.hedge_flag, sizeof(pd.hedge_flag));
		BCGetIntFieldByName(handle, row, "lvol8", &pd.volume);
		BCGetIntFieldByName(handle, row, "lvol10", &pd.multiple);
		BCGetDoubleFieldByName(handle, row, "damt21", &pd.hold_price);
		BCGetDoubleFieldByName(handle, row, "damt26", &pd.open_price);
		BCGetDoubleFieldByName(handle, row, "damt1", &pd.hold_margin);
		BCGetDoubleFieldByName(handle, row, "damt8", &pd.td_settle_price);
		BCGetDoubleFieldByName(handle, row, "damt7", &pd.yd_settle_price);

		std::unique_lock<std::mutex> lck(mutex_);
		info_[cust_no].posi_detail.push_back(pd);
		
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 854186);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", (char*)cust_no.c_str());

	{
		std::unique_lock<std::mutex> lck(mutex_);
		info_[cust_no].posi_detail.clear();
	}

	MyBCRequest(handle, std::bind(fetcher, cust_no, std::placeholders::_1, std::placeholders::_2), BCMSG_ERR_TO_OP);

	::PostMessage(
		theApp.GetMainWnd()->GetSafeHwnd(),
		WM_USER_CUST_DYNAMIC_INFO,
		CUST_DYNAMIC_POSI_DETAIL_CHANGED,
		get_identifer(cust_no));
}

void cust_dynamic_info_worker::request_cust_order(BCHANDLE handle, const std::string & cust_no)
{
	auto fetcher = [this](const std::string &cust_no, HANDLE handle, int row) {
		cust_order_st o = { 0 };
		//char sholder_ac_no[16] = { 0 };
		//BCGetStringFieldByName(handle, row, "sholder_ac_no", sholder_ac_no, sizeof(sholder_ac_no));
		BCGetStringFieldByName(handle, row, "sholder_ac_no", o.cust_no, sizeof(o.cust_no));
		assert(cust_no == o.cust_no);

		BCGetIntFieldByName(handle, row, "lserial0", &o.orderseq);
		BCGetStringFieldByName(handle, row, "scust_no", o.oper_no, sizeof(o.oper_no));
		BCGetStringFieldByName(handle, row, "smarket_code", o.exchangeid, sizeof(o.exchangeid));
		BCGetStringFieldByName(handle, row, "sstock_code", o.productid, sizeof(o.productid));
		BCGetStringFieldByName(handle, row, "sdate1", o.delivery_date, sizeof(o.delivery_date));
		BCGetStringFieldByName(handle, row, "sphone", o.instrumentid, sizeof(o.instrumentid));
		BCGetStringFieldByName(handle, row, "sstatus0", o.order_status, sizeof(o.order_status));
		BCGetStringFieldByName(handle, row, "sstatus3", o.offset_flag, sizeof(o.offset_flag));
		BCGetStringFieldByName(handle, row, "sstatus2", o.direction, sizeof(o.direction));
		BCGetStringFieldByName(handle, row, "sstatus4", o.hedge_flag, sizeof(o.hedge_flag));
		BCGetDoubleFieldByName(handle, row, "damt0", &o.order_price);
		BCGetIntFieldByName(handle, row, "lvol0", &o.origin_total_volume);
		BCGetIntFieldByName(handle, row, "lvol2", &o.total_volume);
		BCGetDoubleFieldByName(handle, row, "damt2", &o.traded_price);
		BCGetIntFieldByName(handle, row, "lvol1", &o.traded_volume);
		BCGetStringFieldByName(handle, row, "sorder0", o.sysid, sizeof(o.sysid));
		BCGetStringFieldByName(handle, row, "stime0", o.order_time, sizeof(o.order_time));
		BCGetStringFieldByName(handle, row, "stime1", o.insert_time, sizeof(o.insert_time));
		BCGetStringFieldByName(handle, row, "sholder_ac_no2", o.trade_no, sizeof(o.trade_no));
		BCGetStringFieldByName(handle, row, "sholder_type", o.order_way, sizeof(o.order_way));
		BCGetStringFieldByName(handle, row, "scust_no2", o.cancel_cust, sizeof(o.cancel_cust));
		BCGetStringFieldByName(handle, row, "stime2", o.cancel_time, sizeof(o.cancel_time));
		BCGetStringFieldByName(handle, row, "sstat_type2", o.order_type, sizeof(o.order_type));
		BCGetStringFieldByName(handle, row, "sserial2", o.seatid, sizeof(o.seatid));
		BCGetStringFieldByName(handle, row, "scurrency_type", o.currency, sizeof(o.currency));
		BCGetStringFieldByName(handle, row, "sstatus1", o.force_close, sizeof(o.force_close));

		if (o.force_close[0] != '0' && force_orderseqs_.find(o.orderseq) == force_orderseqs_.end())
		{
			if (theApp.m_bMsgBee)
				for (int i = 0; i < 5; i++) MessageBeep(MB_ICONINFORMATION);
			force_orderseqs_.insert(o.orderseq);
		}

		std::unique_lock<std::mutex> lck(mutex_);
		info_[cust_no].orders.push_back(o);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 854094);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", (char*)cust_no.c_str());
	BCSetStringFieldByName(handle, 0, "smain_flag", "1");//所有委托单标记
	BCSetStringFieldByName(handle, 0, "sstatus3", "1");//是否不输出合计标记

	{
		std::unique_lock<std::mutex> lck(mutex_);
		info_[cust_no].orders.clear();
	}
	std::set<int> tmp = force_orderseqs_;

	MyBCRequest(handle, std::bind(fetcher, cust_no, std::placeholders::_1, std::placeholders::_2), BCMSG_ERR_TO_OP);

	::PostMessage(
		theApp.GetMainWnd()->GetSafeHwnd(),
		WM_USER_CUST_DYNAMIC_INFO,
		CUST_DYNAMIC_ORDER_CHANGED,
		get_identifer(cust_no));

	if (tmp.size() != force_orderseqs_.size())
	{
		cust_base_info_st cbi = theApp.GetCustBaseInfo(cust_no.c_str());
		cust_basic_st basic = { 0 };
		if (BCRequestCustBasic_850003(handle, cbi.basic.scust_no, basic))
		{
			if (!(basic == cbi.basic))
			{
				cbi.basic = basic;
				theApp.SetCustBaseInfo(cbi);
			}
		}
	}
}

void cust_dynamic_info_worker::request_cust_trade(BCHANDLE handle, const std::string & cust_no)
{
	auto fetcher = [this](const std::string &cust_no, HANDLE handle, int row) {
		cust_trade_st t = { 0 };
		//char sholder_ac_no[16] = { 0 };
		BCGetStringFieldByName(handle, row, "sholder_ac_no", t.cust_no, sizeof(t.cust_no));
		assert(cust_no == t.cust_no);

		BCGetIntFieldByName(handle, row, "lserial0", &t.orderseq);
		//BCGetStringFieldByName(handle, row, "scust_no", o.oper_no, sizeof(o.oper_no));
		BCGetStringFieldByName(handle, row, "smarket_code", t.exchangeid, sizeof(t.exchangeid));
		BCGetStringFieldByName(handle, row, "sstock_code", t.productid, sizeof(t.productid));
		BCGetStringFieldByName(handle, row, "sdate1", t.delivery_date, sizeof(t.delivery_date));
		BCGetStringFieldByName(handle, row, "sbank_acc", t.instrumentid, sizeof(t.instrumentid));
		//BCGetStringFieldByName(handle, row, "sstatus0", t.order_status, sizeof(t.order_status));
		BCGetStringFieldByName(handle, row, "sstatus3", t.offset_flag, sizeof(t.offset_flag));
		BCGetStringFieldByName(handle, row, "sstatus2", t.direction, sizeof(t.direction));
		BCGetStringFieldByName(handle, row, "sstatus4", t.hedge_flag, sizeof(t.hedge_flag));
		BCGetDoubleFieldByName(handle, row, "damt2", &t.price);
		BCGetIntFieldByName(handle, row, "lvol1", &t.volume);
		BCGetStringFieldByName(handle, row, "sorder0", t.sysid, sizeof(t.sysid));
		BCGetIntFieldByName(handle, row, "lvol11", &t.tradeseq);
		BCGetStringFieldByName(handle, row, "sserial2", t.seatid, sizeof(t.seatid));
		BCGetStringFieldByName(handle, row, "scurrency_type", t.currency, sizeof(t.currency));
		BCGetStringFieldByName(handle, row, "stime3", t.time, sizeof(t.time));
		BCGetDoubleFieldByName(handle, row, "damt6", &t.commission);
		BCGetStringFieldByName(handle, row, "scust_no", t.oper_no, sizeof(t.oper_no));
		BCGetStringFieldByName(handle, row, "sstatus0", t.force_close, sizeof(t.force_close));

		std::unique_lock<std::mutex> lck(mutex_);
		info_[cust_no].trades.push_back(t);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 854095);
	BCSetStringFieldByName(handle, 0, "scust_no", g_cfg.oper_code);
	BCSetStringFieldByName(handle, 0, "sholder_ac_no", (char*)cust_no.c_str());
	BCSetStringFieldByName(handle, 0, "sstatus1", "0");//合并标志
	BCSetStringFieldByName(handle, 0, "smain_flag", "1");//所有委托单标记
	BCSetStringFieldByName(handle, 0, "sstatus3", "1");//是否不输出合计标记
	BCSetStringFieldByName(handle, 0, "sstatus0", "0");//汇总标记

	{
		std::unique_lock<std::mutex> lck(mutex_);
		info_[cust_no].trades.clear();
	}

	MyBCRequest(handle, std::bind(fetcher, cust_no, std::placeholders::_1, std::placeholders::_2), BCMSG_ERR_TO_OP);
	::PostMessage(
		theApp.GetMainWnd()->GetSafeHwnd(),
		WM_USER_CUST_DYNAMIC_INFO,
		CUST_DYNAMIC_TRADE_CHANGED,
		get_identifer(cust_no));
}

void cust_dynamic_info_worker::request_sysinfo(BCHANDLE handle)
{
	FetchRowFunc_t fetcher = [this](BCHANDLE handle, int row) {
		BCGetStringFieldByName(handle, 0, "sdate0", sysinfo_.system_date, sizeof(sysinfo_.system_time));
		BCGetStringFieldByName(handle, 0, "sdate2", sysinfo_.check_date, sizeof(sysinfo_.check_date));
		BCGetStringFieldByName(handle, 0, "sdate1", sysinfo_.next_tradingday, sizeof(sysinfo_.next_tradingday));
		BCGetStringFieldByName(handle, 0, "sdate3", sysinfo_.host_date, sizeof(sysinfo_.host_date));
		BCGetStringFieldByName(handle, 0, "sstatus0", sysinfo_.system_status, sizeof(sysinfo_.system_status));
		BCGetStringFieldByName(handle, 0, "stime0", sysinfo_.system_time, sizeof(sysinfo_.system_time));
		sysinfo_.request_time = static_cast<long>(time(NULL));
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 100319);
	BCSetStringFieldByName(handle, 0, "sbranch_code0", "000");
	MyBCRequest(handle, fetcher, BCMSG_ERR_TO_OP);
	::PostMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), WM_USER_SYSTEM_INFO, 0, 0);
}
