// _Gamer.h: interface for the C_Gamer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX__GAMER_H__1C251976_E1CC_41F1_8EBA_2FCAA665F784__INCLUDED_)
#define AFX__GAMER_H__1C251976_E1CC_41F1_8EBA_2FCAA665F784__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "_User.h"


class C_Gamer 
:	public	C_User
{
	bool m_bself;
	DWORD rstopframe;
	DWORD lstopframe;
	DWORD rrunframe;
	DWORD lrunframe;
	DWORD rdashframe;
	DWORD ldashframe;
	DWORD rjumpframe;
	DWORD ljumpframe;
	UserState* state;

	ofstream ofs;

	bool DiePreFrame;
public:
	bool NeedKillSync;
	typedef void (C_Gamer::*DrawFunc)( DrawInstruction& ins ) ;
	static DrawFunc drawFuncs[4] ;
	C_Gamer(C_User& in, bool IsUserSelf);
	C_Gamer(char* nam, float mas,char* sha, char* UserID, DWORD KindOfCharacter, bool IsUserSelf);
	virtual ~C_Gamer();

	void ChangeState(char msg);
	void PacketProcess(Packet& packet);
	void crash_obj_fix_down( C_obj* temp );
	void crash_obj_move_up( C_obj* temp);
	void crash_obj_move_down( C_obj* temp);
	void crash_obj_move_right( C_obj* temp);
	void crash_obj_move_left( C_obj* temp);

	void onDraw();
	void resetframe();

	void SheepDraw( DrawInstruction& ins );
	void ChickenDraw( DrawInstruction& ins );
	void TigerDraw( DrawInstruction& ins );
	void SnakeDraw( DrawInstruction& ins );
	void MeteorDraw();
	bool IsYourSelf() { return m_bself; };
	void GamerKilled();
	void KillOtherPlayer();

	int getState()	{	return state->getState();	}
	void SendState();
private:
	bool CheckInFire(C_Gamer* ob);
};

#endif // !defined(AFX__GAMER_H__1C251976_E1CC_41F1_8EBA_2FCAA665F784__INCLUDED_)
