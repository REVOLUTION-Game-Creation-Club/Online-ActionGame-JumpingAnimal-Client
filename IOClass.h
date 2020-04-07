#pragma once

#include "DrawManager.h"
#include "KeyBoard.h"
#include "ClientSocket.h"

class IOClass
{
	static CDrawManager* drawman;
	static KeyBoard* key;
	static ClientSocket* clientsocket;
	static int GAME_ID, ROOM_ID, USER_ID;
public:
	static HRESULT init(HWND hwnd, DxDisplay* diplay, unsigned int NumberOfLayer, ClientSocket* cs);
	static void Free();
	static CDrawManager* GetDrawManager();
	static KeyBoard* GetKeyBoard();
	static ClientSocket& GetClientSocket();

	static void SetServerIDs(int gameID, int roomID, int userID);
	static int GetServerGameID();
	static int GetServerRoomID();
	static int GetServerUserID();
};
