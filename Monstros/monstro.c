
#include "../Controlador/monstro.h"
#include "../Controlador/coordenada.h"
#include "../Controlador/memoria.h"


#define QUEUE_SIZE 5
#define buf 256



HANDLE hMapFile;
HANDLE moveMutex;
HANDLE hThread;
MemoriaPartilhada* pBuf;

Monstro me;
int tamx;
int tamy;
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

	pBuf = (MemoriaPartilhada*)MapViewOfFile(hMapFile, // handle to map object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0,
		0,
		0); //all file is viewed

	if (pBuf == NULL)
	{
		//_tprintf(TEXT("Could not map view of file (%d).\n"),GetLastError());

		CloseHandle(hMapFile);

		return;
	}
}

int validaJogadorposicao() {

	Coordenada center;

	center.y = me.posicao.y - (VISAO_MONSTRO / 2);
	center.x = me.posicao.x - (VISAO_MONSTRO / 2);

	for (int i = 0; i < VISAO_MONSTRO; i++) {
		for (int j = 0; j < VISAO_MONSTRO; j++) {


		}
	}

	return 0;
}

void IniciaBully() {


	int r = rand() % 100;
	int before = 0;
	int Jogadorposicao;

	while (1) {

		Jogadorposicao = validaJogadorposicao();

	}
}

void IniciaDistraido() {


	int r = rand() % 100;
	int before = 0;
	int Jogadorposicao;

	while (1) {

		Jogadorposicao = validaJogadorposicao();

	}
}

DWORD WINAPI validaPosicao(LPVOID param)
{
	Jogador* sch;
	Coordenada newMonster;

	while (1) {

		//	if (/* posicao jogador = monstro*/1) {
		//		sch = pBuf->clientes;
		//		for (; sch != &pBuf->clientes[MAXJOGADORES + 1];) {
		//			if (sch->posicao.y == me.posicao.y && sch->posicao.x == me.posicao.x) {
		//				sch->saude--;
		//			}
		//		}
		//	}

		//	if (me.energia >= (2 /* duplicatemonster*/ * SAUDE_MONSTRO_DIST) && me.tipo == 0) {

		//		newMonster.y = me.posicao.y;

		//		if (pBuf->mapa[me.posicao.y][me.posicao.x + 1].monstro != 0 &&
		//			pBuf->mapa[me.posicao.y][me.posicao.x + 1].jogador != 0) {
		//			newMonster.x = me.posicao.x + 1;
		//		}
		//		else {
		//			newMonster.x = me.posicao.x - 1;
		//		}

		//		me.energia *= .8;

		//		_stprintf_s(procNome, buf, TEXT("%s %d %d %d %d %d %d %d"),
		//			TEXT("Monstro"), me.tipo, MAXTAMX, MAXTAMY, newMonster.y, newMonster.x, MAXINIMIGOS, me.energia);

		//		ZeroMemory(&si, sizeof(STARTUPINFO)); //Set data to 0
		//		si.cb = sizeof(STARTUPINFO);

		//		CreateProcess(NULL, procNome, NULL, NULL, 0, 0, NULL, NULL, &si, &pi);
		//	}
		//	else if (me.energia >= (2 * SAUDE_MONSTRO_BULLY) && me.tipo == 1) {
		//		newMonster.y = me.posicao.y;
		//		if (pBuf->mapa[me.posicao.y][me.posicao.x + 1].monstro != 0 &&
		//			pBuf->mapa[me.posicao.y][me.posicao.x + 1].jogador != 0) {
		//			newMonster.x = me.posicao.x + 1;
		//		}
		//		else {
		//			newMonster.x = me.posicao.x - 1;
		//		}

		//		me.energia *= .8;

		//		_stprintf_s(procNome, buf, TEXT("%s %d %d %d %d %d %d %d"),
		//			TEXT("Monstro"), me.tipo, MAXTAMX, MAXTAMY, newMonster.y, newMonster.x, MAXINIMIGOS, me.energia);

		//		ZeroMemory(&si, sizeof(STARTUPINFO)); //Set data to 0
		//		si.cb = sizeof(STARTUPINFO);

		//		CreateProcess(NULL, procNome, NULL, NULL, 0, 0, NULL, NULL, &si, &pi);
		//	}

		//	Sleep(1 / 15 * me.lentidao * 1000);
	}


}

int _tmain(int argc, LPTSTR argv[]) {


	if (argc != 8) {
		return -1;
	}

	srand(time(NULL));

	me.tipo = _ttoi(argv[1]);
	tamx = _ttoi(argv[2]);
	tamy = _ttoi(argv[3]);
	me.posicao.y = _ttoi(argv[4]);
	me.posicao.x = _ttoi(argv[5]);

	if (me.tipo == 0) {
		wcscpy_s(me.descricao, 10, TEXT("Distracted"));
		me.lentidao = VELOCIDADE_MONSTRO_BULLY;
	}
	else {
		wcscpy_s(me.descricao, 10, TEXT("Bully"));
		me.lentidao = VELOCIDADE_MONSTRO_DIST;
	}


	me.energia = _ttoi(argv[7]);

	moveMutex = CreateMutex(
		NULL,
		FALSE,
		TEXT("moveMutex"));

	if (moveMutex == NULL) {
		_tprintf(TEXT("CreateMutex error: %d\n"), GetLastError());
		return 1;
	}

	abrirMemoriaPartilhada();


	if (pBuf == NULL) {
		_tprintf(TEXT("Error accessing mapped memory\n"));
		return -1;
	}

	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)validaPosicao, (LPVOID)NULL, 0, NULL);

	if (me.tipo == 0) {
		IniciaDistraido();
	}
	else {
		IniciaBully();
	}


	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);


	return 0;
}
