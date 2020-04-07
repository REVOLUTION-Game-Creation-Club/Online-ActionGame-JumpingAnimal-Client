// DrawManager.h: interface for the CDrawManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWMANAGER_H__F8F95907_09BA_4D91_9A6A_936B88E94A57__INCLUDED_)
#define AFX_DRAWMANAGER_H__F8F95907_09BA_4D91_9A6A_936B88E94A57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DxWindow/DirectX/DxDisplay.h"
#include <string>
#include <fstream>
#include <queue>

using namespace std;

const DWORD MAX_HASHKEY_SIZE = 13;

struct SURFACEDBIT
{
	char BitName[MAX_PATH];
	DxSurface* Surface;
	DWORD width, length;
	SURFACEDBIT* next;
};

struct DrawInstruction
{
	string BitmapName;
	int sourceX, sourceY, sourceWidth, sourceHeight;
	int destX;
	int destY;
};

class CDrawManager  
{
	DxDisplay*		m_Display;
	queue<DrawInstruction>*		m_Surfaces;
	unsigned int	m_NumberOfSurface;
	SURFACEDBIT		m_Bitmap[MAX_HASHKEY_SIZE];

public:
	CDrawManager(DxDisplay*, unsigned int);
	~CDrawManager();

	unsigned int GetMaxLayer();
	void DrawScreen();

	DxSurface* GetBitmapSurface(string BitmapID);

	void DrawOnLayer(unsigned int LayerNO, DrawInstruction instruction);
	DxDisplay*	getDisplay() { return m_Display; }

private:
	DWORD GetHashKey(const char*);
	void AddBit(char &, DWORD width, DWORD length, DWORD rgb);
	SURFACEDBIT* FindBit(string&);
	
};

#endif // !defined(AFX_DRAWMANAGER_H__F8F95907_09BA_4D91_9A6A_936B88E94A57__INCLUDED_)
