
// MouseGenius.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMouseGeniusApp:
// �йش����ʵ�֣������ MouseGenius.cpp
//

class CMouseGeniusApp : public CWinApp
{
public:
	CMouseGeniusApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMouseGeniusApp theApp;