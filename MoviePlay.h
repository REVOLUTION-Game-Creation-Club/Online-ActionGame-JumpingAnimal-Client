#pragma once

#include <windows.h>
#include <DShow.h>
#pragma comment(lib, "strmiids")
#pragma comment(lib, "Quartz")

class CMoviePlay
{
	static CMoviePlay* _this;

	HWND m_hWnd;
	HINSTANCE m_hInstance;

	IVideoWindow *pVW;
	IGraphBuilder *pGraph;
	IMediaControl *pControl;
	IMediaEventEx *pEvents;
public:
	CMoviePlay(void);
	~CMoviePlay(void);
	bool Init(HINSTANCE hInstance, LPTSTR szFile);
	bool Play();
	void destroy(void);
	HWND GethWnd() { return m_hWnd; }
	HRESULT HandleGraphEvent();
	static LRESULT CALLBACK MovieProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
};
