#include ".\movieplay.h"
#include <tchar.h>
#include <atlbase.h>

#define WM_GRAPHNOTIFY WM_USER + 13
#define WM_MVSTART WM_USER + 14

CMoviePlay* CMoviePlay::_this = 0;

CMoviePlay::CMoviePlay(void)
{
	_this = this;

	m_hWnd = NULL;
	m_hInstance = NULL;
	pVW = NULL;
	pGraph = NULL;
	pControl = NULL;
	pEvents = NULL;
}

CMoviePlay::~CMoviePlay(void)
{

}

bool CMoviePlay::Init( HINSTANCE hInstance, LPTSTR szFile )
{
	m_hInstance = hInstance;

	HRESULT hr;

	hr = CoInitialize(NULL);

	if( FAILED(hr) )
		return false;
	
	hr = CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph);

	if( FAILED(hr) )
		return false;

    USES_CONVERSION;
	WCHAR wFile[MAX_PATH];

	if( !szFile )
		return false;

	wcsncpy( wFile, T2W(szFile), NUMELMS(wFile)-1 );
	wFile[MAX_PATH-1] = 0;

	hr = pGraph->RenderFile( wFile, NULL );

	if( FAILED(hr) )
		return false;

	hr = pGraph->QueryInterface( IID_IMediaControl, (void**)&pControl);

	if( FAILED(hr) )
		return false;

	hr = pGraph->QueryInterface( IID_IMediaEventEx, (void**)&pEvents);

	if( FAILED(hr) )
		return false;

	hr = pGraph->QueryInterface( IID_IVideoWindow, (void**)&pVW);

	if( FAILED(hr) )
		return false;

	hr = pVW->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN );

	if( FAILED(hr) )
		return false;

	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance = m_hInstance;
	WndClass.lpfnWndProc = (WNDPROC)_this->MovieProc;
	WndClass.lpszClassName = "MoviePlay";
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	if( !RegisterClass(&WndClass) )
		return false;

	MSG Message;

	_this->m_hWnd = CreateWindow( "MoviePlay", "FBH Project", WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_CLIPCHILDREN,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,m_hInstance,NULL);

	if( _this->m_hWnd )
	{
		SendMessage( _this->m_hWnd, WM_MVSTART, 0, 0 );

		while(GetMessage(&Message,0,0,0)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	else
	{
		_this->destroy();
		CoUninitialize();
		return false;
	}
	_this->destroy();
	CoUninitialize();

	return true;
}

bool CMoviePlay::Play()
{
	HRESULT hr;

	hr = pVW->put_MessageDrain((OAHWND)_this->m_hWnd);

	if( FAILED(hr) )
		return false;
	
	hr = pVW->put_FullScreenMode( OATRUE );

	if( FAILED(hr) )
		return false;

	pControl->Run();
	return true;
}

void CMoviePlay::destroy(void)
{
	if( pControl )
	{
		pControl->Stop();
	}

	if( pVW )
	{
		pVW->put_FullScreenMode( OAFALSE );
		pVW->put_Visible(OAFALSE);
		pVW->SetWindowForeground(OAFALSE);
		pVW->put_Owner(NULL);
	}

	if( pEvents )
		pEvents->SetNotifyWindow( (OAHWND)NULL, 0, 0 );

	if( pControl )
		pControl->Release();
	pControl = NULL;

	if( pEvents )
		pEvents->Release();
	pEvents = NULL;

	if( pGraph )
		pGraph->Release();
	pGraph = NULL;

	if( pVW )
		pVW->Release();

	pVW = NULL;
	m_hInstance = NULL;
	m_hWnd = NULL;
}

HRESULT CMoviePlay::HandleGraphEvent(void)
{
	LONG evCode, evParam1, evParam2;
	HRESULT hr=S_OK;

	// Make sure that we don't access the media event interface
	// after it has already been released.
	if (!pEvents)
		return S_OK;

	// Process all queued events
	while(SUCCEEDED(pEvents->GetEvent(&evCode, (LONG_PTR *) &evParam1,
		(LONG_PTR *) &evParam2, 0)))
	{
		// Free memory associated with callback, since we're not using it
		hr = pEvents->FreeEventParams(evCode, evParam1, evParam2);

		// If this is the end of the clip, close
		if(EC_COMPLETE == evCode)
		{
			SendMessage( _this->m_hWnd, WM_DESTROY, 0, 0 );
		}
	}

	return hr;
}

LRESULT CALLBACK CMoviePlay::MovieProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	HRESULT hr;

    switch( message )
	{
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		switch( wParam )
		{
		case VK_ESCAPE:
			SendMessage( hWnd, WM_DESTROY, 0, 0 );
			break;
		}
		break;
	case WM_MVSTART:
		hr = _this->pEvents->SetNotifyWindow( (OAHWND)_this->m_hWnd, WM_GRAPHNOTIFY, 0 );

		if( FAILED(hr) )
			return false;

		hr = _this->pVW->put_Owner( (OAHWND)_this->m_hWnd );

		if( FAILED(hr) )
			return false;

		SetFocus(_this->m_hWnd);

		if( !_this->Play() )
		{
			MessageBox( NULL, "동영상 재생중에 문제가 생겼습니다.", "Error", MB_OK | MB_ICONERROR );
			exit(0);
		}
		break;
	case WM_GRAPHNOTIFY:
		_this->HandleGraphEvent();
		break;
	case WM_DESTROY:		
		PostQuitMessage(0);
		break;
	}

	if( _this->pVW )
		_this->pVW->NotifyOwnerMessage((LONG_PTR) _this->m_hWnd, message, wParam, lParam);

	return DefWindowProc( hWnd, message, wParam, lParam );
}