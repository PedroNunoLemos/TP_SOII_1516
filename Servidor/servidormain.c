#include <windows.h>
#include <tchar.h>
#include "labirinto.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow)
{



	HANDLE hMutex = CreateMutex(NULL, TRUE, TEXT("SERVIDORDUNGEON"));

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, TEXT("_"),TEXT("oups") , MB_OK);
		CloseHandle(hMutex);
		return -1;
	}


	//MessageBox(NULL, TEXT("Whee!"), TEXT("Hello World!"), MB_OK);

	Labirinto lab;
	lab = CriaLabirinto(lab, 70, 70);
	


	return 0;
}
