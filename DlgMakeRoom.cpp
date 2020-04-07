#include "stdafx.h"
#include "LobbyClient.h"
#include "DlgMakeRoom.h"

DlgMakeRoom::DlgMakeRoom( DxDisplay* disp )
: FrameView( disp )
{
}

DlgMakeRoom::~DlgMakeRoom()
{
}

void DlgMakeRoom::onCreate()
{
	FrameView::onCreate();

	OKButton = new ButtonCtrl( 0, display );
	OKButton->create( _T( "BMP/MakeRoomOKButton.bmp" ), 8, 68, Window::Attr_ShowWindow, this );

	cancelButton = new ButtonCtrl( 1, display );
	cancelButton->create( _T( "BMP/MakeRoomCancelButton.bmp" ), 124, 68, Window::Attr_ShowWindow, this );

	roomTitleEdit = new EditBoxCtrl( 2, display );
	roomTitleEdit->create( _T( "BMP/MakeRoomEditBoxBack.bmp" ),
		72, 33, this, 0, -1, _T( "BMP/CaretEng.bmp" ), _T( "BMP/CaretHan.bmp" ) );
	roomTitleEdit->setTextColor( RGB( 0, 0, 0 ) );
}

void DlgMakeRoom::onDraw()
{
	FrameView::onDraw();
}

bool DlgMakeRoom::process()
{
	if( FrameView::process() == false )
		return false;

	return true;
}

void DlgMakeRoom::showWindow(bool show )
{
	FrameView::showWindow(show);
}

CString DlgMakeRoom::getTitle()
{
	return roomTitleEdit->getString();
}

void DlgMakeRoom::setEditFocus()
{
	return roomTitleEdit->setFocus();
}

void DlgMakeRoom::makeRoom()
{
	if( roomTitleEdit->getString().IsEmpty() == FALSE )
		parentWnd->postMessage( WM_DLG_MAKE_ROOM_OK );
}

void DlgMakeRoom::cancel()
{
	parentWnd->postMessage( WM_DLG_MAKE_ROOM_CANCEL );
}
void DlgMakeRoom::parseMessage( DWORD message, WPARAM wParam, LPARAM lParam )
{
	if( message != CR_DOWN )
		return;

	if( wParam == 0 )
	{
		makeRoom();
	}
	if( wParam == 1 )
		cancel();
}