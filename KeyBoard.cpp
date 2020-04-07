// KeyBoard.cpp: implementation of the KeyBoard class.
//
//////////////////////////////////////////////////////////////////////

#include "KeyBoard.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/************************************************************************/
/* 생성자
/************************************************************************/
KeyBoard::KeyBoard()
{
	g_lpDI = NULL;
	ZeroMemory(diks, sizeof(diks));
}

/************************************************************************/
/* 소멸자
/************************************************************************/
KeyBoard::~KeyBoard()
{
	if(g_lpDI != NULL)
		g_lpDI->Release();
	g_lpDI = NULL;
}

/************************************************************************/
/* DirectInput을 초기화 하는 함수 InitDirectInput( 윈도우 핸들 )
/************************************************************************/
HRESULT KeyBoard::InitDirectInput(HWND hWnd)
{
	HRESULT hr;
	
	if( FAILED( hr = DirectInput8Create( (HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void **) &g_lpDI, NULL) ) )
		return hr;

	//DI 생성
	if( FAILED( hr = g_lpDI->CreateDevice(GUID_SysKeyboard, &g_lpDIKeyBoard, NULL) ) )
		return hr;

	//키보드 데이터형식 설정
	if( FAILED( hr = g_lpDIKeyBoard->SetDataFormat( &c_dfDIKeyboard) ) )
		return hr;

	//협력레벨 설정
	if( FAILED( hr = g_lpDIKeyBoard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY ) ) )
		return hr;

	return S_OK;
}

/************************************************************************/
/* DirectInput의 상태를 업데이트 하는 함수 UpdateInputState( 윈도우 핸들 )
/************************************************************************/
HRESULT KeyBoard::UpdateInputState(HWND hWnd)
{
	HRESULT hr;	
	
	if( NULL == g_lpDIKeyBoard)
		return S_OK;

	hr = g_lpDIKeyBoard->Acquire();
	
	if( FAILED( hr = g_lpDIKeyBoard->GetDeviceState( sizeof(diks), &diks ) ) )		
		return hr;

	return S_OK;
}

/************************************************************************/
/* DirectInput을 해제
/************************************************************************/
VOID KeyBoard::FreeDirectInput()
{
	if(g_lpDIKeyBoard)
		g_lpDIKeyBoard->Unacquire();
	if(g_lpDIKeyBoard)
	{
		g_lpDIKeyBoard->Release();
		g_lpDIKeyBoard = NULL;
	}
	if(g_lpDI)
	{
		g_lpDI->Release();
		g_lpDI = NULL;
	}	
}

LPDIRECTINPUTDEVICE8 KeyBoard::GetDevice()
{
	return g_lpDIKeyBoard;
}