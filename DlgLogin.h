#ifndef  __DlgLogin_H
#define  __DlgLogin_H

#include "DxWindow/Windows/FrameView.h"
#include "DxWindow/Windows/Controls/ButtonCtrl.h"
#include "DxWindow/Windows/Controls/EditBoxCtrl.h"


#define  WM_DLGLOGIN_CONNECT			100
#define  WM_DLGLOGIN_EXIT				101


class DlgLogin : public FrameView
{
public:
			DlgLogin( DxDisplay* disp );
	virtual	~DlgLogin();

	virtual	void			onCreate();

	virtual	void			onDraw();
	virtual	bool			process();
			
			void			connect();
			void			exit();

			CString			getAccount();
			void			setAccountFocus();

protected:
	virtual	void			parseMessage( DWORD message, WPARAM wParam, LPARAM lParam );
private:
			ButtonCtrl*		btnConnect;
			ButtonCtrl*		btnExit;
			EditBoxCtrl*	editAccount;
};

#endif