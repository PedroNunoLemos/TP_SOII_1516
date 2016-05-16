#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <strsafe.h>

#include "..\Controlador\pipes.h"
#include "..\Controlador\constantes.h"

#include "JogoServidor.h"
#include "motorjogo.h"

#define _CRT_SECURE_NO_WARNINGS

DWORD WINAPI AtendeCliente(LPVOID param);

BOOL JOGO_ONLINE = FALSE, JogoCliente_COMECOU = FALSE;

HANDLE hPipeA[MAXJOGADORES];

DWORD total = 0;

JogoServidor *jogo;



//int _tmain(int argc, LPTSTR argv[]) {
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow) {

	HANDLE hThread = NULL;
	BOOL pLigado = FALSE;
	DWORD dwThreadID = 0;
	HANDLE hPipe = INVALID_HANDLE_VALUE;


	int i;


	HANDLE hMutex = CreateMutex(NULL, TRUE, TEXT("SERVIDORDUNGEON"));

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		

		char ch = getchar();

		CloseHandle(hMutex);
		return -1;
	}

	jogo = malloc(sizeof(JogoServidor));
	jogo->jogadoresLigados = 0;
	


	for (i = 0; i < MAXJOGADORES; i++) {

	
		hPipe = criaPipeEscutaServidor();

		if (hPipe == INVALID_HANDLE_VALUE) {
			exit(-1);
		}


		pLigado = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);


		if (pLigado) {


			hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtendeCliente, (LPVOID)hPipe, 0, NULL);

			if (hThread == NULL) {
				exit(-1);
			}
			else
				CloseHandle(hThread);
		}
		else
			CloseHandle(hPipe);

	}

	free(jogo);
	return 0;
}

DWORD WINAPI AtendeCliente(LPVOID param) {


	DWORD nlidos, nescritos, i;
	BOOL ret = FALSE;

	JogoCliente *jog;
	jog = malloc(sizeof(JogoCliente));

	//trabalhar somente com 1 cliente especifico
	HANDLE cliente = (HANDLE)param;

	do {


		ret = lePipeJogoClienteComRetVal(cliente, jog);

		if (ret == 1)
			break;


		if (ret == 0)
		{

			if (jog->comando == 1)
			{

				if (JOGO_ONLINE == FALSE && JogoCliente_COMECOU == FALSE)
				{
					hPipeA[total] = criaPipeCliente();

					total++;


					jogo->jogadores[jogo->jogadoresLigados].pidJogador = jog->pidCliente;

					criaJogo(jogo);

					criaJogador(jogo, "", jog->pidCliente);
	
					atualizaMapaCliente(jogo, jog,
						jogo->jogadores[jogo->jogadoresLigados].posicao.x,
						jogo->jogadores[jogo->jogadoresLigados].posicao.y
						);


					jogo->jogadoresLigados++;

					jog->respostaComando = 1;

					JOGO_ONLINE = TRUE;
					JogoCliente_COMECOU = 1;

				}
				else
					jog->respostaComando = 0;
			}

			else if (jog->comando == 2)
			{


				jog->respostaComando = 0;

			}
			else if (jog->comando == 3)
			{
				if (JOGO_ONLINE == TRUE && JogoCliente_COMECOU == 1) {

					hPipeA[total] = criaPipeCliente();

					total++;

					jog->respostaComando = 1;

					jogo->jogadores[jogo->jogadoresLigados].pidJogador = jog->pidCliente;

					jogo->jogadoresLigados++;

				}

				else
					jog->respostaComando = 0;
			}
			else
				_tcscpy_s(jog->mensagem, 30, TEXT("nenhuma opcao valida"));


			escrevePipeJogoCliente(cliente, jog);

			for (i = 0; i < total; i++)
				escrevePipeJogoCliente(hPipeA[i], jog);

		}

	} while (1);





	if (!DisconnectNamedPipe(cliente)) {
		_tperror(TEXT("Erro ao desligar o pipe!"));
		exit(-1);
	}

	CloseHandle(cliente);

	if (jogo->jogadoresLigados > 0)
		jogo->jogadoresLigados--;

	if (JOGO_ONLINE == TRUE && jogo->jogadoresLigados == 0)
	{
		free(jog);
		exit(-1);
	}



	return 0;
}
