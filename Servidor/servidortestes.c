#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <strsafe.h>

#include "..\Controlador\pipes.h"
#include "..\Controlador\constantes.h"

#include "JogoServidor.h"

#define _CRT_SECURE_NO_WARNINGS

DWORD WINAPI AtendeCliente(LPVOID param);


BOOL JOGO_ONLINE = FALSE, JogoCliente_COMECOU = FALSE;

HANDLE hPipeA[MAXJOGADORES];

DWORD total = 0;

JogoServidor *jogo;



int _tmain(int argc, LPTSTR argv[]) {

	HANDLE hThread = NULL;
	BOOL pLigado = FALSE;
	DWORD dwThreadID = 0;
	HANDLE hPipe = INVALID_HANDLE_VALUE;


	int i;

#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	jogo = malloc(sizeof(JogoServidor));
	jogo->jogadoresLigados = 0;

	for (i = 0; i < MAXJOGADORES; i++) {

		//Duplo sentido na comunicação

		_tprintf(TEXT("[SERVIDOR] Vou criar o pipe '%s' de recepcao... (CreateNamedPipe)\n"), PIPE_ENVIO);

		hPipe = criaPipeEscutaServidor();

		if (hPipe == INVALID_HANDLE_VALUE) {
			_tprintf(TEXT("CreateNamedPipe falhou, erro=%d\n"), GetLastError());
			exit(-1);
		}

		_tprintf(TEXT("[SERVIDOR] Esperar ligação de um cliente... (ConnectNamedPipe)\n"));

		pLigado = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

		//Lancar thread para cada novo cliente (hPipe[i] ou i)

		if (pLigado) {

			_tprintf(TEXT("[SERVIDOR] Um cliente ligou-se...Vou criar uma thread p/ ele\n"));;

			hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtendeCliente, (LPVOID)hPipe, 0, NULL);

			if (hThread == NULL) {
				_tprintf(TEXT("Erro na criação da thread. Erro=%d. \n"), GetLastError());
				exit(-1);
			}
			else
				CloseHandle(hThread);
		}
		else
			CloseHandle(hPipe);

	}
	_tprintf(TEXT("Sistema vai encerrar!"));
	free(jogo);
	return 0;
}

DWORD WINAPI AtendeCliente(LPVOID param) {


	DWORD nlidos, nescritos, i;
	BOOL ret = FALSE;

	JogoCliente jog;


	//trabalhar somente com 1 cliente especifico
	HANDLE cliente = (HANDLE)param;

	do {


		ret = lePipeJogoCliente(cliente, &jog);

		if (ret != 0)
		{

			if (jog.comando == 1)
			{

				if (JOGO_ONLINE == FALSE) {

					hPipeA[total] = criaPipeCliente();

					total++;

					jogo->jogadores[jogo->jogadoresLigados].pidJogador = jog.pidCliente;

					jogo->jogadoresLigados++;
					jog.respostaComando = 1;
					JOGO_ONLINE = TRUE;


				}
				else
					jog.respostaComando = 0;
			}

			else if (jog.comando == 2)
			{
				if (JOGO_ONLINE == TRUE && JogoCliente_COMECOU == FALSE &&
					jog.pidCliente == jogo->jogadores[0].pidJogador) {

					jog.respostaComando = 1;

					JogoCliente_COMECOU = TRUE;

				}
				else
					jog.respostaComando = 0;

			}
			else if (jog.comando == 3)
			{
				if (JOGO_ONLINE == TRUE && JogoCliente_COMECOU == 1) {

					hPipeA[total] = criaPipeCliente();

					total++;
					jog.respostaComando = 1;

					jogo->jogadores[jogo->jogadoresLigados].pidJogador = jog.pidCliente;

					jogo->jogadoresLigados++;

				}

				else
					jog.respostaComando = 0;
			}
			else
				_tcscpy_s(jog.buf, 30, TEXT("nenhuma opcao valida"));


			escrevePipeJogoCliente(cliente, &jog);

			for (i = 0; i < total; i++)
				escrevePipeJogoCliente(hPipeA[i], &jog);

		}

	} while (1);



	_tprintf(TEXT("[SERVIDOR] Vou desligar o pipe(DisconnectNamedPipe/CloseHandle)\n"));


	if (!DisconnectNamedPipe(cliente)) {
		_tperror(TEXT("Erro ao desligar o pipe!"));
		exit(-1);
	}

	//Sleep(2000);
	_tprintf(TEXT("[SERVIDOR] Vou desligar o pipe... (CloseHandle)\n"));
	CloseHandle(cliente);

	return 0;
}
