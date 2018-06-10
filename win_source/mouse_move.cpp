#include "serial_comm.h"

//#include "resource.h"

// extern HWND hWnd;
// 
extern int mean_x;
extern int mean_y;
extern int mean_z;

int tmp_x;
int tmp_y;


void MoveMouse(int before_x, int before_y, short x, short y, short z)
{
	int rate;
	int CurPos_x, CurPos_y;		
	
	rate = 30;

	CurPos_x = (mean_x - x)/rate;
	CurPos_y = (mean_y - y)/rate;

	tmp_x = before_x - CurPos_x;
	tmp_y = before_y - CurPos_y;

	SetCursorPos((before_x - CurPos_x), (before_y - CurPos_y));	
}

void MouseLeft(void)
{
	mouse_event(MOUSEEVENTF_LEFTDOWN, tmp_x, tmp_y, NULL, NULL);		
}

void MouseLeftUp(void)
{
	mouse_event(MOUSEEVENTF_LEFTUP, tmp_x, tmp_y, NULL, NULL);		
}

void MouseRight(void)
{
	mouse_event(MOUSEEVENTF_RIGHTDOWN, tmp_x, tmp_y, NULL, NULL);
}

void MouseRightUp(void)
{
	mouse_event(MOUSEEVENTF_RIGHTUP, tmp_x, tmp_y, NULL, NULL);
}