#include "classFBHGame.h"
#include "DxWindow/Windows/FontCatalog.h"
#include "Protocol.h"
#include <fstream>
#include <string>
using namespace std;
#define FIRE 0
#define ICE 1
#define FIGUREOFENDKILL 10

using namespace std;

FBHGame::FBHGame(HWND hWnd, DxDisplay* DisPlay, unsigned int NumberOfLayer, ClientSocket& cs )
{
	IOClass::init(hWnd, DisPlay, NumberOfLayer, &cs);
        
	phy = new C_physics(0, 1.0f);
	
	int istage = 0;

	switch( istage )
	{
	case FIRE:
		background.BitmapName = "BMP\\¹è°æ\\stage-fire.bmp";
		background.sourceWidth = 800;
		background.sourceHeight = 600;
		background.destX = 0;
		background.destY = 0;
		background.sourceX = 0;
		background.sourceY = 0;
		break;
	case ICE:
		background.BitmapName = "BMP\\¹è°æ\\stage-fire.bmp";
		background.sourceWidth = 800;
		background.sourceHeight = 600;
		background.destX = 0;
		background.destY = 0;
		background.sourceX = 0;
		background.sourceY = 0;
		break;
	}
	for( int i = 0 ; i < 4 ; i++)
		gamer[i] = NULL;

	meteors = NULL;
	WinnerIndex = -1;
	IsInit = false;
	m_bGameOver = false;
}

FBHGame::~FBHGame(void)
{
	for(int i = 0; i < 4; i++)
	{
		if( gamer[i] != NULL)
		{
			delete gamer[i];
			gamer[i] = NULL;
		}
	}

	phy->fixed_obj_remove();
	delete phy;
	IOClass::Free();

	while(meteors)
	{
		C_Meteor* temp = meteors->nextMeteor;
		delete meteors;
		meteors = temp;
	}
}

void FBHGame::GameOver()
{
	for(int i = 0; i < 4; i++)
	{
		if( gamer[i] != NULL)
		{
			delete gamer[i];
			gamer[i] = NULL;
		}
	}

	phy->fixed_obj_remove();
	
	while(meteors)
	{
		C_Meteor* temp = meteors->nextMeteor;
		delete meteors;
		meteors = temp;
	}
	m_bGameOver = false;
	BoardTime = 0;
	WinnerDrawCounter = 0;
	WinnerIndex = -1;
}

void FBHGame::packetParsing(Packet& packet )
{
	DWORD targetGamer;
	packet >> targetGamer;
	if( targetGamer >= 4)
	{
		if( targetGamer == INIT_GAMESTATE && !IsInit)
		{
			DWORD MAPNO;
			packet >> MAPNO;
			char MapName[20];
			sprintf((char*)MapName, "Config\\m%d.txt", MAPNO);
			ifstream Map(MapName);
			while(!Map.eof())
			{
				int TILENO, pos_x, pos_y;
				Map >> TILENO >> pos_x >> pos_y;
				if(TILENO == -1)
					break;
				sprintf(MapName, "TileNo%d", TILENO);
				C_obj* tempTile = new C_obj(MapName, 1.0f, "square");
				tempTile->setedge(pos_x*25, pos_y*25, 25, 25);
				phy->fixed_obj_include(tempTile);
			}
			Map.close();

			char NameBuf[20];
			DWORD pos_x = 0, pos_y = 0;
			int UserCount;
			packet >> UserCount;

			for( int i = 0 ; i < UserCount ; i++)
			{
				if(Char_Kinds[i] == -1)
					continue;
				packet >> NameBuf >> pos_x >> pos_y;
				if( i == IOClass::GetServerUserID())
					gamer[i] = new C_Gamer(NameBuf, 1.0f, "square", Accounts[i], Char_Kinds[i], true);
				else
					gamer[i] = new C_Gamer(NameBuf, 1.0f, "square", Accounts[i], Char_Kinds[i], false);

				gamer[i]->setedge(pos_x + XTERM, pos_y + YTERM, COLLISIONW, COLLISIONH);
				phy->movable_obj_include(gamer[i]);
			}

			DWORD GameID;
			packet >> GameID;
			IOClass::SetServerIDs(GameID, IOClass::GetServerRoomID(), IOClass::GetServerUserID());
			IsInit = true;
			GameLimitTime = GetTickCount() + 180000;
		}
		if(targetGamer == METEOR_GOOD || targetGamer == METEOR_BAD)
		{
			int p_x, p_y, e_x, e_y;
			packet >> p_x >> p_y >> e_x >> e_y;
			AddMeteor(targetGamer, p_x, p_y, e_x, e_y);
		}
		if(targetGamer == TIME_OVER)
		{
			int i;
			for(i = 0 ; i < 4 ; i++)
				if(gamer[i] != NULL)
				{
					Packet DiePacket;
					int x, y;
					gamer[i]->getxy(&x, &y);
					DiePacket << ORSTOP << x << y << (int)0 << (int)0;
					gamer[i]->PacketProcess(DiePacket);
				}
			m_bGameOver = true;
			BoardTime = GetTickCount();
			WinnerIndex = -1;
		}
		if(targetGamer == KILL)
		{
			packet >> targetGamer;
			gamer[targetGamer]->CountNumberOfKill();
		}
	}
	else 
		gamer[targetGamer]->PacketProcess(packet);
}

void FBHGame::AddMeteor(DWORD Kind, int x, int y, int end_x, int end_y)
{
	C_Meteor* temp = new C_Meteor(Kind, x, y, end_x, end_y);

	temp->nextMeteor = meteors;
	meteors = temp;
}

void FBHGame::DelMeteor(C_Meteor* DelNode)
{
	phy->movable_obj_exclude(DelNode);

	if(meteors == DelNode)
		meteors = meteors->nextMeteor;
	else
	{
		C_Meteor* tempMt;
		for(tempMt = meteors ; !(tempMt->nextMeteor == DelNode || tempMt->nextMeteor == NULL) ; tempMt = tempMt->nextMeteor);
		if(tempMt->nextMeteor == NULL)
			return;
		tempMt->nextMeteor = DelNode->nextMeteor;
	}
}

bool FBHGame::Process( bool i_isOnChatMode )
{
	if(IsInit){
		if(m_bGameOver)
		{
			if(GetTickCount() - BoardTime < 5000)
			{
				BGDraw();
				phy->fixed_obj_draw();

				MeteorDraw();
				
				int i;
				for( i = 0; i < 4; i++ )
					if(gamer[i] != NULL)
						CharDraw(gamer[i]);

				DrawInstruction ins;
				ins.BitmapName = "BMP\\UI\\inter-result.bmp";
				ins.sourceX = 0;
				ins.sourceY = 0;
				ins.sourceWidth = 360;
				ins.sourceHeight = 240;
				ins.destX = 240;
				ins.destY = 180;
				IOClass::GetDrawManager()->DrawOnLayer(3, ins);

				ins.sourceX = 0;
				ins.sourceY = 0;
				ins.sourceWidth = 50;
				ins.sourceHeight = 50;

				for( i = 0 ; i < 4 ; i++ )
				{
					char FileName[40];
					if(gamer[i] != NULL)
					{
						switch(gamer[i]->GetKindOfChar())
						{
						case CHICKEN:
							sprintf(FileName, "BMP\\´ß\\normal.bmp");
							break;
						case TIGER:
							sprintf(FileName, "BMP\\³Ä¿ËÀÌ\\normal.bmp");
							break;
						case SNAKE:
							sprintf(FileName, "BMP\\¹ì\\normal.bmp");
							break;
						case SHEEP:
							sprintf(FileName, "BMP\\¾ç\\normal.bmp");
							break;
						}

						switch(i)
						{
						case 0:
							ins.destX = 278;
							ins.destY = 268;
							break;
						case 1:
							ins.destX = 433;
							ins.destY = 268;
							break;
						case 2:
							ins.destX = 278;
							ins.destY = 342;
							break;
						case 3:
							ins.destX = 433;
							ins.destY = 342;
							break;
						}

						ins.BitmapName = FileName;
						IOClass::GetDrawManager()->DrawOnLayer(3, ins);
					}
				}

				if(WinnerIndex != -1 && (++WinnerDrawCounter % 24) / 12)
				{
					ins.BitmapName = "BMP\\UI\\win.bmp";
					ins.sourceX = 0;
					ins.sourceY = 0;
					ins.sourceWidth = 50;
					ins.sourceHeight = 30;

					switch(WinnerIndex)
					{
					case 0:
						ins.destX = 257;
						ins.destY = 242;
						break;
					case 1:
						ins.destX = 412;
						ins.destY = 242;
						break;
					case 2:
						ins.destX = 257;
						ins.destY = 316;
						break;
					case 3:
						ins.destX = 412;
						ins.destY = 316;
						break;
					}

					IOClass::GetDrawManager()->DrawOnLayer(3, ins);
				}

				IOClass::GetDrawManager()->DrawScreen();

				for( int i = 0 ; i < 4 ; i++)
					if(gamer[i] != NULL)
						switch(i)
						{
						case 0:	IOClass::GetDrawManager()->getDisplay()->drawText(FontCatalog::handleFont[4], gamer[i]->GetID(), 338, 268, true, RGB(65, 165, 140), RGB(0, 0, 0));
							break;
						case 1:	IOClass::GetDrawManager()->getDisplay()->drawText(FontCatalog::handleFont[4], gamer[i]->GetID(), 494, 268, true, RGB(65, 165, 140), RGB(0, 0, 0));
							break;
						case 2:	IOClass::GetDrawManager()->getDisplay()->drawText(FontCatalog::handleFont[4], gamer[i]->GetID(), 338, 342, true, RGB(65, 165, 140), RGB(0, 0, 0));
							break;
						case 3: IOClass::GetDrawManager()->getDisplay()->drawText(FontCatalog::handleFont[4], gamer[i]->GetID(), 494, 342, true, RGB(65, 165, 140), RGB(0, 0, 0));
							break;
						}
			}
			else
			{
				Packet sendPacket(REQ_GAME_QUIT);
				sendPacket << IOClass::GetServerGameID();
				IOClass::GetClientSocket().sendPacket(sendPacket);
					IsInit = false;
			}
		}

		else
		{
		char temp = ProcessKey();

		if( !i_isOnChatMode )
			for( int i = 0 ; i < 4 ; i++)
				if(gamer[i] != NULL)
					gamer[i]->ChangeState(temp);

		C_Meteor* tempMt = meteors, *nextMt;
		while(tempMt)
		{
			nextMt = tempMt->nextMeteor;
			tempMt->process(phy);
			if(tempMt->IsDeleteTime())
			{
				DelMeteor(tempMt);
				delete tempMt;
			}
			tempMt = nextMt;
		}

		if(GetTickCount() - SyncTime > 500)
		{
			gamer[IOClass::GetServerUserID()]->SendState();
		}

		phy->step();
		
		Draw();
			OverCheck();
		}
	}

	return true;
}
void FBHGame::OverCheck()
{
	int i;
	bool GameOverState = false;
	for(i = 0 ; i < 4 ; i++)
		if(gamer[i] != NULL)
			if(gamer[i]->GetNumberOfKill() == FIGUREOFENDKILL)
			{
				WinnerIndex = i;
				GameOverState = true;
			}

	if(GameOverState)
	{
		
		for(i = 0 ; i < 4 ; i++)
			if(gamer[i] != NULL)
			{
				Packet DiePacket;
				int x, y;
				gamer[i]->getxy(&x, &y);
				DiePacket << ORSTOP << x << y << (int)0 << (int)0;
				gamer[i]->PacketProcess(DiePacket);
			}
		m_bGameOver = true;
		BoardTime = GetTickCount();
	}
}

void FBHGame::CharDraw(C_obj* in)
{
	in->onDraw();
}

void FBHGame::BGDraw()
{
	IOClass::GetDrawManager()->DrawOnLayer(0, background);
}

void FBHGame::Draw()
{
	BGDraw();
	phy->fixed_obj_draw();

	MeteorDraw();
	
	int i;

	for(i = 0; i < 4; i++ )
	{
		if(gamer[i] != NULL)
		{
			CharDraw(gamer[i]);		
		}
	}
	DrawInstruction ins;
	char FileName[30];
	ins.sourceX = 0;
	ins.sourceY = 0;
	ins.sourceWidth = 26;
	ins.sourceHeight = 29;
	DWORD Time = GameLimitTime - GetTickCount(), ch = 3;
	Time /= 1000;
	if(Time >= 100 && Time < 1000) ch = 0;
	else if(Time >= 10 && Time < 100) ch = 1;
	else if(Time < 10) ch = 2;

	switch(ch)
	{
	case 0:
		sprintf(FileName, "BMP\\counter\\%d.bmp", Time/100);
		ins.BitmapName = FileName;
		ins.destX = 623;
		ins.destY = 61;
		IOClass::GetDrawManager()->DrawOnLayer(2, ins);
	case 1:
		sprintf(FileName, "BMP\\counter\\%d.bmp", Time%100/10);
		ins.BitmapName = FileName;
		ins.destX = 652;
		ins.destY = 61;
		IOClass::GetDrawManager()->DrawOnLayer(2, ins);
	case 2:
		sprintf(FileName, "BMP\\counter\\%d.bmp", Time%10);
		ins.BitmapName = FileName;
		ins.destX = 681;
		ins.destY = 61;
		IOClass::GetDrawManager()->DrawOnLayer(2, ins);
	default:
		break;
	}

	ins.BitmapName = "BMP\\counter\\second.bmp";
	ins.sourceX = 0;
	ins.sourceY = 0;
	ins.sourceWidth = 32;
	ins.sourceHeight = 32;
	ins.destX = 714;
	ins.destY = 59;
	IOClass::GetDrawManager()->DrawOnLayer(2, ins);

	IOClass::GetDrawManager()->DrawScreen();

	for( i = 0 ; i < 4 ; i++)
		if(gamer[i] != NULL)
		{
			IOClass::GetDrawManager()->getDisplay()->drawText( FontCatalog::handleFont[6], gamer[i]->GetID(), gamer[i]->x + 25 - ( ( strlen(gamer[i]->GetID()) / 2 ) * 10 ) - XTERM, gamer[i]->y - 17, true, RGB(65, 165, 140), RGB(0,0,0) );
			IOClass::GetDrawManager()->getDisplay()->drawText( FontCatalog::handleFont[6], gamer[i]->GetID(), gamer[i]->x + 25 - ( ( strlen(gamer[i]->GetID()) / 2 ) * 10 ) - 800 - XTERM, gamer[i]->y - 17, true, RGB(65, 165, 140), RGB(0,0,0) );
		}
}

void FBHGame::MeteorDraw()
{
	C_Meteor* temp = meteors;
	while(temp)
	{
		temp->onDraw();
		temp = temp->nextMeteor;
	}
}

char FBHGame::ProcessKey()
{
	char status = 0;
	IOClass::GetKeyBoard()->UpdateInputState( NULL );

	if( !IOClass::GetKeyBoard()->GetDevice() )
		return -1;

	if ( KEYDOWN( IOClass::GetKeyBoard()->diks, DIK_RIGHT ) )
	{
		status |= 0x02;
	}
	else if( KEYDOWN( IOClass::GetKeyBoard()->diks, DIK_LEFT ) )
	{
		status |= 0x08;
	}
	if( KEYDOWN( IOClass::GetKeyBoard()->diks, DIK_UP) || KEYDOWN( IOClass::GetKeyBoard()->diks, DIK_SPACE) )
	{	
		status |= 0x01;
	}
	else if( KEYDOWN( IOClass::GetKeyBoard()->diks, DIK_DOWN) )
	{
		status |= 0x04;
	}

	return status;
}