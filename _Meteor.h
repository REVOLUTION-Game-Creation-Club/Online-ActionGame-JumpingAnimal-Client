#pragma once
#include "_user.h"
#include "C_physics.h"

class C_Meteor :
	public C_User
{
	bool Falling;
	bool m_bWarning;

	bool Adjusted[4];

	DWORD ExistTime;

	DWORD DrawCounter;
	DWORD WarningCounter;
	DWORD BlinkCounter;

	DWORD MeteorUp, MeteorNumber;
	int UP[4];
	DWORD Down;
public:
	C_Meteor* nextMeteor;

	int start_x, start_y, end_x, end_y;

public:
	C_Meteor(int meteorKind, int start_pos_x, int start_pos_y, int end_pos_x, int end_pos_y);
	virtual ~C_Meteor(void);

	void crash_obj_move_up		( C_obj* temp );
	void crash_obj_move_down	( C_obj* temp );
	void crash_obj_move_left	( C_obj* temp );
	void crash_obj_move_right	( C_obj* temp );

	void crash_obj_fix_down		( C_obj* temp );

	void Crash_with_Meteor( C_obj* temp );

	bool IsDeleteTime();

	void onDraw();

	void process(C_physics* phy);
};
