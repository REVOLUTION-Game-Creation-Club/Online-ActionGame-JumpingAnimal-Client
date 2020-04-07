#include ".\_meteor.h"
#include "_Gamer.h"

C_Meteor::C_Meteor(int meteorKind, int start_pos_x, int start_pos_y, int end_pos_x, int end_pos_y)
:	C_User("meteor", 1.0f, "square", "meteor", meteorKind)
{
	Falling = true;
	m_bWarning = true;
	int i;
	for(i = 0 ; i < 4 ; i++)
		Adjusted[i] = false;

	nextMeteor = NULL;

	x = start_x = start_pos_x;
	y = start_y = start_pos_y - 25;

	end_x = end_pos_x;
	end_y = end_pos_y;

	DrawCounter = 0;
	WarningCounter = 0;
	MeteorNumber = MeteorUp = 0;
	for( i = 0 ; i < 4 ; i++)
		UP[i] = 139;
	BlinkCounter = (meteorKind == METEOR_GOOD) ? 8 : 6;
	Down = 0;
}

C_Meteor::~C_Meteor(void)
{
}

void C_Meteor::process(C_physics* phy)
{
	if(m_bWarning)
	{
		if(BlinkCounter == 0)
		{
			MeteorUp++;
			int k = MeteorUp/5 > 4 ? 4 : MeteorUp/5;
			int i;
			for( i = 0 ; i < k ; i++)
				UP[i] -= 13;

			DrawInstruction Mins;
			char MFileName[30];

			for( i = 0 ; i < 4 ; i++ ) {
				sprintf(MFileName, "BMP\\tile\\umeteor%d.bmp", i);
				Mins.BitmapName = MFileName;
				Mins.sourceX = 0;
				Mins.sourceY = 0;
				Mins.sourceWidth = 80;
				Mins.sourceHeight = 70;
				Mins.destX = 478;
				Mins.destY = UP[i];

				IOClass::GetDrawManager()->DrawOnLayer(1, Mins);
			}

			Mins.BitmapName = "BMP\\tile\\boom.bmp";
			Mins.sourceX = 0;
			Mins.sourceY = 0;
			Mins.sourceWidth = 80;
			Mins.sourceHeight = 70;
			Mins.destX = 478;
			Mins.destY = 139;

			IOClass::GetDrawManager()->DrawOnLayer(1, Mins);

			if(UP[3] + 70 < 0)
				m_bWarning = false;
			else
				return;
		}
		
		DrawInstruction ins;
		char FileName[30];
		sprintf(FileName, "BMP\\tile\\meteor_back%d.bmp", (int)((++WarningCounter%12)/6));
		ins.BitmapName = FileName;
		ins.sourceX = 0;
		ins.sourceY = 0;
		ins.sourceWidth = 800;
		ins.sourceHeight = 600;
		ins.destX = 0;
		ins.destY = 0;

		IOClass::GetDrawManager()->DrawOnLayer(1, ins);

		if(WarningCounter%6 == 0)
			BlinkCounter--;
		
		if((WarningCounter%8)/4)
		{
			ins.BitmapName = "BMP\\tile\\warning.bmp";
			ins.sourceX = 0;
			ins.sourceY = 0;
			ins.sourceWidth = 50;
			ins.sourceHeight = 25;
			ins.destX = end_x;
			ins.destY = end_y;

			IOClass::GetDrawManager()->DrawOnLayer(2, ins);
		}
	}
	else if(Falling)
	{
		if(y + (end_y - start_y)/30 > end_y)
		{
			Falling = false;
			setedge(end_x, end_y, 80, 70);
			phy->movable_obj_include(this);
		}
		else
		{
				x = x + (end_x - start_x)/30;
				y = y + (end_y - start_y)/30;
		}
	}
}

void C_Meteor::Crash_with_Meteor( C_obj* temp )
{
	if((m_KindOfCharacter == METEOR_GOOD || m_KindOfCharacter == METEOR_BAD))
		return;

	if(strcmp(temp->getname(), "meteor") != 0)
	{
		C_Gamer* gamer = (C_Gamer*) temp;
		if(gamer->getState() == DIE || gamer->getState() == ODIE)
			return;
		int i = gamer->getname()[0] - '0';
		if(Adjusted[ i ] == false)
		{
			Adjusted[ i ] = true;
			if(m_KindOfCharacter == GOOD_FIRE)
			{
				if(gamer->repitition != 2)
				{
					gamer->repitition = 2;
					gamer->FastTime = GetTickCount();
				}
			}
			if(m_KindOfCharacter == BAD_FIRE)
			{
				gamer->GamerKilled();
			}
		}
	}
}

void C_Meteor::onDraw()
{
	DrawInstruction ins;
	char FileName[30];
	if(m_KindOfCharacter != GOOD_FIRE && m_KindOfCharacter != BAD_FIRE)
	{
		sprintf(FileName, "BMP\\tile\\dmeteor%d.bmp", ++Down%6/3);
		ins.BitmapName = FileName;
		ins.sourceX = 0;
		ins.sourceY = 0;
		ins.sourceWidth = 80;
		ins.sourceHeight = 70;
		ins.destX = x;
		ins.destY = y;

		IOClass::GetDrawManager()->DrawOnLayer(2, ins);
	}
	else
	{
		int ImageNo = (DrawCounter++%24)/3;
		sprintf(FileName, "BMP\\tile\\fire%d.bmp", ImageNo);
		ins.BitmapName = FileName;
		ins.sourceX = 0;
		ins.sourceY = 0;
		ins.sourceWidth = 100;
		ins.sourceHeight = 50;
		ins.destX = x;
		ins.destY = y;

		IOClass::GetDrawManager()->DrawOnLayer(2, ins);
	}
}

bool C_Meteor::IsDeleteTime()
{
	if(Falling)
		return false;
	if(m_KindOfCharacter == GOOD_FIRE
		|| m_KindOfCharacter == BAD_FIRE)
	{
		if(GetTickCount() - ExistTime > 5000)
			return true;
	}
	return false;
}

void C_Meteor::crash_obj_move_up( C_obj* temp )
{
	Crash_with_Meteor(temp);
}
void C_Meteor::crash_obj_move_down( C_obj* temp )
{
	Crash_with_Meteor(temp);
}
void C_Meteor::crash_obj_move_left( C_obj* temp )
{
	Crash_with_Meteor(temp);
}
void C_Meteor::crash_obj_move_right( C_obj* temp )
{
	Crash_with_Meteor(temp);
}

void C_Meteor::crash_obj_fix_down( C_obj* temp )
{
	if(m_KindOfCharacter == METEOR_GOOD || m_KindOfCharacter == METEOR_BAD)
	{
		int p_x, p_y;
		getxy(&p_x, &p_y);
		setedge(p_x - 10, p_y+20, 100, 50);
		m_KindOfCharacter = (m_KindOfCharacter == METEOR_GOOD)?GOOD_FIRE:BAD_FIRE;
		ExistTime = GetTickCount();
	}
	C_obj::crash_obj_fix_down(temp);
}