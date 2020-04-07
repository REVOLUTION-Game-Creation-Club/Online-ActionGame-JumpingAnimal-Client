// KeyBoard.cpp: implementation of the KeyBoard class.
//
//////////////////////////////////////////////////////////////////////

#include "KeyBoard.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/************************************************************************/
/* ������
/************************************************************************/
KeyBoard::KeyBoard()
{
	g_lpDI = NULL;
	ZeroMemory(diks, sizeof(diks));
}

/************************************************************************/
/* �Ҹ���
/************************************************************************/
KeyBoard::~KeyBoard()
{
	if(g_lpDI != NULL)
		g_lpDI->Release();
	g_lpDI = NULL;
}

/************************************************************************/
/* DirectInput�� �ʱ�ȭ �ϴ� �Լ� InitDirectInput( ������ �ڵ� )
/************************************************************************/
HRESULT KeyBoard::InitDirectInput(HWND hWnd)
{
	HRESULT hr;
	
	if( FAILED( hr = DirectInput8Create( (HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void **) &g_lpDI, NULL) ) )
		return hr;

	//DI ����
	if( FAILED( hr = g_lpDI->CreateDevice(GUID_SysKeyboard, &g_lpDIKeyBoard, NULL) ) )
		return hr;

	//Ű���� ���������� ����
	if( FAILED( hr = g_lpDIKeyBoard->SetDataFormat( &c_dfDIKeyboard) ) )
		return hr;

	//���·��� ����
	if( FAILED( hr = g_lpDIKeyBoard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY ) ) )
		return hr;

	return S_OK;
}

/************************************************************************/
/* DirectInput�� ���¸� ������Ʈ �ϴ� �Լ� UpdateInputState( ������ �ڵ� )
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
/* DirectInput�� ����
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