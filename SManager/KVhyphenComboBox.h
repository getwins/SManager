#pragma once
#include "SMutility.h"
// CKVhyphenComboBox

class CKVhyphenComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CKVhyphenComboBox)

public:
	CKVhyphenComboBox();
	virtual ~CKVhyphenComboBox();

protected:
	CString m_SelKey;
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	CString GetCurSelKey();
	CString GetCurSelVal();
	hyphenstr GetCurSelKV();
	void SelectKey(CString key);
};

class CCTComboBox : public CKVhyphenComboBox
{
	DECLARE_DYNAMIC(CCTComboBox)

	DECLARE_MESSAGE_MAP()

	virtual void PreSubclassWindow();
};

class CMTComboBox : public CKVhyphenComboBox
{
	DECLARE_DYNAMIC(CMTComboBox)

	DECLARE_MESSAGE_MAP()

	virtual void PreSubclassWindow();
};

class CCustComboBox : public CKVhyphenComboBox
{
	DECLARE_DYNAMIC(CCustComboBox)

	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
};

class CExchangeComboBox : public CKVhyphenComboBox
{
	DECLARE_DYNAMIC(CExchangeComboBox)

	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
};

class CProductComboBox : public CKVhyphenComboBox
{
	DECLARE_DYNAMIC(CProductComboBox)

	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	void FilterByExchangeid(const char *exchangeid);
};

class CCustClassComboBox : public CKVhyphenComboBox
{
public:
	CCustClassComboBox(int level) : m_level(level){}

	DECLARE_DYNAMIC(CCustClassComboBox)

	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	int m_level;
};

class CTradeLimitTypeComboBox : public CKVhyphenComboBox
{
public:
	DECLARE_DYNAMIC(CTradeLimitTypeComboBox)

	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
};