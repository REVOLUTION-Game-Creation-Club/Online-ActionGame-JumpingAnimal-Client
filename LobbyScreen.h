#define _LobbyScreen_h_
#ifndef _LobbyScreen_h_
class LobbyScreen{
public:
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
	virtual	void	parseMessage( DWORD message, WPARAM wParam, LPARAM lParam );

private:
	void			processInkey();

	void			onAckLogin(Packet& packet);
	void			onAckPing(Packet& packet);
	void			onNtfConnect(Packet& packet);
	void			onAckDisconnect(Packet& packet);

	void			onAckEnterLobby(Packet& packet);
	void			onAckLobbyUserList(Packet& packet);
	void			onAckRoomList(Packet& packet);
	void			onAckUserInfo(Packet& packet);
	void			onAckLobbyChat(Packet& packet);
	void			onAckMakeRoom(Packet& packet);
	void			onAckEnterRoom(Packet& packet);
	void			onAckLeaveLobby(Packet& packet);

	void			onDrawUserInfo();
	void			onDrawRoomInfo();
	void			onDrawChatMessage();
	void			onDrawRoomUserInfo();

	bool			pressedKey;
	ScreenMode		screenMode;
	int				myBoardNo;
	bool			playGame;
	bool			exitGame;

	DlgLogin*		dlgLogin;
	DlgMakeRoom*	dlgMakeRoom;

	DxSurface*		surfLobbyBackground;

	ButtonCtrl*		joinButton[10];
	ButtonCtrl*		makeRoomButton;
	ButtonCtrl*		fastJoinButton;
	ButtonCtrl*		userInfoButton;
	ButtonCtrl*		sendButton;
	ButtonCtrl*		exitLobbyButton;

	EditBoxCtrl*	editChatMessage;

	CList	< char*, char* >	listLobbyChatMessage;
	CList	< char*, char* >	listRoomChatMessage;

	int				userCount;
	int				roomCount;
	User			userInfoList[10];
	Room			roomInfoList[10];


	User			userInfo;

	// ¹æ
	DxSurface*		surfCharacter[4];
	DxSurface*		surfTeamBoard[4];
	CheckBoxCtrl*	surfMajorBoard;
	CheckBoxCtrl*	surfReadyBoard;

	ButtonCtrl*		gameModeChangeButton;
	CheckBoxCtrl*	characterSelectButton[4];
	ButtonCtrl*		teamSelectButton[4];
	ButtonCtrl*		mapChangeButton;
	CheckBoxCtrl*	readyGameButton;
	CheckBoxCtrl*	teamKillSelectButton;
	ButtonCtrl*		kickButton;
	ButtonCtrl*		exitRoomButton;

	static	ClientSocket*	clientSocket;

private:
	void			intoRoom(void);
	void			outroRoom(void);
	void			intoLobby(void);
	void			intoMakeRoom(void);
	void			outroMakeRoom(void);
}

#endif