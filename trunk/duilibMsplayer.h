
// duilibMsplayer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "PlayFrameWnd.h"

// CduilibMsplayerApp:
// �йش����ʵ�֣������ duilibMsplayer.cpp
//

class CduilibMsplayerApp : public CWinApp
{
public:
	CduilibMsplayerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CduilibMsplayerApp theApp;