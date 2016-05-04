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
	
char buffer[]="Write this text to file";
DWORD dwWritten; // number of bytes written to file
HANDLE hFile;  

hFile=CreateFile("file.txt",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
if(hFile==INVALID_HANDLE_VALUE)
  {
//    MessageBox(0,"Could not create/open a file","Error",16);
    return 0;
  }
WriteFile(hFile,buffer,sizeof(buffer),&dwWritten,0);

    for (int y = 0; y < lab.tamx; y++) {
            for (int  x = 0; x < lab.tamy ; x++) {
                Celula tile = tmp.celulas[x][y];
                if (tile == ) this.ctx.fillStyle = '#351330';
                else if (tile == 1) this.ctx.fillStyle = '#64908A';
                else this.ctx.fillStyle = '#424254';
                this.ctx.fillRect(x * this.scale, y * this.scale, this.scale, this.scale);
            }
        }

DWORD dwPtr = SetFilePointer( hFile, dwWritten, NULL, FILE_END); //set pointer position to end file

WriteFile(hFile,buffer,sizeof(buffer),&dwPtr,NULL); 

CloseHandle(hFile);

	return 0;
}
