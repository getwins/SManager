#include "stdafx.h"
#include "MyBCRequest.h"


#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>


int g_drtpno;
extern int g_funcno;
extern char g_oper[16];

//extern int g_funcno;
//extern std::string g_oper_code;

BCResult MyBCRequest(BCHANDLE handle, FetchRowFunc_t fetcher)
{
	BCResult ret;
	int record;
	//int retcode;

	if (!BCCallRequest(handle, g_drtpno, 0, g_funcno, 10000, &ret.errcode, ret.errmsg))
		goto failed;

	BCGetRetCode(handle, &ret.errcode);
	BCGetStringFieldByName(handle, 0, "vsmess", ret.errmsg, sizeof(ret.errmsg));

	if (ret.errcode)
		goto failed;

	BCGetRecordCount(handle, &record);
	for (int i = 0; i < record; i++)
	{
		if (fetcher)
			fetcher(handle, i);
	}

	while (BCHaveNextPack(handle))
	{
		if (!BCCallNext(handle, 10000, &ret.errcode, ret.errmsg))
			goto failed;

		BCGetRetCode(handle, &ret.errcode);
		BCGetStringFieldByName(handle, 0, "vsmess", ret.errmsg, sizeof(ret.errmsg));
		if (ret.errcode)
			goto failed;

		BCGetRecordCount(handle, &record);
		for (int i = 0; i < record; i++)
		{
			if (fetcher)
				fetcher(handle, i);
		}
	}

failed:
	//if(MsgToMessageBox)
	//	if(ret.errcode)
	//		::AfxMessageBox(ret.errmsg, MB_ICONSTOP | MB_OK);
	//	else
	//		::AfxMessageBox(ret.errmsg, MB_ICONINFORMATION | MB_OK);
	//if (MsgToOutput)
	//	PostOutputMsg(ret.errmsg);

	//if (ret.errcode) 
	//{
	//	if(whichMsgTowhere & BCMSG_ERR_TO_MB)
	//		::AfxMessageBox(ret.errmsg, MB_ICONWARNING | MB_OK);
	//	if(whichMsgTowhere & BCMSG_ERR_TO_OP)
	//		PostOutputMsg(ret.errmsg);
	//}
	//else
	//{
	//	if(whichMsgTowhere & BCMSG_NOR_TO_MB)
	//		::AfxMessageBox(ret.errmsg, MB_ICONINFORMATION | MB_OK);
	//	if((whichMsgTowhere & BCMSG_NOR_TO_OP) && strlen(ret.errmsg) > 0)
	//		PostOutputMsg(ret.errmsg);
	//}

	return ret;
}

bool MyBCCLTInit(char *host, int port)
{
	if (!BCCCLTInit(100))
		return false;

	g_drtpno = BCAddDrtpNode(host, port);

	if (g_drtpno < 0)
		return false;

	BCSetDebugSwitch(true);

	BCHANDLE handle = BCNewHandle("cpack.dat");
	if (handle == NULL)
		return false;
	BCDeleteHandle(handle);

	return true;
}


BCResult BCRequestOperLogin_854000(char *oper_code, char *oper_passwd)
{
	Scoped_BCHANDLE handle;
	BCSetRequestType(handle, 854000);

	BCSetStringFieldByName(handle, 0, "scust_no", oper_code);
	BCSetStringFieldByName(handle, 0, "semp_pwd", oper_passwd);

	TCHAR host[MAX_PATH];
	::gethostname(host, MAX_PATH);
	BCSetStringFieldByName(handle, 0, "sphone", host);
	BCSetStringFieldByName(handle, 0, "sphone2", "34-02-86-70-4B-A2");
	BCSetStringFieldByName(handle, 0, "sholder_type2", "B"); //应用程序标识
	
	return MyBCRequest(handle);
}

BCResult BCRequestOperLogout_854001(char *oper_code, char *oper_passwd)
{
	Scoped_BCHANDLE handle;
	BCSetRequestType(handle, 854001);

	BCSetStringFieldByName(handle, 0, "scust_no", oper_code);
	BCSetStringFieldByName(handle, 0, "semp_pwd", oper_passwd);
	return MyBCRequest(handle);
}

BCResult BCRequestQryOperPerm_860003(BCHANDLE handle, char *oper_code, char *type, std::vector<oper_sm_perm_st> &o_perms)
{
	FetchRowFunc_t fetcher = [&](BCHANDLE handle, int row)
	{
		oper_sm_perm_st perm = { 0 };
		BCGetStringFieldByName(handle, row, "scust_no", perm.cust_no, sizeof(perm.cust_no));
		BCGetStringFieldByName(handle, row, "sserial0", perm.type, sizeof(perm.type));
		BCGetStringFieldByName(handle, row, "ssectypes", perm.param, sizeof(perm.param));
		o_perms.push_back(perm);
	};

	BCResetHandle(handle);
	BCSetRequestType(handle, 860003);
	BCSetStringFieldByName(handle, 0, "scust_no2", g_oper);
	BCSetStringFieldByName(handle, 0, "scust_no", oper_code);
	BCSetStringFieldByName(handle, 0, "sserial0", type);
	BCSetStringFieldByName(handle, 0, "ssectypes", "");

	return MyBCRequest(handle, fetcher);
}

BCResult BCRequestSetOperPerm_860004(BCHANDLE handle, char *flag, char *oper_code, char *type, char *param)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 860004);
	BCSetStringFieldByName(handle, 0, "scust_no2", g_oper);
	BCSetStringFieldByName(handle, 0, "sstatus0", flag);
	BCSetStringFieldByName(handle, 0, "scust_no", oper_code);
	BCSetStringFieldByName(handle, 0, "sserial0", type);
	BCSetStringFieldByName(handle, 0, "ssectypes", param);
	
	return MyBCRequest(handle);
}

BCResult BCRequestQryOper_851201(BCHANDLE handle, char *oper_code)
{
	BCResetHandle(handle);
	BCSetRequestType(handle, 851201);
	BCSetStringFieldByName(handle, 0, "scust_no2", g_oper);
	BCSetStringFieldByName(handle, 0, "scust_no", oper_code);
	
	return MyBCRequest(handle);
}