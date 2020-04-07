

#include "FMN_ERR.h"


//---------------------------------------------------------------------------------------
//- static member variables initialization
//---------------------------------------------------------------------------------------

const int FMN_ERR::LOGTODEBUG = 0;
const int FMN_ERR::LOGTOFILE = 1;
const int FMN_ERR::LOGTOCONSOL = 2;
	
int FMN_ERR::m_nLoggingMethod = FMN_ERR::LOGTODEBUG;
FMN_ERR* FMN_ERR::m_pInstance = NULL;
BOOL FMN_ERR::m_arnIDSlot[FMN_ERR_MAX_IDSLOTS];
FMN_ERR_MAPIDNAME* FMN_ERR::m_pMapIDName = NULL;
void (*FMN_ERR::_OutputLog)(char* szLog) = NULL;

HANDLE FMN_ERR::m_hConsol = NULL;
FILE*  FMN_ERR::m_fp = NULL;

BOOL FMN_ERR::bMemLeakCheckerActived = FALSE;
int FMN_ERR::nOldMemLeakCheckerState = 0;

const char* szVerInfo = "FMN_ERR-0.5.31-Copyright 2004. March Song.Chun.Jong";



//---------------------------------------------------------------------------------------
//- initialize functions
//---------------------------------------------------------------------------------------

FMN_ERR::FMN_ERR()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); 
}

FMN_ERR::~FMN_ERR()
{
	Destroy();
}


void FMN_ERR::Destroy()
{

	if(!m_pInstance)
		return;

	if(m_fp)
	{
		fclose(m_fp);
		m_fp = NULL;
	}

	if(m_hConsol)
	{
		m_hConsol = ::GetStdHandle(STD_OUTPUT_HANDLE);
		if(m_hConsol != INVALID_HANDLE_VALUE)
			::FreeConsole();

		m_hConsol = NULL;
	}

	if(m_pMapIDName)
	{
		delete m_pMapIDName;
		m_pMapIDName = NULL;
	}
	
	memset(m_arnIDSlot, 0, sizeof(m_arnIDSlot));

	if(m_pInstance)
	{
		FMN_ERR* temp = m_pInstance;
		
		m_pInstance = NULL;		
		delete temp;		
	}


}


FMN_ERR* FMN_ERR::GetInstance()
{

	if(!m_pInstance)
	{

		m_pInstance = new FMN_ERR();

		memset(m_arnIDSlot, 0, sizeof(m_arnIDSlot));
		m_pMapIDName = new FMN_ERR_MAPIDNAME;

		switch(m_nLoggingMethod) {

		case LOGTODEBUG:
			
			_OutputLog = _OutputLog_Debug;
			break;

		case LOGTOCONSOL:

			if(::AllocConsole() == FALSE)
			{
				m_hConsol = NULL;
				Destroy();
				return NULL;
			}
			else
			{
				// FMN: 새로할당한 콘솔을 읽어옴
				m_hConsol = ::GetStdHandle(STD_OUTPUT_HANDLE);
				if(m_hConsol == INVALID_HANDLE_VALUE)
				{
					m_hConsol = NULL;
					Destroy();
					return NULL;
				}
			}

			_OutputLog = _OutputLog_Consol;
			break;

		case  LOGTOFILE:
		{
			char szBuff[255];

			sprintf(szBuff, "_FMNERRLOG_%s.log", GetST(1));			
			m_fp = fopen(szBuff,"wb");

			if(!m_fp)
			{
				Destroy();
				return NULL;
			}
		
			_OutputLog = _OutputLog_File;

			break;
		}
		default:
			break;
		}
	}

	return m_pInstance;

}


void FMN_ERR::CleanUp()
{
	Destroy();
}



//---------------------------------------------------------------------------------------
//- utility functions
//---------------------------------------------------------------------------------------


char* FMN_ERR::MakeStr(char* pszFormat, ...)
{
	
	va_list argList;
	static char szBuf[256];

	va_start(argList, pszFormat);
    vsprintf(szBuf, pszFormat, argList);    
    va_end(argList);

	return szBuf;
		
}


char* FMN_ERR::GetST(int nType)
{
	
	static char szTime[100];

	SYSTEMTIME st;	
	::GetLocalTime(&st);

	switch(nType) {
	case 0:
		sprintf(szTime, "%02d/%02d-%02d:%02d:%02d.%01d", st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		break;
	case 1:
		sprintf(szTime, "%02d%02d_%02d%02d%02d", st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		break;
	default:
		break;
	}	

	return szTime;

}


void FMN_ERR::SetLoggingMethod(int nLoggingMethod)
{
	m_nLoggingMethod = nLoggingMethod;	
}


BOOL FMN_ERR::Register(int* pnID, char* szName)
{

	if(!m_pInstance)
		return FALSE;

	if(!GetEmptySlot(pnID))
		return FALSE;

	m_arnIDSlot[*pnID] = TRUE;
	(*m_pMapIDName)[*pnID] = szName;
	
	return TRUE;

}


BOOL FMN_ERR::UnRegister(int nID)
{
	
	if(!m_pInstance)
		return FALSE;

	if(!m_arnIDSlot[nID])
		return FALSE;

	m_arnIDSlot[nID] = FALSE;

	(*m_pMapIDName)[nID] = NULL;
	m_pMapIDName->erase(nID);

	return TRUE;

}


BOOL FMN_ERR::GetEmptySlot(int* pnID)
{

	for(int i=1;i<FMN_ERR_MAX_IDSLOTS;i++)
	{
		if(!m_arnIDSlot[i])
		{
			*pnID = i;
			return TRUE;
		}
	}

	return FALSE;

}



//---------------------------------------------------------------------------------------
//- Logger Functions
//---------------------------------------------------------------------------------------

	
void FMN_ERR::AddLog2(int nID, char* pszFormat, ...)
{
	
	if(!m_pInstance)
		return;
	
	va_list argList;
	static char szBuf[256];
	static char szBuf2[256];	

	va_start(argList, pszFormat);
    vsprintf(szBuf, pszFormat, argList);    

	if(nID)
		sprintf(szBuf2, " FMN_ERR [%s]:[ %s ] - %s \n", GetST(0), (*m_pMapIDName)[nID], szBuf);
	else
		sprintf(szBuf2, " FMN_ERR [%s]:[ #NOID# ] - %s \n", GetST(0), szBuf);

	_OutputLog(szBuf2);

    va_end(argList);
		
}


void FMN_ERR::AddLog(int nID, char* pszMessage, BOOL noAddLineFeed)
{

	if(!m_pInstance)
		return;

	static char szBuf[256];	

	if(nID)
		sprintf(szBuf, " FMN_ERR [%s]:[ %s ] - %s ", GetST(0), (*m_pMapIDName)[nID], pszMessage);
	else
		sprintf(szBuf, " FMN_ERR [%s]:[ #NOID# ] - %s ", GetST(0), pszMessage);

	strcpy(&szBuf[strlen(szBuf)], "\n\r");
	
	_OutputLog(szBuf);

}



void FMN_ERR::_OutputLog_Debug(char* szLog)
{
	OutputDebugString(szLog);
}


void FMN_ERR::_OutputLog_Consol(char* szLog)
{
	
	DWORD dwSize;

	if(!m_hConsol)	
		return;

	::WriteFile(m_hConsol, szLog, strlen(szLog), &dwSize, 0);

}


void FMN_ERR::_OutputLog_File(char* szLog)
{

	if(!m_fp)
		return;

	fprintf(m_fp, "%s%c%c", szLog, 0x0d, 0x0a);

}


//---------------------------------------------------------------------------------------
//- MemoryLeak Checker Function
//---------------------------------------------------------------------------------------

int FMN_ERR::StartMemoryLeakChecker(DWORD dwFlag)
{

	int nCurDebugFlag;

	
	// FMN: get current debug state 
	nCurDebugFlag =  _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

	// FMN: save current dbgflag
	nOldMemLeakCheckerState = nCurDebugFlag;

	if(!dwFlag)
	{
		nCurDebugFlag |= _CRTDBG_ALLOC_MEM_DF;
		nCurDebugFlag |= _CRTDBG_LEAK_CHECK_DF;
		nCurDebugFlag |= _CRTDBG_DELAY_FREE_MEM_DF;

		dwFlag = nCurDebugFlag;
	}

	return _CrtSetDbgFlag(dwFlag); 

}


void FMN_ERR::StopMemoryLeakChecker()
{

	if(!nOldMemLeakCheckerState)
		return;

	_CrtSetDbgFlag(nOldMemLeakCheckerState);
}


void FMN_ERR::CheckMemoryLeak()
{

	if(!_CrtCheckMemory()) 
	{
		if(m_pInstance)
		{
			_OutputLog(" FMN_ERR: Memory Leak Occured! \n");
		}
		else
		{
			::MessageBox(NULL, " FMN_ERR: Memory Leak Occured! \n","FMN_ERR", MB_OK);
		}

	}

}