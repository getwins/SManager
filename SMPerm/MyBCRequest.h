#pragma once

#include <functional>
#include <string>
#include <vector>
#include "nbccclt.h"


class Scoped_BCHANDLE
{
public:
	Scoped_BCHANDLE() {
		handle_ = BCNewHandle("cpack.dat");
	}
	~Scoped_BCHANDLE() {
		BCDeleteHandle(handle_);
	}
	operator BCHANDLE () {
		return handle_;
	}
private:
	BCHANDLE handle_;
};


typedef std::function<void(BCHANDLE, int)> FetchRowFunc_t;

struct BCResult
{
	BCResult()
	{
		errcode = 0;
		memset(errmsg, 0, sizeof(errmsg));
	}

	int errcode;
	char errmsg[256];

	operator bool() { return errcode == 0; };
	operator std::string() { return errmsg; };

	friend std::ostream& operator << (std::ostream &os, BCResult &result)
	{
		os << "errcode=" << result.errcode << ",";
		if (!result.errcode && strlen(result.errmsg) == 0)
			os << "errmsg=³É¹¦";
		else
			os << "errmsg=" << result.errmsg;
		return os;
	}
};


BCResult MyBCRequest(BCHANDLE handle, FetchRowFunc_t fetcher = FetchRowFunc_t());

bool MyBCCLTInit(char *host, int port);

BCResult BCRequestOperLogin_854000(char *oper_code, char *oper_passwd);
BCResult BCRequestOperLogout_854001(char *oper_code, char *oper_passwd);

struct oper_sm_perm_st
{
	char cust_no[16];
	char type[5];
	char param[201];
};

BCResult BCRequestQryOperPerm_860003(BCHANDLE handle, char *oper_code, char *type, std::vector<oper_sm_perm_st> &o_perms);
BCResult BCRequestSetOperPerm_860004(BCHANDLE handle, char *flag, char *oper_code, char *type, char *param);

BCResult BCRequestQryOper_851201(BCHANDLE handle, char *oper_code);