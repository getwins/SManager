// KVhyphenComboBox.cpp : 实现文件
//

#include "stdafx.h"
#include "SManager.h"
#include "KVhyphenComboBox.h"


// CKVhyphenComboBox

IMPLEMENT_DYNAMIC(CKVhyphenComboBox, CComboBox)

CKVhyphenComboBox::CKVhyphenComboBox()
{

}

CKVhyphenComboBox::~CKVhyphenComboBox()
{
}

void CKVhyphenComboBox::PreSubclassWindow()
{
	this->ModifyStyleEx(CBS_SORT, CBS_DROPDOWNLIST, 0);
	CComboBox::PreSubclassWindow();
}

CString CKVhyphenComboBox::GetCurSelKey()
{
	int sel = GetCurSel();
	if (sel >= 0 && sel < GetCount()) {
		CString str;
		GetLBText(sel, str);
		hyphenstr hstr(str);
		return hstr.left;
	}

	return "";
}

CString CKVhyphenComboBox::GetCurSelVal()
{
	int sel = GetCurSel();
	if (sel >= 0 && sel < GetCount()) {
		CString str;
		GetLBText(sel, str);
		hyphenstr hstr(str);
		return hstr.right;
	}

	return CString();
}

hyphenstr CKVhyphenComboBox::GetCurSelKV()
{
	int sel = GetCurSel();
	if (sel >= 0 && sel < GetCount()) {
		CString str;
		GetLBText(sel, str);
		return hyphenstr(str);
	}
	return hyphenstr("");
}

void CKVhyphenComboBox::SelectKey(CString key)
{
	if (this->GetSafeHwnd() == NULL) {
		m_SelKey = key;
		return;
	}

	SetCurSel(-1);
	//int sel = -1;
	for (int i = 0; i < this->GetCount(); ++i)
	{
		CString str;
		GetLBText(i, str);
		hyphenstr hstr(str);
		if (hstr.left == key) {
			this->SetCurSel(i);
			break;
		}
	}
}


BEGIN_MESSAGE_MAP(CKVhyphenComboBox, CComboBox)
END_MESSAGE_MAP()



// CKVhyphenComboBox 消息处理程序


IMPLEMENT_DYNAMIC(CCTComboBox, CKVhyphenComboBox)

BEGIN_MESSAGE_MAP(CCTComboBox, CKVhyphenComboBox)
END_MESSAGE_MAP()

void CCTComboBox::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	theApp.WaitBaseInfoReady();
	for (int i = 0; i < theApp.m_BaseInfo.commission_no.size(); i++)
	{
		this->AddString(CString(theApp.m_BaseInfo.commission_no[i].template_no) + "-" + theApp.m_BaseInfo.commission_no[i].template_name);
	}

	if (m_SelKey.GetLength() > 0)
	{
		for (int i = 0; i < theApp.m_BaseInfo.commission_no.size(); i++)
		{
			if (m_SelKey == theApp.m_BaseInfo.commission_no[i].template_no)
			{
				this->SetCurSel(i);
				break;
			}
		}
	}

	CKVhyphenComboBox::PreSubclassWindow();
}




IMPLEMENT_DYNAMIC(CMTComboBox, CKVhyphenComboBox)

BEGIN_MESSAGE_MAP(CMTComboBox, CKVhyphenComboBox)
END_MESSAGE_MAP()

void CMTComboBox::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	theApp.WaitBaseInfoReady();
	for (int i = 0; i < theApp.m_BaseInfo.margin_no.size(); i++)
	{
		this->AddString(CString(theApp.m_BaseInfo.margin_no[i].template_no) + "-" + theApp.m_BaseInfo.margin_no[i].template_name);
	}

	if (m_SelKey.GetLength() > 0)
	{
		for (int i = 0; i < theApp.m_BaseInfo.margin_no.size(); i++)
		{
			if (m_SelKey == theApp.m_BaseInfo.margin_no[i].template_no)
			{
				this->SetCurSel(i);
				break;
			}
		}
	}

	CKVhyphenComboBox::PreSubclassWindow();
}

IMPLEMENT_DYNAMIC(CCustComboBox, CKVhyphenComboBox)

BEGIN_MESSAGE_MAP(CCustComboBox, CKVhyphenComboBox)
END_MESSAGE_MAP()

void CCustComboBox::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	std::vector<cust_base_info_st> cbis = theApp.GetCustBaseInfoList();
	for (int i = 0; i < cbis.size(); i++)
	{
		this->AddString(CString(cbis[i].basic.scust_no) + "-" + cbis[i].basic.sname);
	}

	if (m_SelKey.GetLength() > 0)
	{
		for (int i = 0; i < cbis.size(); i++)
		{
			if (m_SelKey == cbis[i].basic.scust_no)
			{
				this->SetCurSel(i);
				break;
			}
		}
	}

	CKVhyphenComboBox::PreSubclassWindow();
}



IMPLEMENT_DYNAMIC(CExchangeComboBox, CKVhyphenComboBox)

BEGIN_MESSAGE_MAP(CExchangeComboBox, CKVhyphenComboBox)
END_MESSAGE_MAP()

void CExchangeComboBox::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	theApp.WaitBaseInfoReady();
	//AddString(CString("!-") + "全部");
 
	for each (exchange_st e in theApp.m_BaseInfo.exchanges)
	{
		AddString(CString(e.exchangeid) + "-" + CString(e.chinese_name));
	}


	if (m_SelKey.GetLength() > 0)
	{
		
		for (int i = 0; i < theApp.m_BaseInfo.exchanges.size(); i++)
		{
			if (m_SelKey == theApp.m_BaseInfo.exchanges[i].exchangeid)
			{
				this->SetCurSel(i);
				break;
			}
		}
	}

	CKVhyphenComboBox::PreSubclassWindow();
}

IMPLEMENT_DYNAMIC(CProductComboBox, CKVhyphenComboBox)

BEGIN_MESSAGE_MAP(CProductComboBox, CKVhyphenComboBox)
END_MESSAGE_MAP()

void CProductComboBox::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	theApp.WaitBaseInfoReady();

	for each (exchange_product_st ep in theApp.m_BaseInfo.exchange_product)
	{
		this->AddString(CString(ep.productid) + "-" + ep.product_name);
	}

	if (m_SelKey.GetLength() > 0)
	{

		for (int i = 0; i < theApp.m_BaseInfo.exchange_product.size(); i++)
		{
			if (m_SelKey == theApp.m_BaseInfo.exchange_product[i].productid)
			{
				this->SetCurSel(i);
				break;
			}
		}
	}

	CKVhyphenComboBox::PreSubclassWindow();
}

void CProductComboBox::FilterByExchangeid(const char * exchangeid)
{
	theApp.WaitBaseInfoReady();
	this->ResetContent();
	for each (exchange_product_st ep in theApp.m_BaseInfo.exchange_product)
	{
		if (exchangeid == NULL || strcmp(exchangeid, ep.exchangeid) == 0)
			this->AddString(CString(ep.productid) + "-" + ep.product_name);

	}
}



IMPLEMENT_DYNAMIC(CCustClassComboBox, CKVhyphenComboBox)

BEGIN_MESSAGE_MAP(CCustClassComboBox, CKVhyphenComboBox)
END_MESSAGE_MAP()
void CCustClassComboBox::PreSubclassWindow()
{
	theApp.WaitBaseInfoReady();
	for each (cust_class_st var in theApp.m_BaseInfo.cust_classes)
	{
		if (var.level == m_level)
			this->AddString(CString(var.subclass_no) + "-" + var.subclass_name);
	}
}

IMPLEMENT_DYNAMIC(CTradeLimitTypeComboBox, CKVhyphenComboBox)

BEGIN_MESSAGE_MAP(CTradeLimitTypeComboBox, CKVhyphenComboBox)
END_MESSAGE_MAP()

void CTradeLimitTypeComboBox::PreSubclassWindow()
{
	this->AddString(CString("0-只可平仓"));
	this->AddString(CString("1-禁止交易"));

	if (m_SelKey.GetLength() > 0)
	{
		if(m_SelKey == "0")
			this->SetCurSel(0);
		else if(m_SelKey == "1")
			this->SetCurSel(1);
	}

	CKVhyphenComboBox::PreSubclassWindow();
}
