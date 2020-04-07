#pragma once
#include "c_obj.h"
#include "UserState.h"
#define MAX_USER_ID_LENGTH 128
//#include "stdinclude.h"

class C_User :
	public C_obj
{
protected:
	char			m_ID[MAX_USER_ID_LENGTH];
	DWORD			m_KindOfCharacter;
	int			m_NumberOfKill;

public:
	C_User(char* nam, float mas,char* sha, char const* UserID, DWORD KindOfCharacter);
	~C_User(void);

	char const* GetID();
	void SetID(const char* _id) { 
		strcpy(m_ID,_id);
	}

	void SetKindOfChar(DWORD in)	{	m_KindOfCharacter = in;	}
	DWORD GetKindOfChar();
	int GetNumberOfKill();

	void CountNumberOfKill();
	void DecountNumberOfKill();

	virtual void ChangeState(char msg) {};
	virtual void crash_obj_fix_down( C_obj* temp ) {};
};