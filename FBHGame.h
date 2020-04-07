#pragma once

#include <AfxTempl.h>

#include "DxWindow/DirectX/DxDisplay.h"
#include "DxWindow/Windows/FrameView.h"

#include "DxWindow/Windows/Controls/CheckBoxCtrl.h"
#include "DxWindow/Windows/Controls/ButtonCtrl.h"
#include "DxWindow/Windows/Controls/EditBoxCtrl.h"

#include "ClientSocket.h"
#include "_User.h"
#include "C_obj.h"
#include "C_physics.h"
#include "KeyBoard.h"

#include "classFBHGame.h"
#include "IOClass.h"

enum IDOFCONTROL {
	EDITCHATMSG = 0,
	EXITBUTTON,
};

// for ¸»Ç³¼±
typedef struct _ChatInfo {
	int frame;
	char name[127];
	char msg[127];
	int team;
} CHATINFO, *LPCHATINFO;

class CFBHGame : public FrameView
{
public:
            bool			playGame;
			bool			exitGame;

			CFBHGame( HWND hWnd, DxDisplay* disp, ClientSocket* cs );
	virtual ~CFBHGame(void);

	virtual void			onCreate();
	virtual void			onDestroy();

	virtual void			onDraw();
	virtual bool			process();

	virtual void			preTranslateMessage( MSG* msg );

	static ClientSocket*	getClientSocket(){ return clientSocket; }

			void			onSocketConnect( WPARAM wParam, LPARAM lParam );
			void			onSocketClose( WPARAM wParam, LPARAM lParam );
			void			packetParsing( Packet& packet );
			void			drawFps();

protected:
	virtual void			parseMessage( DWORD message, WPARAM wParam, LPARAM lParam );

private:
			void			onAckGameChat(Packet& packet);
			void			onAckGameQuit(Packet& packet);
			void			onReqGame(Packet& packet);
			void			onDrawChatMessage();
			void			DrawUi();
			IDirectDrawSurface7* DDLoadBitMap( IDirectDraw7 *pdd, LPCSTR szBitmap,	int dx, int dy, int memLoc );
			HRESULT			DDCopyBitmap(IDirectDrawSurface7 *pdds, HBITMAP hbm, int x, int y, int dx, int dy);

			HWND			handleMainFrm;

			EditBoxCtrl*	editChatMessage;
			ButtonCtrl*		exitButton;

	static	ClientSocket*	clientSocket;
	static	KeyBoard*		pKey;
	static	HWND			gamehwnd;

			C_physics*		m_pPhysics;
			bool			m_bOnChatMode;

			FBHGame*		m_cFBH;
			DWORD			m_dwFrame;
			DWORD			m_dwReady;
			CHATINFO		m_ciChat[4];
public:
	void userRespon( int index, bool _allrespond );
	FBHGame* getGame() { return m_cFBH; }
	void BoundCheck( int i, DrawInstruction &ins );
	void init(void);
	void ReadyStartDraw(void);

	FBHGame* GetFBHGame()	{	return m_cFBH;	}
};