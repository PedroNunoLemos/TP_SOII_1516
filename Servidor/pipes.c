
#include <windows.h>
#include <tchar.h>
#include "labirinto.h"
#include "../Controlador/Jogo.h"


#define PIPE_NAME TEXT("\\\\.\\pipe\\servidor_cl")
#define PIPE_NAME_INBOUND TEXT("\\\\.\\pipe\\cl_serv")

int fim = 0;

#define N_MAX_PLAYERS 10
#define TAM 25

HANDLE PlayersPipe[N_MAX_PLAYERS];

DWORD WINAPI AtendeCliente(LPVOID param);
DWORD WINAPI RecebeClientes(LPVOID param);




void startserver(Jogo jogo) {

	DWORD n;
	HANDLE hThread;
	TCHAR buf[TAM];

	//Invocar a thread que inscreve novos leitores
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecebeClientes, (LPVOID)&jogo, 0, NULL);
	n = 0;

	do {


		//Escrever para todos os leitores inscritos
		for (int i = 0; i < jogo.numjogadores; i++)

			if (!WriteFile(jogo.PlayersPipe[i], buf, _tcslen(buf)*sizeof(TCHAR), &n, NULL)) {
				_tperror(TEXT("[ERRO] Escrever no pipe... (WriteFile)\n"));
				exit(-1);
			}

		_tprintf(TEXT("[ESCRITOR] Enviei %d bytes aos %d leitores... (WriteFile)\n"), n, jogo.numjogadores);

	} while (_tcsncmp(buf, TEXT("fim"), 3));



	fim = 1;

	//Esperar a thread recebeLeitores terminar
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

}

DWORD WINAPI RecebeClientes(LPVOID param) {

	Jogo *jogo = param;

	HANDLE hpipe;
	TCHAR buf[TAM];

	while (!fim && jogo->numjogadores < 9) {

	_tprintf(TEXT("[ESCRITOR] Vou passar à criação de uma cópia do pipe '%s' ... (CreateNamedPipe)\n"), PIPE_NAME);
		jogo->PlayersPipe[jogo->numjogadores] = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_OUTBOUND, PIPE_WAIT | PIPE_TYPE_MESSAGE
			| PIPE_READMODE_MESSAGE, N_MAX_PLAYERS, TAM * sizeof(TCHAR), TAM * sizeof(TCHAR),
			1000, NULL);

		if (jogo->PlayersPipe[N_MAX_PLAYERS] == INVALID_HANDLE_VALUE) {
			_tperror(TEXT("Erro na ligação ao leitor!"));
			exit(-1);
		}

		hpipe = CreateNamedPipe(PIPE_NAME_INBOUND, PIPE_ACCESS_INBOUND, PIPE_WAIT | PIPE_TYPE_MESSAGE
			| PIPE_READMODE_MESSAGE, N_MAX_PLAYERS, TAM * sizeof(TCHAR), TAM * sizeof(TCHAR),
			1000, NULL);

		if (hpipe == INVALID_HANDLE_VALUE) {
			_tperror(TEXT("Erro na criação do pipe!"));
			exit(-1);
		}


		jogo->pipeJogo = hpipe;

		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtendeCliente, (LPVOID)jogo, 0, NULL);

		_tprintf(TEXT("[ESCRITOR] Esperar ligação de um leitor... (ConnectNamedPipe)\n"));
		if (!ConnectNamedPipe(jogo->PlayersPipe[jogo->numjogadores], NULL)) {
			_tperror(TEXT("Erro na ligação ao leitor!"));
			exit(-1);
		}

		 jogo->numjogadores++;
	
	}
	
	for (int i = 0; i < jogo->numjogadores; i++) {
		DisconnectNamedPipe(jogo->PlayersPipe[i]);
		_tprintf(TEXT("[ESCRITOR] Vou desligar o pipe... (CloseHandle)\n"));
		CloseHandle(jogo->PlayersPipe[i]);
	}

}


DWORD WINAPI AtendeCliente(LPVOID param) {

	Jogo *_jogo = param;

	HANDLE pipe = _jogo->pipeJogo;

	TCHAR buf[TAM];
	DWORD n;

	while (1)
	{


		//Ler do Pipe
		ReadFile(pipe, buf, TAM, &n, NULL);
		buf[n / sizeof(TCHAR)] = '\0';


		//escrver para todos

		for (int i = 0; i < _jogo->numjogadores; i++) {



			//WriteFile(PlayersPipe[i], buf, _tcsclen(buf)*sizeof(TCHAR), &n, NULL);
			WriteFile(_jogo->PlayersPipe[i], buf, _tcsclen(buf)*sizeof(TCHAR), &n, NULL);

		}

	}

}

