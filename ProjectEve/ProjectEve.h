
// ProjectEve.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CProjectEveApp:
// �� Ŭ������ ������ ���ؼ��� ProjectEve.cpp�� �����Ͻʽÿ�.
//

class CProjectEveApp : public CWinApp
{
public:
	CProjectEveApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CProjectEveApp theApp;