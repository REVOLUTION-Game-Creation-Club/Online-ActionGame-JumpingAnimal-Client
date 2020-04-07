#include "Avata.h"

CAvata::CAvata()
{
	pos_x = 0;
	pos_y = 0;

	hair = 0;
	head = 0;
	body = 0;
	leg = 0;
	accessary = 0;
	back = 0;
}

CAvata::~CAvata()
{
	delete surfHair;
	delete surfHead;
	delete surfBody;
	delete surfLeg;
	delete surfAccessary;
	delete surfBack;
}

void CAvata::init()
{
	display->createSurfaceFromBitmap( &surfHair, _T( "BMP/AVATAHair.bmp" ), 0, 0 );
	display->createSurfaceFromBitmap( &surfHead, _T( "BMP/AVATAHead.bmp" ), 0, 0 );
	display->createSurfaceFromBitmap( &surfBody, _T( "BMP/AVATABody.bmp" ), 0, 0 );
	display->createSurfaceFromBitmap( &surfLeg, _T( "BMP/AVATALeg.bmp" ), 0, 0 );
	display->createSurfaceFromBitmap( &surfAccessary, _T( "BMP/AVATAAccessary.bmp" ), 0, 0 );
	display->createSurfaceFromBitmap( &surfBack, _T( "BMP/AVATABack.bmp" ), 0, 0 );
}

void CAvata::destroy()
{
}

void CAvata::Draw()
{
	display->setColorKey(RGB(0,0,0));
	display->colorKeyBlt(pos_x,pos_y,surfBack);
	display->colorKeyBlt(pos_x,pos_y,surfLeg);
	display->colorKeyBlt(pos_x,pos_y,surfBody);
	display->colorKeyBlt(pos_x,pos_y,surfHead);
	display->colorKeyBlt(pos_x,pos_y,surfHair);
	display->colorKeyBlt(pos_x,pos_y,surfAccessary);
}