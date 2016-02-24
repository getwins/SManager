#pragma once

#include <vector>
#include <iostream>
#include <map>

struct oper_sm_perm_st
{
	char cust_no[16];
	char type[5];
	char param[201];
};

//function 851345 description  "��������ģ���ܱ��ѯ"  reqlevel 2
struct commission_template_no_st
{
	char template_no[16];
	char template_name[201];
};

//function 851347 description  "��֤����ģ���ܱ��ѯ"  reqlevel 2
struct margin_template_no_st
{
	char template_no[16];
	char template_name[201];
};

struct cust_class_st
{
	int level;
	char subclass_no[13];
	char subclass_name[41];
	char level_name[41];
	int parent_level;
	char parent_subclass_no[13];
	char parent_subclass_name[41];
	int id;
};

struct cust_basic_st
{
	char	scust_no[11];		//0.1 �ͻ���
	char	sname[21];		//6.����
	char	cust_class[13]; //�ͻ���
	char	sstatus0[2];	//108.״̬���ͻ�״̬
	char	card_type[3];
	char	card_no[51];
};

struct rc_desc_st
{
	char cust_no[16];
	char classflag[5];
	char type[5];
	char paramlist[201];
};

//
///* ������ͻ���			output:sholder_ac_no
//�ͻ�����			sname
//�ͻ�ȫ��			sall_name
//֤������			scust_type2
//֤������			scert_no
//�ʱ�			spost_code
//��ַ			saddr
//�绰			sphone
//�ֻ�			sphone3
//����			sphone2
//�����ʼ�			semail
//����			scust_auth2
//����֤��			scust_limit2
//��ͬ���			sbankname2
//�ͻ���			sserial1
//�����ʽ���			sstatus2
//��������			sdate0
//��������			sdate1
//�ͻ�����			sstatus1
//ָ���´���1			scust_auth
//ָ���´������֤��1			scust_limit
//ָ���´���2			spager
//ָ���´������֤��2			scard0
//ָ���´���3			sstation1
//ָ���´������֤��3			scard1
//�ʽ������1			scert_no2
//�ʽ���������֤��1			sbank_acc
//�ʽ������2			scert_addr
//�ʽ���������֤��2			sbank_acc2
//��ע			snote
//�ͻ�״̬			sstatus3
//�������			semp_no
//��������Ա			scust_no
//��������Ա			scust_no2
//��֯��������			sname2
//��֯�������			scusttypes
//������������			sbankname
//�����������			ssectypes
//������ */			vsvarstr1

struct cust_base_info_st
{
	//char	scust_no[11];		//0.1 �ͻ���
	//char	sname[21];		//6.����
	//char	cust_class[13]; //�ͻ���
	//char	sstatus0[2];	//108.״̬���ͻ�״̬
	//char	card_type[3];
	//char	card_no[51];
	cust_basic_st basic;
	commission_template_no_st commission_temp;
	margin_template_no_st margin_temp;
	std::vector<rc_desc_st> rcs;
};

typedef std::vector<cust_base_info_st>::iterator cbi_iter_t;
struct cust_login_info_st
{
	char cur_login_date[9];
	char cur_login_time[9];
	char cur_login_ip[31];
	char cur_login_mac[31];
	char cur_app_name[2];
	char login_status[2];
};
//854196
//�ʽ��ʺ�	sholder_ac_no
//����	scurrency_type
//�ͻ����	sall_name
//��������	sdate0
//���ս��	damt3
//�����ʽ�	damt26
//�򶳽�	damt29
//������	damt30
//����������	damt33
//������	damt6
//��֤��	damt13
//����֤��	damt1
//ƽ��ӯ��	damt11
//����ӯ��	damt8
//��;�ʽ�	damt10
//δ����ӯ��	damt18
//�������	damt4
//���ճ���	damt5
//��̬Ȩ��	damt28
//�����ʽ�	damt25
//�ܱ�֤��	damt23
//�ܽ�������֤��	damt24
//��Ѻ���	damt19
//���ý��	damt31
//���ռ���	sstatus3
//���ն�1	damt22
//���ն�2	damt21
struct cust_capital_st
{
	char cust_no[16];
	char scurrency_type[3];
	double yd_balance; //���ս��damt3
	double available; //�����ʽ�damt26
	double margin; //�ܱ�֤��
	double buy_frzn_margin; //�򶳽ᱣ֤��
	double sell_frzn_margin; //�򶳽ᱣ֤��
	double commission; //������
	double frzn_commission;//����������
	double offset_profit; //ƽ��ӯ��
	double float_profit;// ����ӯ��
	double dynamic_capital; //��̬Ȩ��
	double frzn_moeny; //�����ʽ�
	double td_money_in;
	double td_money_out;
	char risk_level[2];
	double risk_degree;
};

inline bool operator == (const cust_capital_st &lhs, const cust_capital_st &rhs)
{
	return lhs.yd_balance == rhs.yd_balance
		&& lhs.available == rhs.available
		&& lhs.margin == rhs.margin
		&& lhs.buy_frzn_margin == rhs.buy_frzn_margin
		&& lhs.sell_frzn_margin == rhs.sell_frzn_margin
		&& lhs.commission == rhs.commission
		&& lhs.frzn_commission == rhs.frzn_commission
		&& lhs.offset_profit == rhs.offset_profit
		&& lhs.float_profit == rhs.float_profit
		&& lhs.dynamic_capital == rhs.dynamic_capital
		&& lhs.frzn_moeny == rhs.frzn_moeny
		&& lhs.td_money_in == rhs.td_money_in
		&& lhs.td_money_out == rhs.td_money_out
		&& strcmp(lhs.risk_level, rhs.risk_level) == 0
		&& lhs.risk_degree == rhs.risk_degree;
}

inline bool operator != (const cust_capital_st &lhs, const cust_capital_st &rhs)
{
	return !(lhs == rhs);
}
//function 854186 description  "ʵʱ�ֲ���ϸ��ѯ"  reqlevel 3
//��������		sdate2
//��������		sdate3
//�ɽ����		lvol9
//ϯλ��		sserial2
//�ͻ���		sholder_ac_no
//�ͻ����		sname
//������		smarket_code
//����		sstatus3
//���ױ���		sholder_ac_no2
//Ʒ�ִ���		sstock_code
//������		sdate1
//������־		sstatus2
//Ͷ����־		sstatus4
//�ֲ�����		lvol8
//�ֲּ۸�		damt21
//���ּ۸�		damt26
//�ֱֲ�֤��		damt1
//��������֤��		damt2
//ÿ������		lvol10
//�ֲ�����		sorder0
//������ȼ�		sorder1
//�������ɽ���		sserial0
//�������		spost_code2
//��Ϻ�Լ����		sbankname
//������		damt8
//������		damt7

struct cust_td_position_detail_st
{
	char cust_no[16];
	char trade_date[9];
	char open_date[9];
	int trade_seq;
	char exchangeid[2];
	char scurrency_type[2];
	char tradeid[16];
	char product[9];
	char delivery_date[9];
	char instrumentid[41];
	char direction[2];
	char hedge_flag[2];
	int volume;
	int multiple;
	double hold_price;
	double open_price;
	double hold_margin;
	double yd_settle_price;
	double td_settle_price;
	double float_profit;
	double last_price;
};

//function 854048 description  "��ѯ�ͻ��ֲ�"  reqlevel 1
//����		sdate2
//����������		smarket_code
//�ͻ���		sholder_ac_no
//�ͻ�����		sname
//Ʒ�ִ���		sstock_code
//������		sdate1
//Ͷ�����		sstatus4
//��������		sstatus2
//�ֲ�����		lvol5
//�������		lvol12
//�ֲ־���		damt21
//����ӯ��		damt8
//���ױ���		sholder_ac_no2
//ϯλ��		sserial2
//���־���		damt26
//ÿ������		lvol10
//���¼�		damt23
//damt0
//
//�ͻ���		sholder_ac_no
//�ͻ�����		sname
//��ϲ��Դ���		spost_code2
//��ϲ�������		scust_limit
//��Ϻ�Լ		sbankname
//
//��֤��		damt1
//����֤��		damt2
//��ֲ־���		damt21
//���ֲ־���		damt26
//���ױ���		sholder_ac_no2
//������		smarket_code
//ϯλ��		sserial2

struct cust_position_st
{
	char cust_no[16];
	char open_date[9];
	char exchangeid[2];
	//char scurrency_type[2];
	char seatid[13];
	char trade_no[16];
	char product[9];
	char delivery_date[9];
	char instrumentid[41];
	char direction[2];
	char hedge_flag[2];
	int positions;
	int td_positions;
	double hold_average_price;
	double float_profit;
	double open_average_price;
	double last_price;
	int multiple;
	double settle_price;
	double market_profit;
	double marign;
	int can_offset_vol; //��ƽ��
	int td_can_offset_vol; //��ƽ����
};

//struct cust_position_id_st
//{
//	char cust_no[16];
//	char instrumentid[41];
//	char direction[2];
//	int volume;
//};
//
//std::ostream& operator << (std::ostream &os, const cust_position_id_st &posi_id)
//{
//	return os << posi_id.cust_no << " "
//		<< posi_id.instrumentid << " "
//		<< posi_id.direction << " "
//		<< posi_id.volume;
//}
//
//std::istream& operator >> (std::ostream &is, cust_position_id_st &posi_id)
//{
//	return is >> posi_id.cust_no >> posi_id.instrumentid >> posi_id.direction >> posi_id.volume;
//}
//
//inline bool operator < (const cust_position_id_st &lhs, const cust_position_id_st &rhs)
//{
//	if (strcmp(lhs.cust_no, rhs.cust_no) >= 0) return false;
//	if (strcmp(lhs.instrumentid, rhs.instrumentid) >= 0) return false;
//	if (strcmp(lhs.direction, rhs.direction) >= 0) return false;
//	if (lhs.volume >= rhs.volume) return false;
//	return true;
//}
//
//inline bool operator == (const cust_position_st &p, const cust_position_id_st &id)
//{
//	return strcmp(p.cust_no, id.cust_no) == 0
//		&& strcmp(p.instrumentid, id.instrumentid) == 0
//		&& strcmp(p.direction, id.direction) == 0
//		&& p.positions == id.volume;
//}
//function 852102 description  "��ʷʵʱί�в�ѯ"  reqlevel 3
//����		sdate0
//ί�к�		lserial0
//����Ա��		scust_no
//�ͻ���		sholder_ac_no
//�ͻ�����		sname
//������		smarket_code
//Ʒ�ִ���		sstock_code
//������		sdate1
//ί�е�״̬		sstatus0
//��ƽ���		sstatus1
//Ͷ�����		sstatus2
//��������		sstatus3
//ί�м�		damt0
//ί����		lvol0
//ʣ����		lvol1
//�ɽ���		damt1
//�ɽ���		lvol2
//ϵͳ��		sorder0
//ί��ʱ��		stime0
//�걨ʱ��		stime1
//���ױ���		sholder_ac_no2
//ί�з�ʽ		sholder_type
//�����û�		scust_no2
//����ʱ��		stime2
//ί�е�����		sstatus4
//ϯλ��		sserial2
//����		scurrency_type
//��ע		snote
//��������		scust_type
//�ɽ�����		scust_type2
//ֹ��ӯ����		damt3
//����ʱ��		stime3
//�������		sstat_type
//���Դ���		spost_code2
//��Լ1		sphone
//��Լ2		sphone2
//��Լ3		sphone3
//��Լ4		spager
//˽����Ϣ		sstation1
//˽�б�ʶ		semp_no
//˽����ˮ��		scust_limit
//��С�ɽ�����		lvol8

//function 854094 description  "��̨��ѯ����ί�е�"  reqlevel 3
//ί�к�		lserial0
//����Ա		scust_no
//�ͻ���		sholder_ac_no
//�ͻ�����		sname
//����������		smarket_code
//Ʒ�ִ���		sstock_code
//������		sdate1
//ί�е�״̬		sstatus0
//��ƽ���		sstatus3
//Ͷ�����		sstatus4
//��������		sstatus2
//ί�м�		damt0
//ί����		lvol0
//ʣ����		lvol2
//�ɽ���		damt2
//�ɽ���		lvol1
//ϵͳ��		sorder0
//ί��ʱ��		stime0
//�걨ʱ��		stime1
//���ױ���		sholder_ac_no2
//ί�з�ʽ		sholder_type
//�����û�		scust_no2
//����ʱ��		stime2
//ί�е�����		sstat_type2
//ϯλ��		sserial2
//����		scurrency_type
//��ע		snote
//�޸�����		lwithdraw_flag
//ǿƽ��־		sstatus1
//�µ�ϯλ		vsvarstr1
//���ر�־		smain_flag
//��������		scust_type
//�ɽ�����		scust_type2
//ֹ��ӯ����		damt3
//����ʱ��		stime3
//�������		sstat_type
//���Դ���		spost_code2
//��Լ1		sphone
//��Լ2		sphone2
//��Լ3		sphone3
//��Լ4		spager
//���ᱣ֤��		damt32
//����������		damt33
//˽����Ϣ		sstation1
//˽�б�ʶ		semp_no
//˽����ˮ��		scust_limit
//��С�ɽ����� 		lvol8

struct cust_order_st
{
	char cust_no[16];
	char date[9];
	int orderseq;
	char oper_no[11];
	char exchangeid[2];
	char productid[9];
	char delivery_date[9];
	char instrumentid[31];
	char order_status[2];
	char offset_flag[2];
	char hedge_flag[2];
	char direction[2];
	double order_price;
	int origin_total_volume;
	int total_volume;
	double traded_price;
	int traded_volume;
	char sysid[11];
	char order_time[9];
	char insert_time[9];
	char trade_no[16];
	char order_way[3];
	char cancel_cust[11];
	char cancel_time[9];
	char order_type[2];
	char seatid[13];
	char currency[3];
	char force_close[2];
};

//function 852103 description  "��ʷʵʱ�ɽ���ѯ"  reqlevel 3
//����		sdate0
//ί�к�		lserial0
//�ͻ���		sholder_ac_no
//�ͻ�����		sname
//���ױ���		sholder_ac_no2
//������		smarket_code
//Ʒ�ִ���		sstock_code
//������		sdate1
//��������		sstatus3
//�ɽ���		damt1
//�ɽ���		lvol2
//��ƽ���		sstatus1
//Ͷ�����		sstatus2
//ϵͳ��		sorder0
//�ɽ���		lvol3
//�������ɽ���		sorder2
//ϯλ��		sserial2
//����		scurrency_type
//�ɽ�ʱ��		stime0
//ǿƽ��־		sstatus0

//function 854095 description  "��̨��ѯ���ճɽ�"  reqlevel 3
///* ���룺����Ա��ϯλ�ţ��ͻ��ţ��ͻ��࣬��Լ�ţ��ϲ���־�����гɽ�����ǣ����֣��Ƿ�����ϼƱ�ǣ��µ�����Ա�����ܱ�� */
//input:scust_no, sserial2, sholder_ac_no, sserial1, scust_auth, sstatus1, smain_flag, scurrency_type, sstatus3, scust_no2, sstatus0
///* �����ί�кţ��ͻ��ţ��ͻ����ƣ����ױ��룬���������룬Ʒ�ִ��룬�����ڣ��ɽ������ɽ��ۣ���ƽ��ǣ�Ͷ����ǣ���������ϵͳ�ţ��ɽ��ţ��������ɽ��ţ�ϯλ�ţ����֣��ɽ�ʱ�䣬�����ѣ�����Ա��ǿƽ��־ */
//output : lserial0, sholder_ac_no, sname, sholder_ac_no2, smarket_code, sstock_code, sdate1, lvol1, damt2, sstatus3, sstatus4, sstatus2, sorder0, lvol11, sserial0, sserial2, scurrency_type, stime3, damt6, scust_no, sstatus0

//
//�����ί�к�			output : lserial0
//�ͻ���			sholder_ac_no
//�ͻ�����			sname
//���ױ���			sholder_ac_no2
//����������			smarket_code
//Ʒ�ִ���			sstock_code
//������			sdate1
//�ɽ���			lvol1
//�ɽ���			damt2
//��ƽ���			sstatus3
//Ͷ�����			sstatus4
//��������			sstatus2
//ϵͳ��			sorder0
//�ɽ���			lvol11
//�������ɽ���			sserial0
//ϯλ��			sserial2
//����			scurrency_type
//�ɽ�ʱ��			stime3
//������			damt6
//����Ա			scust_no
//ǿƽ��־			sstatus0


struct cust_trade_st
{
	char cust_no[16];
	char date[9];
	int orderseq;
	char trade_no[16];
	char exchangeid[2];
	char productid[9];
	char delivery_date[9];
	char instrumentid[31];
	char direction[2];
	double price;
	int volume;
	char offset_flag[2];
	char hedge_flag[2];
	char sysid[11];
	int tradeseq;
	char seatid[13];
	char currency[3];
	char time[9];
	double commission;
	char oper_no[31];
	char force_close[2];
};


struct cust_dynamic_info_st
{
	cust_login_info_st login_info; //��¼��Ϣ
	cust_capital_st captial; //�ʽ���Ϣ
	std::vector<cust_position_st> positions;
	std::vector<cust_td_position_detail_st> posi_detail;
	std::vector<cust_order_st> orders;
	std::vector<cust_trade_st> trades;
};

//struct custinfo_st
//{
//	//char	scust_no[11];		//0.1 �ͻ���
//	//char	sname[21];		//6.����
//	//char	sstatus0[2];	//108.״̬���ͻ�״̬
//	cust_base_info_st base_info;
//	cust_login_info_st login_info; //��¼��Ϣ
//	cust_capital_st captial; //�ʽ���Ϣ
//	std::vector<cust_position_st> position;
//	std::vector<cust_td_position_detail_st> posi_detail;
//};

//function 851230 description  "��������ѯ"  reqlevel 2
struct exchange_st
{
	char exchangeid[2];
	char chinese_name[41];
	char chinese_all_name[41];
	char english_name[51];
	char status[2];
};

struct seat_st
{
	char seat_no[15];
	char seat_name[21];
	char member_no[21];
	char seat_status[2];

};

//function 851238 description  "Ʒ�ֲ�ѯ"  reqlevel 1
//Ʒ�ִ���			sstock_code
//ÿ������			damt13
//������򣨽��׹���			sstatus1
//��С����			lvol0
//�������			lvol1
//���ֲ�			lvol2
//��ͣ����			damt0
//��ͣ����			damt1
//��ƽ�������ʣ�����			damt2
//��ƽ�������ʣ���������			damt3
//ƽ���������ʣ�����			damt4
//ƽ���������ʣ���������			damt5
//�����������ʣ�����			damt6
//�����������ʣ���������			damt7
//Ͷ����֤�𰴽����ȡ			damt8
//Ͷ����֤���ʰ�������ȡ			damt9
//��ֵ��֤�𰴽����ȡ			damt10
//��ֵ��֤���ʰ�������ȡ			damt11
//����			scurrency_type
//Ʒ������			sstatus2
//״̬			sstatus3
//��С�䶯��λ			damt12
//������պ��ս���			lvol3
//Ʒ������			sname
//��ĸ			lvol4
//ָ��			lvol5
//�������			sstatus4
//��ֵ��ƽ�������ʣ�����			damt14
//��ֵ��ƽ�������ʣ���������			damt15
//��ֵƽ���������ʣ�����			damt16
//��ֵƽ���������ʣ���������			damt17
//��ֵ�����������ʣ�����			damt18
//��ֵ�����������ʣ���������			damt19
//������ƽ�������ʣ�����			damt20
//������ƽ�������ʣ���������			damt21
//����ƽ���������ʣ�����			damt22
//����ƽ���������ʣ���������			damt23
//���������������ʣ�����			damt24
//���������������ʣ���������			damt25
//������֤�𰴽����ȡ			damt26
//������֤���ʰ�������ȡ			damt27
//�Ƿ�ʹ�������Ѻ			scust_type2
//��Լ����			sholder_ac_no2
//��Լ���			lvol6
//���ʽ			smain_flag2

struct product_st
{
	char productid[9];
	char name[21];
	char inst_rule[16];
	char trade_rule[16];
	char currency_type[3];
	char type[2];
	char status[2];
	double multiple;
	double price_tick;
	int min_lots;
	int max_lots;
	int max_position;
	double upper_limit_rate;
	double lower_limit_rate;
	double commission_rate_by_volume;
	double commission_rate_by_money;
	double margin_rate_by_volume;
	double margin_rate_by_money;
};

//function 851240 description  "��������ӦƷ�ֲ�ѯ"  reqlevel 1
///* ���룺����Ա������������ */
//input:scust_no, smarket_code
//	/* ��������������룬Ʒ�ִ��룬Ʒ������ */
//	output : smarket_code, sstock_code, sname
struct exchange_product_st
{
	char exchangeid[2];
	char productid[9];
	char product_name[21];
};


//function 854088 description  "���׺�Լ������Ϣ��ѯ"  reqlevel 2
//
//����������		smarket_code
//Ʒ�ִ���		sstock_code
//Ʒ������		sname2
//������		sdate1
//��Լ��		scust_auth
//���ֲ���		lvol5
//ÿ������		lvol10
//�������		scurrency_type2
//��С����		damt15
//�������		damt14
//���� 		scurrency_type
//��С�䶯��		damt6
//��Լ״̬		sstatus1
//function 854044 description  "��ѯ�ɽ��׺�Լ"  reqlevel 1
///* ���룺���������룬ϯλ�� */
//input:smarket_code, sserial2
//	/* �����������ӿڲ�Ҫʹ��sstatus0���ѱ�ԭ��ռ���ˣ����������룬Ʒ�ִ��룬�����ڣ���Լ�ţ���Լ���ƣ�ÿ�����������֣���С�䶯��λ����Լ״̬,�������̼�, ���ս����, ������ͣ��, ���յ�ͣ�ۣ���Լ�Ƿ�����Ϻ��������Ż� */
//	output : smarket_code, sstock_code, sdate1, scust_auth, scust_auth2, lvol10, scurrency_type, damt6, sstatus1, damt31, damt32, damt27, damt28, sstatus3
//
struct instrument_st
{
	char exchangeid[2];
	char productid[9];
	char product_name[21];
	char delivery_date[9];
	char delivery_rule[3];
	char instrumentid[21];
	char currency_type[3];
	int max_position;
	int multiple;
	double min_lots;
	double max_lots;
	double price_tick;
	char status[2];
};

inline bool operator < (const instrument_st &lhs, const instrument_st &rhs)
{
	return strcmp(lhs.instrumentid, rhs.instrumentid) < 0;
}


//function 851339 description  "��������ģ���ѯ"
//ģ���			sholder_ac_no
//ģ������			scusttypes
//������			smarket_code
//Ʒ��			sstock_code
//������			sdate0
//��ƽ�������ʣ�����			damt2
//��ƽ�������ʣ���������			damt3
//��ƽ���ӷ���1������			damt4
//��ƽ���ӷ���1����������			damt5
//��ƽ���ӷ���2������			damt6
//��ƽ���ӷ���2����������			damt7
//ƽ���������ʣ�����			damt8
//ƽ���������ʣ���������			damt9
//ƽ�񸽼ӷ���1������			damt10
//ƽ�񸽼ӷ���1����������			damt11
//ƽ�񸽼ӷ���2������			damt12
//ƽ�񸽼ӷ���2����������			damt13
//�����������ʣ�����			damt14
//�����������ʣ���������			damt15
//��ֵ��ƽ�������ʣ�����			damt16
//��ֵ��ƽ�������ʣ���������			damt17
//��ֵ��ƽ���ӷ���1(�����)			damt18
//��ֵ��ƽ���ӷ���1����������			damt19
//��ֵ��ƽ���ӷ���2(�����)			damt20
//��ֵ��ƽ���ӷ���2����������			damt21
//��ֵƽ���������ʣ�����			damt22
//��ֵƽ���������ʣ���������			damt23
//��ֵƽ�񸽼ӷ���1(�����)			damt24
//��ֵƽ�񸽼ӷ���1����������			damt25
//��ֵƽ�񸽼ӷ���2(�����)			damt26
//��ֵƽ�񸽼ӷ���2����������			damt27
//��ֵ�����������ʣ�����			damt28
//��ֵ�����������ʣ���������			damt29
//������ƽ�������ʣ�����			damt30
//������ƽ�������ʣ���������			damt31
//����ƽ���������ʣ�����			damt32
//����ƽ���������ʣ���������			damt33
//���������������ʣ�����			damt0
//���������������ʣ���������			damt1
struct commission_template_st
{
	char template_no[16];
	char template_name[201];
	char exchangeid[2];
	char productid[9];
	char delivery_date[9];
	double commission_rate_by_money;
	double commission_rate_by_volume;
	double offset_td_commission_rate_by_money;
	double offset_td_commission_rate_by_volume;
};

//function 851341 description  "��֤����ģ���ѯ"
struct margin_template_st
{
	char template_no[16];
	char template_name[201];
	char exchangeid[2];
	char productid[9];
	//char delivery_date[9];
	double margin_rate_by_money;
	double margin_rate_by_volume;
};

struct cust_commission_rate_st
{
	char cust_no[16];
	char exchangeid[2];
	char productid[9];
	char delivery_date[9];
	double commission_rate_by_money;
	double commission_rate_by_volume;
	double offset_td_commission_rate_by_money;
	double offset_td_commission_rate_by_volume;
};

struct cust_margin_rate_st
{
	char cust_no[16];
	char exchangeid[2];
	char productid[9];
	char delivery_date[9];
	double margin_rate_by_money;
	double margin_rate_by_volume;
};

struct system_info_st
{
	char system_date[9];
	char check_date[9];
	char next_tradingday[9];
	char host_date[9];
	char system_status[2];
	char system_time[9];
	long request_time;
};

struct operator_info_st
{

};

struct dict_st
{
	int dict_class;
	char dict_code[21];
	char dict_note[81];
};

//inline std::ostream operator << (std::ostream &os, dict_st &dict)
//{
//	return os << dict.dict_class << dict.dict_code << dict.dict_note
//}

//struct his_order_st
//{
//
//};

struct baseinfo_st
{
	//system_info_st sysinfo;
	std::vector<dict_st> dicts;
	std::vector<exchange_st> exchanges;
	std::vector<product_st> products;
	std::vector<seat_st> seats;
	std::vector<exchange_product_st> exchange_product;
	std::vector<instrument_st> instruments;
	std::vector<commission_template_no_st> commission_no;
	//std::vector<commission_template_st> commissions;
	std::map<std::string, std::vector<commission_template_st>> commissions;
	std::vector<margin_template_no_st> margin_no;
	//StrMtMap_t margins;
	std::map<std::string, std::vector<margin_template_st>> margins;
	std::vector<cust_class_st> cust_classes;
};

struct inst_market_st
{
	double last_price;
	double bid_price;
	double ask_price;
	double range; //�ǵ�
	double highest_price;
	double lowest_price;
	double open_price;
	double	upperlimit_price;
	///��ͣ���
	double	lowerlimit_price;
	int bid_vol;
	int ask_vol;
	int volume;
	int delta;
	double pre_close_price;
	///��ͣ���
	double settlement_price;
	double his_highest_price;
	double his_lowest_price;
};