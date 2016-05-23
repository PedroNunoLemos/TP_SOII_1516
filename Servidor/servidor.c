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

#define MOVE_MUTEX TEXT("moveMutex")

DWORD WINAPI AtendeCliente(LPVOID param);

BOOL JOGO_ONLINE = FALSE, JogoCliente_COMECOU = FALSE;

HANDLE hPipeA[MAXJOGADORES];
HANDLE moveMutex;

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

	WaitForSingleObject(hThread, INFINITE);

	free(jogo);
	return 0;
}

DWORD WINAPI AtendeCliente(LPVOID param) {


	DWORD i;
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

					criaJogador(jogo, TEXT(""), jog->pidCliente);

					atualizaMapaServidor(jogo, jog, jogo->jogadores[jogo->jogadoresLigados].posicao.x,
						jogo->jogadores[jogo->jogadoresLigados].posicao.y);

					atualizaMapaCliente(jogo, jog,
						jogo->jogadores[jogo->jogadoresLigados].posicao.x - 7,
						jogo->jogadores[jogo->jogadoresLigados].posicao.y - 7
						);

			

					jog->jogador = jogo->jogadores[jogo->jogadoresLigados];

					jogo->clientes[jogo->jogadoresLigados] = *jog;

					jogo->jogadoresLigados++;

					jog->respostaComando = 1;

					JOGO_ONLINE = TRUE;
					JogoCliente_COMECOU = 1;

				}
				else
					jog->respostaComando = 0;
			}

			else if (jog->comando == 5)
			{

				WaitForSingleObject(moveMutex, INFINITE);



				int x = 0;
				int y = 0;
				int r = 0;

				int ox = 0;
				int oy = 0;


				x = jog->jogador.posicao.x;
				y = jog->jogador.posicao.y;

				ox = x;
				oy = y;

				if (jog->moveuDirecao == 1) if (validaMovimentoBase(jogo->mapa, x, y - 1)) y--; //Mover Para Cima
				if (jog->moveuDirecao == 2) if (validaMovimentoBase(jogo->mapa, x, y + 1)) y++; //Mover Para Baixo
				if (jog->moveuDirecao == 3) if (validaMovimentoBase(jogo->mapa, x + 1, y))  x++; //Mover Para Esquerda
				if (jog->moveuDirecao == 4) if (validaMovimentoBase(jogo->mapa, x - 1, y))  x--; //Mover Para  Direita

				

				jog->jogador.posicao.x = x;
				jog->jogador.posicao.y = y;


				atualizaMapaServidor(jogo, jog, ox, oy);

				atualizaJogadorServidor(jogo, *jog);

				atualizaMapaCliente(jogo, jog, x - 7, y - 7);

				jog->respostaComando = 1;

				ReleaseMutex(moveMutex);

			}
			else if (jog->comando == 3)
			{
				if (JOGO_ONLINE == TRUE && JogoCliente_COMECOU == 1) {

					hPipeA[total] = criaPipeCliente();

					total++;

					jogo->jogadores[jogo->jogadoresLigados].pidJogador = jog->pidCliente;

					criaJogador(jogo, TEXT(""), jog->pidCliente);

					atualizaJogadorServidor(jogo, *jog);

					jog->jogador = jogo->jogadores[jogo->jogadoresLigados];

					atualizaMapaServidor(jogo, jog, 
						jogo->jogadores[jogo->jogadoresLigados].posicao.x,
						jogo->jogadores[jogo->jogadoresLigados].posicao.y);

					atualizaMapaCliente(jogo, jog,
						jogo->jogadores[jogo->jogadoresLigados].posicao.x - 7,
						jogo->jogadores[jogo->jogadoresLigados].posicao.y - 7
						);



					jogo->clientes[jogo->jogadoresLigados] = *jog;

					jogo->jogadoresLigados++;

					jog->respostaComando = 1;

				}

				else
					jog->respostaComando = 0;
			}
			else
				_tcscpy_s(jog->mensagem, 30, TEXT("nenhuma opcao valida"));


			

			escrevePipeJogoCliente(cliente, jog);

			//for (i = 0; i < jogo->jogadoresLigados; i++)
			//	lePipeJogoCliente(hPipeA[i],jog);

		}

	} while (1);





	if (!DisconnectNamedPipe(cliente)) {
		_tperror(TEXT("Erro ao desligar o pipe!"));
		exit(-1);
	}

	CloseHandle(cliente);

	if (jogo->jogadoresLigados > 0)
	{
		jogo->jogadores[jogo->jogadoresLigados].pidJogador = 0;
		jogo->jogadores[jogo->jogadoresLigados].posicao.x = -1;
		jogo->jogadores[jogo->jogadoresLigados].posicao.y = -1;
		jogo->jogadores[jogo->jogadoresLigados].saude = 10;
		jogo->jogadores[jogo->jogadoresLigados].lentidao = 5;

		jogo->jogadores[jogo->jogadoresLigados].qtdOranges = 0;
		jogo->jogadores[jogo->jogadoresLigados].qtdCafeinas = 0;
		jogo->jogadores[jogo->jogadoresLigados].qtdPedras = 0;
		jogo->jogadores[jogo->jogadoresLigados].qtdVitaminas = 0;


		jogo->jogadoresLigados--;

	}

	if (JOGO_ONLINE == TRUE && jogo->jogadoresLigados == 0)
	{
		free(jog);
		exit(-1);
	}



	return 0;
}


