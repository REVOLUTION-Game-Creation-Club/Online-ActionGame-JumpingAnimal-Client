#include "stdafx.h"
#include "LobbyClient.h"
#include "Room.h"

GameRoom::GameRoom()
{
	roomIndex		=	-1;
	roomNo			=	-1;
	gameMode		=	0;
	mapKind			=	0;
	userCount		=	0;
	limitCount		=	4;
}

GameRoom::GameRoom(int _roomIndex, int _roomNo, CString& t, int lc )
{
	roomIndex		=  _roomIndex;
	roomNo			=	_roomNo;
	wait			=	false;
	gameMode		=	0;
	mapKind			=	0;
	userCount		=	0;
	limitCount		=	lc;

	title = t;
}

GameRoom::~GameRoom()
{
}
