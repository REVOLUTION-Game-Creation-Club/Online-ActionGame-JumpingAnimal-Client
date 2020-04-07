//////////////////////////////////////////////////////////////////////
//
// UserState.h: interface for the UserState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERSTATE_H__FCF797E7_BD74_410E_81CB_38E4FAB97F79__INCLUDED_)
#define AFX_USERSTATE_H__FCF797E7_BD74_410E_81CB_38E4FAB97F79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "_User.h"
// 점프 하고 밑에 5칸
// 앞뒤 속도 약간 빠르게

const char USER_STATE_UP	=	0x1;
const char USER_STATE_RIGHT	=	0x2;
const char USER_STATE_DOWN	=	0x4;
const char USER_STATE_LEFT	=	0x8;
/*
const int DASH_READY_TIME	=	7;
const int DASH_DELAY_TIME	=	7;
const int DELAY_INFINITE	=	0x7fffffff;
const int DASH_TIME			=	10;
const int SJUMP_READY_TIME	=	7;
const int USER_KILLED_TIME	=	50;

const float RIGHT_RUN_VAL	=	5.0f;
const float LEFT_RUN_VAL	=	-5.0f;
const float	RIGHT_DASH_VAL	=	11.0f;
const float LEFT_DASH_VAL	=	-11.0f;
const float JUMP_VAL		=	15.0f;
const float FALL_VAL		=	-25;*/

const int DASH_READY_TIME	=	7;
const int DASH_DELAY_TIME	=	2;
const int DELAY_INFINITE	=	0x7fffffff;
const int DASH_TIME			=	10;
const int SJUMP_READY_TIME	=	7;
const int USER_KILLED_TIME	=	50;

const float RIGHT_RUN_VAL	=	5.0f;
const float LEFT_RUN_VAL	=	-5.0f;
const float	RIGHT_DASH_VAL	=	11.0f;
const float LEFT_DASH_VAL	=	-11.0f;
const float JUMP_VAL		=	15.0f;
const float FALL_VAL		=	-25;

#include "state.h"

class UserState
{
protected:
	int Time;
public:
	UserState();
	virtual ~UserState();

	virtual UserState* ChangeUserState(char msg, C_obj& ob) = 0;

	bool KeyDown(char input, char flag);
	virtual bool StopState() {	return true;	}
	virtual int getState() = 0;

	void SendState(C_obj& ob, int state);
};

class Die : public UserState
{
public:
	Die();

	bool StopState()	{	return false;	}
	UserState* ChangeUserState(char msg, C_obj& ob);
	int getState()	{	return	DIE;	}
};

class ODie : public UserState
{
public:
	bool StopState()	{	return false;	}
	UserState* ChangeUserState(char msg, C_obj& ob);
	int getState()	{	return ODIE;	}
};

class RStop : public UserState
{
public:
	RStop(C_obj& ob);
	virtual ~RStop();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return false;	}
	int getState() { return RSTOP; }
};

class LStop : public UserState
{
public:
	LStop(C_obj& ob);
	virtual ~LStop();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return false;	}
	int getState() { return LSTOP; }
};

class RRun : public UserState
{
public:
	RRun(C_obj& ob);
	virtual ~RRun();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return false;	}
	int getState() { return RRUN; }
};

class LRun : public UserState
{
public:
	LRun(C_obj& ob);
	virtual ~LRun();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return false;	}
	int getState() { return LRUN; }
};

class RReadyDash : public UserState
{
public:
	RReadyDash(C_obj& ob);
	virtual ~RReadyDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return false;	}
	int getState() { return RREADYDASH; }
};

class LReadyDash : public UserState
{
public:
	LReadyDash(C_obj& ob);
	virtual ~LReadyDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return false;	}
	int getState() { return LREADYDASH; }
};

class RDelayDash : public UserState
{
public:
	RDelayDash(C_obj& ob);
	virtual ~RDelayDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return false;	}
	int getState() { return RDELAYDASH; }
};

class LDelayDash : public UserState
{
public:
	LDelayDash(C_obj& ob);
	virtual ~LDelayDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return false;	}
	int getState() { return LDELAYDASH; }
};

class RDash : public UserState
{
public:
	RDash(C_obj& ob);
	virtual ~RDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return false;	}
	int getState() { return RDASH; }
};

class LDash : public UserState
{
public:
	LDash(C_obj& ob);
	virtual ~LDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return false;	}
	int getState() { return LDASH; }
};

class RDelay : public UserState
{
public:
	RDelay(C_obj& ob);
	virtual ~RDelay();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return false;	}
	int getState() { return RDELAY; }
};

class LDelay : public UserState
{
public:
	LDelay(C_obj& ob);
	virtual ~LDelay();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return false;	}
	int getState() { return LDELAY; }
};

class RFJump : public UserState
{
public:
	RFJump(C_obj& ob);
	virtual ~RFJump();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return RFJUMP; }
};

class LFJump : public UserState
{
public:
	LFJump(C_obj& ob);
	virtual ~LFJump();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return LFJUMP; }
};

class RFRun : public UserState
{	
public:
	RFRun(C_obj& ob);
	virtual ~RFRun();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return RFRUN; }
};

class LFRun : public UserState
{
public:
	LFRun(C_obj& ob);
	virtual ~LFRun();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return LFRUN; }
};

class RFReadyDash : public UserState
{
public:
	RFReadyDash(C_obj& ob);
	virtual ~RFReadyDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return RFREADYDASH; }
};

class LFReadyDash : public UserState
{
public:
	LFReadyDash(C_obj& ob);
	virtual ~LFReadyDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return LFREADYDASH; }
};

class RFDelayDash : public UserState
{
public:
	RFDelayDash(C_obj& ob);
	virtual ~RFDelayDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return RFDELAYDASH; }
};

class LFDelayDash : public UserState
{
public:
	LFDelayDash(C_obj& ob);
	virtual ~LFDelayDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return LFDELAYDASH; }
};

class RFDash : public UserState
{
public:
	RFDash(C_obj& ob);
	virtual ~RFDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return RFDASH; }
};

class LFDash : public UserState
{
public:
	LFDash(C_obj& ob);
	virtual ~LFDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return LFDASH; }
};

class RFalling : public UserState
{
public:
	RFalling(C_obj& ob);
	virtual ~RFalling();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return RFALLING; }
};

class LFalling : public UserState
{
public:
	LFalling(C_obj& ob);
	virtual ~LFalling();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return LFALLING; }
};

class RReadySJump : public UserState
{
public:
	RReadySJump(C_obj& ob);
	virtual ~RReadySJump();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return RREADYSJUMP; }
};

class LReadySJump : public UserState
{
public:
	LReadySJump(C_obj& ob);
	virtual ~LReadySJump();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return LREADYSJUMP; }
};

class RRSRun : public UserState
{
public:
	RRSRun(C_obj& ob);
	virtual ~RRSRun();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return RRSRUN; }
};

class LRSRun : public UserState
{
public:
	LRSRun(C_obj& ob);
	virtual ~LRSRun();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return LRSRUN; }
};

class RRSReadyDash : public UserState
{
public:
	RRSReadyDash(C_obj& ob);
	virtual ~RRSReadyDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return RRSREADYDASH; }
};

class LRSReadyDash : public UserState
{
public:
	LRSReadyDash(C_obj& ob);
	virtual ~LRSReadyDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return LRSREADYDASH; }
};

class RRSDelayDash : public UserState
{
public:
	RRSDelayDash(C_obj& ob);
	virtual ~RRSDelayDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return RRSDELAYDASH; }
};

class LRSDelayDash : public UserState
{
public:
	LRSDelayDash(C_obj& ob);
	virtual ~LRSDelayDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return LRSDELAYDASH; }
};

class RRSDash : public UserState
{
public:
	RRSDash(C_obj& ob);
	virtual ~RRSDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return RRSDASH; }
};

class LRSDash : public UserState
{
public:
	LRSDash(C_obj& ob);
	virtual ~LRSDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return LRSDASH; }
};

class RSJump : public UserState
{
public:
	RSJump(C_obj& ob);
	virtual ~RSJump();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return RSJUMP; }
};

class LSJump : public UserState
{
public:
	LSJump(C_obj& ob);
	virtual ~LSJump();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return LSJUMP; }
};

class RSRun : public UserState
{
public:
	RSRun(C_obj& ob);
	virtual ~RSRun();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return RSRUN; }
};

class LSRun : public UserState
{
public:
	LSRun(C_obj& ob);
	virtual ~LSRun();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return LSRUN; }
};

class RSReadyDash : public UserState
{
public:
	RSReadyDash(C_obj& ob);
	virtual ~RSReadyDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return RSREADYDASH; }
};

class LSReadyDash : public UserState
{
public:
	LSReadyDash(C_obj& ob);
	virtual ~LSReadyDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return LSREADYDASH; }
};

class RSDelayDash : public UserState
{
public:
	RSDelayDash(C_obj& ob);
	virtual ~RSDelayDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return RSDELAYDASH; }
};

class LSDelayDash : public UserState
{
public:
	LSDelayDash(C_obj& ob);
	virtual ~LSDelayDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return LSDELAYDASH; }
};

class RSDash : public UserState
{
public:
	RSDash(C_obj& ob);
	virtual ~RSDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return RSDASH; }
};

class LSDash : public UserState
{
public:
	LSDash(C_obj& ob);
	virtual ~LSDash();

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() {	return true;	}
	int getState() { return LSDASH; }
};

//Other User State
class ORStop : public UserState
{
public:
	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() { return false; }
	int getState()	{ return ORSTOP; }
};

class OLStop : public UserState
{
public:
	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() { return false; }
	int getState()	{ return OLSTOP; }
};

class ORRun : public UserState
{
public:
	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() { return false; }
	int getState()	{ return ORRUN; }
};

class OLRun : public UserState
{
public:
	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() { return false; }
	int getState()	{ return OLRUN; }
};

class ORDash : public UserState
{
public:
	ORDash()	{	Time = DASH_TIME;	}

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() { return false; }
	int getState()	{ return ORDASH; }
};

class OLDash : public UserState
{
public:
	OLDash()	{	Time = DASH_TIME;	}

	UserState* ChangeUserState(char msg, C_obj& ob);

	bool StopState() { return false; }
	int getState()	{ return OLDASH; }
};

class ORJump : public UserState
{
public:
	UserState* ChangeUserState(char msg, C_obj& ob);

	int getState() { return ORJUMP;}
};

class OLJump : public UserState
{
public:
	UserState* ChangeUserState(char msg, C_obj& ob);

	int getState() { return OLJUMP;}
};

class ORJDash : public UserState
{
public:
	ORJDash()	{	Time = DASH_TIME;	}

	UserState* ChangeUserState(char msg, C_obj& ob);

	int getState() { return ORJDASH;}
};

class OLJDash : public UserState
{
public:
	OLJDash()	{	Time = DASH_TIME;	}

	UserState* ChangeUserState(char msg, C_obj& ob);

	int getState() { return OLJDASH;}
};
#endif