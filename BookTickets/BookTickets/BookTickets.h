
// BookTickets.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBookTicketsApp: 
// �йش����ʵ�֣������ BookTickets.cpp
//

class CBookTicketsApp : public CWinApp
{
public:
	CBookTicketsApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBookTicketsApp theApp;