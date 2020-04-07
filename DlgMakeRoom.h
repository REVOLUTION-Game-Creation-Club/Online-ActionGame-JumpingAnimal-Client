#ifndef  __DlgMakeRoom_H
#define  __DlgMakeRoom_H

#include "DxWindow/Windows/FrameView.h"
#include "DxWindow/Windows/Controls/ButtonCtrl.h"
#include "DxWindow/Windows/Controls/EditBoxCtrl.h"


#define  WM_DLG_MAKE_ROOM_OK		102
#define  WM_DLG_MAKE_ROOM_CANCEL	103


class DlgMakeRoom : public FrameView
{
public:
	DlgMakeRoom( DxDisplay* disp );
	virtual	~DlgMakeRoom();

	virtual	void			onCreate();

	virtual	void			onDraw();
	virtual	bool			process();

	CString					getTitle();
	void					emptyTitle(){roomTitleEdit->setString("");}
	void					setEditFocus();
	void					makeRoom();
	void					cancel();
	virtual	void			showWindow( bool show = true );
protected:
	virtual	void			parseMessage( DWORD message, WPARAM wParam, LPARAM lParam );

private:
	ButtonCtrl*		OKButton;
	ButtonCtrl*		cancelButton;
	EditBoxCtrl*	roomTitleEdit;
};

#endif