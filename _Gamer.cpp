// _Gamer.cpp: implementation of the C_Gamer class.
//
//////////////////////////////////////////////////////////////////////

#include "_Gamer.h"
#include "state.h"
#include "protocol.h"
#include "DxWindow/DirectX/DxSound.h"

extern DxSound*		g_dxSound;

C_Gamer::DrawFunc C_Gamer::drawFuncs[4] = {
	C_Gamer::ChickenDraw,
	C_Gamer::TigerDraw,
	C_Gamer::SnakeDraw,
	C_Gamer::SheepDraw
} ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
C_Gamer::C_Gamer(C_User& in, bool IsUserSelf)
: C_User(in.getname(), in.getmass(), in.getshafe(), (char*)in.GetID(), in.GetKindOfChar())
{
	m_bself = IsUserSelf;
	if(IsUserSelf)
		state = new LStop(*this);
	else
		state = new OLStop();

#ifndef _DEBUG	
	ofs.open("log.txt");
#else
	ofs.open("log.txt", ios_base::_Nocreate);
	ofs.close();
#endif

	rstopframe = 0;
	lstopframe = 0;
	rrunframe = 0;
	lrunframe = 0;
	rdashframe = 0;
	ldashframe = 0;
	rjumpframe = 0;
	ljumpframe = 0;

	DiePreFrame = false;
	NeedKillSync = false;
}

C_Gamer::C_Gamer(char* nam, float mas,char* sha, char* UserID, DWORD KindOfCharacter, bool IsUserSelf)
: C_User(nam, mas, sha, UserID, KindOfCharacter)
{
	m_bself = IsUserSelf;
	if(IsUserSelf)
		state = new LStop(*this);
	else
		state = new OLStop();

#ifndef _DEBUG	
	ofs.open("log.txt");
#else
	ofs.open("log.txt", ios_base::_Nocreate);
	ofs.close();
#endif

	rstopframe = 0;
	lstopframe = 0;
	rrunframe = 0;
	lrunframe = 0;
	rdashframe = 0;
	ldashframe = 0;
	rjumpframe = 0;
	ljumpframe = 0;

	DiePreFrame = false;
	NeedKillSync = false;
}

C_Gamer::~C_Gamer()
{
	switch( state->getState() )
	{
	case DIE: delete (Die*)state; break;
	case ODIE: delete (ODie*)state; break;
	case RSTOP: delete (RStop*)state; break;
	case RRUN: delete (RRun*)state; break;
	case RREADYDASH: delete (RReadyDash*)state; break;
	case RDELAYDASH: delete (RDelayDash*)state; break;
	case RDASH: delete (RDash*)state; break;
	case RDELAY: delete (RDelay*)state; break;
	case RFJUMP: delete (RFJump*)state; break;
	case RFRUN: delete (RFRun*)state; break;
	case RFREADYDASH: delete (RFReadyDash*)state; break;
	case RFDELAYDASH: delete (RFDelayDash*)state; break;
	case RFDASH: delete (RFDash*)state; break;
	case RFALLING: delete (RFalling*)state; break;
	case RREADYSJUMP: delete (RReadySJump*)state; break;
	case RRSRUN: delete (RRSRun*)state; break;
	case RRSREADYDASH: delete (RRSReadyDash*)state; break;
	case RRSDELAYDASH: delete (RRSDelayDash*)state; break;
	case RRSDASH: delete (RRSDash*)state; break;
	case RSJUMP: delete (RSJump*)state; break;
	case RSRUN: delete (RSRun*)state; break;
	case RSREADYDASH: delete (RSReadyDash*)state; break;
	case RSDELAYDASH: delete (RSDelayDash*)state; break;
	case RSDASH: delete (RSDash*)state; break;

	case LSTOP: delete (LStop*)state; break;
	case LRUN: delete (LRun*)state; break;
	case LREADYDASH: delete (LReadyDash*)state; break;
	case LDELAYDASH: delete (LDelayDash*)state; break;
	case LDASH: delete (LDash*)state; break;
	case LDELAY: delete (LDelay*)state; break;
	case LFJUMP: delete (LFJump*)state; break;
	case LFRUN: delete (LFRun*)state; break;
	case LFREADYDASH: delete (LFReadyDash*)state; break;
	case LFDELAYDASH: delete (LFDelayDash*)state; break;
	case LFDASH: delete (LFDash*)state; break;
	case LFALLING: delete (LFalling*)state; break;
	case LREADYSJUMP: delete (LReadySJump*)state; break;
	case LRSRUN: delete (LRSRun*)state; break;
	case LRSREADYDASH: delete (LRSReadyDash*)state; break;
	case LRSDELAYDASH: delete (LRSDelayDash*)state; break;
	case LRSDASH: delete (LRSDash*)state; break;
	case LSJUMP: delete (LSJump*)state; break;
	case LSRUN: delete (LSRun*)state; break;
	case LSREADYDASH: delete (LSReadyDash*)state; break;
	case LSDELAYDASH: delete (LSDelayDash*)state; break;
	case LSDASH: delete (LSDash*)state; break;

	case ORSTOP: delete (ORStop*)state; break;
	case ORRUN: delete (ORRun*)state; break;
	case ORDASH: delete (ORDash*)state; break;
	case ORJUMP: delete (ORJump*)state; break;
	case ORJDASH: delete (ORJDash*)state; break;
	case OLSTOP: delete (OLStop*)state; break;
	case OLRUN: delete (OLRun*)state; break;
	case OLDASH: delete (OLDash*)state; break;
	case OLJUMP: delete (OLJump*)state; break;
	case OLJDASH: delete (OLJDash*)state; break;
	}
	state = NULL;

#ifndef _DEBUG
	ofs.close();
#endif
}

void C_Gamer::SheepDraw( DrawInstruction& ins )
{
	switch( state->getState() )	{
		case DIE:
		case ODIE:
			ins.BitmapName = "BMP\\Ghost.bmp";
			break;
		case LSTOP:
		case OLSTOP:		
			if( lstopframe >= 0 && lstopframe <= 200 )
			{
				ins.BitmapName = "BMP\\¾ç\\normal.bmp";
				lstopframe++;
			}
			else if( lstopframe > 200 && lstopframe <= 280 )
			{
				ins.BitmapName = "BMP\\¾ç\\WAIT\\wt1.bmp";
				lstopframe++;
			}
			else if( lstopframe > 280 && lstopframe <= 340 )
			{
				ins.BitmapName = "BMP\\¾ç\\WAIT\\wt2.bmp";
				lstopframe++;
			}
			else if( lstopframe > 340 && lstopframe <= 370 )
			{
				ins.BitmapName = "BMP\\¾ç\\WAIT\\wt3.bmp";
				lstopframe++;
			}
			else if( lstopframe > 370 && lstopframe <= 390 )
			{
				ins.BitmapName = "BMP\\¾ç\\WAIT\\wt4.bmp";
				lstopframe++;
			}

			if( lstopframe > 390 )
			{
				lstopframe = 201;
			}
			break;
		case RSTOP:
		case ORSTOP:
			if( rstopframe >= 0 && rstopframe <= 200 )
			{
				ins.BitmapName = "BMP\\¾ç\\rnormal.bmp";
				rstopframe++;
			}
			else if( rstopframe > 200 && rstopframe <= 280 )
			{
				ins.BitmapName = "BMP\\¾ç\\WAIT\\rwt1.bmp";
				rstopframe++;
			}
			else if( rstopframe > 280 && rstopframe <= 340 )
			{
				ins.BitmapName = "BMP\\¾ç\\WAIT\\rwt2.bmp";
				rstopframe++;
			}
			else if( rstopframe > 340 && rstopframe <= 370 )
			{
				ins.BitmapName = "BMP\\¾ç\\WAIT\\rwt3.bmp";
				rstopframe++;
			}
			else if( rstopframe > 370 && rstopframe <= 390 )
			{
				ins.BitmapName = "BMP\\¾ç\\WAIT\\rwt4.bmp";
				rstopframe++;
			}

			if( rstopframe > 390 )
			{
				rstopframe = 201;
			}
			break;
		case LRUN:
		case OLRUN:
		case LREADYDASH:
		case LDELAYDASH:
			if( lrunframe >= 0 && lrunframe <= 8 )
			{
				ins.BitmapName = "BMP\\¾ç\\WALK\\wk1.bmp";
				lrunframe++;
			}
			else if( lrunframe > 8 && lrunframe <= 16 )
			{
				ins.BitmapName = "BMP\\¾ç\\WALK\\wk2.bmp";
				lrunframe++;
			}
			else if( lrunframe > 16 && lrunframe <= 24 )
			{
				ins.BitmapName = "BMP\\¾ç\\WALK\\wk3.bmp";
				lrunframe++;
			}
			else if( lrunframe > 24 && lrunframe <= 40 )
			{
				ins.BitmapName = "BMP\\¾ç\\WALK\\wk4.bmp";
				lrunframe++;
			}
			else if( lrunframe > 40 && lrunframe <= 48 )
			{
				ins.BitmapName = "BMP\\¾ç\\WALK\\wk5.bmp";
				lrunframe++;
			}

			if( lrunframe > 48 )
			{
				lrunframe = 0;				
			}
			break;
		case RRUN:
		case ORRUN:
		case RREADYDASH:
		case RDELAYDASH:
			if( rrunframe >= 0 && rrunframe <= 8 )
			{
				ins.BitmapName = "BMP\\¾ç\\WALK\\rwk1.bmp";
				rrunframe++;				
			}
			else if( rrunframe > 8 && rrunframe <= 16 )
			{
				ins.BitmapName = "BMP\\¾ç\\WALK\\rwk2.bmp";
				rrunframe++;
			}
			else if( rrunframe > 16 && rrunframe <= 24 )
			{
				ins.BitmapName = "BMP\\¾ç\\WALK\\rwk3.bmp";
				rrunframe++;
			}
			else if( rrunframe > 24 && rrunframe <= 40 )
			{
				ins.BitmapName = "BMP\\¾ç\\WALK\\rwk4.bmp";
				rrunframe++;
			}
			else if( rrunframe > 40 && rrunframe <= 48 )
			{
				ins.BitmapName = "BMP\\¾ç\\WALK\\rwk5.bmp";
				rrunframe++;
			}

			if( rrunframe > 48 )
			{
				rrunframe = 0;				
			}
			break;
		case LDASH:		
		case LFDASH:
		case LRSDASH:
		case LSDASH:		
		case OLDASH:
		case OLJDASH:
		case LDELAY:
			if( ldashframe >= 0 && ldashframe <= 10 )
			{
				ins.BitmapName = "BMP\\¾ç\\DASH\\ds3.bmp";
				ldashframe++;
			}
			else if( ldashframe > 10 && ldashframe <= 19 )
			{
				ins.BitmapName = "BMP\\¾ç\\DASH\\ds5.bmp";
				ldashframe++;
			}

			if( ldashframe > 19 )
			{
				ldashframe = 0;
			}
			break;
		case RDASH:
		case RFDASH:		
		case RRSDASH:
		case RSDASH:
		case ORDASH:
		case ORJDASH:
		case RDELAY:
			if( rdashframe >= 0 && rdashframe <= 10 )
			{
				ins.BitmapName = "BMP\\¾ç\\DASH\\rds3.bmp";
				rdashframe++;
			}
			else if( rdashframe > 10 && rdashframe <= 19 )
			{
				ins.BitmapName = "BMP\\¾ç\\DASH\\rds5.bmp";
				rdashframe++;
			}

			if( rdashframe > 19 )
			{
				rdashframe = 0;
			}
			break;
		case LFJUMP:
		case LREADYSJUMP:
		case LSJUMP:
		case OLJUMP:
		case LFRUN:
		case LRSRUN:
		case LFREADYDASH:
		case LFDELAYDASH:
		case LRSREADYDASH:
		case LRSDELAYDASH:
		case LSREADYDASH:
		case LSDELAYDASH:
		case LFALLING:
		//case LDELAY:
		case LSRUN:
			if( ljumpframe >= 0 && ljumpframe <= 15 )
			{
				ins.BitmapName = "BMP\\¾ç\\JUMP\\jf1.bmp";
				ljumpframe++;
			}
			else if( ljumpframe > 15 && ljumpframe <= 20 )
			{
				ins.BitmapName = "BMP\\¾ç\\JUMP\\jf2.bmp";
				ljumpframe++;
			}
			else if( ljumpframe > 20 )
			{
				ins.BitmapName = "BMP\\¾ç\\JUMP\\jf3.bmp";
				ljumpframe++;
			}
			break;
		case RFJUMP:
		case RREADYSJUMP:
		case RSJUMP:
		case ORJUMP:
		case RFRUN:
		case RRSRUN:
		case RFREADYDASH:
		case RFDELAYDASH:
		case RRSREADYDASH:
		case RRSDELAYDASH:
		case RSREADYDASH:
		case RSDELAYDASH:
		case RFALLING:
		//case RDELAY:
		case RSRUN:
			if( rjumpframe >= 0 && rjumpframe <= 15 )
			{
				ins.BitmapName = "BMP\\¾ç\\JUMP\\rjf1.bmp";
				rjumpframe++;
			}
			else if( rjumpframe > 15 && rjumpframe <= 20 )
			{
				ins.BitmapName = "BMP\\¾ç\\JUMP\\rjf2.bmp";
				rjumpframe++;
			}
			else if( rjumpframe > 20 )
			{
				ins.BitmapName = "BMP\\¾ç\\JUMP\\rjf3.bmp";
				rjumpframe++;
			}
			break;
	}
}

void C_Gamer::ChickenDraw( DrawInstruction& ins )
{
	switch( state->getState() )	{
		case DIE:
		case ODIE:
			ins.BitmapName = "BMP\\Ghost.bmp";
			break;
		case LSTOP:
		case OLSTOP:		
			if( lstopframe >= 0 && lstopframe <= 200 )
			{
				ins.BitmapName = "BMP\\´ß\\normal.bmp";
				lstopframe++;
			}
			else if( lstopframe > 200 && lstopframe <= 280 )
			{
				ins.BitmapName = "BMP\\´ß\\WAIT\\wt1.bmp";
				lstopframe++;
			}
			else if( lstopframe > 280 && lstopframe <= 340 )
			{
				ins.BitmapName = "BMP\\´ß\\WAIT\\wt2.bmp";
				lstopframe++;
			}
			else if( lstopframe > 340 && lstopframe <= 370 )
			{
				ins.BitmapName = "BMP\\´ß\\WAIT\\wt3.bmp";
				lstopframe++;
			}
			else if( lstopframe > 370 && lstopframe <= 390 )
			{
				ins.BitmapName = "BMP\\´ß\\WAIT\\wt4.bmp";
				lstopframe++;
			}
			else if( lstopframe > 390 && lstopframe <= 410 )
			{
				ins.BitmapName = "BMP\\´ß\\WAIT\\wt5.bmp";
				lstopframe++;
			}
			else if( lstopframe > 410 && lstopframe <= 440 )
			{
				ins.BitmapName = "BMP\\´ß\\JUMP\\jf4.bmp";
				lstopframe++;
			}

			if( lstopframe > 440 )
			{
				lstopframe = 201;
			}
			break;
		case RSTOP:
		case ORSTOP:
			if( rstopframe >= 0 && rstopframe <= 200 )
			{
				ins.BitmapName = "BMP\\´ß\\rnormal.bmp";
				rstopframe++;
			}
			else if( rstopframe > 200 && rstopframe <= 280 )
			{
				ins.BitmapName = "BMP\\´ß\\WAIT\\rwt1.bmp";
				rstopframe++;
			}
			else if( rstopframe > 280 && rstopframe <= 340 )
			{
				ins.BitmapName = "BMP\\´ß\\WAIT\\rwt2.bmp";
				rstopframe++;
			}
			else if( rstopframe > 340 && rstopframe <= 370 )
			{
				ins.BitmapName = "BMP\\´ß\\WAIT\\rwt3.bmp";
				rstopframe++;
			}
			else if( rstopframe > 370 && rstopframe <= 390 )
			{
				ins.BitmapName = "BMP\\´ß\\WAIT\\rwt4.bmp";
				rstopframe++;
			}
			else if( rstopframe > 390 && rstopframe <= 410 )
			{
				ins.BitmapName = "BMP\\´ß\\WAIT\\rwt5.bmp";
				rstopframe++;
			}
			else if( rstopframe > 410 && rstopframe <= 440 )
			{
				ins.BitmapName = "BMP\\´ß\\JUMP\\rjf4.bmp";
				rstopframe++;
			}

			if( rstopframe > 440 )
			{
				rstopframe = 201;
			}
			break;
		case LRUN:
		case OLRUN:
		case LREADYDASH:
		case LDELAYDASH:
			if( lrunframe >= 0 && lrunframe <= 8 )
			{
				ins.BitmapName = "BMP\\´ß\\WALK\\wk1.bmp";
				lrunframe++;
			}
			else if( lrunframe > 8 && lrunframe <= 16 )
			{
				ins.BitmapName = "BMP\\´ß\\WALK\\wk2.bmp";
				lrunframe++;
			}
			else if( lrunframe > 16 && lrunframe <= 24 )
			{
				ins.BitmapName = "BMP\\´ß\\WALK\\wk3.bmp";
				lrunframe++;
			}
			else if( lrunframe > 24 && lrunframe <= 40 )
			{
				ins.BitmapName = "BMP\\´ß\\WALK\\wk4.bmp";
				lrunframe++;
			}

			if( lrunframe > 40 )
			{
				lrunframe = 0;				
			}
			break;
		case RRUN:
		case ORRUN:
		case RREADYDASH:
		case RDELAYDASH:
			if( rrunframe >= 0 && rrunframe <= 8 )
			{
				ins.BitmapName = "BMP\\´ß\\WALK\\rwk1.bmp";
				rrunframe++;				
			}
			else if( rrunframe > 8 && rrunframe <= 16 )
			{
				ins.BitmapName = "BMP\\´ß\\WALK\\rwk2.bmp";
				rrunframe++;
			}
			else if( rrunframe > 16 && rrunframe <= 24 )
			{
				ins.BitmapName = "BMP\\´ß\\WALK\\rwk3.bmp";
				rrunframe++;
			}
			else if( rrunframe > 24 && rrunframe <= 40 )
			{
				ins.BitmapName = "BMP\\´ß\\WALK\\rwk4.bmp";
				rrunframe++;
			}

			if( rrunframe > 40 )
			{
				rrunframe = 0;
			}
			break;
		case LDASH:		
		case LFDASH:
		case LRSDASH:
		case LSDASH:
		case OLDASH:
		case OLJDASH:
		case LDELAY: // 19  2, 3, 5, 6
			if( ldashframe >= 0 && ldashframe <= 4 )
			{
				ins.BitmapName = "BMP\\´ß\\DASH\\ds2.bmp";
				ldashframe++;
			}
			else if( ldashframe > 4 && ldashframe <= 9 )
			{
				ins.BitmapName = "BMP\\´ß\\DASH\\ds3.bmp";
				ldashframe++;
			}
			else if( ldashframe > 9 && ldashframe <= 14 )
			{
				ins.BitmapName = "BMP\\´ß\\DASH\\ds5.bmp";
				ldashframe++;
			}
			else if( ldashframe > 14 && ldashframe <= 20 )
			{
				ins.BitmapName = "BMP\\´ß\\DASH\\ds6.bmp";
				ldashframe++;
			}

			if( ldashframe > 20 )
			{
				ldashframe = 0;
			}
			break;
		case RDASH:
		case RFDASH:		
		case RRSDASH:
		case RSDASH:
		case ORDASH:
		case ORJDASH:
		case RDELAY:
			if( rdashframe >= 0 && rdashframe <= 4 )
			{
				ins.BitmapName = "BMP\\´ß\\DASH\\rds2.bmp";
				rdashframe++;
			}
			else if( rdashframe > 4 && rdashframe <= 9 )
			{
				ins.BitmapName = "BMP\\´ß\\DASH\\rds3.bmp";
				rdashframe++;
			}
			else if( rdashframe > 9 && rdashframe <= 14 )
			{
				ins.BitmapName = "BMP\\´ß\\DASH\\rds5.bmp";
				rdashframe++;
			}
			else if( rdashframe > 14 && rdashframe <= 20 )
			{
				ins.BitmapName = "BMP\\´ß\\DASH\\rds6.bmp";
				rdashframe++;
			}

			if( rdashframe > 20 )
			{
				rdashframe = 0;
			}
			break;
		case LFJUMP:
		case LREADYSJUMP:
		case LSJUMP:
		case OLJUMP:
		case LFRUN:
		case LRSRUN:
		case LFREADYDASH:
		case LFDELAYDASH:
		case LRSREADYDASH:
		case LRSDELAYDASH:
		case LSREADYDASH:
		case LSDELAYDASH:
		case LFALLING:
		//case LDELAY:
		case LSRUN:
			if( ljumpframe >= 0 && ljumpframe <= 15 )
			{
				ins.BitmapName = "BMP\\´ß\\JUMP\\jf1.bmp";
				ljumpframe++;
			}
			else if( ljumpframe > 15 && ljumpframe <= 20 )
			{
				ins.BitmapName = "BMP\\´ß\\JUMP\\jf2.bmp";
				ljumpframe++;
			}
			else if( ljumpframe > 20 )
			{
				ins.BitmapName = "BMP\\´ß\\JUMP\\jf3.bmp";
				ljumpframe++;
			}
			break;
		case RFJUMP:
		case RREADYSJUMP:
		case RSJUMP:
		case ORJUMP:
		case RFRUN:
		case RRSRUN:
		case RFREADYDASH:
		case RFDELAYDASH:
		case RRSREADYDASH:
		case RRSDELAYDASH:
		case RSREADYDASH:
		case RSDELAYDASH:
		case RFALLING:
		//case RDELAY:
		case RSRUN:
			if( rjumpframe >= 0 && rjumpframe <= 15 )
			{
				ins.BitmapName = "BMP\\´ß\\JUMP\\rjf1.bmp";
				rjumpframe++;
			}
			else if( rjumpframe > 15 && rjumpframe <= 20 )
			{
				ins.BitmapName = "BMP\\´ß\\JUMP\\rjf2.bmp";
				rjumpframe++;
			}
			else if( rjumpframe > 20 )
			{
				ins.BitmapName = "BMP\\´ß\\JUMP\\rjf3.bmp";
				rjumpframe++;
			}
			break;
	}
}

void C_Gamer::TigerDraw( DrawInstruction& ins )
{
	switch( state->getState() )	{
		case DIE:
		case ODIE:
			ins.BitmapName = "BMP\\Ghost.bmp";
			break;
		case LSTOP:
		case OLSTOP:		
			if( lstopframe >= 0 && lstopframe <= 200 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\normal.bmp";
				lstopframe++;
			}
			else if( lstopframe > 200 && lstopframe <= 280 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\WAIT\\wt1.bmp";
				lstopframe++;
			}
			else if( lstopframe > 280 && lstopframe <= 340 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\WAIT\\wt2.bmp";
				lstopframe++;
			}
			else if( lstopframe > 340 && lstopframe <= 370 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\WAIT\\wt3.bmp";
				lstopframe++;
			}
			else if( lstopframe > 370 && lstopframe <= 390 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\WAIT\\wt4.bmp";
				lstopframe++;
			}
			else if( lstopframe > 390 && lstopframe <= 430 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\WAIT\\wt5.bmp";
				lstopframe++;
			}
			else if( lstopframe > 430 && lstopframe <= 460 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\JUMP\\jf4.bmp";
				lstopframe++;
			}

			if( lstopframe > 460 )
			{
				lstopframe = 201;
			}
			break;
		case RSTOP:
		case ORSTOP:
			if( rstopframe >= 0 && rstopframe <= 200 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\rnormal.bmp";
				rstopframe++;
			}
			else if( rstopframe > 200 && rstopframe <= 280 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\WAIT\\rwt1.bmp";
				rstopframe++;
			}
			else if( rstopframe > 280 && rstopframe <= 340 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\WAIT\\rwt2.bmp";
				rstopframe++;
			}
			else if( rstopframe > 340 && rstopframe <= 370 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\WAIT\\rwt3.bmp";
				rstopframe++;
			}
			else if( rstopframe > 370 && rstopframe <= 390 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\WAIT\\rwt4.bmp";
				rstopframe++;
			}
			else if( rstopframe > 390 && rstopframe <= 430 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\WAIT\\rwt5.bmp";
				rstopframe++;
			}
			else if( rstopframe > 430 && rstopframe <= 460 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\JUMP\\rjf4.bmp";
				rstopframe++;
			}

			if( rstopframe > 460 )
			{
				rstopframe = 201;
			}
			break;
		case LRUN:
		case OLRUN:
		case LREADYDASH:
		case LDELAYDASH:
			if( lrunframe >= 0 && lrunframe <= 8 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\WALK\\wk1.bmp";
				lrunframe++;
			}
			else if( lrunframe > 8 && lrunframe <= 16 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\WALK\\wk2.bmp";
				lrunframe++;
			}
			else if( lrunframe > 16 && lrunframe <= 24 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\WALK\\wk3.bmp";
				lrunframe++;
			}
			else if( lrunframe > 24 && lrunframe <= 40 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\WALK\\wk4.bmp";
				lrunframe++;
			}

			if( lrunframe > 40 )
			{
				lrunframe = 0;				
			}
			break;
		case RRUN:
		case ORRUN:
		case RREADYDASH:
		case RDELAYDASH:
			if( rrunframe >= 0 && rrunframe <= 8 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\WALK\\rwk1.bmp";
				rrunframe++;				
			}
			else if( rrunframe > 8 && rrunframe <= 16 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\WALK\\rwk2.bmp";
				rrunframe++;
			}
			else if( rrunframe > 16 && rrunframe <= 24 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\WALK\\rwk3.bmp";
				rrunframe++;
			}
			else if( rrunframe > 24 && rrunframe <= 40 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\WALK\\rwk4.bmp";
				rrunframe++;
			}

			if( rrunframe > 40 )
			{
				rrunframe = 0;				
			}
			break;
		case LDASH:		
		case LFDASH:
		case LRSDASH:
		case LSDASH:		
		case OLDASH:
		case OLJDASH:
		case LDELAY:
			if( ldashframe >= 0 && ldashframe <= 2 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\DASH\\ds1.bmp";
				ldashframe++;
			}
			else if( ldashframe > 2 && ldashframe <= 4 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\DASH\\ds2.bmp";
				ldashframe++;
			}
			else if( ldashframe > 4 && ldashframe <= 12 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\DASH\\ds3.bmp";
				ldashframe++;
			}
			else if( ldashframe > 12 && ldashframe <= 15 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\DASH\\ds4.bmp";
				ldashframe++;
			}

			if( ldashframe > 15 )
			{
				ldashframe = 0;
			}
			break;
		case RDASH:
		case RFDASH:		
		case RRSDASH:
		case RSDASH:
		case ORDASH:
		case ORJDASH:
		case RDELAY:
			if( rdashframe >= 0 && rdashframe <= 2 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\DASH\\rds1.bmp";
				rdashframe++;
			}
			else if( rdashframe > 2 && rdashframe <= 4 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\DASH\\rds2.bmp";
				rdashframe++;
			}
			else if( rdashframe > 4 && rdashframe <= 12 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\DASH\\rds3.bmp";
				rdashframe++;
			}
			else if( rdashframe > 12 && rdashframe <= 15 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\DASH\\rds4.bmp";
				rdashframe++;
			}

			if( rdashframe > 15 )
			{
				rdashframe = 0;
			}
			break;
		case LFJUMP:
		case LREADYSJUMP:
		case LSJUMP:
		case OLJUMP:
		case LFRUN:
		case LRSRUN:
		case LFREADYDASH:
		case LFDELAYDASH:
		case LRSREADYDASH:
		case LRSDELAYDASH:
		case LSREADYDASH:
		case LSDELAYDASH:
		case LFALLING:
		//case LDELAY:
		case LSRUN:
			if( ljumpframe >= 0 && ljumpframe <= 15 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\JUMP\\jf1.bmp";
				ljumpframe++;
			}
			else if( ljumpframe > 15 && ljumpframe <= 20 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\JUMP\\jf2.bmp";
				ljumpframe++;
			}
			else if( ljumpframe > 20 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\JUMP\\jf3.bmp";
				ljumpframe++;
			}
			break;
		case RFJUMP:
		case RREADYSJUMP:
		case RSJUMP:
		case ORJUMP:
		case RFRUN:
		case RRSRUN:
		case RFREADYDASH:
		case RFDELAYDASH:
		case RRSREADYDASH:
		case RRSDELAYDASH:
		case RSREADYDASH:
		case RSDELAYDASH:
		case RFALLING:
		//case RDELAY:
		case RSRUN:
			if( rjumpframe >= 0 && rjumpframe <= 15 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\JUMP\\rjf1.bmp";
				rjumpframe++;
			}
			else if( rjumpframe > 15 && rjumpframe <= 20 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\JUMP\\rjf2.bmp";
				rjumpframe++;
			}
			else if( rjumpframe > 20 )
			{
				ins.BitmapName = "BMP\\³Ä¿ËÀÌ\\JUMP\\rjf3.bmp";
				rjumpframe++;
			}
			break;
	}
}

void C_Gamer::SnakeDraw( DrawInstruction& ins )
{
	switch( state->getState() )	{
		case DIE:
		case ODIE:
			ins.BitmapName = "BMP\\Ghost.bmp";
			break;
		case LSTOP:
		case OLSTOP:		
			if( lstopframe >= 0 && lstopframe <= 200 )
			{
				ins.BitmapName = "BMP\\¹ì\\normal.bmp";
				lstopframe++;
			}
			else if( lstopframe > 200 && lstopframe <= 280 )
			{
				ins.BitmapName = "BMP\\¹ì\\WAIT\\wt1.bmp";
				lstopframe++;
			}
			else if( lstopframe > 280 && lstopframe <= 340 )
			{
				ins.BitmapName = "BMP\\¹ì\\WAIT\\wt2.bmp";
				lstopframe++;
			}
			else if( lstopframe > 340 && lstopframe <= 370 )
			{
				ins.BitmapName = "BMP\\¹ì\\WAIT\\wt3.bmp";
				lstopframe++;
			}
			else if( lstopframe > 370 && lstopframe <= 390 )
			{
				ins.BitmapName = "BMP\\¹ì\\WAIT\\wt4.bmp";
				lstopframe++;
			}
			else if( lstopframe > 390 && lstopframe <= 420 )
			{
				ins.BitmapName = "BMP\\¹ì\\JUMP\\jf4.bmp";
				lstopframe++;
			}

			if( lstopframe > 420 )
			{
				lstopframe = 201;
			}
			break;
		case RSTOP:
		case ORSTOP:
			if( rstopframe >= 0 && rstopframe <= 200 )
			{
				ins.BitmapName = "BMP\\¹ì\\rnormal.bmp";
				rstopframe++;
			}
			else if( rstopframe > 200 && rstopframe <= 280 )
			{
				ins.BitmapName = "BMP\\¹ì\\WAIT\\rwt1.bmp";
				rstopframe++;
			}
			else if( rstopframe > 280 && rstopframe <= 340 )
			{
				ins.BitmapName = "BMP\\¹ì\\WAIT\\rwt2.bmp";
				rstopframe++;
			}
			else if( rstopframe > 340 && rstopframe <= 370 )
			{
				ins.BitmapName = "BMP\\¹ì\\WAIT\\rwt3.bmp";
				rstopframe++;
			}
			else if( rstopframe > 370 && rstopframe <= 390 )
			{
				ins.BitmapName = "BMP\\¹ì\\WAIT\\rwt4.bmp";
				rstopframe++;
			}
			else if( rstopframe > 390 && rstopframe <= 420 )
			{
				ins.BitmapName = "BMP\\¹ì\\JUMP\\rjf4.bmp";
				rstopframe++;
			}

			if( rstopframe > 420 )
			{
				rstopframe = 201;
			}
			break;
		case LRUN:
		case OLRUN:
		case LREADYDASH:
		case LDELAYDASH:
			if( lrunframe >= 0 && lrunframe <= 8 )
			{
				ins.BitmapName = "BMP\\¹ì\\WALK\\wk1.bmp";
				lrunframe++;
			}
			else if( lrunframe > 8 && lrunframe <= 16 )
			{
				ins.BitmapName = "BMP\\¹ì\\WALK\\wk2.bmp";
				lrunframe++;
			}
			else if( lrunframe > 16 && lrunframe <= 24 )
			{
				ins.BitmapName = "BMP\\¹ì\\WALK\\wk3.bmp";
				lrunframe++;
			}
			else if( lrunframe > 24 && lrunframe <= 30 )
			{
				ins.BitmapName = "BMP\\¹ì\\WALK\\wk4.bmp";
				lrunframe++;
			}

			if( lrunframe > 30 )
			{
				lrunframe = 0;				
			}
			break;
		case RRUN:
		case ORRUN:
		case RREADYDASH:
		case RDELAYDASH:
			if( rrunframe >= 0 && rrunframe <= 8 )
			{
				ins.BitmapName = "BMP\\¹ì\\WALK\\rwk1.bmp";
				rrunframe++;				
			}
			else if( rrunframe > 8 && rrunframe <= 16 )
			{
				ins.BitmapName = "BMP\\¹ì\\WALK\\rwk2.bmp";
				rrunframe++;
			}
			else if( rrunframe > 16 && rrunframe <= 24 )
			{
				ins.BitmapName = "BMP\\¹ì\\WALK\\rwk3.bmp";
				rrunframe++;
			}
			else if( rrunframe > 24 && rrunframe <= 30 )
			{
				ins.BitmapName = "BMP\\¹ì\\WALK\\rwk4.bmp";
				rrunframe++;
			}

			if( rrunframe > 30 )
			{
				rrunframe = 0;				
			}
			break;
		case LDASH:		
		case LFDASH:
		case LRSDASH:
		case LSDASH:		
		case OLDASH:
		case OLJDASH:
		case LDELAY:
			if( ldashframe >= 0 && ldashframe <= 2 )
			{
				ins.BitmapName = "BMP\\¹ì\\DASH\\ds1.bmp";
				ldashframe++;
			}
			else if( ldashframe > 2 && ldashframe <= 4 )
			{
				ins.BitmapName = "BMP\\¹ì\\DASH\\ds2.bmp";
				ldashframe++;
			}
			else if( ldashframe > 4 && ldashframe <= 12 )
			{
				ins.BitmapName = "BMP\\¹ì\\DASH\\ds3.bmp";
				ldashframe++;
			}
			else if( ldashframe > 12 && ldashframe <= 15 )
			{
				ins.BitmapName = "BMP\\¹ì\\DASH\\ds4.bmp";
				ldashframe++;
			}

			if( ldashframe > 15 )
			{
				ldashframe = 0;
			}
			break;
		case RDASH:
		case RFDASH:		
		case RRSDASH:
		case RSDASH:
		case ORDASH:
		case ORJDASH:
		case RDELAY:
			if( rdashframe >= 0 && rdashframe <= 2 )
			{
				ins.BitmapName = "BMP\\¹ì\\DASH\\rds1.bmp";
				rdashframe++;
			}
			else if( rdashframe > 2 && rdashframe <= 4 )
			{
				ins.BitmapName = "BMP\\¹ì\\DASH\\rds2.bmp";
				rdashframe++;
			}
			else if( rdashframe > 4 && rdashframe <= 12 )
			{
				ins.BitmapName = "BMP\\¹ì\\DASH\\rds3.bmp";
				rdashframe++;
			}
			else if( rdashframe > 12 && rdashframe <= 15 )
			{
				ins.BitmapName = "BMP\\¹ì\\DASH\\rds4.bmp";
				rdashframe++;
			}

			if( rdashframe > 15 )
			{
				rdashframe = 0;
			}
			break;
		case LFJUMP:
		case LREADYSJUMP:
		case LSJUMP:
		case OLJUMP:
		case LFRUN:
		case LRSRUN:
		case LFREADYDASH:
		case LFDELAYDASH:
		case LRSREADYDASH:
		case LRSDELAYDASH:
		case LSREADYDASH:
		case LSDELAYDASH:
		case LFALLING:
		//case LDELAY:
		case LSRUN:
			if( ljumpframe >= 0 && ljumpframe <= 15 )
			{
				ins.BitmapName = "BMP\\¹ì\\JUMP\\jf1.bmp";
				ljumpframe++;
			}
			else if( ljumpframe > 15 && ljumpframe <= 20 )
			{
				ins.BitmapName = "BMP\\¹ì\\JUMP\\jf2.bmp";
				ljumpframe++;
			}
			else if( ljumpframe > 20 )
			{
				ins.BitmapName = "BMP\\¹ì\\JUMP\\jf3.bmp";
				ljumpframe++;
			}
			break;
		case RFJUMP:
		case RREADYSJUMP:
		case RSJUMP:
		case ORJUMP:
		case RFRUN:
		case RRSRUN:
		case RFREADYDASH:
		case RFDELAYDASH:
		case RRSREADYDASH:
		case RRSDELAYDASH:
		case RSREADYDASH:
		case RSDELAYDASH:
		case RFALLING:
		//case RDELAY:
		case RSRUN:
			if( rjumpframe >= 0 && rjumpframe <= 15 )
			{
				ins.BitmapName = "BMP\\¹ì\\JUMP\\rjf1.bmp";
				rjumpframe++;
			}
			else if( rjumpframe > 15 && rjumpframe <= 20 )
			{
				ins.BitmapName = "BMP\\¹ì\\JUMP\\rjf2.bmp";
				rjumpframe++;
			}
			else if( rjumpframe > 20 )
			{
				ins.BitmapName = "BMP\\¹ì\\JUMP\\rjf3.bmp";
				rjumpframe++;
			}
			break;
	}
}

void C_Gamer::onDraw()
{
	DrawInstruction ins;

	(this->*drawFuncs[m_KindOfCharacter])(ins);
 // X =  x - XTERM Y = y - YTERM
	ins.destX = x - XTERM;
	ins.destY = y - YTERM;
	ins.sourceX = 0;
	ins.sourceY = 0;
	ins.sourceWidth = 50;
	ins.sourceHeight = 50;

	if( x - XTERM > ( 800 - XTERM ) )
		x = 0;
	else if( x - XTERM < ( 0 - XTERM ) )
		x = 800;

	if( (x - XTERM) >= 750 )
	{
		ins.sourceX = 0;
		ins.sourceWidth = 800 - (x - XTERM);
		ins.sourceY = 0;
		ins.sourceHeight = 50;
		// ¿À¸¥ÂÊº®¿¡ °É¸®´Â ºÎºÐ
		IOClass::GetDrawManager()->DrawOnLayer(1, ins);

		ins.destX = 0;
		ins.sourceX =  800 - (x - XTERM);
		ins.sourceWidth = 50 - ins.sourceX;
		// ¿ÞÂÊº®¿¡ °É¸®´Â ºÎºÐ
		IOClass::GetDrawManager()->DrawOnLayer(1, ins);
	}
	else if( (x - XTERM) < 0 )
	{
		ins.destX = (x - XTERM) + 800;
		ins.sourceX = 0;
		ins.sourceWidth = 800 - ins.destX;
		ins.sourceY = 0;
		ins.sourceHeight = 50;
		// ¿À¸¥ÂÊº®¿¡ °É¸®´Â ºÎºÐ
		IOClass::GetDrawManager()->DrawOnLayer(1, ins);

		ins.destX = (x - XTERM);
		ins.sourceX = -ins.destX;
		ins.sourceWidth = 50 - ins.sourceX;
		ins.destX = 0;
		// ¿ÞÂÊº®¿¡ °É¸®´Â ºÎºÐ
		IOClass::GetDrawManager()->DrawOnLayer(1, ins);
	}
	else IOClass::GetDrawManager()->DrawOnLayer(1, ins);


	if( y <= 0 )
	{
		ins.destY = 0;
		ins.sourceY = -y;
		ins.sourceHeight = 50 + y;
		IOClass::GetDrawManager()->DrawOnLayer(1, ins);
	}
}

void C_Gamer::ChangeState(char msg)
{
	UserState* temp = (UserState*)state->ChangeUserState(msg, *this);
	
#ifndef _DEBUG
	ofs << "State Number : " << state->getState();
	switch( temp->getState() )
	{
		case RFJUMP:
		case RREADYSJUMP:
		case RSJUMP:
		case ORJUMP:
		case RFRUN:
		case RRSRUN:
		case RFREADYDASH:
		case RFDELAYDASH:
		case RRSREADYDASH:
		case RRSDELAYDASH:
		case RSREADYDASH:
		case RSDELAYDASH:
		case RFALLING:
		//case RDELAY:
		case RSRUN:
			ofs << " RJUMP : " << rjumpframe << endl;
			break;
		case LFJUMP:
		case LREADYSJUMP:
		case LSJUMP:
		case OLJUMP:
		case LFRUN:
		case LRSRUN:
		case LFREADYDASH:
		case LFDELAYDASH:
		case LRSREADYDASH:
		case LRSDELAYDASH:
		case LSREADYDASH:
		case LSDELAYDASH:
		case LFALLING:
		//case LDELAY:
		case LSRUN:
			ofs << " LJUMP : " << ljumpframe << endl;
			break;
		case RDASH:
		case RFDASH:		
		case RRSDASH:
		case RSDASH:
		case ORDASH:
		case ORJDASH:
		case RDELAY:
			ofs << " RDASH : " << rdashframe << endl;
			break;
		case LDASH:
		case LFDASH:		
		case LRSDASH:
		case LSDASH:
		case OLDASH:
		case OLJDASH:
		case LDELAY:
			ofs << " LDASH : " << ldashframe << endl;
			break;
		default:
			ofs << endl;
			break;
	}
#endif

	if( temp != state )
	{
 		switch( state->getState() )
		{
		case DIE: delete (Die*)state;						resetframe();		break;

		case RSTOP:	delete (RStop*)state;					resetframe();		break;
		case RRUN: delete (RRun*)state;						break;//			rrunframe = 0;		break;
		case RREADYDASH: delete (RReadyDash*)state;			break;//								break;
		case RDELAYDASH: delete (RDelayDash*)state;			break;//								break;
		case RDASH: delete (RDash*)state;					break;//			rdashframe = 0;		break;
		case RDELAY: delete (RDelay*)state;					break;//								break;
		case RFJUMP: delete (RFJump*)state;					break;//			rjumpframe = 0;		break;
		case RFRUN: delete (RFRun*)state;					break;//			rrunframe = 0;		break;
		case RFREADYDASH: delete (RFReadyDash*)state;		break;//								break;
		case RFDELAYDASH: delete (RFDelayDash*)state;		break;//								break;
		case RFDASH: delete (RFDash*)state;					break;//			rdashframe = 0;		break;
		case RFALLING: delete (RFalling*)state;				break;//								break;
		case RREADYSJUMP: delete (RReadySJump*)state;		resetframe();		break;
		case RRSRUN: delete (RRSRun*)state;					resetframe();		break;
		case RRSREADYDASH: delete (RRSReadyDash*)state;		break;//								break;
		case RRSDELAYDASH: delete (RRSDelayDash*)state;		break;//								break;
		case RRSDASH: delete (RRSDash*)state;				break;//			rdashframe = 0;		break;
		case RSJUMP: delete (RSJump*)state;					resetframe();		break;
		case RSRUN: delete (RSRun*)state;					resetframe();		break;
		case RSREADYDASH: delete (RSReadyDash*)state;		resetframe();		break;
		case RSDELAYDASH: delete (RSDelayDash*)state;		break;//								break;
		case RSDASH: delete (RSDash*)state;					break;//			rdashframe = 0;		break;

		case LSTOP: delete (LStop*)state;					resetframe();		break;
		case LRUN: delete (LRun*)state;						break;//			lrunframe = 0;		break;
		case LREADYDASH: delete (LReadyDash*)state;			break;//								break;
		case LDELAYDASH: delete (LDelayDash*)state;			break;//								break;
		case LDASH: delete (LDash*)state;					break;//			ldashframe = 0;		break;
		case LDELAY: delete (LDelay*)state;					break;//								break;
		case LFJUMP: delete (LFJump*)state;					break;//			ljumpframe = 0;		break;
		case LFRUN: delete (LFRun*)state;					break;//			lrunframe = 0;		break;
		case LFREADYDASH: delete (LFReadyDash*)state;		break;//								break;
		case LFDELAYDASH: delete (LFDelayDash*)state;		break;//								break;
		case LFDASH: delete (LFDash*)state;					break;//			ldashframe = 0;		break;
		case LFALLING: delete (LFalling*)state;				break;//								break;
		case LREADYSJUMP: delete (LReadySJump*)state;		resetframe();		break;
		case LRSRUN: delete (LRSRun*)state;					resetframe();		break;
		case LRSREADYDASH: delete (LRSReadyDash*)state;		break;//								break;
		case LRSDELAYDASH: delete (LRSDelayDash*)state;		break;//								break;
		case LRSDASH: delete (LRSDash*)state;				break;//			ldashframe = 0;		break;
		case LSJUMP: delete (LSJump*)state;					resetframe();		break;
		case LSRUN: delete (LSRun*)state;					resetframe();		break;
		case LSREADYDASH: delete (LSReadyDash*)state;		resetframe();		break;
		case LSDELAYDASH: delete (LSDelayDash*)state;		break;//								break;
		case LSDASH: delete (LSDash*)state;					break;//			ldashframe = 0;		break;

		}
		state = temp;
	}
}

void C_Gamer::PacketProcess(Packet& packet)
{
	int StateNO;
	packet >> StateNO;

	if(IsYourSelf() && StateNO != ODIE)
		return;

	if(NeedKillSync && StateNO != ODIE)
		return;

	int pos_x, pos_y;
	packet >> pos_x >> pos_y;
	this->setxy(pos_x, pos_y);

	float v_x, v_y;
	packet >> v_x >> v_y;
	this->setvelocity(v_x, v_y);

	switch(StateNO)
	{ //need to change by Protocol Name
	case ORSTOP :	resetframe();
		delete (ORStop*)state;	state = new ORStop;											break;
	case ORRUN	:	resetframe();
		delete (ORRun*)state;	state = new ORRun;	/*this->impulsex(RIGHT_RUN_VAL);*/		break;
	case ORDASH	:	resetframe();
		delete (ORDash*)state;	state = new ORDash;	/*this->impulsex(RIGHT_DASH_VAL);*/		break;
	case ORJUMP	:	
					if( state->getState() == OLJUMP )
					{
						resetframe();
					}
					delete (ORJump*)state;	state = new ORJump;	
		/*this->impulsex(RIGHT_RUN_VAL);	this->impulsey(JUMP_VAL);*/									break;
	case ORJDASH:	resetframe();
		delete (ORJump*)state;	state = new ORJDash; /*this->impulsex(RIGHT_DASH_VAL);*/	break;

	case OLSTOP	:	resetframe();
		delete (OLStop*)state;	state = new OLStop;											break;
	case OLRUN	:	resetframe();
		delete (OLRun*)state;	state = new OLRun;	/*this->impulsex(LEFT_RUN_VAL);*/		break;
	case OLDASH	:	resetframe();
		delete (OLDash*)state;	state = new OLDash;	/*this->impulsex(LEFT_DASH_VAL);*/		break;
	case OLJUMP	:
					if( state->getState() == OLJUMP )
					{
						resetframe();
					}
					delete (OLJump*)state;	state = new OLJump;	
		/*this->impulsex(LEFT_RUN_VAL);	this->impulsey(JUMP_VAL);*/										break;
	case OLJDASH:	resetframe();
		delete (OLJDash*)state;	state = new OLJDash;/*this->impulsex(LEFT_DASH_VAL);*/		break;
	case ODIE	:	
		if(IsYourSelf() && (getState() != DIE && getState() != ODIE))
		{
			resetframe();
			delete (Die*)state;
			state = new Die;
			NeedKillSync = false;
		}
		else if(!IsYourSelf())
		{
			resetframe();
			delete (ODie*)state;
			state = new ODie;
			NeedKillSync = false;
		}
		break;
	}
}
void C_Gamer::crash_obj_move_right(C_obj* temp)
{
	C_User* ob = (C_User*)temp;

	if(strcmp(ob->getname(), "meteor") == 0)
		return;

	C_Gamer* gm = (C_Gamer*)temp;
	if(state->getState() == DIE
		|| state->getState() == ODIE
		|| gm->getState() == DIE
		|| gm->getState() == ODIE)
		return;
}

void C_Gamer::crash_obj_move_left(C_obj* temp)
{
	C_User* ob = (C_User*)temp;

	if(strcmp(ob->getname(), "meteor") == 0)
		return;

	C_Gamer* gm = (C_Gamer*)temp;
	if(state->getState() == DIE
		|| state->getState() == ODIE
		|| gm->getState() == DIE
		|| gm->getState() == ODIE)
		return;
}

void C_Gamer::crash_obj_move_up(C_obj* temp)
{
	/*
	C_User* ob = (C_User*)temp;

	if(strcmp(ob->getname(), "meteor") == 0)
		return;

	if(state->getState() == DIE
		|| state->getState() == ODIE)
		return;

	C_obj::crash_obj_move_up(temp);
	*/
}
void C_Gamer::SendState()
{
	int StateNo;

 	if(state->getState() == DIE && DiePreFrame)
		return;
	else
		DiePreFrame = false;

	switch(state->getState())
	{
	case RFALLING:
	case RDELAYDASH:
	case RSTOP:
	case RDELAY:
		StateNo = ORSTOP;
		break;
	case RRUN:
	case RREADYDASH:
		StateNo = ORRUN;
		break;
	case RDASH:
		StateNo = ORDASH;
		break;
	case RFRUN:
	case RFJUMP:
	case RFREADYDASH:
	case RFDELAYDASH:
	case RREADYSJUMP:
	case RRSRUN:
	case RRSREADYDASH:
	case RRSDELAYDASH:
	case RSJUMP:
	case RSRUN:
	case RSREADYDASH:
	case RSDELAYDASH:
		StateNo = ORJUMP;
		break;
	case RFDASH:
	case RRSDASH:
	case RSDASH:
		StateNo = ORJDASH;
		break;

	case LSTOP:
	case LDELAY:
	case LFALLING:
		StateNo = OLSTOP;
		break;
	case LRUN:
	case LREADYDASH:
	case LDELAYDASH:
		StateNo = OLRUN;
		break;
	case LDASH:
		StateNo = OLDASH;
		break;
	case LFRUN:
	case LFREADYDASH:
	case LFDELAYDASH:
	case LFJUMP:
	case LREADYSJUMP:
	case LRSRUN:
	case LRSREADYDASH:
	case LRSDELAYDASH:
	case LSJUMP:
	case LSRUN:
	case LSREADYDASH:
	case LSDELAYDASH:
		StateNo = OLJUMP;
		break;
	case LFDASH:
	case LRSDASH:
	case LSDASH:
		StateNo = OLJDASH;
		break;

	case DIE:
		StateNo = ODIE;
		DiePreFrame = true;
	}

	state->SendState(*this, StateNo);
}

void C_Gamer::crash_obj_move_down(C_obj* temp)
{
	C_User* ob = (C_User*)temp;

	if(strcmp(ob->getname(), "meteor") == 0)
		return;

	C_Gamer* gm = (C_Gamer*)temp;

	if(state->getState() == DIE
		|| state->getState() == ODIE
		|| gm->getState() == DIE
		|| gm->getState() == ODIE)
		return;
	if(IsYourSelf() && !gm->NeedKillSync)
	{
		KillOtherPlayer();
		gm->GamerKilled();
		return;
	}

	C_obj::crash_obj_move_down(temp);
}

void C_Gamer::KillOtherPlayer()
{
	int targetID = getname()[0] - '0' - 1;
	Packet KillPacket(REQ_GAME);
	KillPacket << IOClass::GetServerGameID() << IOClass::GetServerUserID() << KILL << IOClass::GetServerRoomID();
	IOClass::GetClientSocket().sendPacket(KillPacket);
}

void C_Gamer::GamerKilled()
{
	setvelocity(0.0, 0.0);
	//state->SendState(*this, ODIE);
	
	int targetID = getname()[0] - '0' - 1;
	Packet SendPacket(REQ_GAME);
	SendPacket << IOClass::GetServerGameID() << targetID << ODIE << IOClass::GetServerRoomID();
	int x, y;
	getxy(&x, &y);
	float v_x, v_y;
	getvelocity(&v_x, &v_y);
	SendPacket << x << y << v_x << v_y;
	IOClass::GetClientSocket().sendPacket(SendPacket);

	delete (Die*)state;
	if(IsYourSelf())
		state = new Die();
	else
		state = new ODie();

	NeedKillSync = true;
}

void C_Gamer::crash_obj_fix_down( C_obj* ob )
{
	if(state->StopState() && IsYourSelf())
	{
		if( state->getState() >= RSTOP && state->getState() <= RSDASH )
		{
			delete state;
			state = new RStop(*this);
			resetframe();
		}
		else if( state->getState() >= LSTOP && state->getState() <= LSDASH )
		{
			delete state;
			state = new LStop(*this);
			resetframe();
		}
	}

	if(state->getState() == DIE
		|| state->getState() == ODIE )
	{
		return;
	}
	C_obj::crash_obj_fix_down(ob);
}

void C_Gamer::resetframe()
{
	rstopframe = 0;
	lstopframe = 0;
	rrunframe = 0;
	lrunframe = 0;
	rdashframe = 0;
	ldashframe = 0;
	rjumpframe = 0;
	ljumpframe = 0;
}
