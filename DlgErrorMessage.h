#ifndef  _DlgErrorMessage_H_
#define  _DlgErrorMessage_H_

#include "DxWindow/Windows/FrameView.h"
#include "DxWindow/Windows/Controls/ButtonCtrl.h"
#include "DxWindow/Windows/Controls/EditBoxCtrl.h"

#define WM_ERROR_MESSAGE_OK		104

class DlgErrorMessage : public FrameView
{
public:
	DlgErrorMessage( DxDisplay* disp );
	virtual	~DlgErrorMessage();

	virtual	void			onCreate();

	virtual	void			onDraw();
	virtual	bool			process();
	void					setMessage(CString _message) { message = _message; }
	CString					getMessage();
protected:
	virtual	void			parseMessage( DWORD message, WPARAM wParam, LPARAM lParam );

private:
	ButtonCtrl*		btnOKButton;
	CString			message;
};

#endif