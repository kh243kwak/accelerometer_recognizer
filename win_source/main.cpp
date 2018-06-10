#include "main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   PSTR szCmdLine, int iCmdShow)
{
	DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL,WndProc);
	g_hInst = hInstance;
	return 0;
}

BOOL CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	hWnd = hwnd;    

	hListStatus = GetDlgItem(hWnd, IDC_LIST_STATUS);

	switch (message)
	{
	case WM_INITDIALOG:
		{	
			SetClassLong(hwnd, GCL_HICON, (LONG)LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON1)));
						
			Sending_flag = 1;
			
			read_config_file();
			
			Open(boudrate, com_port);
			
			return TRUE;
		}   
		
	case WM_CLOSE:
		EndDialog(hwnd,IDOK);
		break;

	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDOK:
				Sending_flag = 0;
			case IDCANCEL:
				EndDialog(hwnd,IDOK);   
				break;
			}
			return TRUE;
		}
	}
	return FALSE;
}

int read_config_file(void)
{
	FILE * config_file;
	char buf[50];

	config_file = fopen(CONFIG_FILE, "r+t");
	if(config_file == NULL)
	{
		return FALSE;
	}
	fgets(buf, sizeof(buf), config_file);
	sscanf(buf, "%*s = %d", &com_port);
	fgets(buf, sizeof(buf), config_file);
	sscanf(buf, "%*s = %d", &boudrate);

	if(com_port < 1)
		return FALSE;

	if( (boudrate != 9600) &&
		(boudrate != 14400) &&
		(boudrate != 19200) &&
		(boudrate != 38400) &&
		(boudrate != 57600) &&
		(boudrate != 115200) )
		return FALSE;
	
	fclose(config_file);
	return TRUE;
}