#ifndef _MAIN_H
#define _MAIN_H

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <commctrl.h>
#include "resource.h"

#define CONFIG_FILE		"config.txt"

//////////////////////////////////////////////////////////////////////////
// Variable 
//////////////////////////////////////////////////////////////////////////

HWND hWnd;				
HINSTANCE g_hInst;      
HWND hListStatus;		
int Sending_flag = 0;
int boudrate;          
int com_port;         

//////////////////////////////////////////////////////////////////////////
// Operation 
//////////////////////////////////////////////////////////////////////////
BOOL CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


BOOL Open(int, int);			
int read_config_file(void);		

#endif