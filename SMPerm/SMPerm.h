
// SMPerm.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSMPermApp: 
// �йش����ʵ�֣������ SMPerm.cpp
//

class CSMPermApp : public CWinApp
{
public:
	CSMPermApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSMPermApp theApp;