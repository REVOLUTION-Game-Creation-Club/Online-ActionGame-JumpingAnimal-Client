#ifndef _AVATA_H_
#define _AVATA_H_

#include "DxWindow/DirectX/DxDisplay.h"

class CAvata
{
public:
	int pos_x;
	int pos_y;
	RECT rect;
public:
	int hair;
	int head;
	int body;
	int leg;
	int accessary;
	int back;
public:
	CAvata();
	~CAvata();
	void init();
	void destory();
	void Draw();

	DxSurface*		surfHair;
	DxSurface*		surfHead;
	DxSurface*		surfBody;
	DxSurface*		surfLeg;
	DxSurface*		surfAccessary;
	DxSurface*		surfBack;
}
#endif