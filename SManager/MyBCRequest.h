#pragma once

#include <afxstr.h>
#include <functional>
#include <string>
#include <iostream>
#include "nbccclt.h"


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
	operator CString() { return errmsg; };
};

inline std::ostream& operator << (std::ostream &os, const BCResult &result)
{
	return os << "errcode=" << result.errcode << ",errmsg=" << result.errmsg;
}

#define BCMSG_ERR_TO_MB 0x00000001
#define BCMSG_NOR_TO_MB	0x00000002
#define BCMSG_ERR_TO_OP 0x00000004
#define BCMSG_NOR_TO_OP 0x00000008

#define BCMSG_NO		0x00000000
#define BCMSG_TOMB		(0x00000001 | 0x00000002)
#define BCMSG_TOOP		(0x00000004 | 0x00000008)
#define BCMSG_TOALL		(0x00000001 | 0x00000002 | 0x00000004 |  0x00000008)

#define BCMSG_ERR_TOALL (0x00000001 | 0x00000004)
#define BCMSG_NOR_NOMB	(0x00000001 | 0x00000004 |  0x00000008)


BCResult MyBCRequest(BCHANDLE handle, FetchRowFunc_t fetcher = FetchRowFunc_t(), int whichMsgTowhere = BCMSG_ERR_TO_OP);
