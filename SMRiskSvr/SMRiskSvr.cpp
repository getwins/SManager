// SMRiskSvr.cpp : �������̨Ӧ�ó������ڵ㡣

//#include "stdafx.h"
#include <stdlib.h>
#include <exception>
#include <thread>
#include <mutex>
#include <vector>
#include <ctime>
#include <chrono>
#include <iterator>
#include <algorithm>
#include <map>
#include <atomic>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "main.h"
#include "nbccclt.h"
#include "MyBCRequest.h"
#include "conf.h"
#include "MdApi.h"

int g_drtpno;
std::mutex g_cout_mutex;

std::atomic_int g_force_seq(0);

std::mutex g_RCforce_mutex;
std::vector<rc_desc> g_RCforce;

struct cc_cp_st
{
	//std::mutex mu;
	cust_capital_st cc;
	std::vector<cust_position_st> cps;
	double force_line;
};
typedef std::vector<cc_cp_st> ccp_vec_t;

std::mutex g_ccp_mu;
ccp_vec_t g_ccp;

void BCCLT_init()
{
	if (!BCCCLTInit(1))
		throw std::runtime_error("BCCCLTInit failed");
	g_drtpno = BCAddDrtpNode((char*)g_conf.drtp.host.c_str(), g_conf.drtp.port);
	if (g_drtpno < 0)
		throw std::runtime_error("BCAddDrtpNode failed");

	BCHANDLE handle = BCNewHandle("cpack.dat");
	if (handle == NULL)
		throw std::runtime_error("BCNewHandle failed");
	BCDeleteHandle(handle);
}


int force_cust_positions(BCHANDLE handle, posi_vec_t &ps)
{
	int ret = 0;
	BCResult result;
	ts_param_st ts_param;
	std::map<std::string, std::string> pcache;
	for (posi_vec_t::iterator pit = ps.begin(); pit != ps.end(); ++pit)
	{
		//ϯλ���ǿ���״̬����
		if (pcache.find(pit->seatid) == pcache.end())
		{
			result = BCRequestQryTradeParam_851217(handle, pit->seatid, "MSTATUS", ts_param);
			pcache.insert(std::make_pair(pit->seatid, ts_param.para_val));
			if (strcmp(ts_param.para_val, "1"))
				continue;
		}
		else
		{
			if (pcache[pit->seatid] != "1")
				continue;
		}
		
		result = BCRequestPosiOffsetVolume_854058(handle, *pit);
		if (!result)
		{
			SAFE_COUT_BEGINE << "��ѯ�ͻ��ֲֵĿ�ƽ����ʧ��," << FmtPosiID(*pit) << "," << result << SAFE_COUT_END;
			continue;
		}

		if (pit->can_offset_vol == 0)
			continue;

		inst_market_st m = { 0 };
		result = BCRequestInstMarketData_851503(handle, pit->instrumentid, m);
		if (!result)
		{
			SAFE_COUT_BEGINE << pit->instrumentid << "��Լ�������ݲ�ѯʧ��," << result
				<< FmtPosiID(*pit) << "��ǿƽ�˳ֲ�ʧ�ܣ�" << SAFE_COUT_END;
			continue;
		}

		result = BCRequestOffsetOrderInsert(handle, *pit, m);
		if (!result)
		{
			SAFE_COUT_BEGINE << FmtPosiID(*pit) << ",����ǿƽָ��ʧ��," << result << SAFE_COUT_END;
			continue;
		}

		SAFE_COUT_BEGINE << FmtPosiID(*pit) << ",����ǿƽָ��ɹ�" << SAFE_COUT_END;
		ret++;
	}
	return ret;
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

void market_calc_capital()
{
	SAFE_COUT_BEGINE << "��̬�������ͻ�Ȩ�棭ǿƽ�߳̿�ʼ����" << SAFE_COUT_END;
	Scoped_BCHANDLE handle;
	BCResult result;
	for (;;)
	{
		depth_market_data_st dmd = { 0 };
		std::unique_lock<std::mutex> lkc(g_market_mu);
		g_market_cv.wait(lkc, []()->bool { return !g_qdmd.empty(); });
		ccp_vec_t ccp;

		{
			std::unique_lock<std::mutex> lkc(g_ccp_mu);
			ccp = g_ccp;
		}

		if (!MdApi_popDepthMarketData(dmd))
			continue;

		for (ccp_vec_t::iterator it = ccp.begin(); it != ccp.end(); ++it)
		{
			if (!it->cps.empty())
			{
				double tmp_fp = 0;
				for (posi_vec_t::iterator pit = it->cps.begin(); pit != it->cps.end(); ++pit)
				{
					if (strcmp(dmd.InstrumentID, pit->instrumentid) == 0)
						pit->float_profit = calcProfit(*pit, dmd.LastPrice);
					tmp_fp += pit->float_profit;
				}

				if (it->cc.float_profit != tmp_fp)
				{
					it->cc.available += tmp_fp - it->cc.float_profit;
					it->cc.dynamic_capital += tmp_fp - it->cc.float_profit;
					it->cc.float_profit = tmp_fp;
				}
			}
			
			if (it->cc.dynamic_capital < it->force_line)
			{
				int vol = force_cust_positions(handle, it->cps);
				if (vol > 0)
				{
					SAFE_COUT_BEGINE << dmd.InstrumentID << "���¼�" << dmd.LastPrice << "����ͻ�����ӯ��" << it->cc.float_profit << ","
						<< "��̬Ȩ��" << it->cc.dynamic_capital << ",Ȩ��С��ǿƽ��" << it->force_line << "����ǿƽ"
						<< SAFE_COUT_END;
					result = BCRequestCustStatusSet_851005(handle, it->cc.cust_no, "4");
					if (!result)
					{
						SAFE_COUT_BEGINE << it->cc.cust_no << "���ÿͻ�ֻ��ƽ��״̬ʧ��," << result << SAFE_COUT_END;
					}
				}

			}
		}


		//for each(depth_market_data_st var in MdApi_GetDepthMarketData())
		//{
		//	for (ccp_vec_t::iterator it = ccp.begin(); it != ccp.end(); ++it)
		//	{
		//		//int times = 0;
		//		//it->cc.float_profit = 0;
		//		double tmp_fp = 0;
		//		for (posi_vec_t::iterator pit = it->cps.begin(); pit != it->cps.end(); ++pit)
		//		{
		//			if (strcmp(var.InstrumentID, pit->instrumentid) == 0)
		//				pit->float_profit = calcProfit(*pit, var.LastPrice);
		//			tmp_fp += pit->float_profit;
		//		}

		//		if (it->cc.float_profit != tmp_fp)
		//		{
		//			it->cc.available += tmp_fp - it->cc.float_profit;
		//			it->cc.dynamic_capital += tmp_fp - it->cc.float_profit;
		//			it->cc.float_profit = tmp_fp;
		//		}

		//		if (it->cc.dynamic_capital < it->force_line)
		//		{
		//			if (force_cust_positions(handle, it->cps) > 0)
		//			{
		//				result = BCRequestCustStatusSet_851005(handle, it->cc.cust_no, "4");
		//				if (!result)
		//				{
		//					SAFE_COUT_BEGINE << it->cc.cust_no << "���ÿͻ�ֻ��ƽ��״̬ʧ��," << result << SAFE_COUT_END;
		//				}
		//			}
		//			
		//		}
		//	}
		//}
	}
}

void force()
{
	SAFE_COUT_BEGINE << "��̨Ȩ����£�ǿƽ�߳̿�ʼ����" << SAFE_COUT_END;
	BCResult result;
	cust_capital_st sc;
	Scoped_BCHANDLE handle;
	for (; ;)
	{
		std::this_thread::sleep_for(std::chrono::seconds(15));
	
		ccp_vec_t ccp;
		{
			std::unique_lock<std::mutex> lkc(g_ccp_mu);
			ccp = g_ccp;
		}

		for (ccp_vec_t::iterator it = ccp.begin(); it != ccp.end(); ++it)
		{
			result = BCRequestCustCaptial_854196(handle, it->cc.cust_no, sc);

			if (!result)
			{
				SAFE_COUT_BEGINE << it->cc.cust_no << "��ѯ�ͻ�Ȩ��ʧ�ܣ�����̽��ͻ��Ƿ�ǿƽ" << SAFE_COUT_END;
				continue;
			}

			if (sc.dynamic_capital >= it->force_line)
				continue;

			//SAFE_COUT_BEGINE << it->cc.cust_no << "Ȩ�����" << lowerlimit << "," << "��ʼǿƽ�ͻ��ֲ�" << SAFE_COUT_END;

			posi_vec_t p;
			result = BCRequestCustPosition_854048(handle, it->cc.cust_no, p);
			if (!result)
			{
				SAFE_COUT_BEGINE << it->cc.cust_no << "��ѯ�ͻ��ֲ�ʧ�ܣ�" << result << SAFE_COUT_END;
				continue;
			}

			if (p.empty())
				continue;

			if (!force_cust_positions(handle, p))
				continue;

			SAFE_COUT_BEGINE << "��ѯ��̨ȡ�ÿͻ���̬Ȩ��" << sc.dynamic_capital << "С��ǿƽ��" << it->force_line << "����ǿƽ" << SAFE_COUT_END;

			result = BCRequestCustStatusSet_851005(handle, it->cc.cust_no, "4");
			if (!result)
			{
				SAFE_COUT_BEGINE << it->cc.cust_no << "���ÿͻ�ֻ��ƽ��״̬ʧ��," << result << SAFE_COUT_END;
			}
		}

		//std::vector<rc_desc>  d;
		//{
		//	std::unique_lock<std::mutex> lkc(g_RCforce_mutex);
		//	d = g_RCforce;
		//}
		//for (std::vector<rc_desc>::iterator it = d.begin(); it != d.end(); ++it)
		//{
		//	double lowerlimit = atof(it->paramlist);
		//	//cc_cp_st ccp;

		//	//if (lowerlimit == 0)
		//	//	continue;

		//	result = BCRequestCustCaptial_854196(handle, it->cust_no, sc);
		//	
		//	if (!result)
		//	{
		//		SAFE_COUT_BEGINE << it->cust_no  << "��ѯ�ͻ�Ȩ��ʧ�ܣ�����̽��ͻ��Ƿ�ǿƽ" << SAFE_COUT_END;
		//		continue;
		//	}

		//	if (sc.dynamic_capital >= lowerlimit)
		//		continue;

		//	//SAFE_COUT_BEGINE << it->cust_no << "Ȩ�����" << lowerlimit << "," << "��ʼǿƽ�ͻ��ֲ�" << SAFE_COUT_END;
		//	
		//	posi_vec_t p;
		//	result = BCRequestCustPosition_854048(handle, it->cust_no, p);
		//	if (!result)
		//	{
		//		SAFE_COUT_BEGINE << it->cust_no << "��ѯ�ͻ��ֲ�ʧ�ܣ�" << result << SAFE_COUT_END;
		//		continue;
		//	}

		//	if (p.empty())
		//		continue;

		//	if (!force_cust_positions(handle, p))
		//		continue;
		//	
		//	result = BCRequestCustStatusSet_851005(handle, it->cust_no, "4");
		//	if (!result)
		//	{
		//		SAFE_COUT_BEGINE << it->cust_no << "���ÿͻ�ֻ��ƽ��״̬ʧ��," << result << SAFE_COUT_END;
		//	}
		//}
	}
}

void qry_capital_position()
{
	SAFE_COUT_BEGINE << "��ѯ�ͻ�Ȩ��ͳֲ��߳̿�ʼ����" << SAFE_COUT_END;
	BCResult result;
	Scoped_BCHANDLE handle;
	//cust_capital_st sc;
	posi_vec_t ps;
	int local_seq = 0;
	std::vector<rc_desc> d;
	ccp_vec_t::iterator ccpit;
	for (;;)
	{
		std::this_thread::sleep_for(std::chrono::seconds(5));

		if (g_force_seq > local_seq)
		{
			//���С��ǿƽ�ͻ����и��£����¼���g_ccp�пͻ�

			{
				std::unique_lock<std::mutex> lkc(g_RCforce_mutex);
				d = g_RCforce;
			}

			{
				//ɾ��g_ccp����Ȩ��С��ǿ�ڱ����
				std::unique_lock<std::mutex> lkc(g_ccp_mu);
				ccpit = std::remove_if(g_ccp.begin(), g_ccp.end(), [&](cc_cp_st &c)->bool {
					return std::find_if(d.begin(), d.end(), [&](rc_desc &desc)->bool {
						return strcmp(c.cc.cust_no, desc.cust_no) == 0;
					}) == d.end();
				});

				g_ccp.erase(ccpit, g_ccp.end());

				//����µ�Ȩ��С��ǿƽ�ͻ���g_ccp
				std::for_each(d.begin(), d.end(), [](rc_desc &desc) {
					if (std::find_if(g_ccp.begin(), g_ccp.end(), [&](cc_cp_st &c)->bool {
						return strcmp(desc.cust_no, c.cc.cust_no) == 0;
					}) == g_ccp.end())
					{
						cc_cp_st ccp;
						strcpy(ccp.cc.cust_no, desc.cust_no);
						ccp.force_line = atof(desc.paramlist);
						g_ccp.push_back(ccp);
					}
				});

				//�޸Ĳ���ֵ
				std::for_each(g_ccp.begin(), g_ccp.end(), [&](cc_cp_st &c) {
					std::vector<rc_desc>::iterator dit;
					dit = std::find_if(d.begin(), d.end(), [&](rc_desc &desc)->bool {
						return strcmp(c.cc.cust_no, desc.cust_no) == 0;
					});
					if (dit != d.end())
						c.force_line = atof(dit->paramlist);
				});
			}

			local_seq = g_force_seq;
		}

		for (ccpit = g_ccp.begin(); ccpit != g_ccp.end(); ++ccpit)
		{
			cust_capital_st cap;
			result = BCRequestCustCaptial_854196(handle, ccpit->cc.cust_no, cap);
			if (!result)
			{
				SAFE_COUT_BEGINE << ccpit->cc.cust_no << "��ѯ�ͻ�Ȩ��ʧ�ܣ�����̽��ͻ��Ƿ�ǿƽ" << SAFE_COUT_END;
				continue;
			}

			if (ccpit->cc.margin != cap.margin)
			{
				result = BCRequestCustPosition_854048(handle, ccpit->cc.cust_no, ccpit->cps);
				std::for_each(ccpit->cps.begin(), ccpit->cps.end(), [](cust_position_st &p) { MdApi_SubscribeMarketData(p.instrumentid); });
			}
			ccpit->cc = cap;

		}
	}
}

void clearance(cfg_exch_clearance_st exch_clr)
{
	SAFE_COUT_BEGINE << exch_clr.exchangeid << "��������������߳̿�ʼ����" << SAFE_COUT_END;
	Scoped_BCHANDLE handle;
	BCResult result;
	std::vector<rc_desc> forbid_desc;
	boost::posix_time::time_duration td_early, td_late;
	
	//SAFE_COUT_BEGINE << exch_clr.exchangeid << "day:" << exch_clr.day_clear << SAFE_COUT_END;
	//SAFE_COUT_BEGINE << exch_clr.exchangeid << "nigth:" << exch_clr.nigth_clear << SAFE_COUT_END;

	if (exch_clr.nigth_clear > exch_clr.day_clear)
	{
		td_early = exch_clr.day_clear;
		td_late = exch_clr.nigth_clear;	
	}
	else
	{
		td_early = exch_clr.nigth_clear;
		td_late = exch_clr.day_clear;
	}

	for (;;)
	{
		boost::posix_time::ptime pt_now =  boost::posix_time::second_clock::local_time();
		//boost::posix_time::time_duration td = boost::posix_time::duration_from_string("15:25:00");
		//SAFE_COUT_BEGINE << pt_now << SAFE_COUT_END;
		//SAFE_COUT_BEGINE << pt_now.time_of_day() << SAFE_COUT_END;
		boost::posix_time::ptime pt_sleep_until;
		if (pt_now.time_of_day() <= td_early)
		{
			pt_sleep_until = boost::posix_time::ptime(pt_now.date(), td_early);
			
		}
		else if (pt_now.time_of_day() > td_early && pt_now.time_of_day() <= td_late)
		{
			pt_sleep_until = boost::posix_time::ptime(pt_now.date(), td_late);
		}
		else if (pt_now.time_of_day() > td_late)
		{
			pt_sleep_until = boost::posix_time::ptime(pt_now.date() + boost::gregorian::days(1), td_early);
		}
		//SAFE_COUT_BEGINE << pt_now << SAFE_COUT_END;
		SAFE_COUT_BEGINE << exch_clr.exchangeid << "�����������������ʱ��" << pt_sleep_until << SAFE_COUT_END;
		time_t t = mktime(&boost::posix_time::to_tm(pt_sleep_until));
		std::this_thread::sleep_until(std::chrono::system_clock::from_time_t(t));
		
		SAFE_COUT_BEGINE << exch_clr.exchangeid << "��������ʼ�������" << SAFE_COUT_END;
		result = BCRequestRC_860000(handle, forbid_desc, "0001");
		if (!result)
		{
			SAFE_COUT_BEGINE << "��ѯ�ͻ���ֹ��ҹ����ʧ��," << result << SAFE_COUT_END;
			continue;
		}

		for each (rc_desc var in forbid_desc)
		{
			int forbid = atoi(var.paramlist);
			if (forbid != 1)
				continue;
			
			posi_vec_t all_p;
			result = BCRequestCustPosition_854048(handle, var.cust_no, all_p);
			if (!result)
			{
				SAFE_COUT_BEGINE << "��ѯ�ͻ�" << var.cust_no << "�ֲ�ʧ�ܣ�ǿƽ�ͻ��ֲ�ʧ��!" << SAFE_COUT_END;
				continue;
			}
			posi_vec_t ep;
			std::copy_if(all_p.begin(), all_p.end(), std::back_inserter(ep),
				([&](const cust_position_st &p)->bool { return exch_clr.exchangeid == p.exchangeid; }));

			force_cust_positions(handle, ep);
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

int main()
try
{
	////clearance("");
	//bool test = true;
	//std::condition_variable cv;
	//std::mutex mu;
	//std::unique_lock<std::mutex> lkc(mu);
	//cv.wait_for(lkc, std::chrono::hours(1), [&]()->bool {return test; });

	//test = false;
	//cv.wait_for(lkc, std::chrono::seconds(10), [&]()->bool {return test; });
	read_conf();
	BCCLT_init();
	std::cout << "BCCLT��ʼ���ɹ�" << std::endl;
	MdApi_init();
	
	std::for_each(g_conf.exch_clrs.begin(), g_conf.exch_clrs.end(),
		([](cfg_exch_clearance_st &clr) { new std::thread(std::bind(clearance, clr)); }));
	
	std::thread qry_cp_the(qry_capital_position);
	std::thread market_force(market_calc_capital);
	std::thread backend_force(force);
	//int max_seq = 0;
	Scoped_BCHANDLE handle;
	BCResult result;
	for (;;)
	{
		int seq = 0;
		result = BCRequestRCMaxSeq_860002(handle, seq, "", "0002", "");
		if (result)
		{
			if (seq > g_force_seq)
			{
				std::unique_lock<std::mutex> lkc(g_RCforce_mutex);
				result = BCRequestRC_860000(handle, g_RCforce, "0002");
				if (!result)
				{
					SAFE_COUT_BEGINE << "��ѯ�ͻ�ǿƽ��ع���ʧ�ܣ�" << result << SAFE_COUT_END;
				}
				else
				{
					g_force_seq = seq;
				}
			}
		}
		else
		{
			SAFE_COUT_BEGINE << "��ѯ�ͻ�ǿƽ��ع������������ʧ��" << result << SAFE_COUT_END;
		}
		
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
    return 0;
}
catch (const std::exception &err)
{
	std::cerr << "�������쳣��" << err.what() << std::endl;
}

