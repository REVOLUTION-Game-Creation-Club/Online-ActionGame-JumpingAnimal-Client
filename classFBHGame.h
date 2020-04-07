#pragma once

#include "IOClass.h"
#include "_Gamer.h"
#include "_Meteor.h"
#include "C_physics.h"

class FBHGame
{
	C_physics* phy;

	C_Meteor* meteors;
	DrawInstruction background;
	bool m_bGameOver;
	DWORD	WinnerIndex;
	DWORD	BoardTime;

	DWORD	WinnerDrawCounter;
	DWORD	SyncTime;
	DWORD GameLimitTime;
public:
	char Accounts[4][20];
	int Char_Kinds[4];
	int Char_Color[4];
	bool IsInit;

	FBHGame(HWND hWnd, DxDisplay* DisPlay, unsigned int NumberOfLayer, ClientSocket& cs);
	~FBHGame(void);

	bool Process(bool i_isOnChatMode);
	void Draw();
	char ProcessKey();
	void packetParsing(Packet& packet);
	C_Gamer* gamer[4];

	void GameOver();

	void AddMeteor(DWORD Kind, int x, int y, int end_x, int end_y);
	void DelMeteor(C_Meteor* DelNode);

private:
	void CharDraw(C_obj* in);
	void BGDraw();	
	void MeteorDraw();
	void OverCheck();
};
