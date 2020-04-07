#include "stdafx.h"
#include "LobbyClient.h"
#include "DlgErrorMessage.h"

DlgErrorMessage::DlgErrorMessage( DxDisplay* disp )
: FrameView( disp )
{
}

DlgErrorMessage::~DlgErrorMessage()
{
}

void DlgErrorMessage::onCreate()
{
	FrameView::onCreate();

	btnOKButton = new ButtonCtrl( 0, display );
	btnOKButton->create( _T( "BMP/ErrorMessageOKButton.bmp" ), 8, 68, Window::Attr_ShowWindow, this );

	message = "더이상 방을 만들 수 없습니다.";
}

void DlgErrorMessage::onDraw()
{
	FrameView::onDraw();

	drawText( Font_Medium_Normal,message.GetBuffer(0), 20, 45, RGB( 0, 0, 0 ) );
}

bool DlgErrorMessage::process()
{
	if( FrameView::process() == false )
		return false;

	return true;
}

CString DlgErrorMessage::getMessage()
{
	return message;
}

void DlgErrorMessage::parseMessage( DWORD message, WPARAM wParam, LPARAM lParam )
{
	if( message != CR_DOWN )
		return;

	if( wParam == 0 )
	{
		parentWnd->postMessage( WM_ERROR_MESSAGE_OK );
	}
}