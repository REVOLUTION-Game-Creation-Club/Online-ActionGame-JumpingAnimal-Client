#include "stdafx.h"
#include "LobbyClient.h"
#include ".\fbhgame.h"
#include "Protocol.h"
#include "_Gamer.h"
#include "DxWindow/DirectX/DxSound.h"

extern char status = 0;

#define VIDEOMEMORY 0
#define SYSTEMMEMORY 1
#define VISIBLECHATFRAME 200

extern DxSound *g_dxSound;

#define FIGUREOFENDKILL 10

int randomize( int min, int max )
{	
	return ( (rand() % max) + min );
}

ClientSocket*			CFBHGame::clientSocket = 0;
KeyBoard*				CFBHGame::pKey = 0;
HWND					CFBHGame::gamehwnd = 0;

CFBHGame::CFBHGame( HWND hWnd, DxDisplay* disp, ClientSocket* cs)
	: FrameView( disp ), exitGame(false), handleMainFrm( hWnd ), m_bOnChatMode( false )
{
	srand( (unsigned int) ( time(NULL) + rand() ) );

	clientSocket = cs;

	gamehwnd = hWnd;

	m_cFBH = new FBHGame(hWnd, disp, 4, *cs );

	m_dwFrame = 0;

	for( int i = 0 ; i < 4; i++ )
        ZeroMemory( (void*)&m_ciChat[i], sizeof( m_ciChat[i] ) );
}

CFBHGame::~CFBHGame(void)
{
	delete m_cFBH;
}

extern "C" IDirectDrawSurface7 *CFBHGame::DDLoadBitMap( IDirectDraw7 *pdd, LPCSTR szBitmap, int dx, int dy, int memLoc )
{
	HBITMAP				hbm;
	BITMAP				bm;
	DDSURFACEDESC2		ddsd;
	IDirectDrawSurface7	*pdds;

	// 리소스로 로드
	hbm = (HBITMAP)LoadImage( GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, dx, dy, LR_CREATEDIBSECTION );

	if( hbm == NULL )
		hbm = (HBITMAP)LoadImage( GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, dx, dy, LR_LOADFROMFILE );

	if( hbm == NULL )
		return NULL;

	GetObject( hbm, sizeof(BITMAP), &bm ); // 비트맵의 정보를 가져온다
    
	// Surface 생성
	ZeroMemory( &ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;

	if( memLoc == 0 )
	{
		// 비디오 메모리에 먼저 생생해본다 -> 시스템 메모리에 생성
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	}
	else
	{
		// 시스템 메모리에 생성
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	}

	ddsd.dwHeight = bm.bmHeight;
	ddsd.dwWidth = bm.bmWidth;

    if( FAILED( pdd->CreateSurface( &ddsd, &pdds, NULL ) ) )
		return NULL;

	DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);

	DeleteObject(hbm);

	return pdds;
}

HRESULT CFBHGame::DDCopyBitmap(IDirectDrawSurface7 *pdds, HBITMAP hbm, int x, int y, int dx, int dy)
{
	HDC                 hdcImage;
	HDC                 hdc;
	BITMAP              bm;
	DDSURFACEDESC2      ddsd;
	HRESULT             hr;

	if (hbm == NULL || pdds == NULL)
		return E_FAIL;

	//
	// make sure this surface is restored.
	//
	pdds->Restore();

	//
	//  select bitmap into a memoryDC so we can use it.
	//
	hdcImage = CreateCompatibleDC(NULL);
	if (!hdcImage)
		OutputDebugString("createcompatible dc failed\n");
	SelectObject(hdcImage, hbm);

	//
	// get size of the bitmap
	//
	GetObject(hbm, sizeof(bm), &bm);    // get size of bitmap
	dx = dx == 0 ? bm.bmWidth  : dx;    // use the passed size, unless zero
	dy = dy == 0 ? bm.bmHeight : dy;

	//
	// get size of surface.
	//
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	pdds->GetSurfaceDesc(&ddsd);

	if ((hr = pdds->GetDC(&hdc)) == DD_OK)
	{
		StretchBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, x, y, dx, dy, SRCCOPY);
		pdds->ReleaseDC(hdc);
	}

	DeleteDC(hdcImage);

	return hr;
}

void CFBHGame::onCreate()
{
	FrameView::onCreate();

	// 각종 필요한 컨트롤들 셋팅
	editChatMessage = new EditBoxCtrl( EDITCHATMSG, display );
	editChatMessage->create( _T( "BMP/UI/EditBoxBack.bmp" ),
		25 + 20, 479 + 11, this, 0, -1, _T( "BMP/UI/CaretEng.bmp" ), _T( "BMP/UI/CaretHan.bmp" ) );
	editChatMessage->setColorKey(RGB(65, 165, 140));

	editChatMessage->setIMEWIndowHandle( handleMainFrm );
	editChatMessage->enableChar( false );

	exitButton = new ButtonCtrl( EXITBUTTON, display );
	exitButton->create( _T("BMP/UI/ExitButton.bmp"), 762, 531, Window::Attr_ShowWindow, this );
	exitButton->setColorKey( RGB(65, 165, 140) );
	exitButton->enable( true );

	playGame = true;
}

void CFBHGame::onDestroy()
{
	FrameView::onDestroy();
}

void CFBHGame::onSocketConnect( WPARAM wParam, LPARAM lParam )
{

}

void CFBHGame::onSocketClose( WPARAM wParam, LPARAM lParam )
{
}

/************************************************************************/
/*	패킷 처리 함수
/************************************************************************/
void CFBHGame::packetParsing( Packet& packet )
{
	switch( packet.id() )
	{
		// 패킷 처리
	case	ACK_GAME_CHAT :			onAckGameChat(packet); break;
	case	ACK_GAME_QUIT :			onAckGameQuit(packet); break;
	case	REQ_GAME	:			onReqGame(packet);		break;
	default:
		break;
	}
}

void CFBHGame::onReqGame(Packet& packet)
{
	Packet pk;
	pk.copyToBuffer( packet.getPacketBuffer(), packet.getDataFieldSize() );

	DWORD state;
	pk >> state;
	if( state == INIT_GAMESTATE )
	{
		DWORD MapNO;
		pk >> MapNO;

		char NameBuf[20];
		DWORD pos_x = 0, pos_y = 0;
		int UserCount;
		pk >> UserCount;

		for( int i = 0; i < UserCount; i++ )
		{
			pk >> NameBuf >> pos_x >> pos_y;
			strncpy( m_ciChat[i].name, NameBuf, strlen(NameBuf) );
			m_ciChat[i].frame = VISIBLECHATFRAME;
		}
	}

	m_cFBH->packetParsing(packet);
}

//  채팅 메세지
void CFBHGame::onAckGameChat(Packet& packet)
{
	char account[127];
	char message[127];

	packet >> account >> message;

	for( int i = 0; i < 4; i++ )
	{
		// 이름이 같으묜
		if( !strcmp( m_ciChat[i].name, account ) )
		{
			if( strcmp( message, "" ) ) // NULL 이 아니면
			{
				strcpy( m_ciChat[i].msg, message );
				m_ciChat[i].frame = VISIBLECHATFRAME;
			}
		}
	}
}
void CFBHGame::onAckGameQuit(Packet& packet)
{
	exitGame = true;
	g_dxSound->stop(10);
	g_dxSound->playWav(0);

	Packet sendPacket( REQ_LOBBY_USER_LIST );
	clientSocket->sendPacket( sendPacket );
}

void CFBHGame::parseMessage( DWORD message, WPARAM wParam, LPARAM lParam )
{
	/* 게임내에서 컨트롤끼리 통신 & Status를 변경 할때 사용 */
	if( message == CR_DOWN )
	{
		if( wParam == EXITBUTTON )
		{
			Packet sendPacket(REQ_GAME_QUIT);
			sendPacket << IOClass::GetServerGameID();
			clientSocket->sendPacket(sendPacket);
		}
	}
}

void CFBHGame::preTranslateMessage( MSG* msg )
{
	FrameView::preTranslateMessage( msg );

	if( msg->message == WM_KEYDOWN )
	{
		if( msg->wParam == VK_RETURN )
		{
			if( m_bOnChatMode )
			{
				// 채팅모드면 보냄
				Packet sendPacket( REQ_GAME_CHAT );
				// 말풍선
				if( strcmp( editChatMessage->getString().GetBuffer(0), "") )
				{
					sendPacket << m_cFBH->gamer[ IOClass::GetServerUserID() ]->getname();
					sendPacket << editChatMessage->getString().GetBuffer( 0 );
					clientSocket->sendPacket( sendPacket );
				}
				editChatMessage->empty();

				m_bOnChatMode = false;
				editChatMessage->enableIME(false);
				editChatMessage->enableChar(false);
			}
			else
			{
				//아니면 채팅모드로
				m_bOnChatMode = true;
				editChatMessage->enableIME(true);
				editChatMessage->enableChar(true);

				// 다 뿌렸다고 인정
				for( int i = 0; i < 4; i++ )
				{
					if( !strcmp(m_cFBH->gamer[ IOClass::GetServerUserID() ]->getname(), m_ciChat[i].name ) )
					{
						m_ciChat[i].frame = -1;
						break;
					}
				}
			}
		}
	}

	if( msg->message == WM_MOUSEMOVE )
	{
		this->onMouseMove( msg->wParam, msg->pt );
	}
}

bool CFBHGame::process()
{
	if( FrameView::process() == false )
		return false;

	if( playGame == true )
	{
		m_cFBH->Process( m_bOnChatMode );		
	}

	if( exitGame == true )
	{
		m_cFBH->GameOver();
		return false;
	}

	if( m_bOnChatMode )
		editChatMessage->setFocus();

	return true;
}

void CFBHGame::onDraw()
{
	FrameView::onDraw();

	DrawUi();
}

void CFBHGame::BoundCheck( int i, DrawInstruction &ins )
{
	if( (m_cFBH->gamer[i]->x - XTERM) >= 750 )
	{
		ins.sourceX = 0;
		ins.sourceWidth = 800 - (m_cFBH->gamer[i]->x - XTERM);
		ins.sourceY = 0;
		ins.sourceHeight = 50;
		// 오른쪽벽에 걸리는 부분
		IOClass::GetDrawManager()->DrawOnLayer(2, ins);

		ins.destX = 0;
		ins.sourceX =  800 - (m_cFBH->gamer[i]->x - XTERM);
		ins.sourceWidth = 50 - ins.sourceX;
		// 왼쪽벽에 걸리는 부분
		IOClass::GetDrawManager()->DrawOnLayer(2, ins);
	}
	else if( (m_cFBH->gamer[i]->x - XTERM) < 0 )
	{
		ins.destX = (m_cFBH->gamer[i]->x - XTERM) + 800;
		ins.sourceX = 0;
		ins.sourceWidth = 800 - ins.destX;
		ins.sourceY = 0;
		ins.sourceHeight = 50;
		// 오른쪽벽에 걸리는 부분
		IOClass::GetDrawManager()->DrawOnLayer(1, ins);

		ins.destX = (m_cFBH->gamer[i]->x - XTERM);
		ins.sourceX = -ins.destX;
		ins.sourceWidth = 50 - ins.sourceX;
		ins.destX = 0;
		// 왼쪽벽에 걸리는 부분
		IOClass::GetDrawManager()->DrawOnLayer(1, ins);
	}
	else IOClass::GetDrawManager()->DrawOnLayer(2, ins);
}

void CFBHGame::DrawUi()
{
#ifdef _DEBUG
	drawFps();
#endif

	// back under pannel
	DrawInstruction backpannel;
	backpannel.BitmapName = "BMP\\UI\\inter_under.bmp";
	backpannel.destX = 0;
	backpannel.destY = 524;
	backpannel.sourceHeight = 76;
	backpannel.sourceWidth = 800;
	backpannel.sourceX = 0;
	backpannel.sourceY = 0;
	IOClass::GetDrawManager()->DrawOnLayer(2, backpannel);

	// for player 1 pannel
	DrawInstruction self;
	self.BitmapName = "BMP\\UI\\p1char.bmp";
	self.sourceHeight = 82;
	self.sourceWidth = 235;
	self.destX = 9;
	self.destY = 509;
	self.sourceX = 0;
	self.sourceY = 0;
	IOClass::GetDrawManager()->DrawOnLayer(2, self);

	// for 2 3 4player pannel
	DrawInstruction otherpannel;
	otherpannel.BitmapName = "BMP\\UI\\p23char.bmp";
	otherpannel.sourceHeight = 72;
	otherpannel.sourceWidth = 160;
	otherpannel.destX = 254;
	otherpannel.destY = 520;
	otherpannel.sourceX = 0;
	otherpannel.sourceY = 0;
	IOClass::GetDrawManager()->DrawOnLayer(2, otherpannel);

	otherpannel.destX = 424;
	otherpannel.destY = 520;
	IOClass::GetDrawManager()->DrawOnLayer(2, otherpannel);

	otherpannel.BitmapName = "BMP\\UI\\p4char.bmp";
	otherpannel.destX = 594;
	otherpannel.destY = 520;
	IOClass::GetDrawManager()->DrawOnLayer(2, otherpannel);

	DrawInstruction pannelconnect;
	pannelconnect.BitmapName = "BMP\\UI\\inter-connect.bmp";
	pannelconnect.sourceWidth = 39;
	pannelconnect.sourceHeight = 59;
	pannelconnect.sourceX = 0;
	pannelconnect.sourceY = 0;

	for( int i = 0 ; i < 3; i++ )
	{
		pannelconnect.destX = 227 + 170 * i;
		pannelconnect.destY = 517;
		IOClass::GetDrawManager()->DrawOnLayer(2, pannelconnect);
	}

	// for character
	DrawInstruction ins;
	ins.sourceHeight = 50;
	ins.sourceWidth = 50;
	ins.destX = 27;
	ins.destY = 525;
	ins.sourceX = 0;
	ins.sourceY = 0;

	for( int i = 0; i < 4; i++ )
	{
		if( m_cFBH->gamer[i] != NULL ) // NULL 이면 다 헛방
		{
			if( m_cFBH->gamer[i]->IsYourSelf() ) // 내가 항상 좌측 아래 : 0번째 그림
			{
				switch( m_cFBH->gamer[i]->GetKindOfChar() ) {
					case SHEEP:
						ins.BitmapName = "BMP\\양\\normal.bmp";
						break;
					case CHICKEN:
						ins.BitmapName = "BMP\\닭\\normal.bmp";
						break;
					case TIGER:
						ins.BitmapName = "BMP\\냐옹이\\normal.bmp";
						break;
					case SNAKE:
						ins.BitmapName = "BMP\\뱀\\normal.bmp";
						break;
				}
				IOClass::GetDrawManager()->DrawOnLayer(2, ins); // 내 그림 그리기

				// for character cursor
				DrawInstruction cursor;
				cursor.sourceWidth = 50;
				cursor.sourceHeight = 50;
				cursor.sourceX = 0;
				cursor.sourceY = 0;
				cursor.destX = m_cFBH->gamer[i]->x - XTERM;
				cursor.destY = m_cFBH->gamer[i]->y - 50 - 7;

				if( m_dwFrame >= 0 && m_dwFrame <= 7 )
				{
					cursor.BitmapName = "BMP\\UI\\byby1.bmp";
					m_dwFrame++;
				}
				else if( m_dwFrame > 7 && m_dwFrame <= 21 )
				{
					cursor.BitmapName = "BMP\\UI\\byby2.bmp";
					m_dwFrame++;
				}
				else if( m_dwFrame > 21 && m_dwFrame <= 28 )
				{
					cursor.BitmapName = "BMP\\UI\\byby3.bmp";
					m_dwFrame++;
				}
				else if( m_dwFrame > 28 )
				{
					cursor.BitmapName = "BMP\\UI\\byby3.bmp";
					m_dwFrame = 0;
				}
				BoundCheck( i, cursor );

				// 이름 뿌리기
				char strName[32];
				ZeroMemory(strName, 32);
				wsprintf(strName, "%s", m_cFBH->gamer[i]->GetID() );				
				int nameLength = strlen(strName);
				drawText( Font_Large_Bold, strName, 92 + 65 - ( ( nameLength / 2 ) * 11 ), 524, RGB(0,0,0) );

				int avail = FIGUREOFENDKILL - m_cFBH->gamer[i]->GetNumberOfKill();
				wsprintf(strName, "last %d Kill", avail );
				nameLength = strlen(strName);
				drawText( Font_Large_Bold, strName, 151 - ( (nameLength / 2 ) * 11 ), 554, RGB(0,0,0) );
				break;
			}
		}
	}

	for( int i = 0, index = 0; i < 4; i++ )
	{
		if( m_cFBH->gamer[i] != NULL )
		{
			if( !m_cFBH->gamer[i]->IsYourSelf() ) // 내가 아니다
			{
				switch( m_cFBH->gamer[i]->GetKindOfChar() ) {
					case SHEEP:
						ins.BitmapName = "BMP\\양\\normal.bmp";
						break;
					case CHICKEN:
						ins.BitmapName = "BMP\\닭\\normal.bmp";
						break;
					case TIGER:
						ins.BitmapName = "BMP\\냐옹이\\normal.bmp";
						break;
					case SNAKE:
						ins.BitmapName = "BMP\\뱀\\normal.bmp";
						break;
				}
				ins.destY = 531;
				ins.destX = 276 + index * 170;	// Another index 번째에 그린다
				IOClass::GetDrawManager()->DrawOnLayer(2, ins); // 남그림 그리기

				// 남이름 뿌리기				
				char strName[32];
				ZeroMemory(strName, 32);
				wsprintf(strName, "%s", m_cFBH->gamer[i]->GetID() );				
				int nameLength = strlen(strName);
				drawText( Font_Medium_Bold, strName, 332 + ( index * 170 ) + 34  - ( ( nameLength / 2 ) * 11 ), 532, RGB(0,0,0) );
				int avail = FIGUREOFENDKILL - m_cFBH->gamer[i]->GetNumberOfKill();
				wsprintf(strName, "%d avail", avail );
				nameLength = strlen(strName);
				drawText( Font_Medium_Bold, strName, 376 + ( index * 170) - ( (nameLength / 2 ) * 11 ), 556, RGB(0,0,0) );

				index++;
			}
		}
	}

	if( editChatMessage->isEnabledChar() )
	{
		DrawInstruction Self;
		Self.sourceX = 0;
		Self.sourceY = 0;
		Self.sourceHeight = 56;
		Self.sourceWidth = 208;

		// 기린다
		for( int i = 0; i < 4; i++ )
		{
			if( m_cFBH->gamer[i] != NULL )
			{
				if( !strcmp( m_cFBH->gamer[ IOClass::GetServerUserID() ]->getname(), m_ciChat[i].name ) )
				{
					switch( m_ciChat[i].team ){
					case WHITE:
						Self.BitmapName = "BMP\\UI\\white-big.bmp";
						break;
					case BLUE:
						Self.BitmapName = "BMP\\UI\\blue-big.bmp";
						break;
					case RED:
						Self.BitmapName = "BMP\\UI\\red-big.bmp";
						break;
					case BLACK:
						Self.BitmapName = "BMP\\UI\\black-big.bmp";
						break;
					}
					Self.destX = 25;
					Self.destY = 479;
					IOClass::GetDrawManager()->DrawOnLayer(2, Self);
				}
			}
		}		
	}

	onDrawChatMessage();
}

void CFBHGame::drawFps()
{
	int FPS = (int)display->getFPS();
	char strfps[32];
	wsprintf(strfps, "Fps : %d", FPS);

	drawText( Font_Medium_Normal, strfps, 0, 0, RGB(0,0,0) );
}

void CFBHGame::onDrawChatMessage(void)
{
	// 칼라 흰 0 파 1 빨 2 검 3
	DrawInstruction Self;
	Self.sourceX = 0;
	Self.sourceY = 0;
	Self.sourceHeight = 56;
	Self.sourceWidth = 208;

	DrawInstruction Another;
	Another.sourceX = 0;
	Another.sourceY = 0;
	Another.sourceHeight = 49;
	Another.sourceWidth = 165;

	bool findme = false;
	// 자기꺼
	if( m_cFBH->gamer[ IOClass::GetServerUserID() ] != NULL ) // NULL이 아니면
	{
		if( m_cFBH->gamer[ IOClass::GetServerUserID() ]->IsYourSelf() ) // 내다
		{		
			for( int i = 0; i < 4; i++ )
			{
				if( !strcmp( m_cFBH->gamer[ IOClass::GetServerUserID() ]->getname(), m_ciChat[i].name) )
				{
					if( strcmp( m_ciChat[i].msg, "") )
					{
						// 기린다
						switch( m_ciChat[i].team ){
								case WHITE:
									Self.BitmapName = "BMP\\UI\\white-big.bmp";
									break;
								case BLUE:
									Self.BitmapName = "BMP\\UI\\blue-big.bmp";
									break;
								case RED:
									Self.BitmapName = "BMP\\UI\\red-big.bmp";
									break;
								case BLACK:
									Self.BitmapName = "BMP\\UI\\black-big.bmp";
									break;
						}
						Self.destX = 25;
						Self.destY = 479;
						IOClass::GetDrawManager()->DrawOnLayer(3, Self);

						drawText( Font_Medium_Normal, m_ciChat[i].msg, 25 + 20, 479 + 11, RGB( 0, 0, 0 ) );
						m_ciChat[i].frame--;

						if( m_ciChat[i].frame < 0 )
						{
							strcpy( m_ciChat[i].msg, "" ); // 메세지 뿌릴만큼 뿌렸고 초기화
							m_ciChat[i].frame = VISIBLECHATFRAME;					
						}
					}
					findme = true;
					break;
				}
			}
		}
	}

	for( int i = 0, index = 0; i < 4; i++ )
	{
		if( m_cFBH->gamer[i] != NULL )
		{
            if( !m_cFBH->gamer[i]->IsYourSelf() ) // 내가아니여
			{
				if( strcmp(m_ciChat[i].msg, "") ) // 메세지가 있으면
				{
					// 기린다
					switch( m_ciChat[i].team ){
						case WHITE:
							Another.BitmapName = "BMP\\UI\\white-small.bmp";
							break;
						case BLUE:
							Another.BitmapName = "BMP\\UI\\blue-small.bmp";
							break;
						case RED:
							Another.BitmapName = "BMP\\UI\\red-small.bmp";
							break;
						case BLACK:
							Another.BitmapName = "BMP\\UI\\black-small.bmp";
							break;
					}
					switch( index )
					{
					case 0:
						Another.destX = 251;
						Another.destY = 492;
						break;
					case 1:
						Another.destX = 420;
						Another.destY = 492;
						break;
					case 2:
						Another.destX = 591;
						Another.destY = 492;
						break;
					default:
						Another.destX = 15;
						Another.destY = 15;
						break;
					}
					IOClass::GetDrawManager()->DrawOnLayer(2, Another);

					switch( index )
					{
					case 0:
						drawText( Font_Medium_Normal, m_ciChat[i].msg, 251 + 10, 492 + 7, RGB( 0, 0, 0 ) );
						break;
					case 1:
						drawText( Font_Medium_Normal, m_ciChat[i].msg, 420 + 10, 492 + 7, RGB( 0, 0, 0 ) );
						break;
					case 2:
						drawText( Font_Medium_Normal, m_ciChat[i].msg, 591 + 10, 492 + 7, RGB( 0, 0, 0 ) );
						break;
					default:
						drawText( Font_Medium_Normal, m_ciChat[i].msg, 15 + 10, 15, RGB( 0, 0, 0 ) );
						break;
					}
					m_ciChat[i].frame--;

					if( m_ciChat[i].frame < 0 )
					{
						strcpy( m_ciChat[i].msg, "" ); // 메세지 뿌릴만큼 뿌렸고 초기화
						m_ciChat[i].frame = VISIBLECHATFRAME;							
					}
				}
				index++;
			}
		}
	}
}

/* 죽는거는??? */
void CFBHGame::userRespon( int _index, bool allrespond )
{
	
}

void CFBHGame::init(void)
{
	m_dwFrame = 0;

	for( int i = 0 ; i < 4; i++ )
		ZeroMemory( (void*)&m_ciChat[i], sizeof( m_ciChat[i] ) );
}

void CFBHGame::ReadyStartDraw(void)
{

}
