#pragma once

#include "stdafx.h"
#include "nbccclt.h"

template<typename T>
COLORREF comp_color(T lhs, T rhs, COLORREF great, COLORREF less, COLORREF equal)
{
	if (lhs > rhs)
		return great;
	else if (lhs < rhs)
		return less;
	else
		return equal;
}

inline COLORREF comp_color(const char *direction, double LastPrice, double HoldPrice, COLORREF great, COLORREF less, COLORREF equal)
{
	if (strcmp(direction, "0") == 0)
	{
		return comp_color<double>(LastPrice, HoldPrice, great, less, equal);
	}
	else if (strcmp(direction, "1") == 0)
	{
		return comp_color<double>(HoldPrice, LastPrice, great, less, equal);
	}
	return COLORREF();
}

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

struct hyphenstr
{
	hyphenstr(const CString &str)
	{
		int pos = str.Find('-');
		if (pos >= 0 && pos < str.GetLength()) {
			left = str.Left(pos);
			right = str.Right(str.GetLength() - pos - 1);
		}
	}
	CString left, right;
	//hyphenstr(const std::string &str) {
	//	size_t pos = str.find('-');
	//	if (pos != std::string::npos) {
	//		left = str.substr(pos);
	//		right = str.substr(pos + 1);
	//	}
	//}

	//std::string left;
	//std::string right;
};

