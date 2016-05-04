#include <windows.h>
#include <tchar.h>
#include "labirinto.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow)
{



	Labirinto lab;

	char buffer[MAXTAMY + 4];
	int x = 0;
	int y = 0;

	HANDLE hMutex = CreateMutex(NULL, TRUE, TEXT("SERVIDORDUNGEON"));

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, TEXT("_"), TEXT("oups"), MB_OK);
		CloseHandle(hMutex);
		return -1;
	}


	lab = CriaLabirinto(lab, 70, 70, 10);


	return 0;
}
