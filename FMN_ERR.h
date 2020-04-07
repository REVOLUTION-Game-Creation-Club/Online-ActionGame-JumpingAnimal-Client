/*
 * FMN_ERR.H
 * 
 * Name		  : FMN_ERR <<Singleton>>
 * Purpose    : Logging Error or Message To debug window, cosol and file , Detecting Memory Leak
 * Type       : C++ / Win32
 * Version    : 0.5.1
 * 
 * Author     : Song Chun Jong(silveraxe@daum.net)
 *
 * Remarks    : 
 *				- FMN::GetInstance() : init FMN_ERR and return instance
 *				- FMN::Cleanup()     : Be sure locate function at end of WinMain()
 *									   or You will see Memory Leak!
 *              
 * 
 * Last Update : 2004. March
 *
 */



#ifndef _FMN_ERR_H_
#define _FMN_ERR_H_

#include <windows.h>
#include <windowsx.h>

// FMN: disappear warning message (stl associated..)
// - identifier was truncated to '255' characters in the debug information -
#pragma warning(disable:4786)   

// FMN: for detect memory leak functions
#include <CRTDBG.H>

#include <map>
using namespace std;


#define FMN_ERR_MAX_IDSLOTS             255
#define FMN_ERR_MAX_IDNAME              255


typedef std::map<int, char* > FMN_ERR_MAPIDNAME;


class FMN_ERR  
{

private:
	FMN_ERR();
	virtual ~FMN_ERR();


//---------------------------------------------------------------------------------------
//- Variables
//---------------------------------------------------------------------------------------

public:
	static const int LOGTODEBUG;
	static const int LOGTOFILE;
	static const int LOGTOCONSOL;

protected:
	// FMN: 0 -> VC IDE Debug Wnd(Using OutputDebugString), 1 -> File, 2-> Consol
	static int m_nLoggingMethod;


private:
		
	static FMN_ERR* m_pInstance;

	static BOOL m_arnIDSlot[FMN_ERR_MAX_IDSLOTS];
	static FMN_ERR_MAPIDNAME* m_pMapIDName;

	static HANDLE m_hConsol;
	static FILE*  m_fp;


//---------------------------------------------------------------------------------------
//- Functions
//---------------------------------------------------------------------------------------

public:
	static FMN_ERR* GetInstance();
	static void SetLoggingMethod(int nLoggingMethod);
	
	static BOOL Register(int* pnID, char* szName);
	static BOOL UnRegister(int nID);

	static void AddLog(int nID, char* pszMessage, BOOL noAddLineFeed = TRUE);
	static void AddLog2(int nID, char* pszFormat, ...);

	static void CleanUp();

	static char* MakeStr(char* pszFormat, ...);

	static BOOL GetEmptySlot(int* pnID);


//---------------------------------------------------------------------------------------
//- MemoryLeak Checker Functions
//---------------------------------------------------------------------------------------

public:
	static int StartMemoryLeakChecker(DWORD dwFlag = 0);
	static void StopMemoryLeakChecker();

	static void CheckMemoryLeak();
		

protected:
	static void Destroy();
	static char* GetST(int nID);

	static void (*_OutputLog)(char* szLog);

	static void _OutputLog_Debug(char* szLog);
	static void _OutputLog_Consol(char* szLog);
	static void _OutputLog_File(char* szLog);


//---------------------------------------------------------------------------------------
//- MemoryLeak Checker Variables
//---------------------------------------------------------------------------------------

protected:
	static BOOL bMemLeakCheckerActived;
	static int	nOldMemLeakCheckerState;


public:
	static const char* szVerInfo;

};


#endif 
