#ifndef  __GameScreen_H
#define  __GameScreen_H

#include <AfxTempl.h>

#include "DxWindow/DirectX/DxDisplay.h"
#include "DxWindow/Windows/FrameView.h"

#include "DxWindow/Windows/Controls/CheckBoxCtrl.h"
#include "DxWindow/Windows/Controls/ButtonCtrl.h"
#include "DxWindow/Windows/Controls/EditBoxCtrl.h"
#include "DxWindow/Windows/Controls/ListCtrl.h"

#include "User.h"
#include "Room.h"

#include "ClientSocket.h"
#include "DlgLogin.h"
#include "DlgMakeRoom.h"
#include "DlgErrorMessage.h"

#include "FBHGame.h"
#include "DxWindow/DirectX/DxSound.h"

class GameScreen : public FrameView
{
public:
	enum ScreenMode
	{
		// 0: 없음 1: 로고 2: 동영상 3: 로그인 4: 로비 5: 방 6: 게임중 7: 로딩 8: 에러메세지
		GM_NONE = 0,
		GM_TITLE,
		GM_AVI,
		GM_LOGIN,
		GM_LOBBY,
		GM_MAKEROOM,
		GM_ROOM,
		GM_GAME,
		GM_LOADING,
		GM_ERROR_MESSAGE,
	};
public:
		C_Gamer *tempUser[4];		
		CFBHGame*		game;
		bool			playLobby;

		GameScreen( HWND hwnd, DxDisplay* disp, ClientSocket* cs );
	virtual	~GameScreen();

	virtual	void			onCreate();
	virtual	void			onDestroy();

	virtual	void			onDraw();
	virtual	bool			process();

	virtual	void			preTranslateMessage( MSG* msg );

	static	ClientSocket*	getClientSocket(){ return clientSocket; }

			void			onSocketConnect( WPARAM wParam, LPARAM lParam );
			void			onSocketClose( WPARAM wParam, LPARAM lParam );
			void			packetParsing( Packet& packet );
protected:
	virtual	void			parseMessage( DWORD message, WPARAM wParam, LPARAM lParam );
private:
			void			processInkey();

			void			onAckLogin(Packet& packet);
			void			onAckPing(Packet& packet);
			void			onNtfConnect(Packet& packet);
			void			onAckDisconnect(Packet& packet);
			
			void			onAckEnterLobby(Packet& packet);
			void			onAckLobbyUserList(Packet& packet);
			void			onAckLobbyRoomList(Packet& packet);
			void			onAckUserInfo(Packet& packet);
			void			onAckUserLocation(Packet& packet);
			void			onAckLobbyChat(Packet& packet);
			void			onAckMakeRoom(Packet& packet);
			void			onAckEnterRoom(Packet& packet);
			void			onAckLeaveRoom(Packet& packet);
			void			onAckDeleteRoom(Packet& packet);
			void			onAckLeaveLobby(Packet& packet);
			void			onAckRestRoom(Packet& packet);
			void			onAckWait(Packet& packet);
			void			onAckResume(Packet& packet);
			void			onAckRoomChat(Packet& packet);			
			void			onAckRoomUserList(Packet& packet);
			void			onAckGameModeChange(Packet& packet);
			void			onAckChracterSelect(Packet& packet);
			void			onAckReady(Packet& packet);
			void			onAckGameStart(Packet& packet);

			void			onAckKillSum(Packet& packet);
			void			onAckTeamKill(Packet& packet);
			void			onAckKick(Packet& packet);
			void			onAckTeamSelect(Packet& packet);
			void			onAckMapChange(Packet& packet);

			void			onAckChangeMajor(Packet& packet);

			HWND			handleMainFrm;

			void			onDrawFPS();
			void			onDrawLobbyUserInfo();
			void			onDrawRoomInfo();
			void			onDrawLobbyChat();

			void			onDrawRoomTitle();
			void			onDrawGameMode();
			void			onDrawRoomUserInfo();
			void			onDrawRoomSmallMap();
			void			onDrawRoomChat();
				
			bool			pressedKey;
			ScreenMode		screenMode;
			
			char			myTitle[127];
			int				myLobbyNo;
			int				myRoomIndex;
			int				myRoomNo;			//for broadcast
			int				mySlotNo;			//for identify self
			int				myMapKind;
			int				myGameMode;
			int				myUserCount;
			
			bool			exitLobby;

			DlgLogin*		dlgLogin;
			DlgMakeRoom*	dlgMakeRoom;
			DlgErrorMessage* dlgErrorMessage;
			DxSurface*		surfLobbyBackground;
			DxSurface*		surfRoomBackground;
			
			ButtonCtrl*		joinButton;
			ButtonCtrl*		makeRoomButton;
//			ButtonCtrl*		fastJoinButton;
//			ButtonCtrl*		userInfoButton;
//			ButtonCtrl*		sendButton;
			ButtonCtrl*		exitLobbyButton;

			EditBoxCtrl*	editChatMessage;

			CList	< char*, char* >	listLobbyChatMessage;
			CList	< char*, char* >	listRoomChatMessage;

			int				userInfoCount;
			int				roomInfoCount;

			ListCtrl*		listUser;
			ListCtrl*		listRoom;

			CList	< GameRoom, GameRoom& >	listRoomInfo;
			
			User			userInfo;
			User			roomUser[4];
			// 방
			DxSurface*		surfCharacter[4];
			DxSurface*		surfTeamBoard[4];
			DxSurface*		surfSmallMap[2];
			
			CheckBoxCtrl*	surfMajorBoard;
			CheckBoxCtrl*	surfReadyBoard;

//			ButtonCtrl*		gameModeChangeButton;
			CheckBoxCtrl*	characterSelectButton[4];
			CheckBoxCtrl*	teamSelectButton[4];
//			ButtonCtrl*		mapChangeButton;
			CheckBoxCtrl*	readyGameButton;
//			CheckBoxCtrl*	teamKillSelectButton;
//			ButtonCtrl*		kickButton;
			ButtonCtrl*		exitRoomButton;
			ButtonCtrl*		roomResetButton;
			
	static	ClientSocket*	clientSocket;

private:
			void			outroLogin(void);
			void			intoRoom(void);
			void			outroRoom(void);
			void			intoLobby(void);
			void			intoMakeRoom(void);
			void			outroMakeRoom(void);
};

#endif