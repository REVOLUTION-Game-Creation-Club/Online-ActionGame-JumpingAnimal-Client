#include "stdafx.h"
#include "LobbyClient.h"
#include "User.h"

User::User()
{
	slotNo			=	0;	
	ready			=   false;
	major			=	false;
	team			=	0;		// 0: �� ���� 1: �� 1
	character		=	0;
	score			=	0;
	scoreRank		=	0;
	killSum			=	0;
	killRank		=	0;
	gameOver		=	false;
	isEmpty			=	true;
	location		=	0;	// 0: �α� ���� 1: �α� �� 2: �κ� 3: �� 4: ���� �� �׿� ��: ����
}

User::~User()
{
}
