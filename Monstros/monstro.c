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

#include "motorMonstro.h"

#define QUEUE_SIZE 5
#define buf 256



HANDLE hMapFile;
HANDLE moveMutex;
HANDLE servidorMutex;
HANDLE hThread;
JogoServidor *jogo;


int mdup;
int mtipo;
int menergia;
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



DWORD WINAPI atualizaMonstro(LPVOID param)
{

	int mid = (int)param;

	Monstro novoMonstro;
	Monstro me;
	Jogador sch;
	int nid = -1;
	int pode = 0;
	int tr = 0;

	while (1)
	{

		WaitForSingleObject(servidorMutex, INFINITE);

		me = jogo->monstros[mid];

		if (me.contadorMovimento == 0) {

			AtualizaMapaMonstro(jogo, mid, me.posicao.x, me.posicao.y);


			for (int i = 0; i < jogo->jogadoresLigados; i++) {

				if (jogo->clientes[i].jogo.jogador.posicao.y != me.posicao.y
					|| jogo->clientes[i].jogo.jogador.posicao.x != me.posicao.x)
					pode = 1;


				if (jogo->clientes[i].jogo.jogador.posicao.y == me.posicao.y
					&& jogo->clientes[i].jogo.jogador.posicao.x == me.posicao.x && pode == 1)
				{

					jogo->clientes[i].jogo.jogador.saude--;

					jogo->monstros[mid].energia += 1;

					_tprintf(TEXT("monstro..:  id:%d energia: %d \n "),mid,
						jogo->monstros[mid].energia
						);

					pode = 0;

				}
			}

			if (jogo->monstrosCriados < MAXINIMIGOS)
			{
				if (me.energia >= (1.6  * SAUDE_MONSTRO_DIST) && me.tipo == DISTRAIDO)
				{

					jogo->monstros[me.id].energia = (SAUDE_MONSTRO_DIST*1.6) / 2;

					tr = aleatorio(1, N_CASAS, me.contadorMovimento);


					_stprintf_s(procNome, 256,
						TEXT("%s %d %d %d %d"),
						TEXT("Monstro"), //0
						0,// tipo Monstro
						jogo->monstros[me.id].energia, //Energia
						tr, // valor de N,
						me.id // se é criado de raiz ou Dup 
						); //2



					ZeroMemory(&si, sizeof(STARTUPINFO)); //Set data to 0
					si.cb = sizeof(STARTUPINFO);

					_tprintf(TEXT("duplicando o monstro..: %d %d %d %d \n"), 0,
						jogo->monstros[me.id].energia, tr, me.id);

					_tprintf(procNome); _tprintf("\n");

					CreateProcess(NULL, procNome, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
				}

				if (me.energia >= (1.6  * SAUDE_MONSTRO_BULLY) && me.tipo == BULLY)
				{

					jogo->monstros[me.id].energia = (SAUDE_MONSTRO_BULLY*1.6) / 2;

					tr = aleatorio(1, N_CASAS, me.contadorMovimento);

					_stprintf_s(procNome, 256,
						TEXT("%s %d %d %d %d"),
						TEXT("Monstro"), //0
						1,// tipo Monstro
						jogo->monstros[me.id].energia, //Energia
						tr, // valor de N,
						me.id // se é criado de raiz ou Dup 
						); //2


					ZeroMemory(&si, sizeof(STARTUPINFO)); //Set data to 0
					si.cb = sizeof(STARTUPINFO);

					_tprintf(TEXT("duplicando o monstro..: %d %d %d %d \n"), 1,
						jogo->monstros[me.id].energia, tr, me.id);

					_tprintf(procNome); _tprintf("\n");

					CreateProcess(NULL, procNome, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
				}

			}
		}


		ReleaseMutex(servidorMutex);

	}//fim while(1);

	return 0;
}

int _tmain(int argc, LPTSTR argv[]) {


	int tid = 0;

	//if (argc != 6) {
	//	return -1;
	//}



	mtipo = _ttoi(argv[1]);
	menergia = _ttoi(argv[2]);
	nSpaces = _ttoi(argv[3]);
	mdup = _ttoi(argv[4]);

	_tprintf(TEXT("criado procid : %d parametros: tipo : %d ener: %d n: %d dup : %d \n"),
		 GetCurrentProcessId(),_ttoi(argv[1]), _ttoi(argv[2]), _ttoi(argv[3]), _ttoi(argv[4]));



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

	WaitForSingleObject(servidorMutex, INFINITE);

	tid = criaMonstro(jogo, mtipo, menergia, nSpaces, mdup);

	_tprintf(TEXT("criando o monstro..: tipo : %d ener: %d n: %d dup : %d \n"), mtipo, menergia, nSpaces, mdup);

	ReleaseMutex(servidorMutex);

	if (tid == -1)
	{
		_tprintf(TEXT("Erro ao criar o monstro"));
		_gettch();
		return -1;
	}


	_tprintf(TEXT("Monstro Criado .: tipo %d dup: %d n %d x: %d y : %d \n"), 
		jogo->monstros[tid].tipo,
		mdup,
		jogo->monstros[tid].n_casas,
		jogo->monstros[tid].posicao.x,
		jogo->monstros[tid].posicao.y
		);


	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)atualizaMonstro, (LPVOID)tid, 0, NULL);

	WaitForSingleObject(hThread, INFINITE);


	//UnmapViewOfFile(jogo);

	//CloseHandle(hMapFile);


	return 0;
}
