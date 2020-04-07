#ifndef  __gameRoom_H_
#define  __gameRoom_H_
#include <Afx.h>
#include <AfxTempl.h>
#include "User.h"

class GameRoom
{
public:
	GameRoom();
	GameRoom(int _roomIndex, int _roomNo, CString& t,int lc);
	virtual	~GameRoom();
	void			enter(User* user);
	void			leave(User* user);
	int				getRoomIndex() { return roomIndex;}
	void			setRoomIndex(int _roomIndex) { roomIndex = _roomIndex;}
	int				getRoomNo() { return roomNo;}
	void			setRoomNo(int _roomNo) { roomNo = _roomNo;}
	CString			getTitle(){ return	title; }
	void			setTitle(CString _title){ title = _title; }

	bool			isWait() { return wait; }
	int				getGameMode() { return gameMode; }
	void			setGameMode( int _gameMode ) { gameMode = _gameMode; }
	int				getMapKind() { return mapKind; }
	void			setMapKind( int _mapKind ) { mapKind = _mapKind; }
	int				getUserCount() {return userCount; }
	void			setUserCount( int _userCount) {userCount = _userCount; }

private:
	int				roomIndex;			// 방 순서
	int				roomNo;				// 고유 번호
	CString			title;
	bool			wait;
	int				gameMode;
	int				mapKind;
	int				userCount;
	int				limitCount;
};

#endif