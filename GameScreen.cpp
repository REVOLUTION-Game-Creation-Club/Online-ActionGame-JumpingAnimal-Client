#include "stdafx.h"
#include "LobbyClient.h"
#include "GameScreen.h"
#include "Protocol.h"
#include "Define.h"

ClientSocket*		GameScreen::clientSocket = 0;
DxSound *g_dxSound = NULL;

GameScreen::GameScreen( HWND hwnd, DxDisplay* disp, ClientSocket* cs )
	: FrameView( disp ), exitLobby( false ), handleMainFrm( hwnd )
{
	clientSocket = cs;
	game = NULL;

	game = new CFBHGame( hwnd, disp, cs);
	game->create( NULL, 0, 0, Window::Attr_ShowWindow, 0 );
}

GameScreen::~GameScreen()
{
}

void GameScreen::onCreate()
{
	FrameView::onCreate();

	display->createSurfaceFromBitmap( &surfLobbyBackground, _T( "BMP/Lobby.bmp" ), 0, 0 );
	display->createSurfaceFromBitmap( &surfRoomBackground, _T( "BMP/Room.bmp" ), 0, 0 );

	display->createSurfaceFromBitmap( &surfCharacter[0], _T( "BMP/Character1.bmp" ), 0, 0 );
	display->createSurfaceFromBitmap( &surfCharacter[1], _T( "BMP/Character2.bmp" ), 0, 0 );
	display->createSurfaceFromBitmap( &surfCharacter[2], _T( "BMP/Character3.bmp" ), 0, 0 );
	display->createSurfaceFromBitmap( &surfCharacter[3], _T( "BMP/Character4.bmp" ), 0, 0 );
	display->createSurfaceFromBitmap( &surfTeamBoard[0], _T( "BMP/TeamBoard1.bmp" ), 0, 0 );
	display->createSurfaceFromBitmap( &surfTeamBoard[1], _T( "BMP/TeamBoard2.bmp" ), 0, 0 );
	display->createSurfaceFromBitmap( &surfTeamBoard[2], _T( "BMP/TeamBoard3.bmp" ), 0, 0 );
	display->createSurfaceFromBitmap( &surfTeamBoard[3], _T( "BMP/TeamBoard4.bmp" ), 0, 0 );

	display->createSurfaceFromBitmap( &surfSmallMap[0], _T( "BMP/SmallMap1.bmp" ), 0, 0 );
	display->createSurfaceFromBitmap( &surfSmallMap[1], _T( "BMP/SmallMap2.bmp" ), 0, 0 );

	//////////////////////////////////////////////////////////////////////////
	// 로비 초기화
	//////////////////////////////////////////////////////////////////////////

	joinButton = new ButtonCtrl( ID_JOIN_ROOM_BTN, display );
	joinButton->create( _T( "BMP/JoinButton.bmp" ), LOBBY_ROOM_JOIN_BTN_X, LOBBY_ROOM_JOIN_BTN_Y, Window::Attr_ShowWindow, this );
	joinButton->enable( false );

	// 방만들기 버튼 10
	makeRoomButton = new ButtonCtrl( ID_MAKE_ROOM_BTN, display );
	makeRoomButton->create( _T( "BMP/MakeRoomButton.bmp" ), LOBBY_MAKE_ROOM_X, LOBBY_MAKE_ROOM_Y, Window::Attr_ShowWindow, this );
	makeRoomButton->enable( false );

	listUser =  new ListCtrl(ID_LOBBY_USER_LIST,display);
	listUser->create(_T("BMP/UserListBack.bmp"),_T("BMP/UserListSelectBar.bmp"),LOBBY_USER_LIST_X,LOBBY_USER_LIST_Y,Window::Attr_ShowWindow, this);
	listUser->createScrollBar(_T("BMP/UserScrollBack.bmp"),_T("BMP/UserScrollBar.bmp"),_T("BMP/UserScrollDec.bmp"),_T("BMP/UserScrollInc.bmp"),ScrollBarCtrl::SB_Vertical);

	listRoom =  new ListCtrl(ID_LOBBY_ROOM_LIST,display);
	listRoom->create(_T("BMP/RoomListBack.bmp"),_T("BMP/RoomListSelectBar.bmp"),LOBBY_ROOM_LIST_X,LOBBY_ROOM_LIST_Y,Window::Attr_ShowWindow, this);
	listRoom->createScrollBar(_T("BMP/RoomScrollBack.bmp"),_T("BMP/RoomScrollBar.bmp"),_T("BMP/RoomScrollDec.bmp"),_T("BMP/RoomScrollInc.bmp"),ScrollBarCtrl::SB_Vertical);

	// 나가기 버튼 21
	exitLobbyButton = new ButtonCtrl( ID_LOBBY_EXIT_BTN, display );
	exitLobbyButton->create( _T( "BMP/ExitLobbyButton.bmp" ), LOBBY_EXIT_BTN_X, LOBBY_EXIT_BTN_Y, Window::Attr_ShowWindow, this );
	exitLobbyButton->enable( false );
	// 방 전체 삭제 버튼
//	roomResetButton = new ButtonCtrl(ID_ROOM_RESET_BTN,display);
//	roomResetButton->create(_T("BMP/LobbyRoomResetButton.bmp"),LOBBY_ROOM_RESET_BTN_X,LOBBY_ROOM_RESET_BTN_Y,Window::Attr_ShowWindow, this);
//	roomResetButton->showWindow(false);


	// 채팅 메시지 22
	editChatMessage = new EditBoxCtrl( ID_EDIT_BOX_CTL, display );
	editChatMessage->create( _T( "BMP/EditBoxBack.bmp" ),
		LOBBY_CHAT_EDIT_X, LOBBY_CHAT_EDIT_Y, this, 0, -1, _T( "BMP/CaretEng.bmp" ), _T( "BMP/CaretHan.bmp" ) );
	editChatMessage->setColorKey(RGB(65, 165, 140));

	editChatMessage->setIMEWIndowHandle( handleMainFrm );

	dlgLogin = new DlgLogin( display );
	dlgLogin->create( _T( "BMP/LoginBackground.bmp" ), 200, 150, Window::Attr_ShowWindow | Window::Attr_Movable, this );

	dlgMakeRoom = new DlgMakeRoom( display );
	dlgMakeRoom->create( _T( "BMP/MakeRoomBackground.bmp" ), 290, 200, Window::Attr_ShowWindow | Window::Attr_Movable, this );
	dlgMakeRoom->showWindow( false );

	dlgErrorMessage = new DlgErrorMessage( display );
	dlgErrorMessage->create( _T( "BMP/ErrorMessageBackground.bmp" ), 290, 200, Window::Attr_ShowWindow | Window::Attr_Movable, this );
	dlgErrorMessage->showWindow( false );

	//////////////////////////////////////////////////////////////////////////
	// 방 초기화
	//////////////////////////////////////////////////////////////////////////
	// 게임방식 선택버튼 30
//	gameModeChangeButton = new ButtonCtrl(ID_GAME_MODE_CHANGE_BTN, display);
//	gameModeChangeButton->create(_T("BMP/GameModeSelectButton.bmp"),ROOM_GAME_MODE_BTN_X,ROOM_GAME_MODE_BTN_Y,Window::Attr_ShowWindow, this);
//	gameModeChangeButton->showWindow(false);
	// 케릭터 선택버튼 31,32,33,34
	characterSelectButton[0] = new CheckBoxCtrl(ID_CHARACTER_SELECT_BTN1,display);
	characterSelectButton[0]->create(_T("BMP/CharacterSelectButton1.bmp"),ROOM_CHARACTER_X1,ROOM_CHARACTER_Y1,Window::Attr_ShowWindow, this);
	characterSelectButton[0]->showWindow(false);
	characterSelectButton[1] = new CheckBoxCtrl(ID_CHARACTER_SELECT_BTN2,display);
	characterSelectButton[1]->create(_T("BMP/CharacterSelectButton2.bmp"),ROOM_CHARACTER_X2,ROOM_CHARACTER_Y2,Window::Attr_ShowWindow, this);
	characterSelectButton[1]->showWindow(false);
	characterSelectButton[2] = new CheckBoxCtrl(ID_CHARACTER_SELECT_BTN3,display);
	characterSelectButton[2]->create(_T("BMP/CharacterSelectButton3.bmp"),ROOM_CHARACTER_X3,ROOM_CHARACTER_Y3,Window::Attr_ShowWindow, this);
	characterSelectButton[2]->showWindow(false);
	characterSelectButton[3] = new CheckBoxCtrl(ID_CHARACTER_SELECT_BTN4,display);
	characterSelectButton[3]->create(_T("BMP/CharacterSelectButton4.bmp"),ROOM_CHARACTER_X4,ROOM_CHARACTER_Y4,Window::Attr_ShowWindow, this);
	characterSelectButton[3]->showWindow(false);
	// 팀 선택버튼 35,36,37,38
	teamSelectButton[0] = new CheckBoxCtrl(ID_TEAM_SELECT_BTN1,display);
	teamSelectButton[0]->create(_T("BMP/TeamSelectButton1.bmp"),ROOM_TEAM_SELECT_X1,ROOM_TEAM_SELECT_Y1,Window::Attr_ShowWindow, this);
	teamSelectButton[0]->showWindow(false);
	teamSelectButton[1] = new CheckBoxCtrl(ID_TEAM_SELECT_BTN2,display);
	teamSelectButton[1]->create(_T("BMP/TeamSelectButton2.bmp"),ROOM_TEAM_SELECT_X2,ROOM_TEAM_SELECT_Y2,Window::Attr_ShowWindow, this);
	teamSelectButton[1]->showWindow(false);
	teamSelectButton[2] = new CheckBoxCtrl(ID_TEAM_SELECT_BTN3,display);
	teamSelectButton[2]->create(_T("BMP/TeamSelectButton3.bmp"),ROOM_TEAM_SELECT_X3,ROOM_TEAM_SELECT_Y3,Window::Attr_ShowWindow, this);
	teamSelectButton[2]->showWindow(false);
	teamSelectButton[3] = new CheckBoxCtrl(ID_TEAM_SELECT_BTN4,display);
	teamSelectButton[3]->create(_T("BMP/TeamSelectButton4.bmp"),ROOM_TEAM_SELECT_X4,ROOM_TEAM_SELECT_Y4,Window::Attr_ShowWindow, this);
	teamSelectButton[3]->showWindow(false);
	// 맵 바꾸기 버튼 39
//	mapChangeButton = new ButtonCtrl(ID_MAP_CHANGE_BTN,display);
//	mapChangeButton->create(_T("BMP/MapChangeButton.bmp"),ROOM_CHANGE_MAP_X,ROOM_CHANGE_MAP_Y,Window::Attr_ShowWindow, this);
//	mapChangeButton->showWindow(false);
	// 게임 준비 버튼 40
	readyGameButton = new CheckBoxCtrl(ID_READY_BTN,display);
	readyGameButton->create(_T("BMP/StartGameButton.bmp"),ROOM_READY_X,ROOM_READY_Y,Window::Attr_ShowWindow, this);
	readyGameButton->showWindow(false);
	// 팀킬 버튼 41
//	teamKillSelectButton = new CheckBoxCtrl(ID_TEAM_KILL_BTN,display);
//	teamKillSelectButton->create(_T("BMP/TeamKillButton.bmp"),ROOM_TEAM_KILL_X,ROOM_TEAM_KILL_Y,Window::Attr_ShowWindow, this);
//	teamKillSelectButton->showWindow(false);
	// 강퇴 버튼 42
//	kickButton = new ButtonCtrl(ID_KICK_BTN,display);
//	kickButton->create(_T("BMP/KickButton.bmp"),ROOM_KICK_X,ROOM_KICK_Y,Window::Attr_ShowWindow, this);
//	kickButton->showWindow(false);
	// 방 나가기 버튼 43
	exitRoomButton = new ButtonCtrl(ID_ROOM_EXIT_BTN,display);
	exitRoomButton->create(_T("BMP/ExitRoomButton.bmp"),ROOM_EXIT_X,ROOM_EXIT_Y,Window::Attr_ShowWindow, this);
	exitRoomButton->showWindow(false);

	g_dxSound = new DxSound();

	g_dxSound->initialize(handleMainFrm);
	g_dxSound->loadWav(0,"SOUND/로비.wav");
	g_dxSound->loadWav(1,"SOUND/버튼.wav");
	g_dxSound->loadWav(2,"SOUND/닭.wav");
	g_dxSound->loadWav(3,"SOUND/호랑이.wav");
	g_dxSound->loadWav(4,"SOUND/뱀.wav");
	g_dxSound->loadWav(5,"SOUND/양.wav");
	g_dxSound->loadWav(6,"SOUND/점프.wav");
	g_dxSound->loadWav(7,"SOUND/volcano.wav");
	g_dxSound->loadWav(9,"SOUND/숏점프.wav");
	g_dxSound->loadWav(10,"SOUND/화산BGM.wav");
	g_dxSound->playWav(0);
	dlgLogin->setAccountFocus();
	screenMode = GM_LOGIN;

	pressedKey = false;

	playLobby = false;

	myLobbyNo = 0;
	myRoomIndex = -1;
	myRoomNo  = -1;
	mySlotNo = -1;
	myMapKind = 0;
	myGameMode = 0;
	myUserCount = 0;

	userInfoCount = 0;
	roomInfoCount = 0;
}

void GameScreen::onDestroy()
{
	FrameView::onDestroy();

	delete surfLobbyBackground;
	delete surfRoomBackground;
	delete surfCharacter[0];
	delete surfCharacter[1];
	delete surfCharacter[2];
	delete surfCharacter[3];
	delete surfTeamBoard[0];
	delete surfTeamBoard[1];
	delete surfTeamBoard[2];
	delete surfTeamBoard[3];
	delete surfSmallMap[0];
	delete surfSmallMap[1];

	if( game )
		game->onDestroy();

	delete game;
	game = NULL;

	g_dxSound->release();
	delete g_dxSound;

	while( listLobbyChatMessage.GetCount() > 0 )
		delete listLobbyChatMessage.RemoveHead();
	while( listRoomChatMessage.GetCount() > 0 )
		delete listRoomChatMessage.RemoveHead();
}

void GameScreen::onSocketConnect( WPARAM wParam, LPARAM lParam )
{
	// 게임에서 소캣 연결
	game->onSocketConnect(wParam, lParam );
}

void GameScreen::onSocketClose( WPARAM wParam, LPARAM lParam )
{
	// 게임에서 소캣 닫는거
	//game->onSocketClose(wParam, lParam);
}

void GameScreen::packetParsing(Packet& packet)
{
	switch( packet.id() )
	{
	case	ACK_LOGIN :				onAckLogin(packet);					break;
	case	ACK_PING :				onAckPing(packet);					break;
	case	NTF_CONNECT :			onNtfConnect(packet);				break;
	case	ACK_DISCONNECT :		onAckDisconnect(packet);			break;
	case	ACK_ENTER_LOBBY:		onAckEnterLobby(packet);			break;
	case	ACK_LEAVE_LOBBY:		onAckLeaveLobby(packet);			break;
	case	ACK_LEAVE_ROOM :		onAckLeaveRoom(packet);				break;
	case	ACK_DELETE_ROOM :		onAckDeleteRoom(packet);			break;
	case	ACK_RESET_ROOM:			onAckRestRoom(packet);				break;
	case	ACK_LOBBY_USER_LIST :	onAckLobbyUserList(packet);			break;
	case	ACK_USER_INFO :			onAckUserInfo(packet);				break;
	case	ACK_USER_LOCATION:		onAckUserLocation(packet);			break;
	case	ACK_LOBBY_CHAT :		onAckLobbyChat(packet);				break;
	case	ACK_LOBBY_ROOM_LIST :	onAckLobbyRoomList(packet);			break;
	case	ACK_MAKE_ROOM:			onAckMakeRoom(packet);				break;
	case	ACK_ENTER_ROOM :		onAckEnterRoom(packet);				break;

	case	ACK_WAIT:				onAckWait(packet);					break;
	case	ACK_RESUME:				onAckResume( packet);				break;
	case	ACK_ROOM_CHAT:			onAckRoomChat(packet);				break;
	case	ACK_ROOM_USER_LIST:		onAckRoomUserList(packet);			break;
	case	ACK_GAME_MODE_CHANGE:	onAckGameModeChange(packet);		break;
	case	ACK_CHARACTER_SELECT:	onAckChracterSelect(packet);		break;
	case	ACK_GAME_READY :		onAckReady(packet);					break;
	case	ACK_GAME_START :		onAckGameStart(packet);				break;
	case	ACK_KILL_SUM:			onAckKillSum(packet);				break;
	case	ACK_TEAM_KILL:			onAckTeamKill(packet);				break;
	case	ACK_KICK:				onAckKick(packet);					break;
	case	ACK_TEAM_SELECT:		onAckTeamSelect(packet);			break;
	case	ACK_MAP_CHANGE:			onAckMapChange(packet);				break;
	case	ACK_CHANGE_MAJOR :		onAckChangeMajor(packet);			break;
	default:
		// 게임에서 파싱하는거 
		game->packetParsing( packet );		break;
	}
}

void GameScreen::parseMessage( DWORD message, WPARAM wParam, LPARAM lParam )
{
	switch(screenMode)
	{
	default:
	case GM_LOGIN:
		if( message == WM_DLGLOGIN_CONNECT )
		{
			dlgLogin->showWindow( false );
			ifstream ofs;
			ofs.open("Config\\ip.txt");
			char t[128];
			ofs >> t;
			ofs.close();

			clientSocket->connectTo( t, 9630 );
			outroLogin();
		}

		if( message == WM_DLGLOGIN_EXIT )
		{
			exitLobby = true;
			g_dxSound->playWav(1,false);
		}
		dlgLogin->setAccountFocus();
		break;
	case GM_MAKEROOM:
		if(	message == WM_DLG_MAKE_ROOM_OK )
		{
			g_dxSound->playWav(1,false);

			Packet sendPacket( REQ_MAKE_ROOM );
			sendPacket << dlgMakeRoom->getTitle().GetBuffer(0);
			clientSocket->sendPacket( sendPacket );
			outroMakeRoom();
			dlgMakeRoom->emptyTitle();
		}

		if( message == WM_DLG_MAKE_ROOM_CANCEL )
		{
			g_dxSound->playWav(1,false);
			dlgMakeRoom->emptyTitle();
			outroMakeRoom();
		}
		dlgMakeRoom->setEditFocus();
		break;
	case GM_LOBBY:
		if( message == CR_LISTCTRL_SELECTEDITEM )
		{
		}

		if( message == WM_ERROR_MESSAGE_OK )
		{
			dlgErrorMessage->showWindow(false);
		}

		if( message == CR_DOWN )
		{
			if( wParam == ID_JOIN_ROOM_BTN )
			{
				POSITION pos;
				Packet sendPacket( REQ_ENTER_ROOM );
				sendPacket << listRoom->getSelectedIndex();
				pos = listRoomInfo.FindIndex(listRoom->getSelectedIndex());
				if( pos != 0)
				{
					sendPacket << listRoomInfo.GetAt(pos).getRoomNo();
					clientSocket->sendPacket( sendPacket );
				}
				g_dxSound->playWav(1,false);
			}
			if( wParam == ID_MAKE_ROOM_BTN)
			{
				intoMakeRoom();
				g_dxSound->playWav(1,false);
				return;
			}
			if( wParam == ID_FAST_JOIN_BTN)
			{

			}
			if( wParam == ID_USER_INFO_BTN)
			{
				Packet sendPacket( REQ_USER_INFO );
				sendPacket << dlgLogin->getAccount().GetBuffer(0); // 원하는 아이디....
				clientSocket->sendPacket( sendPacket );
			}
			if( wParam == ID_SEND_BTN)
			{
			}
			if( wParam == ID_LOBBY_EXIT_BTN)
			{
				exitLobby = true;
				g_dxSound->playWav(1,false);
			}
			if( wParam == ID_ROOM_RESET_BTN)
			{
				Packet sendPacket( REQ_RESET_ROOM );
				clientSocket->sendPacket( sendPacket );
			}
		}
		editChatMessage->setFocus();
		break;
	case GM_ROOM:
		if( message == CR_DOWN )
		{
			// 게임방식변경버튼 30
			if( wParam == ID_GAME_MODE_CHANGE_BTN)
			{
				Packet sendPacket( REQ_GAME_MODE_CHANGE );
				sendPacket << mySlotNo;
				sendPacket << myGameMode;
				clientSocket->sendPacket( sendPacket );
			}
			// 케릭터 선택버튼 31,32,33,34
			if( wParam == ID_CHARACTER_SELECT_BTN1)
			{
				Packet sendPacket( REQ_CHARACTER_SELECT );
				sendPacket << mySlotNo;
				sendPacket << 0;
				clientSocket->sendPacket( sendPacket );
				g_dxSound->playWav(2,false);
			}
			if( wParam == ID_CHARACTER_SELECT_BTN2)
			{
				Packet sendPacket( REQ_CHARACTER_SELECT );
				sendPacket << mySlotNo;
				sendPacket << 1;
				clientSocket->sendPacket( sendPacket );
				g_dxSound->playWav(3,false);
			}
			if( wParam == ID_CHARACTER_SELECT_BTN3)
			{
				Packet sendPacket( REQ_CHARACTER_SELECT );
				sendPacket << mySlotNo;
				sendPacket << 2;
				clientSocket->sendPacket( sendPacket );
				g_dxSound->playWav(4,false);
			}
			if( wParam == ID_CHARACTER_SELECT_BTN4)
			{
				Packet sendPacket( REQ_CHARACTER_SELECT );
				sendPacket << mySlotNo;
				sendPacket << 3;
				clientSocket->sendPacket( sendPacket );
				g_dxSound->playWav(5,false);
			}

			// 팀 선택버튼 35,36,37,38
			if( wParam == ID_TEAM_SELECT_BTN1)
			{
				Packet sendPacket( REQ_TEAM_SELECT );
				sendPacket << mySlotNo;
				sendPacket << 0;
				clientSocket->sendPacket( sendPacket );
				g_dxSound->playWav(1,false);
			}
			if( wParam == ID_TEAM_SELECT_BTN2)
			{
				Packet sendPacket( REQ_TEAM_SELECT );
				sendPacket << mySlotNo;
				sendPacket << 1;
				clientSocket->sendPacket( sendPacket );
				g_dxSound->playWav(1,false);
			}
			if( wParam == ID_TEAM_SELECT_BTN3)
			{
				Packet sendPacket( REQ_TEAM_SELECT );
				sendPacket << mySlotNo;
				sendPacket << 2;
				clientSocket->sendPacket( sendPacket );
				g_dxSound->playWav(1,false);
			}
			if( wParam == ID_TEAM_SELECT_BTN4)
			{
				Packet sendPacket( REQ_TEAM_SELECT );
				sendPacket << mySlotNo;
				sendPacket << 3;
				clientSocket->sendPacket( sendPacket );
				g_dxSound->playWav(1,false);
			}

			// 맵 바꾸기 버튼 39
			if( wParam == ID_MAP_CHANGE_BTN)
			{
				Packet sendPacket( REQ_MAP_CHANGE );
				sendPacket << mySlotNo;
				sendPacket << myMapKind;
				clientSocket->sendPacket( sendPacket );
			}
			// 게임 준비 버튼 40
			if( wParam == ID_READY_BTN)
			{
				Packet sendPacket( REQ_GAME_READY );
				sendPacket << mySlotNo;
				sendPacket << roomUser[mySlotNo].getReady();
				clientSocket->sendPacket( sendPacket );
			}
			// 팀킬 버튼 41
			if( wParam == ID_TEAM_KILL_BTN)
			{
			}
			// 강퇴 버튼 42
			if( wParam == ID_KICK_BTN)
			{
			}
			// 방 나가기 버튼 43
			if( wParam == ID_ROOM_EXIT_BTN)
			{
				Packet sendPacket( REQ_TEAM_SELECT );
				sendPacket << mySlotNo;
				sendPacket << 0;
				clientSocket->sendPacket( sendPacket );
				
				sendPacket.clear();
				sendPacket.id( REQ_CHARACTER_SELECT );
				sendPacket << mySlotNo;
				sendPacket << 0;
				clientSocket->sendPacket( sendPacket );

				sendPacket.clear();
				sendPacket.id( REQ_LEAVE_ROOM );
				sendPacket << myRoomIndex << myRoomNo << mySlotNo;
				clientSocket->sendPacket( sendPacket );

				outroRoom();
			}
		}
		editChatMessage->setFocus();
		break;
	case GM_GAME:
		break;
	}
}

void GameScreen::preTranslateMessage( MSG* msg )
{	
	// 게임에서 하는거	
	if( playLobby == true )
		return game->preTranslateMessage( msg );
	else
	{
		if( msg->message == WM_KEYDOWN )
		{
			if( msg->wParam == VK_RETURN && 
				dlgLogin->isShowWindow() == false && 
				dlgMakeRoom->isShowWindow() == false )
			{
				if( screenMode == GM_LOBBY)
				{
					Packet sendPacket( REQ_LOBBY_CHAT );
					sendPacket << editChatMessage->getString().GetBuffer( 0 );
					clientSocket->sendPacket( sendPacket );
					editChatMessage->empty();
				}
				if(screenMode == GM_ROOM)
				{
					Packet sendPacket( REQ_ROOM_CHAT );
					sendPacket << editChatMessage->getString().GetBuffer( 0 );
					clientSocket->sendPacket( sendPacket );

					editChatMessage->empty();
				}
			}
			else if(msg->wParam == VK_RETURN && 
				dlgLogin->isShowWindow() == true)
			{
				dlgLogin->connect();
			}

			else if(msg->wParam == VK_RETURN && 
				dlgMakeRoom->isShowWindow() == true)
			{
				dlgMakeRoom->makeRoom();
			}
			else if(msg->wParam == VK_ESCAPE && 
				dlgMakeRoom->isShowWindow() == true)
			{
				dlgMakeRoom->cancel();
			}

		}
	}

	FrameView::preTranslateMessage( msg );
}

bool GameScreen::process()
{
	static DWORD Curr = GetTickCount();

	if( exitLobby == true )
		return false;

	if( FrameView::process() == false )
		return false;

	// 게임에서 그리는거
	if( playLobby == true )
	{
		if( game->process() == false && GetTickCount() - Curr > 30 )
		{
			// 점수, 승자 그런거 처리
			// 게임 초기화
			game->exitGame = false; // 게임 대기 상태로 ㄱㄱ
			playLobby = false;
			screenMode = GM_ROOM;
			editChatMessage->setFocus(); // 로비 챗 포커싱
			game->getGame()->IsInit = false;
			return true;
		}
		game->draw();
	}
	else
	{
		if( dlgLogin->isShowWindow() == false && dlgMakeRoom->isShowWindow() == false  && game->playGame == false )
			editChatMessage->setFocus();
		onDraw();
	}

	return true;
}

// 서버로 부터 접속되었음을 통보 받았음
void GameScreen::onAckLogin(Packet& packet)
{
}

// 핑 체크
void GameScreen::onAckPing(Packet& packet)
{
}

// 서버 접속
void GameScreen::onNtfConnect(Packet& packet)
{
	// 자신의 아이디를 서버에게 알려준다.
	Packet sendPacket( REQ_ENTER_LOBBY );
	sendPacket << dlgLogin->getAccount().GetBuffer(0);
	clientSocket->sendPacket( sendPacket );
}

// 접속 종료
void GameScreen::onAckDisconnect(Packet& packet)
{
}

// 로비에 입장
void GameScreen::onAckEnterLobby(Packet& packet)
{
	// 로비 유저 리스트 와 룸 리스트를 요청한다.
	Packet sendPacket( REQ_LOBBY_USER_LIST );
	clientSocket->sendPacket( sendPacket );
	sendPacket.clear();
	sendPacket.id( REQ_LOBBY_ROOM_LIST );
	clientSocket->sendPacket( sendPacket );
}

// 방에 입장
void GameScreen::onAckEnterRoom(Packet& packet)
{
	CString message("방을 선택해주십시요");
	bool error = false;
	char title[127],account[127];
	int roomIndex, roomNo, slotNo, userCount;
	bool self;

	packet >> error >> self;

	if( error == true)
		return;
	if( self == false )
	{
		packet >> roomIndex >> roomNo >> title >> slotNo >> account >> userCount;

		listRoomInfo.GetAt(listRoomInfo.FindIndex(roomIndex)).setTitle(title);
		listRoomInfo.GetAt(listRoomInfo.FindIndex(roomIndex)).setUserCount( userCount );
	}

	else
	{
		packet >> myRoomIndex >> myRoomNo >> myTitle >> mySlotNo >> account >> userCount;
		listRoomInfo.GetAt(listRoomInfo.FindIndex(myRoomIndex)).setUserCount( userCount );
		intoRoom();
	}
	listLobbyChatMessage.RemoveAll();

	Packet sendPacket( REQ_LOBBY_USER_LIST );
	clientSocket->sendPacket( sendPacket );
	sendPacket.clear();
	sendPacket.id(REQ_ROOM_USER_LIST);
	clientSocket->sendPacket(sendPacket);
}

void GameScreen::onAckLeaveRoom(Packet& packet)
{
	int roomIndex, roomNo, slotNo, userCount;
	bool self;

	packet >> self >> roomIndex >> roomNo >> slotNo >> userCount;

	if( self == true )
	{
		listRoomChatMessage.RemoveAll();
		roomUser[0].setEmpty(true);
		roomUser[1].setEmpty(true);
		roomUser[2].setEmpty(true);
		roomUser[3].setEmpty(true);
		return;
	}
	else 
	{

		if(userCount == 0)
		{
			Packet sendPacket(REQ_DELETE_ROOM);
			sendPacket << roomIndex << roomNo;
			clientSocket->sendPacket(sendPacket);
		}

		else if( roomNo == myRoomNo )
			roomUser[slotNo].setEmpty( true );
	}

	Packet sendPacket( REQ_LOBBY_USER_LIST );
	clientSocket->sendPacket( sendPacket );
}

void GameScreen::onAckDeleteRoom(Packet& packet)
{
	int roomIndex, roomNo;
	POSITION pos1,pos2, pos3;
	GameRoom pa;
	packet >> roomIndex >> roomNo;

	listRoom->removeAllItem();

	for( pos1 = listRoomInfo.GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
	{
		pa = listRoomInfo.GetNext( pos1 );
		if(pa.getRoomNo() == roomNo)
		{
			listRoomInfo.RemoveAt( pos2 );
		}
	}

	pos3 = listRoomInfo.GetHeadPosition();
	for(int i = 0; pos3 != 0; i++)
	{
		char buf[127];

		GameRoom ss = listRoomInfo.GetNext(pos3);
		ss.setRoomIndex(i);
//		sprintf(buf,"인덱스:%d번호:%d 현재인원:%d 방제:%s",ss.getRoomIndex(),ss.getRoomNo(),ss.getUserCount(),ss.getTitle());
		sprintf(buf,"번호:%d방제:%s",ss.getRoomIndex(),ss.getTitle());

		listRoom->addString(buf);
	}
}

void GameScreen::onAckRestRoom(Packet& packet)
{
	Packet sendPacket( REQ_LOBBY_ROOM_LIST );
	clientSocket->sendPacket( sendPacket );
	outroRoom();
}

// 유저 정보
void GameScreen::onAckLobbyUserList(Packet& packet)
{
	char buffer[155];
	char account[127];

	int count, i, loc;
	listUser->removeAllItem();
	packet >> count;

	for( i = 0 ; i < count ; i++ )
	{
		packet >> account >> loc;
/*
		switch(loc)
		{
		case 2: 
			sprintf(buffer,"%s(대기실)",account);

			break;
		case 3:
			sprintf(buffer,"%s(방)",account);
			break;
		case 4:
			sprintf(buffer,"%s(게임중)",account);
			break;
		}
*/
		sprintf(buffer,"%s",account);
		listUser->addString(buffer);
	}
}

// 방 정보
void GameScreen::onAckLobbyRoomList(Packet& packet)
{
	char buf[127];
	char title[127];

	int count, i, roomIndex, roomNo, userCount;
	
	listRoom->removeAllItem();
	packet >> count;

	for( i = 0; i < count ; i++ )
	{
		packet >> roomIndex;
		packet >> roomNo;
		packet >> title;
		packet >> userCount;
		listRoomInfo.AddTail(GameRoom(roomIndex, roomNo,CString(title),4));
//		sprintf(buf,"인덱스:%d 번호:%d 현재인원:%d 방제:%s",roomIndex, roomNo,userCount,title);
		sprintf(buf,"번호:%d방제:%s",roomIndex,title);
		listRoom->addString(buf);
	}
}

void GameScreen::onAckUserInfo(Packet& packet)
{
	DWORD score,scoreRank, killSum, killRank;

	packet >> score >> scoreRank >> killSum >> killRank;

	userInfo.setScore(score);
	userInfo.setScoreRank(scoreRank);
	userInfo.setKillSum(killSum);
	userInfo.setKillRank(killRank);
}

void GameScreen::onAckUserLocation(Packet& packet)
{

}

//  채팅 메세지
void GameScreen::onAckLobbyChat(Packet& packet)
{
	char* account = new char [127];
	char* message = new char [127];

	packet >> account >> message;

	listLobbyChatMessage.AddTail( account );
	listLobbyChatMessage.AddTail( message );

	if( listLobbyChatMessage.GetCount() > 14 )
	{
		delete listLobbyChatMessage.RemoveHead();
		delete listLobbyChatMessage.RemoveHead();
	}
}

//  채팅 메세지
void GameScreen::onAckRoomChat(Packet& packet)
{
	char* account = new char [127];
	char* message = new char [127];

	packet >> account >> message;

	listRoomChatMessage.AddTail( account );
	listRoomChatMessage.AddTail( message );

	if( listRoomChatMessage.GetCount() > 14 )
	{
		delete listRoomChatMessage.RemoveHead();
		delete listRoomChatMessage.RemoveHead();
	}
}

void GameScreen::onAckMakeRoom(Packet& packet)
{
	int roomIndex, roomNo, userCount;
	char buf[127];
	char title[127];
	bool self;
	packet >> self >> roomIndex >> roomNo >> title >> userCount;

	if(self == true)
	{
		POSITION pos;
		Packet sendPacket( REQ_ENTER_ROOM );
		sendPacket << roomIndex;
		pos = listRoomInfo.FindIndex(roomIndex);
		sendPacket << listRoomInfo.GetAt(pos).getRoomNo();
		clientSocket->sendPacket( sendPacket );
	}
	else
	{
		listRoomInfo.AddTail( GameRoom(roomIndex,roomNo,CString(title),4));
//		sprintf(buf,"인덱스:%d 번호:%d 현재인원:%d 방제:%s",roomIndex, roomNo,userCount,title);
		sprintf(buf,"번호:%d방제:%s",roomIndex,title);
		listRoom->addString(buf);
	}
}

// 게임 종료
void GameScreen::onAckLeaveLobby(Packet& packet)
{
	Packet sendPacket( REQ_LOBBY_USER_LIST );
	clientSocket->sendPacket( sendPacket );
}

void GameScreen::onAckWait(Packet& packet)
{
}

void GameScreen::onAckResume(Packet& packet)
{
}

void GameScreen::onAckRoomUserList(Packet& packet)
{
	// 방의 유저 정보
	char account[127];
	int count, slotNo, team, character;

	packet >> count;

	for(int i = 0; i < count ; i++ )
	{
		packet >> slotNo >> team >> character >> account;
		roomUser[slotNo].setSlotNo(slotNo);
		roomUser[slotNo].setEmpty(false);
		roomUser[slotNo].setTeam(team);
		roomUser[slotNo].setCharacter(character);
		roomUser[slotNo].setAccount(account);
	}
}

void GameScreen::onAckGameModeChange(Packet& packet)
{
	int slotNo = 0, gameMode = 0;
	packet >> slotNo >> gameMode;
	myGameMode = gameMode;
}

void GameScreen::onAckChracterSelect(Packet& packet)
{
	int slotNo;
	int i;
	packet >> slotNo >> i;
	switch(i)
	{
		case 0:
			roomUser[slotNo].setCharacter(0);
			characterSelectButton[1]->setCheck(false);
			characterSelectButton[2]->setCheck(false);
			characterSelectButton[3]->setCheck(false);
			break;
		case 1:
			roomUser[slotNo].setCharacter(1);
			characterSelectButton[0]->setCheck(false);
			characterSelectButton[2]->setCheck(false);
			characterSelectButton[3]->setCheck(false);
			break;
		case 2:
			roomUser[slotNo].setCharacter(2);
			characterSelectButton[0]->setCheck(false);
			characterSelectButton[1]->setCheck(false);
			characterSelectButton[3]->setCheck(false);
			break;
		case 3:
			roomUser[slotNo].setCharacter(3);
			characterSelectButton[0]->setCheck(false);
			characterSelectButton[1]->setCheck(false);
			characterSelectButton[2]->setCheck(false);
			break;
	}
}

void GameScreen::onAckReady(Packet& packet)
{
	int	 slotNo;
	bool ready;
	packet >> slotNo >> ready;
	roomUser[mySlotNo].setReady(ready);
	Packet sendPacket(REQ_GAME_START);
	clientSocket->sendPacket(sendPacket);
}

void GameScreen::onAckGameStart(Packet& packet)
{
	IOClass::SetServerIDs(0, myRoomNo, mySlotNo);

	playLobby = true;
	game->init(); // init : Seek 첫째날 ChatMessage만
	screenMode = GM_GAME;
	g_dxSound->stop(0);
	g_dxSound->playWav(10);
	
	int i;
	for( i = 0 ; i < 4 ; i++)
		game->GetFBHGame()->Char_Kinds[i] = -1;

	for(i = 0 ; i < 4 ; i++)
		if(!roomUser[i].getEmpty())
		{
			strcpy(game->GetFBHGame()->Accounts[roomUser[i].getSlotNo()], roomUser[i].getAccount().GetBuffer(0));
			game->GetFBHGame()->Char_Kinds[roomUser[i].getSlotNo()] = roomUser[i].getCharacter();
			game->GetFBHGame()->Char_Color[roomUser[i].getSlotNo()] = roomUser[i].getTeam();
		}

	Packet sendPacket( REQ_LOBBY_USER_LIST );
	clientSocket->sendPacket( sendPacket );
}

void GameScreen::onAckKillSum(Packet& packet)
{
}

void GameScreen::onAckTeamKill(Packet& packet)
{
}

void GameScreen::onAckKick(Packet& packet)
{
}

void GameScreen::onAckTeamSelect(Packet& packet)
{
	int slotNo;
	int	i;
	packet >> slotNo >> i;
	switch(i)
	{
	case 0:
		roomUser[slotNo].setTeam(0);
		teamSelectButton[1]->setCheck(false);
		teamSelectButton[2]->setCheck(false);
		teamSelectButton[3]->setCheck(false);
		break;
	case 1:
		roomUser[slotNo].setTeam(1);
		teamSelectButton[0]->setCheck(false);
		teamSelectButton[2]->setCheck(false);
		teamSelectButton[3]->setCheck(false);
		break;
	case 2:
		roomUser[slotNo].setTeam(2);
		teamSelectButton[0]->setCheck(false);
		teamSelectButton[1]->setCheck(false);
		teamSelectButton[3]->setCheck(false);
		break;
	case 3:
		roomUser[slotNo].setTeam(3);
		teamSelectButton[0]->setCheck(false);
		teamSelectButton[1]->setCheck(false);
		teamSelectButton[2]->setCheck(false);
		break;
	}
}

void GameScreen::onAckMapChange(Packet& packet)
{
	int slotNo;
	int	i;
	packet >> slotNo >> i;
	myMapKind = i;
}
// 방장을 바꾼다.
void GameScreen::onAckChangeMajor(Packet& packet)
{
}

// 키 입력 처리
void GameScreen::processInkey()
{
}

// 화면 출력
void GameScreen::onDraw()
{
	FrameView::onDraw();
	switch( screenMode )
	{
	case GM_LOGIN:
	case GM_MAKEROOM:
	case GM_LOBBY:
		display->blt(0,0,surfLobbyBackground);
//		onDrawRoomInfo();
//		onDrawLobbyUserInfo();
		onDrawLobbyChat();
		break;
	case GM_ROOM:
		display->blt(0,0,surfRoomBackground);
		onDrawRoomUserInfo();
		onDrawRoomTitle();
//		onDrawGameMode();
		onDrawRoomUserInfo();
//		onDrawRoomSmallMap();
		onDrawRoomChat();
		break;
	case GM_GAME:
		break;
	}

#ifdef _DEBUG
	onDrawFPS();
#endif
}

void GameScreen::onDrawFPS()
{
	char fps[60];
	
	sprintf(fps,"fps: %f",display->getFPS());
	drawText( Font_Medium_Normal, fps, 700, 10, RGB( 0, 0, 0 ) );
}

// 방 정보 출력
void GameScreen::onDrawRoomInfo()
{
/*
	char roomNo[60], userCount[60];
	
	for( int i = 0; i < listRoomInfo.GetSize(); i++ )
	{
		sprintf(roomNo,"%d",listRoomInfo.GetAt(listRoomInfo.FindIndex(i)).getRoomNo());
		drawText( Font_Medium_Normal, roomNo, LOBBY_ROOM_NUM_X, LOBBY_ROOM_NUM_Y+LOBBY_ROOM_GAP_Y*i, RGB( 0, 0, 0 ) );
		drawText( Font_Medium_Normal, listRoomInfo.GetAt(listRoomInfo.FindIndex(i)).getTitle() , LOBBY_ROOM_TITLE_X, LOBBY_ROOM_TITLE_Y+LOBBY_ROOM_GAP_Y*i, RGB( 0, 0, 0 ) );

		sprintf(userCount,"%d",listRoomInfo.GetAt(listRoomInfo.FindIndex(i)).getUserCount());
		drawText( Font_Medium_Normal, userCount , LOBBY_ROOM_USER_COUNT_X, LOBBY_ROOM_USER_COUNT_Y+LOBBY_ROOM_GAP_Y*i, RGB( 0, 0, 0 ) );
	}
*/
}

void GameScreen::onDrawLobbyChat(void)
{
	//  Output chat message
	POSITION pos;
	int count = 0;

	pos = listLobbyChatMessage.GetHeadPosition();
	while( pos != 0 )
	{
		char* account = listLobbyChatMessage.GetNext( pos );
		char* message = listLobbyChatMessage.GetNext( pos );

		drawText( Font_Medium_Normal, account, LOBBY_CHAT_ACCOUNT_X, LOBBY_CHAT_ACCOUNT_Y + count * LOBBY_CHAT_MESSAGE_GAP, RGB( 200, 0, 0 ) );
		drawText( Font_Medium_Normal, message, LOBBY_CHAT_ACCOUNT_X + 7*(strlen(account)+1), LOBBY_CHAT_MESSAGE_Y + count * LOBBY_CHAT_MESSAGE_GAP, RGB( 0, 0, 0 ) );
 		count++;
	}
}
void GameScreen::onDrawRoomTitle(void)
{
	drawText( Font_Medium_Normal,myTitle, ROOM_TITLE_X, ROOM_TITLE_Y, RGB( 0, 0, 0 ) );
}
void GameScreen::onDrawRoomChat(void)
{
	//  Output chat message
	POSITION pos;
	int count = 0;

	pos = listRoomChatMessage.GetHeadPosition();
	while( pos != 0 )
	{
		char* account = listRoomChatMessage.GetNext( pos );
		char* message = listRoomChatMessage.GetNext( pos );

		drawText( Font_Medium_Normal, account, LOBBY_CHAT_ACCOUNT_X, LOBBY_CHAT_ACCOUNT_Y + count * LOBBY_CHAT_MESSAGE_GAP, RGB( 200, 0, 0 ) );
		drawText( Font_Medium_Normal, message, LOBBY_CHAT_ACCOUNT_X + 7*(strlen(account)+1), LOBBY_CHAT_MESSAGE_Y + count * LOBBY_CHAT_MESSAGE_GAP, RGB( 0, 0, 0 ) );

		count++;
	}
}


void GameScreen::onDrawGameMode()
{
	switch(myGameMode)
	{
	case 0:
		drawText( Font_Medium_Normal, "서바이벌 모드", ROOM_GAME_MODE_X, ROOM_GAME_MODE_Y, RGB( 0, 0, 0 ) );
		break;
	case 1:
		drawText( Font_Medium_Normal, "노아이템 모드", ROOM_GAME_MODE_X, ROOM_GAME_MODE_Y, RGB( 0, 0, 0 ) );
		break;
	case 2:
		drawText( Font_Medium_Normal, "아이템 모드", ROOM_GAME_MODE_X, ROOM_GAME_MODE_Y, RGB( 0, 0, 0 ) );
		break;
	}
}
void GameScreen::onDrawRoomUserInfo(void)
{
	POINT pos;
	
	for(int i = 0; i < 4; i++)
	{
		if(roomUser[i].getEmpty() == false)
		{
			switch( i )
			{
			case 0:
				pos.x = ROOM_USER_INFO_X1;
				pos.y = ROOM_USER_INFO_Y1;
				break;
			case 1:
				pos.x = ROOM_USER_INFO_X2;
				pos.y = ROOM_USER_INFO_Y2;
				break;
			case 2:
				pos.x = ROOM_USER_INFO_X3;
				pos.y = ROOM_USER_INFO_Y3;
				break;
			case 3:
				pos.x = ROOM_USER_INFO_X4;
				pos.y = ROOM_USER_INFO_Y4;
				break;
			}

			switch(roomUser[i].getTeam())
			{
			case 0:
				display->blt(pos.x,pos.y,surfTeamBoard[0]);
				break;
			case 1:
				display->blt(pos.x,pos.y,surfTeamBoard[1]);
				break;
			case 2:
				display->blt(pos.x,pos.y,surfTeamBoard[2]);
				break;
			case 3:
				display->blt(pos.x,pos.y,surfTeamBoard[3]);
				break;
			}

			switch(roomUser[i].getCharacter())
			{
			case 0:
				display->blt(pos.x+19,pos.y+25,surfCharacter[0]);
				break;
			case 1:
				display->blt(pos.x+19,pos.y+25,surfCharacter[1]);
				break;
			case 2:
				display->blt(pos.x+19,pos.y+25,surfCharacter[2]);
				break;
			case 3:
				display->blt(pos.x+19,pos.y+25,surfCharacter[3]);
				break;
			}
			/*
			switch(roomUser[i].getReady())
			{
			case false:
				drawText( Font_Medium_Normal, "대기상태", pos.x, pos.y+75, RGB( 0, 0, 0 ) );
				break;
			case true:
				drawText( Font_Medium_Normal, "준비완료", pos.x, pos.y+75, RGB( 0, 0, 0 ) );
				break;
			}
			*/
			drawText( Font_Medium_Normal, roomUser[i].getAccount().GetBuffer(0), pos.x+20, pos.y+95, RGB( 0, 0, 0 ) );
		}
	}
}
void GameScreen::onDrawRoomSmallMap()
{
	switch(myMapKind)
	{
	case 0:
		display->blt(ROOM_SMALL_MAP_X,ROOM_SMALL_MAP_Y,surfSmallMap[0]);
		break;
	case 1:
		display->blt(ROOM_SMALL_MAP_X,ROOM_SMALL_MAP_Y,surfSmallMap[1]);
		break;
	case 2:
		break;
	}
}
void GameScreen::outroLogin(void)
{
	intoLobby();
	if(screenMode != GM_LOBBY)
		screenMode = GM_LOBBY;
}
void GameScreen::intoLobby(void)
{
	listUser->showWindow(true);
	listRoom->showWindow(true);
	joinButton->showWindow( true );
	makeRoomButton->showWindow(true);
//	roomResetButton->showWindow( true );

//	fastJoinButton->showWindow(true);
//	userInfoButton->showWindow(true);
//	sendButton->showWindow(true);
	exitLobbyButton->showWindow(true);

	joinButton->enable( true );
	makeRoomButton->enable( true );
//	roomResetButton->enable( true );
//	fastJoinButton->enable( true );
//	userInfoButton->enable( true );
//	sendButton->enable( true );
	exitLobbyButton->enable( true );

	editChatMessage->setString("");
	editChatMessage->setFocus();

	if(screenMode != GM_LOBBY)
		screenMode = GM_LOBBY;
}

void GameScreen::intoRoom(void)
{
	listUser->showWindow(false);
	listRoom->showWindow(false);
	joinButton->showWindow( false );
	makeRoomButton->showWindow(false);
//	roomResetButton->showWindow( false );
//	fastJoinButton->showWindow(false);
//	userInfoButton->showWindow(false);
//	sendButton->showWindow(false);
	exitLobbyButton->showWindow(false);

	// 방
//	gameModeChangeButton->showWindow(true);
	characterSelectButton[0]->showWindow(true);
	characterSelectButton[1]->showWindow(true);
	characterSelectButton[2]->showWindow(true);
	characterSelectButton[3]->showWindow(true);
	teamSelectButton[0]->showWindow(true);
	teamSelectButton[1]->showWindow(true);
	teamSelectButton[2]->showWindow(true);
	teamSelectButton[3]->showWindow(true);
//	mapChangeButton->showWindow(true);
	readyGameButton->showWindow(true);
//	teamKillSelectButton->showWindow(true);
//	kickButton->showWindow(true);
	exitRoomButton->showWindow(true);

	editChatMessage->setString("");
	editChatMessage->setFocus();
	if(screenMode != GM_ROOM)
		screenMode = GM_ROOM;
}

void GameScreen::outroRoom(void)
{
//	gameModeChangeButton->showWindow(false);
	characterSelectButton[0]->showWindow(false);
	characterSelectButton[1]->showWindow(false);
	characterSelectButton[2]->showWindow(false);
	characterSelectButton[3]->showWindow(false);
	teamSelectButton[0]->showWindow(false);
	teamSelectButton[1]->showWindow(false);
	teamSelectButton[2]->showWindow(false);
	teamSelectButton[3]->showWindow(false);
//	mapChangeButton->showWindow(false);
	readyGameButton->showWindow(false);
//	teamKillSelectButton->showWindow(false);
//	kickButton->showWindow(false);
	exitRoomButton->showWindow(false);
	
	intoLobby();
}
void GameScreen::intoMakeRoom(void)
{
	joinButton->enable( false );
	makeRoomButton->enable( false );
//	roomResetButton->enable( false );
//	fastJoinButton->enable( false );
//	userInfoButton->enable( false );
//	sendButton->enable( false );
	exitLobbyButton->enable( false );
	editChatMessage->enableIME( false );
	editChatMessage->enableChar( false );

	dlgMakeRoom->emptyTitle();
	dlgMakeRoom->showWindow( true );
	dlgMakeRoom->setEditFocus();

	if(screenMode != GM_MAKEROOM)
		screenMode = GM_MAKEROOM;
}

void GameScreen::outroMakeRoom(void)
{
	dlgMakeRoom->showWindow( false );
	int i = 0;
	makeRoomButton->enable( true );
//	roomResetButton->enable( true );
//	fastJoinButton->enable( true );
//	userInfoButton->enable( true );
//	sendButton->enable( true );
	exitLobbyButton->enable( true );
	editChatMessage->enableIME( true );
	editChatMessage->enableChar( true );

	intoLobby();
}