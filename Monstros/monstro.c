#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <strsafe.h>

#include "../Controlador/constantes.h"
#include "../Controlador/monstro.h"
#include "../Controlador/coordenada.h"
#include "../Controlador/memoria.h"


#define QUEUE_SIZE 5
#define buf 256



HANDLE hMapFile;
HANDLE moveMutex;
HANDLE servidorMutex;
HANDLE hThread;
JogoServidor *jogo;



int mid;
int nSpaces;

TCHAR procNome[buf];
STARTUPINFO si;
PROCESS_INFORMATION pi;



void abrirMemoriaPartilhada() {

	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		TEXT("MapaGlobal"));               // name of mapping object

	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not open file mapping object (%d).\n"),
			GetLastError());
		return;
	}

	jogo = (JogoServidor*)MapViewOfFile(hMapFile, // handle to map object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0,
		0,
		0); //all file is viewed

	if (jogo == NULL)
	{
		//_tprintf(TEXT("Could not map view of file (%d).\n"),GetLastError());

		CloseHandle(hMapFile);

		return;
	}

}

void AtualizaMapaMonstro(int ox, int oy) {

	int x = 0, y = 0;
	int r = 0;

	WaitForSingleObject(servidorMutex, INFINITE);

	for (x = 0; x < jogo->mapa.tamx; x++)
	{
		for (y = 0; y < jogo->mapa.tamy; y++)
		{

			if (x == ox && y == oy && jogo->mapa.celula[x][y].monstro == mid) {
				jogo->mapa.celula[x][y].tipoMonstro = -1;
				jogo->mapa.celula[x][y].monstro = -1;
			}

			if (x == jogo->monstros[mid].posicao.x && y == jogo->monstros[mid].posicao.y) {
				jogo->mapa.celula[x][y].tipoMonstro = jogo->monstros[mid].tipo;
				jogo->mapa.celula[x][y].monstro = mid;
			}

		}
	}

	ReleaseMutex(servidorMutex);

}



DWORD WINAPI atualizaMonstro(LPVOID param)
{

	Coordenada novoMonstro;
	Monstro me;
	Jogador sch;

	while (1)
	{

		WaitForSingleObject(servidorMutex, INFINITE);

		me = jogo->monstros[mid];

		AtualizaMapaMonstro(me.posicao.x, me.posicao.y);


		for (int i = 0; i < jogo->jogadoresLigados; i++) {


			if (jogo->clientes[i].jogo.jogador.posicao.y == me.posicao.y
				&& jogo->clientes[i].jogo.jogador.posicao.x == me.posicao.x) {
				jogo->clientes[i].jogo.jogador.saude--;
			}
		}

		ReleaseMutex(servidorMutex);


		//	if (me.energia >= (2 /* duplicatemonster*/ * SAUDE_MONSTRO_DIST) && me.tipo == 0) {

		//		novoMonstro.y = me.posicao.y;

		//		if (mapa->mapa[me.posicao.y][me.posicao.x + 1].monstro != 0 &&
		//			mapa->mapa[me.posicao.y][me.posicao.x + 1].jogador != 0) {
		//			novoMonstro.x = me.posicao.x + 1;
		//		}
		//		else {
		//			novoMonstro.x = me.posicao.x - 1;
		//		}

		//		me.energia *= .8;

		//		_stprintf_s(procNome, buf, TEXT("%s %d %d %d %d %d %d %d"),
		//			TEXT("Monstro"), me.tipo, MAXTAMX, MAXTAMY, novoMonstro.y, novoMonstro.x, MAXINIMIGOS, me.energia);

		//		ZeroMemory(&si, sizeof(STARTUPINFO)); //Set data to 0
		//		si.cb = sizeof(STARTUPINFO);

		//		CreateProcess(NULL, procNome, NULL, NULL, 0, 0, NULL, NULL, &si, &pi);
		//	}
		//	else if (me.energia >= (2 * SAUDE_MONSTRO_BULLY) && me.tipo == 1) {
		//		novoMonstro.y = me.posicao.y;

		//		if (mapa->mapa[me.posicao.y][me.posicao.x + 1].monstro != 0 &&
		//			mapa->mapa[me.posicao.y][me.posicao.x + 1].jogador != 0) {
		//			novoMonstro.x = me.posicao.x + 1;
		//		}
		//		else {
		//			novoMonstro.x = me.posicao.x - 1;
		//		}

		//		me.energia *= .8;

		//		_stprintf_s(procNome, buf, TEXT("%s %d %d %d %d %d %d %d"),
		//			TEXT("Monstro"), me.tipo, MAXTAMX, MAXTAMY, novoMonstro.y, novoMonstro.x, MAXINIMIGOS, me.energia);

		//		ZeroMemory(&si, sizeof(STARTUPINFO)); //Set data to 0
		//		si.cb = sizeof(STARTUPINFO);

		//		CreateProcess(NULL, procNome, NULL, NULL, 0, 0, NULL, NULL, &si, &pi);
		//	}

		Sleep(250);


	}//fim while(1);


}

int _tmain(int argc, LPTSTR argv[]) {


	int tid = 0;

	//if (argc != 6) {
	//	return -1;
	//}



	mid = _ttoi(argv[1]);
	nSpaces = _ttoi(argv[2]);


	srand(time(NULL));


	servidorMutex = OpenMutex(
		MUTEX_ALL_ACCESS,            // request full access
		FALSE,                       // handle not inheritable
		TEXT("ServidorMutex"));  // object name

	moveMutex = CreateMutex(
		NULL,
		FALSE,
		TEXT("moveMutex"));

	if (moveMutex == NULL) {
		_tprintf(TEXT("CreateMutex error: %d\n"), GetLastError());
		return 1;
	}

	abrirMemoriaPartilhada();


	if (jogo == NULL) {
		_tprintf(TEXT("Error accessing mapped memory\n"));
		return -1;
	}

	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)atualizaMonstro, (LPVOID)NULL, 0, NULL);

	WaitForSingleObject(hThread, INFINITE);


	//UnmapViewOfFile(jogo);

	//CloseHandle(hMapFile);


	return 0;
}
