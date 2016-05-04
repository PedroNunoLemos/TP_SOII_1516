#include <windows.h>
#include <tchar.h>
#include "labirinto.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow)
{



	Labirinto lab;

	char buffer[MAXTAMY + 1];
	int x = 0;
	int y = 0;
	
	char tc;


	DWORD dwWritten; // number of bytes written to file
	HANDLE hFile;


	HANDLE hMutex = CreateMutex(NULL, TRUE, TEXT("SERVIDORDUNGEON"));

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, TEXT("_"), TEXT("oups"), MB_OK);
		CloseHandle(hMutex);
		return -1;
	}


	lab = CriaLabirinto(lab, 70, 70, 10);

	hFile = CreateFile("file.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		    MessageBox(0,"Could not create/open a file","Error",16);
		return 0;
	}

	//WriteFile(hFile,buffer,sizeof(buffer),&dwWritten,0);

	for (y = 0; y < lab.tamx; y++) {

		
		for (x = 0; x < lab.tamy; x++) {

			tc = '*';

			if (x == 0) buffer[0] = '\0';

			Celula tile = lab.celula[x][y];
			if (tile.tipo == TipoCelula_VAZIO) tc = (char)(256);
			else if (tile.tipo == TipoCelula_CHAO) tc = '.'; // chao
			else tc = '#'; // parede

			buffer[x] = tc;
		}

		buffer[x + 1] = '\0';
		WriteFile(hFile, buffer, sizeof(buffer), &dwWritten, 0);

	}

	DWORD dwPtr = SetFilePointer(hFile, dwWritten, NULL, FILE_END); //set pointer position to end file

	//WriteFile(hFile,buffer,sizeof(buffer),&dwPtr,NULL); 

	CloseHandle(hFile);

	return 0;
}
