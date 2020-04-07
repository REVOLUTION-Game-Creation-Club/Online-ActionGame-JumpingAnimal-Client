// UserState.cpp: implementation of the UserState class.
//
//////////////////////////////////////////////////////////////////////

#include "UserState.h"
#include "Protocol.h"

//#define SERVER_AND_CLIENT_MODEL
#include "DxWindow/DirectX/DxSound.h"

extern DxSound*		g_dxSound;

UserState::UserState()
{
	
}

UserState::~UserState()
{

}

bool UserState::KeyDown(char input, char flag)
{
	if(input & flag)
		return true;
	return false;
}

void UserState::SendState(C_obj& ob, int state)
{
	Packet SendPacket(REQ_GAME);
	SendPacket << IOClass::GetServerGameID() << IOClass::GetServerUserID() << state << IOClass::GetServerRoomID();
	int x, y;
	ob.getxy(&x, &y);
	float v_x, v_y;
	ob.getvelocity(&v_x, &v_y);
	SendPacket << x << y << v_x << v_y;
	IOClass::GetClientSocket().sendPacket(SendPacket);
}

Die::Die()
{
	Time = USER_KILLED_TIME;
}

UserState* Die::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time == 0 )
	{
		ob.setxy((rand()%4 + 1)*100, 100);
		ob.impulsex(0);
		ob.impulsey(0);
		SendState(ob, ORSTOP);
		return new RFalling(ob);
	}

	return this;
}

UserState* ODie::ChangeUserState(char msg, C_obj& ob)
{
	return this;
};

/* 1 level right */
RStop::RStop(C_obj& ob)
{


}
RStop::~RStop()
{

}

UserState* RStop::ChangeUserState(char msg, C_obj& ob)
{
	if(KeyDown(msg, USER_STATE_LEFT))
	{
		ob.impulsex(LEFT_RUN_VAL);
		SendState(ob, OLRUN); return new LReadyDash(ob);
	}

	if(KeyDown(msg, USER_STATE_RIGHT))
	{
		ob.impulsex(RIGHT_RUN_VAL);
		SendState(ob, ORRUN); return new RReadyDash(ob);
	}

	if(KeyDown(msg, USER_STATE_UP))
	{
		g_dxSound->playWav( 9, false );
		ob.impulsey(JUMP_VAL);
		SendState(ob, ORJUMP);
		return new RFJump(ob);
	}

	if(KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey(FALL_VAL);
		SendState(ob, ORRUN);
	}

	return this;
}

RRun::RRun(C_obj& ob)
{

	

}
RRun::~RRun()
{

}

UserState* RRun::ChangeUserState(char msg, C_obj& ob)
{
	if(msg == 0)
	{
		SendState(ob, ORSTOP); return new RStop(ob);
	}

	if(KeyDown(msg, USER_STATE_LEFT))
	{
		ob.impulsex(LEFT_RUN_VAL);
		SendState(ob, OLRUN); return new LRun(ob);
	}

	if(KeyDown(msg, USER_STATE_UP))
	{
		g_dxSound->playWav( 9, false );
		ob.impulsey(JUMP_VAL);
		ob.impulsex( RIGHT_RUN_VAL );
		SendState(ob, ORJUMP); return new RFRun(ob);
	}


	ob.impulsex(RIGHT_RUN_VAL);

	if(KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey(FALL_VAL);
		SendState(ob, ORRUN);
	}
	return this;
}

RReadyDash::RReadyDash(C_obj& ob)
{
	Time = DASH_READY_TIME;

	

}

RReadyDash::~RReadyDash()
{

}

UserState* RReadyDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		SendState(ob, ORRUN); return new RRun(ob);
	}

	if(msg == 0)
	{
		SendState(ob, ORSTOP); return new RDelayDash(ob);
	}

	if(KeyDown(msg, USER_STATE_LEFT))
	{
		ob.impulsex(LEFT_RUN_VAL);
		SendState(ob, OLRUN); return new LRun(ob);
	}

	if(KeyDown(msg, USER_STATE_UP))
	{
		g_dxSound->playWav( 9, false );
		ob.impulsey(JUMP_VAL);
		SendState(ob, ORJUMP); return new RFRun(ob);
	}

	
	ob.impulsex(RIGHT_RUN_VAL);

	if(KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey(FALL_VAL);
		SendState(ob, ORRUN);
		return new RRun(ob);
	}
	return this;
}

RDelayDash::RDelayDash(C_obj& ob)
{
	Time = DASH_DELAY_TIME;

	

}

RDelayDash::~RDelayDash()
{
	
}

UserState* RDelayDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		SendState(ob, ORSTOP); return new RStop(ob);
	}

	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex(LEFT_RUN_VAL);
		SendState(ob, OLRUN); return new LRun(ob);
	}

	if( KeyDown(msg, USER_STATE_RIGHT) )
	{
		ob.impulsex(RIGHT_DASH_VAL);
		SendState(ob, ORDASH); return new RDash(ob);
	}

	if( KeyDown(msg, USER_STATE_UP) )
	{
		g_dxSound->playWav( 9, false );
		ob.impulsey(JUMP_VAL);
		SendState(ob, ORJUMP); return new RFRun(ob);
	}

	if(KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey(FALL_VAL);
		SendState(ob, ORRUN);
		return new RRun(ob);
	}

	return this;

}

RDash::RDash(C_obj& ob)
{
	Time = DASH_TIME;
}

RDash::~RDash()
{
	
}

UserState* RDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		SendState(ob, ORSTOP); return new RDelay(ob);
	}

	ob.impulsex(RIGHT_DASH_VAL);

	return this;
}

RDelay::RDelay(C_obj& ob)
{
	Time = DASH_DELAY_TIME;
}

RDelay::~RDelay()
{
	
}

UserState* RDelay::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		SendState(ob, ORSTOP); return new RStop(ob);
	}

	return this;
}

/* 1 level left */
LStop::LStop(C_obj& ob)
{

}

LStop::~LStop()
{

}

UserState* LStop::ChangeUserState(char msg, C_obj& ob)
{
	if(KeyDown(msg, USER_STATE_RIGHT))
	{
		ob.impulsex(RIGHT_RUN_VAL);
		SendState(ob, ORRUN); return new RReadyDash(ob);
	}

	if(KeyDown(msg, USER_STATE_LEFT))
	{
		ob.impulsex(LEFT_RUN_VAL);
		SendState(ob, OLRUN); return new LReadyDash(ob);
	}

	if(KeyDown(msg, USER_STATE_UP))
	{
		g_dxSound->playWav( 9, false );
		ob.impulsey(JUMP_VAL);
		SendState(ob, OLJUMP); 
		return new LFJump(ob);
	}

	if(KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey(FALL_VAL);
		SendState(ob, OLRUN);
	}

	return this;
}

LRun::LRun(C_obj& ob)
{

}
LRun::~LRun()
{

}

UserState* LRun::ChangeUserState(char msg, C_obj& ob)
{
	if(msg == 0)
	{
		SendState(ob, OLSTOP); return new LStop(ob);
	}

	if(KeyDown(msg, USER_STATE_RIGHT))
	{
		ob.impulsex(RIGHT_RUN_VAL);
		SendState(ob, ORRUN); return new RRun(ob);
	}

	if(KeyDown(msg, USER_STATE_UP))
	{
		g_dxSound->playWav( 9, false );
		ob.impulsey(JUMP_VAL);
		ob.impulsex(LEFT_RUN_VAL);
		SendState(ob, OLJUMP); return new LFRun(ob);
	}


	ob.impulsex(LEFT_RUN_VAL);

	if(KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey(FALL_VAL);
		SendState(ob, OLRUN);
	}
	return this;
}

LReadyDash::LReadyDash(C_obj& ob)
{
	Time = DASH_READY_TIME;
}

LReadyDash::~LReadyDash()
{
	
}

UserState* LReadyDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	
	if( Time < 0 )
	{
		SendState(ob, OLRUN); return new LRun(ob);
	}

	if(msg == 0)
	{
		SendState(ob, OLSTOP); return new LDelayDash(ob);
	}

	if(KeyDown(msg, USER_STATE_RIGHT))
	{
		ob.impulsex(RIGHT_RUN_VAL);
		SendState(ob, ORRUN); return new RRun(ob);
	}

	if(KeyDown(msg, USER_STATE_UP))
	{
		g_dxSound->playWav( 9, false );
		ob.impulsey(JUMP_VAL);
		SendState(ob, OLJUMP); return new LFRun(ob);
	}


	ob.impulsex(LEFT_RUN_VAL);

	if(KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey(FALL_VAL);
		SendState(ob, OLRUN);
		return new LRun(ob);
	}
	return this;
}

LDelayDash::LDelayDash(C_obj& ob)
{
	Time = DASH_DELAY_TIME;
}

LDelayDash::~LDelayDash()
{
	
}

UserState* LDelayDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		SendState(ob, OLSTOP); return new LStop(ob);
	}

	if( KeyDown(msg, USER_STATE_RIGHT) )
	{
		ob.impulsex(RIGHT_RUN_VAL);
		SendState(ob, ORSTOP); return new RRun(ob);
	}

	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex(LEFT_DASH_VAL);
		SendState(ob, OLDASH); return new LDash(ob);
	}

	if( KeyDown(msg, USER_STATE_UP) )
	{
		g_dxSound->playWav( 9, false );
		ob.impulsey(JUMP_VAL);
		SendState(ob, OLJUMP); return new LFRun(ob);
	}

	if(KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey(FALL_VAL);
		SendState(ob, OLRUN);
		return new LRun(ob);
	}

	return this;
}

LDash::LDash(C_obj& ob)
{	
	Time = DASH_TIME;
}

LDash::~LDash()
{	
	
}

UserState* LDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		SendState(ob, OLSTOP); return new LDelay(ob);
	}

	ob.impulsex(LEFT_DASH_VAL);

	return this;
}

LDelay::LDelay(C_obj& ob)
{
	Time = DASH_DELAY_TIME;
}

LDelay::~LDelay()
{
	
}

UserState* LDelay::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		SendState(ob, OLSTOP); return new LStop(ob);
	}

	return this;
}

/* 2 level right */
RFJump::RFJump(C_obj& ob)
{

}

RFJump::~RFJump()
{

}

UserState* RFJump::ChangeUserState(char msg, C_obj& ob)
{
	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex(LEFT_RUN_VAL);
		SendState(ob, OLJUMP);
		return new LFJump(ob);
	}

	if( msg == 0 )
	{
		SendState(ob, ORJUMP); 
		return new RReadySJump(ob);
	}

	if( KeyDown(msg, USER_STATE_RIGHT))
	{
		ob.impulsex( RIGHT_RUN_VAL );
		SendState(ob, ORJUMP); return new RFReadyDash(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, ORJUMP);
	}

	return this;
}

RFRun::RFRun(C_obj& ob)
{

}

RFRun::~RFRun()
{

}

UserState* RFRun::ChangeUserState(char msg, C_obj& ob)
{
    if( !KeyDown(msg, USER_STATE_UP) )
	{
		return new RRSRun(ob);
	}

	if( !KeyDown(msg, USER_STATE_RIGHT))
	{
		SendState(ob, ORJUMP);
		return new RFJump(ob);
	}

	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex(LEFT_RUN_VAL);
		SendState(ob, OLJUMP); return new LFReadyDash(ob);
	}

	float v_x, v_y;
	ob.getvelocity(&v_x, &v_y);
	if(v_x != RIGHT_RUN_VAL)
	{
		ob.impulsex( RIGHT_RUN_VAL );
		SendState(ob, ORJUMP);
	}
	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, ORJUMP);
	}
	return this;
}

RFReadyDash::RFReadyDash(C_obj& ob)
{
	Time = DASH_READY_TIME;
}

RFReadyDash::~RFReadyDash()
{

}

UserState* RFReadyDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		SendState(ob, ORJUMP); return new RFRun(ob);
	}

	if(!KeyDown(msg, USER_STATE_UP))
	{
		return new RRSRun(ob);
	}

	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex(LEFT_RUN_VAL);
		SendState(ob, OLJUMP); return new LFReadyDash(ob);
	}

	if( !KeyDown(msg, USER_STATE_RIGHT) )
	{
		SendState(ob, ORJUMP); return new RFDelayDash(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, ORJUMP);
		return new RFJump(ob);
	}
	return this;
}

RFDelayDash::RFDelayDash(C_obj& ob)
{
	Time = DASH_DELAY_TIME;
}

RFDelayDash::~RFDelayDash()
{

}

UserState* RFDelayDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		SendState(ob, ORJUMP);
		return new RFJump(ob);
	}

	if( !KeyDown(msg, USER_STATE_UP) )
	{	
		return new RRSRun(ob);
	}

	if( KeyDown(msg, USER_STATE_RIGHT) )
	{
		ob.impulsex(RIGHT_DASH_VAL);
		SendState(ob, ORDASH); return new RFDash(ob);
	}

	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex(LEFT_RUN_VAL);
		SendState(ob, OLJUMP); return new LFDelayDash(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, ORJUMP);
		return new RFJump(ob);
	}
	return this;
}

RFDash::RFDash(C_obj& ob)
{
	Time = DASH_TIME;
}

RFDash::~RFDash()
{
	
}

UserState* RFDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		ob.impulsex(RIGHT_RUN_VAL);
		SendState(ob, ORJUMP); return new RFalling(ob);	
	}

	return this;
}

RFalling::RFalling(C_obj& ob)
{

}

RFalling::~RFalling()
{

}

UserState* RFalling::ChangeUserState(char msg, C_obj& ob)
{
	return this;
}

/* 2 level left */
LFJump::LFJump(C_obj& ob)
{

}

LFJump::~LFJump()
{

}

UserState* LFJump::ChangeUserState(char msg, C_obj& ob)
{
	if( KeyDown(msg, USER_STATE_RIGHT) )
	{
		ob.impulsex(RIGHT_RUN_VAL);
		SendState(ob, ORJUMP);
		return new RFJump(ob);
	}

	if( msg == 0 )
	{
		SendState(ob, OLJUMP);
		return new LReadySJump(ob);
	}

	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex( LEFT_RUN_VAL);
		SendState(ob, OLJUMP); return new LFReadyDash(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, OLJUMP);
	}

	return this;
}

LFRun::LFRun(C_obj& ob)
{

	

}

LFRun::~LFRun()
{

}

UserState* LFRun::ChangeUserState(char msg, C_obj& ob)
{
	if( !KeyDown(msg, USER_STATE_UP) )
	{
		return new LRSRun(ob);
	}

	if( !KeyDown(msg, USER_STATE_LEFT) )
	{
		SendState(ob, OLJUMP);
		return new LFJump(ob);
	}

	if( KeyDown(msg, USER_STATE_RIGHT) )
	{
		ob.impulsex(RIGHT_RUN_VAL);
		SendState(ob, ORJUMP); return new RFRun(ob);
	}

	float v_x, v_y;
	ob.getvelocity(&v_x, &v_y);
	if(v_x != LEFT_RUN_VAL)
	{
		ob.impulsex( LEFT_RUN_VAL );
		SendState(ob, OLJUMP);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, OLJUMP);
	}
	return this;
}

LFReadyDash::LFReadyDash(C_obj& ob)
{
	Time = DASH_READY_TIME;

	

}

LFReadyDash::~LFReadyDash()
{
	
}

UserState* LFReadyDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		SendState(ob, OLJUMP); return new LFRun(ob);
	}

	if( !KeyDown(msg, USER_STATE_UP) )
	{
		return new LRSRun(ob);
	}

	if( KeyDown(msg, USER_STATE_RIGHT) )
	{
		ob.impulsex(RIGHT_RUN_VAL);
		SendState(ob, ORJUMP); return new RFReadyDash(ob);
	}

	if( !KeyDown(msg, USER_STATE_LEFT) )
	{
		SendState(ob, OLJUMP); return new LFDelayDash(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, OLJUMP);
		return new LFJump(ob);
	}
	return this;
}

LFDelayDash::LFDelayDash(C_obj& ob)
{
	Time = DASH_DELAY_TIME;
}

LFDelayDash::~LFDelayDash()
{
	
}

UserState* LFDelayDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		SendState(ob, OLJUMP);
		return new LFJump(ob);
	}

	if( !KeyDown(msg, USER_STATE_UP) )
	{
		return new LRSRun(ob);
	}

	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex(LEFT_DASH_VAL);
		SendState(ob, OLJDASH); return new LFDash(ob);
	}

	if( KeyDown(msg, USER_STATE_RIGHT) )
	{
		ob.impulsex(RIGHT_RUN_VAL);
		SendState(ob, ORJUMP); return new RFDelayDash(ob);
	}
	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, OLJUMP);
		return new LFJump(ob);
	}

	return this;
}

LFDash::LFDash(C_obj& ob)
{
	Time = DASH_TIME;

	

}

LFDash::~LFDash()
{
	
}

UserState* LFDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		ob.impulsex(LEFT_RUN_VAL);
		SendState(ob, OLJUMP); return new LFalling(ob);
	}

	return this;
}

LFalling::LFalling(C_obj& ob)
{

	

}

LFalling::~LFalling()
{

}

UserState* LFalling::ChangeUserState(char msg, C_obj& ob)
{
	return this;
}

/* 3 level right */
RReadySJump::RReadySJump(C_obj& ob)
{

	

}

RReadySJump::~RReadySJump()
{

}

UserState* RReadySJump::ChangeUserState(char msg, C_obj& ob)
{
	if( KeyDown(msg, USER_STATE_UP) )
	{
		g_dxSound->playWav( 6, false );
		ob.impulsey(JUMP_VAL);
		SendState(ob, ORJUMP);
		return new RSJump(ob);
	}

	if( KeyDown(msg, USER_STATE_RIGHT) )
	{
		ob.impulsex(RIGHT_RUN_VAL);
		SendState(ob, ORJUMP); return new RRSReadyDash(ob);
	}

	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex(LEFT_RUN_VAL);
		SendState(ob, OLJUMP); 		
		return new LReadySJump(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, ORJUMP);
	}
	return this;
}

RRSRun::RRSRun(C_obj& ob)
{

	

}

RRSRun::~RRSRun()
{

}

UserState* RRSRun::ChangeUserState(char msg, C_obj& ob)
{
	if( msg == 0 )
	{
		SendState(ob, ORJUMP); 
		return new RReadySJump(ob);
	}

	if( KeyDown( msg, USER_STATE_UP) )
	{
		g_dxSound->playWav( 6, false );
		ob.impulsey(JUMP_VAL);
		SendState(ob, ORJUMP); return new RSRun(ob);
	}

	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex(LEFT_RUN_VAL);
		SendState(ob, OLJUMP); return new LRSReadyDash(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, ORJUMP);
	}

	return this;
}

RRSReadyDash::RRSReadyDash(C_obj& ob)
{
	Time = DASH_READY_TIME;

	

}

RRSReadyDash::~RRSReadyDash()
{
	
}

UserState* RRSReadyDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		return new RRSRun(ob);
	}

	if( KeyDown(msg, USER_STATE_UP) )
	{
		g_dxSound->playWav( 6, false );
		ob.impulsey(JUMP_VAL);
		SendState(ob, ORJUMP); return new RSRun(ob);
	}

	if( msg == 0 )
	{
		SendState(ob, ORJUMP); return new RRSDelayDash(ob);
	}

	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex(LEFT_RUN_VAL);
		SendState(ob, OLJUMP); return new LRSReadyDash(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, ORJUMP);
		return new RReadySJump(ob);
	}
	return this;
}

RRSDelayDash::RRSDelayDash(C_obj& ob)
{
	Time = DASH_DELAY_TIME;
}

RRSDelayDash::~RRSDelayDash()
{
	
}

UserState* RRSDelayDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		SendState(ob, ORJUMP); 
		return new RReadySJump(ob);
	}

	if( KeyDown(msg, USER_STATE_UP) )
	{
		g_dxSound->playWav( 6, false );
		ob.impulsey(JUMP_VAL);
		SendState(ob, ORJUMP); return new RSRun(ob);
	}

	if( KeyDown(msg, USER_STATE_RIGHT) )
	{
		ob.impulsex(RIGHT_DASH_VAL);
		SendState(ob, ORJUMP);	return new RRSDash(ob);
	}

	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex(LEFT_RUN_VAL);
		SendState(ob, OLJUMP); return new LRSReadyDash(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, ORJUMP);
		return new RReadySJump(ob);
	}
	return this;
}

RRSDash::RRSDash(C_obj& ob)
{
	Time = DASH_TIME;

	

}

RRSDash::~RRSDash()
{
	
}

UserState* RRSDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		ob.impulsex(RIGHT_RUN_VAL);
		SendState(ob, ORJUMP); return new RFalling(ob);
	}

	return this;
}

/* 3 level left */
LReadySJump::LReadySJump(C_obj& ob)
{

	

}

LReadySJump::~LReadySJump()
{

}

UserState* LReadySJump::ChangeUserState(char msg, C_obj& ob)
{
	if( KeyDown(msg, USER_STATE_UP) )
	{
		g_dxSound->playWav( 6, false );
		ob.impulsey(JUMP_VAL);
		SendState(ob, OLJUMP); 
		return new LSJump(ob);
	}

	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex(LEFT_RUN_VAL);
		SendState(ob, OLJUMP); return new LRSReadyDash(ob);
	}

	if( KeyDown(msg, USER_STATE_RIGHT) )
	{
		ob.impulsex(RIGHT_RUN_VAL);
		SendState(ob, ORJUMP);
		return new RReadySJump(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, OLJUMP);
	}

	return this;
}

LRSRun::LRSRun(C_obj& ob)
{

	

}

LRSRun::~LRSRun()
{

}

UserState* LRSRun::ChangeUserState(char msg, C_obj& ob)
{
	if( msg == 0 )
	{
		SendState(ob, OLJUMP); 
		return new LReadySJump(ob);
	}

	if( KeyDown( msg, USER_STATE_UP) )
	{
		g_dxSound->playWav( 6, false );
		ob.impulsey(JUMP_VAL);
		SendState(ob, OLJUMP); return new LSRun(ob);
	}

	if( KeyDown(msg, USER_STATE_RIGHT) )
	{
		ob.impulsex(RIGHT_RUN_VAL);
		SendState(ob, ORJUMP); return new RRSReadyDash(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, OLJUMP);
	}

	return this;
}

LRSReadyDash::LRSReadyDash(C_obj& ob)
{
	Time = DASH_READY_TIME;

	

}

LRSReadyDash::~LRSReadyDash()
{
	
}

UserState* LRSReadyDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		return new LRSRun(ob);
	}

	if( KeyDown(msg, USER_STATE_UP) )
	{
		g_dxSound->playWav( 6, false );
		ob.impulsey(JUMP_VAL);
		SendState(ob, OLJUMP); return new LSRun(ob);
	}

	if( msg == 0 )
	{
		return new LRSDelayDash(ob);
	}

	if( KeyDown(msg, USER_STATE_RIGHT) )
	{
		ob.impulsex(RIGHT_RUN_VAL);
		SendState(ob, ORJUMP); return new RRSDelayDash(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, OLJUMP);
		return new LReadySJump(ob);
	}

	return this;
}

LRSDelayDash::LRSDelayDash(C_obj& ob)
{
	Time = DASH_DELAY_TIME;
}

LRSDelayDash::~LRSDelayDash()
{
	
}

UserState* LRSDelayDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		SendState(ob, OLJUMP); 
		return new LReadySJump(ob);
	}

	if( KeyDown(msg, USER_STATE_UP) )
	{
		g_dxSound->playWav( 6, false );
		ob.impulsey(JUMP_VAL);
		SendState(ob, OLJUMP); return new LSRun(ob);
	}

	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex(LEFT_DASH_VAL);
		SendState(ob, OLJDASH); return new LRSDash(ob);
	}

	if( KeyDown(msg, USER_STATE_RIGHT) )
	{
		ob.impulsex(RIGHT_RUN_VAL);
		SendState(ob, ORJUMP); return new RRSReadyDash(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, OLJUMP);
		return new LReadySJump(ob);
	}

	return this;
}

LRSDash::LRSDash(C_obj& ob)
{
	Time = DASH_TIME;

	

}

LRSDash::~LRSDash()
{
	
}

UserState* LRSDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		ob.impulsex(LEFT_RUN_VAL);
		SendState(ob, OLJUMP); return new LFalling(ob);
	}

	return this;
}

/* 4 level right */
RSJump::RSJump(C_obj& ob)
{

	

}

RSJump::~RSJump()
{

}

UserState* RSJump::ChangeUserState(char msg, C_obj& ob)
{
	if( KeyDown(msg, USER_STATE_RIGHT) )
	{
		ob.impulsex( RIGHT_RUN_VAL );
		SendState(ob, ORJUMP); return new RSReadyDash(ob);
	}

	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex( LEFT_RUN_VAL );
		SendState(ob, OLJUMP); return new LSReadyDash(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, ORJUMP);
	}

	return this;
}

RSRun::RSRun(C_obj& ob)
{

	

}

RSRun::~RSRun()
{

}
UserState* RSRun::ChangeUserState(char msg, C_obj& ob)
{
	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex( LEFT_RUN_VAL );
		SendState(ob, OLJUMP); return new LSReadyDash(ob);
	}
	
	if(!KeyDown(msg, USER_STATE_RIGHT))
	{
		SendState(ob, ORJUMP); 
		return new RSJump(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, ORJUMP);
	}

	return this;
}

RSReadyDash::RSReadyDash(C_obj& ob)
{
	Time = DASH_READY_TIME;

	

}

RSReadyDash::~RSReadyDash()
{
	
}

UserState* RSReadyDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		SendState(ob, ORJUMP); return new RSRun(ob);
	}

	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex( LEFT_RUN_VAL );
		SendState(ob, OLJUMP); return new LSReadyDash(ob);
	}

	if( !KeyDown(msg, USER_STATE_RIGHT) )
	{
		SendState(ob, ORJUMP); return new RSDelayDash(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, ORJUMP);
		return new RSJump(ob);
	}

	return this;
}

RSDelayDash::RSDelayDash(C_obj& ob)
{
	Time = DASH_DELAY_TIME;
}

RSDelayDash::~RSDelayDash()
{

}

UserState* RSDelayDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		SendState(ob, ORJUMP); 
		return new RSJump(ob);
	}

	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex( LEFT_RUN_VAL );
		SendState(ob, OLJUMP); return new LSReadyDash(ob);
	}

	if( KeyDown(msg, USER_STATE_RIGHT) )
	{
		ob.impulsex( RIGHT_DASH_VAL );
		SendState(ob, ORJDASH); return new RSDash(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, ORJUMP);
		return new RSJump(ob);
	}

	return this;
}

RSDash::RSDash(C_obj& ob)
{
	Time = DASH_TIME;

	

}

RSDash::~RSDash()
{
	
}

UserState* RSDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		ob.impulsex(RIGHT_RUN_VAL);
		SendState(ob, ORJUMP); return new RFalling(ob);
	}

	return this;
}

/* 4 level left */
LSJump::LSJump(C_obj& ob)
{

	

}
LSJump::~LSJump()
{

}

UserState* LSJump::ChangeUserState(char msg, C_obj& ob)
{
	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex( LEFT_RUN_VAL );
		SendState(ob, OLJUMP); return new LSReadyDash(ob);
	}

	if( KeyDown(msg, USER_STATE_RIGHT) )
	{
		ob.impulsex( RIGHT_RUN_VAL );
		SendState(ob, ORJUMP); return new RSReadyDash(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, OLJUMP);
	}

	return this;
}

LSRun::LSRun(C_obj& ob)
{

	

}

LSRun::~LSRun()
{

}
UserState* LSRun::ChangeUserState(char msg, C_obj& ob)
{
	if( KeyDown(msg, USER_STATE_RIGHT) )
	{
		ob.impulsex( RIGHT_RUN_VAL );
		SendState(ob, ORJUMP); return new RSReadyDash(ob);
	}

	if( !KeyDown(msg, USER_STATE_LEFT ) )
	{
		SendState(ob, OLJUMP); 
		return new LSJump(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, OLJUMP);
	}

	return this;
}

LSReadyDash::LSReadyDash(C_obj& ob)
{
	Time = DASH_READY_TIME;

	

}
LSReadyDash::~LSReadyDash()
{
	
}

UserState* LSReadyDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		SendState(ob, OLJUMP); return new LSRun(ob);
	}

	if( KeyDown(msg, USER_STATE_RIGHT) )
	{
		ob.impulsex( RIGHT_RUN_VAL );
		SendState(ob, ORJUMP); return new RSDelayDash(ob);
	}

	if( !KeyDown(msg, USER_STATE_LEFT) )
	{
		return new LSDelayDash(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, OLJUMP);
		return new LSJump(ob);
	}
	
	return this;
}

LSDelayDash::LSDelayDash(C_obj& ob)
{
	Time = DASH_DELAY_TIME;
}

LSDelayDash::~LSDelayDash()
{
	
}

UserState* LSDelayDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		SendState(ob, OLJUMP); 
		return new LSJump(ob);
	}

	if( KeyDown(msg, USER_STATE_RIGHT) )
	{
		ob.impulsex( RIGHT_RUN_VAL );
		SendState(ob, ORJUMP); return new RSReadyDash(ob);
	}

	if( KeyDown(msg, USER_STATE_LEFT) )
	{
		ob.impulsex( LEFT_DASH_VAL );
		SendState(ob, OLJDASH); return new LSDash(ob);
	}

	if( KeyDown(msg, USER_STATE_DOWN))
	{
		ob.impulsey( FALL_VAL );
		SendState(ob, OLJUMP);
		return new LSJump(ob);
	}

	return this;
}

LSDash::LSDash(C_obj& ob)
{
	Time = DASH_TIME;
}

LSDash::~LSDash()
{
	
}

UserState* LSDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
	{
		ob.impulsex(LEFT_RUN_VAL);
		SendState(ob, OLJUMP); return new LFalling(ob);
	}

	return this;
}

//Other User State
UserState* ORStop::ChangeUserState(char msg, C_obj& ob)
{
	return this;
}

UserState* OLStop::ChangeUserState(char msg, C_obj& ob)
{
	return this;
}

UserState* ORRun::ChangeUserState(char msg, C_obj& ob)
{
	ob.impulsex(RIGHT_RUN_VAL);
	return this;
}

UserState* OLRun::ChangeUserState(char msg, C_obj& ob)
{
	ob.impulsex(LEFT_RUN_VAL);
	return this;
}

UserState* ORJump::ChangeUserState(char msg, C_obj& ob)
{
	return this;
}

UserState* OLJump::ChangeUserState(char msg, C_obj& ob)
{
	return this;
}

UserState* ORDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if(Time < 0)
		return new ORStop;

	ob.impulsex(RIGHT_DASH_VAL);
	return this;
}

UserState* OLDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if(Time < 0)
		return new OLStop;

	ob.impulsex(LEFT_DASH_VAL);
	return this;
}

UserState* ORJDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if( Time < 0 )
		return new ORStop;//return new ORJump;

	return this;
}

UserState* OLJDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if(Time < 0)
		return new OLStop;//return new OLJump;

	return this;
}