#include <windows.h>
#include <tchar.h>
#include "labirinto.h"
#include "pipes.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow)
{



	int x = 0;
	int y = 0;

	Jogo jogo;
	jogo.numjogadores = 0;

	HANDLE hMutex = CreateMutex(NULL, TRUE, TEXT("SERVIDORDUNGEON"));

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, TEXT("Dungeon"), TEXT("Servidor já a correr."), MB_OK);
		CloseHandle(hMutex);
		return -1;
	}
	else
	{
		MessageBox(NULL, TEXT("Dungeon"), TEXT("Escutando.."), MB_OK);
		startserver(jogo);

	}




	return 0;
}


