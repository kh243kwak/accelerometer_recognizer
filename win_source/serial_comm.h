#ifndef _SERIAL_COMM_H
#define _SERIAL_COMM_H

#include <windows.h>
#include <crtdbg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <imm.h>
#include <commctrl.h>
#include "resource.h"
#include "Recognize.h"
#include "process.h"

#define NUMBER	0
#define ENGLISH 1
#define HANGUL	2

//////////////////////////////////////////////////////////////////////////
// Variable
//////////////////////////////////////////////////////////////////////////
extern int Sending_flag;
extern HWND hWnd;

enum EReadTimeout { EReadTimeoutBlocking, EReadTimeoutNonBlocking };

//////////////////////////////////////////////////////////////////////////
// Operation
//////////////////////////////////////////////////////////////////////////
DWORD WINAPI RecvData(void * dummy);

// ���콺 ���� �Լ�
void MoveMouse(int before_x, int before_y, short x, short y, short z);
void MouseLeft(void);
void MouseLeftUp(void);
void MouseRight(void);
void MouseRightUp(void);

// Ű���� �����Լ�
void push_left(void);
void push_right(void);

#endif