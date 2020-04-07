// Tetris.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LobbyClient.h"

#include "MainFrm.h"
#include "MoviePlay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFBHApp

BEGIN_MESSAGE_MAP(CFBHApp, CWinApp)
	//{{AFX_MSG_MAP(CFBHApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFBHApp construction

CFBHApp::CFBHApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFBHApp object

CFBHApp theApp;
CMoviePlay logoPlay;

/////////////////////////////////////////////////////////////////////////////
// CFBHApp initialization

BOOL CFBHApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Revolution-ForBeingHuman Project"));

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

#ifdef _DEBUG
	TCHAR szFilename[MAX_PATH];
	wsprintf(szFilename, ".\\AVI\\INTRO.avi");

	if( !logoPlay.Init( theApp.m_hInstance, szFilename ) )
	{
		MessageBox( NULL, "DirectShow Error", "Error", MB_OK | MB_ICONERROR );
		exit(0);
	}
#endif
	// create and load the frame with its resources

	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	// The one and only window has been initialized, so show and update it.
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	return TRUE;
}
