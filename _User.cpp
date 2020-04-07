#include "_user.h"
#include <windows.h>

C_User::C_User(char* nam, float mas,char* sha, char const* UserID, DWORD KindOfCharacter)
: C_obj(nam, mas, sha)
{	
	strcpy(m_ID, UserID);

	//중요하게 바뀜
	m_KindOfCharacter = KindOfCharacter;
	m_NumberOfKill = 0;
}

C_User::~C_User(void)
{

}

char const* C_User::GetID()
{
	return m_ID;
}

DWORD C_User::GetKindOfChar()
{
	return m_KindOfCharacter;
}

int C_User::GetNumberOfKill()
{
	return m_NumberOfKill;
}

void C_User::CountNumberOfKill()
{
	m_NumberOfKill++;
}

void C_User::DecountNumberOfKill()
{
	m_NumberOfKill--;
}