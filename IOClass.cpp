#include "ioclass.h"

CDrawManager* IOClass::drawman;
KeyBoard* IOClass::key;
ClientSocket* IOClass::clientsocket;
int IOClass::GAME_ID;
int IOClass::ROOM_ID;
int IOClass::USER_ID;

HRESULT IOClass::init(HWND hwnd, DxDisplay* diplay, unsigned int NumberOfLayer, ClientSocket* cs)
{
	drawman = new CDrawManager(diplay, NumberOfLayer);
	key = new KeyBoard();

	clientsocket = cs;

	return key->InitDirectInput(hwnd);	
}

CDrawManager* IOClass::GetDrawManager()
{
	return drawman;
}

KeyBoard* IOClass::GetKeyBoard()
{
	return key;
}

void IOClass::Free()
{
	key->FreeDirectInput();
	delete key;

	delete drawman;
}

ClientSocket& IOClass::GetClientSocket()
{
	return *clientsocket;
}

void IOClass::SetServerIDs(int gameID, int roomID, int userID)
{
	GAME_ID = gameID;
	ROOM_ID = roomID;
	USER_ID = userID;
}

int IOClass::GetServerRoomID()
{
	return ROOM_ID;
}

int IOClass::GetServerGameID()
{
	return GAME_ID;
}

int IOClass::GetServerUserID()
{
	return USER_ID;
}