// DrawManager.cpp: implementation of the CDrawManager class.
//
//////////////////////////////////////////////////////////////////////

#include "DrawManager.h"
#include "DxWindow/Windows/FontCatalog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawManager::CDrawManager(DxDisplay* input, unsigned int NumberOfLayer)
{
	m_Display = input;

	ifstream bitinfo;
	bitinfo.open("Config\\BitInfo.txt");

	bitinfo >> m_NumberOfSurface;
	m_NumberOfSurface = NumberOfLayer;
	m_Surfaces = new queue<DrawInstruction>[m_NumberOfSurface];

	for(unsigned int i = 0 ; i < MAX_HASHKEY_SIZE ; i++)
	{
		ZeroMemory( &m_Bitmap[i], sizeof(m_Bitmap[i]) );
		m_Bitmap[i].next = NULL;
	}

	char Bitname[MAX_PATH];
	DWORD x, y, R, G, B;
	while(!bitinfo.eof())
	{
		bitinfo >> Bitname >> x >> y >> R >> G >> B;
		AddBit( *Bitname, x, y, RGB(R,G,B));
	}

	bitinfo.close();
}

CDrawManager::~CDrawManager()
{
	SURFACEDBIT *temp;

	for(int i = 0 ; i < MAX_HASHKEY_SIZE ; i++)
	{
		while( m_Bitmap[i].next != NULL )	// 아에 할당이 없으면 나감
		{
			while( m_Bitmap[i].next != NULL )
			{
				temp = m_Bitmap[i].next;
				temp->Surface->destroy();
				delete temp->Surface;
				temp->Surface = NULL;
				m_Bitmap[i].next = *( &(m_Bitmap[i].next)->next);
				delete temp;
			}
		}
		// head의 surface destroy
		if( m_Bitmap[i].Surface != NULL )
		{
			m_Bitmap[i].Surface->destroy();
			delete m_Bitmap[i].Surface;
			m_Bitmap[i].Surface = NULL;
		}
	}

	delete [] m_Surfaces;
}

DWORD CDrawManager::GetHashKey(const char* ID)
{
	int strLen = (int)strlen(ID);
	int i, val = 0;
	for(i = 0 ; i < strLen ; i++)
		val += ID[i];

	return val%MAX_HASHKEY_SIZE;
}

void CDrawManager::AddBit(char &sName, DWORD width, DWORD length, DWORD rgb)
{
	DxSurface* tempSurface;
	if(!m_Display->createSurfaceFromBitmap( &tempSurface, &sName, width, length)) return;

	SURFACEDBIT* curNode = &m_Bitmap[GetHashKey( &sName )];
	while(curNode->next != NULL) curNode = curNode->next;

	if( curNode->Surface == 0 )	// Head 일 경우
	{
		strcpy( curNode->BitName, &sName );
		curNode->Surface = tempSurface;
		curNode->width = width;
		curNode->length = length;
		curNode->Surface->setColorKey(rgb);
	}
	else	// Not Head
	{
		SURFACEDBIT* temp = new SURFACEDBIT;
		strcpy( temp->BitName, &sName );
		temp->Surface = tempSurface;
		temp->width = width;
		temp->length = length;
		temp->Surface->setColorKey(rgb);

		curNode->next = temp;
		temp->next = NULL;
	}
}

SURFACEDBIT* CDrawManager::FindBit(string& sName)
{
	SURFACEDBIT* curNode = &m_Bitmap[GetHashKey(sName.c_str())];

	while(curNode != NULL)
	{
		if( !strcmp( curNode->BitName, sName.c_str()) )
			break;

		curNode = curNode->next;
	}

	return curNode;
}

DxSurface* CDrawManager::GetBitmapSurface(string BitmapID)
{
	SURFACEDBIT* temp = FindBit(BitmapID);
	if(temp == NULL)
		return NULL;
	else
		return temp->Surface;
}

void CDrawManager::DrawScreen()
{
	DrawInstruction temp;
	RECT rect;
	
	for( unsigned int i = 0 ; i < m_NumberOfSurface ; i++)
	{
		while(!m_Surfaces[i].empty())
		{
			temp = m_Surfaces[i].front();
			rect.bottom = temp.sourceY + temp.sourceHeight;
			rect.left = temp.sourceX;
			rect.right = temp.sourceX + temp.sourceWidth;
			rect.top = temp.sourceY;
			m_Display->colorKeyBlt(temp.destX, temp.destY ,GetBitmapSurface(temp.BitmapName)->getSurface(), &rect);
			m_Surfaces[i].pop();
		}
	}
}

unsigned int CDrawManager::GetMaxLayer()
{
	return m_NumberOfSurface;
}

void CDrawManager::DrawOnLayer(unsigned int LayerNO, DrawInstruction instruction)
{
	LayerNO = (LayerNO<(m_NumberOfSurface-1)) ? (LayerNO) : (m_NumberOfSurface-1);
	m_Surfaces[LayerNO].push(instruction);
}