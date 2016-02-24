#pragma once

#include <string>
//#include <vector>
//#include <atomic>
#include <condition_variable>
#include <queue>

///�������
struct depth_market_data_st
{
	///������
	char TradingDay[9];
	///��Լ����
	char	InstrumentID[81];
	///����������
	char	ExchangeID[9];
	///��Լ�ڽ������Ĵ���
	char	ExchangeInstID[31];
	///���¼�
	double	LastPrice;
	///�ϴν����
	double	PreSettlementPrice;
	///������
	double	PreClosePrice;
	///��ֲ���
	double	PreOpenInterest;
	///����
	double	OpenPrice;
	///��߼�
	double	HighestPrice;
	///��ͼ�
	double	LowestPrice;
	///����
	int	Volume;
	///�ɽ����
	double	Turnover;
	///�ֲ���
	double	OpenInterest;
	///������
	double	ClosePrice;
	///���ν����
	double	SettlementPrice;
	///��ͣ���
	double	UpperLimitPrice;
	///��ͣ���
	double	LowerLimitPrice;
	///����ʵ��
	double	PreDelta;
	///����ʵ��
	double	CurrDelta;
	///����޸�ʱ��
	char	UpdateTime[9];
	///����޸ĺ���
	int	UpdateMillisec;
	///�����һ
	double	BidPrice1;
	///������һ
	int	BidVolume1;
	///������һ
	double	AskPrice1;
	///������һ
	int	AskVolume1;
	///����۶�
	double	BidPrice2;
	///��������
	int	BidVolume2;
	///�����۶�
	double	AskPrice2;
	///��������
	int	AskVolume2;
	///�������
	double	BidPrice3;
	///��������
	int	BidVolume3;
	///��������
	double	AskPrice3;
	///��������
	int	AskVolume3;
	///�������
	double	BidPrice4;
	///��������
	int	BidVolume4;
	///��������
	double	AskPrice4;
	///��������
	int	AskVolume4;
	///�������
	double	BidPrice5;
	///��������
	int	BidVolume5;
	///��������
	double	AskPrice5;
	///��������
	int	AskVolume5;
	///���վ���
	double	AveragePrice;
	///ҵ������
	char	ActionDay[9];
};

extern std::mutex g_market_mu;
extern std::condition_variable g_market_cv;

//extern std::atomic<size_t> g_qsize;
extern std::queue<depth_market_data_st> g_qdmd;

//typedef std::vector<depth_market_data_st> dmd_vec_t;

int MdApi_init();
int MdApi_UnInit();

int MdApi_SubscribeMarketData(const std::string &inst);

//dmd_vec_t MdApi_GetDepthMarketData();

bool MdApi_popDepthMarketData(depth_market_data_st &o_dmd);

//depth_market_data_st MdApi_GetMarketData(const std::string &inst);
