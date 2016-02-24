#pragma once
#include <afxstr.h>

inline CString Nfmt(double d)
{
	CString str;
	str.Format("%f", d);
	return str;
}

inline CString fundfmt(double fund)
{
	CString str;
	str.Format("%.2f", fund);
	return str;
}

inline CString intfmt(int vol)
{
	CString str;
	str.Format("%d", vol);
	return str;
}

inline CString inst_pricefmt(const char *inst, double price)
{
	CString str;
	str.Format("%.2f", price);
	return str;
}