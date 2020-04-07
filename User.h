#ifndef  _User_H_
#define  _User_H_

#include <Afx.h>

class User
{
public:
	enum LOCATION{
	};

	User();
	virtual	~User();
	int		getSlotNo() { return slotNo; }
	void	setSlotNo(int _slotNo){ slotNo= _slotNo; }

	CString getAccount() { return account; }
	void	setAccount(CString _account) { account = _account; }
	
	bool	getMajor() { return major; }

	int		getTeam() { return team; }
	void	setTeam(int _team) { team = _team; }

	int		getCharacter() { return character; }
	void	setCharacter(int _character) { character = _character; }

	int		getLocation() { return location; }
	void	setLocation(int _location) { location = _location; }

	bool	getGameOver() { return gameOver; }
	void	setGameOver( bool _gameOver ) { gameOver = _gameOver; }

	bool	getReady() {return ready; }
	void	setReady(bool _ready) { ready = _ready; }

	DWORD	getScore() { return score; }
	void	setScore( DWORD _score ) { score = _score; }
	DWORD	getScoreRank() { return scoreRank; }
	void	setScoreRank( DWORD _scoreRank ) { scoreRank = _scoreRank; }
	DWORD	getKillSum() { return killSum; }
	void	setKillSum( DWORD _killSum ) { killSum = _killSum; }
	DWORD	getKillRank() { return killRank; }
	void	setKillRank( DWORD _killRank ) { killRank = _killRank; }
	bool    getEmpty() { return isEmpty; }
	void	setEmpty(bool _empty) { isEmpty = _empty; }

private:	
	int				slotNo;	
	CString			account;
	bool			ready;
	bool			major;
	int				team;		// 0: 팀 없음 1: 팀 1
	int				character;
	DWORD			score;
	DWORD			scoreRank;
	DWORD			killSum;
	DWORD			killRank;
	bool			isEmpty;
	bool			gameOver;
	int				location;	// 0: 로그 오프 1: 로그 인 2: 로비 3: 방 4: 게임 중 그외 값: 유령
};

#endif
