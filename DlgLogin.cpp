#include "stdafx.h"
#include "LobbyClient.h"
#include "DlgLogin.h"
#include "define.h"

DlgLogin::DlgLogin( DxDisplay* disp )
	: FrameView( disp )
{

}

DlgLogin::~DlgLogin()
{
}

void DlgLogin::onCreate()
{
	FrameView::onCreate();

	btnConnect = new ButtonCtrl( 0, display );
	btnConnect->create( _T( "BMP/LoginConnectButton.bmp" ), LOGIN_CONNECT_BTN_X, LOGIN_CONNECT_BTN_Y, Window::Attr_ShowWindow, this );

	btnExit = new ButtonCtrl( 1, display );
	btnExit->create( _T( "BMP/LoginExitButton.bmp" ), LOGIN_EXIT_BTN_X, LOGIN_EXIT_BTN_Y, Window::Attr_ShowWindow, this );

	editAccount = new EditBoxCtrl( 2, display );
	editAccount->create( _T( "BMP/LoginEditBoxBack.bmp" ),
					LOGIN_ID_EDIT_BOX_X, LOGIN_ID_EDIT_BOX_Y, this, 0, -1, _T( "BMP/CaretEng.bmp" ), _T( "BMP/CaretHan.bmp" ) );
	editAccount->setTextColor( RGB( 0, 0, 0 ) );

}

void DlgLogin::onDraw()
{
	FrameView::onDraw();
}

bool DlgLogin::process()
{
	if( FrameView::process() == false )
		return false;

	return true;
}

CString DlgLogin::getAccount()
{
	return editAccount->getString();
}

void DlgLogin::setAccountFocus()
{
	editAccount->setFocus();
}

void DlgLogin::parseMessage( DWORD message, WPARAM wParam, LPARAM lParam )
{
	if( message != CR_DOWN )
		return;

	if( wParam == 0 )
	{
		connect();
	}
	if( wParam == 1 )
		exit();
}

void DlgLogin::connect()
{
	if( editAccount->getString().IsEmpty() == FALSE )
		parentWnd->postMessage( WM_DLGLOGIN_CONNECT );
}

void DlgLogin::exit()
{
		parentWnd->postMessage( WM_DLGLOGIN_EXIT );
}