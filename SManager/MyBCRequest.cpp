#include "stdafx.h"
#include "MyBCRequest.h"
#include "SManager.h"

BCResult MyBCRequest(BCHANDLE handle, FetchRowFunc_t fetcher, int whichMsgTowhere)
{
	BCResult ret;
	int record;
	//int retcode;

	if (!BCCallRequest(handle, g_cfg.drtpno, g_cfg.branchno, g_cfg.funcno, g_cfg.bccall_timeout, &ret.errcode, ret.errmsg))
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
		if (!BCCallNext(handle, g_cfg.bccall_timeout, &ret.errcode, ret.errmsg))
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

	if (ret.errcode) 
	{
		if(whichMsgTowhere & BCMSG_ERR_TO_MB)
			::AfxMessageBox(ret.errmsg, MB_ICONWARNING | MB_OK);
		if(whichMsgTowhere & BCMSG_ERR_TO_OP)
			PostOutputMsg(ret.errmsg);
	}
	else
	{
		if(whichMsgTowhere & BCMSG_NOR_TO_MB)
			::AfxMessageBox(ret.errmsg, MB_ICONINFORMATION | MB_OK);
		if((whichMsgTowhere & BCMSG_NOR_TO_OP) && strlen(ret.errmsg) > 0)
			PostOutputMsg(ret.errmsg);
	}

	return ret;
}