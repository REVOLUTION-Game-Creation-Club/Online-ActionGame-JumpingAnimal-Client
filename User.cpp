#include "stdafx.h"
#include "LobbyClient.h"
#include "User.h"

User::User()
{
	slotNo			=	0;	
	ready			=   false;
	major			=	false;
	team			=	0;		// 0: 팀 없음 1: 팀 1
	character		=	0;
	score			=	0;
	scoreRank		=	0;
	killSum			=	0;
	killRank		=	0;
	gameOver		=	false;
	isEmpty			=	true;
	location		=	0;	// 0: 로그 오프 1: 로그 인 2: 로비 3: 방 4: 게임 중 그외 값: 유령
}

User::~User()
{
}
