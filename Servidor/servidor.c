#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <strsafe.h>

#include "..\Controlador\pipes.h"
#include "..\Controlador\constantes.h"

#include "JogoServidor.h"
#include "servidor.h"
#include "motorjogo.h"

#define _CRT_SECURE_NO_WARNINGS

#define MOVE_MUTEX TEXT("servidorMutex")

BOOL JOGO_ONLINE = FALSE, JogoCliente_COMECOU = FALSE;

HANDLE clientes[MAXJOGADORES];
HANDLE clientes_atualizar[MAXJOGADORES];

HANDLE servidorMutex;

DWORD total = 0;


JogoServidor *jogo;



//int _tmain(int argc, LPTSTR argv[]) {
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow) {

	HANDLE hThread = NULL;
	HANDLE hThreadRec = NULL;
	BOOL pLigado = FALSE;
	BOOL pLigadoRec = FALSE;
	DWORD dwThreadID = 0;
	HANDLE hPipe = INVALID_HANDLE_VALUE;
	HANDLE hPipeRec = INVALID_HANDLE_VALUE;


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
	jogo->totalLigacoes = 0;


	for (i = 0; i < MAXJOGADORES; i++) {



		hPipe = CreateNamedPipe(PIPE_ENVIO, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_WAIT | PIPE_TYPE_MESSAGE
			| PIPE_READMODE_MESSAGE, MAXJOGADORES, sizeof(JogoCliente), sizeof(JogoCliente),
			1000, NULL);


		hPipeRec = CreateNamedPipe(PIPE_RECECAO, PIPE_ACCESS_OUTBOUND | FILE_FLAG_OVERLAPPED, PIPE_WAIT | PIPE_TYPE_MESSAGE
			| PIPE_READMODE_MESSAGE, MAXJOGADORES, sizeof(JogoCliente), sizeof(JogoCliente),
			1000, NULL);


		if (hPipe == INVALID_HANDLE_VALUE) {
			exit(-1);
		}

		if (hPipeRec == INVALID_HANDLE_VALUE) {
			exit(-1);
		}

		clientes[jogo->totalLigacoes] = hPipe;
		clientes_atualizar[jogo->totalLigacoes] = hPipeRec;

		pLigado = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);


		if (pLigado) {




			hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtendeCliente, (LPVOID)jogo->totalLigacoes, 0, NULL);

			jogo->totalLigacoes++;

			if (hThread == NULL) {


				CloseHandle(clientes[jogo->totalLigacoes]);
				CloseHandle(clientes_atualizar[jogo->totalLigacoes]);


				jogo->totalLigacoes--;


				exit(-1);
			}
			else
			{
				//WaitForSingleObject(jogo->hThreadClientes[i], INFINITE);
				CloseHandle(hThread);
			}
		}
		else
		{
			CloseHandle(clientes[jogo->totalLigacoes]);
			CloseHandle(clientes_atualizar[jogo->totalLigacoes]);
		}

	}



	free(jogo);
	return 0;
}



DWORD WINAPI AtendeCliente(LPVOID param) {


	int i, k;
	DWORD nlidos;

	BOOL ret = FALSE;

	//trabalhar somente com 1 cliente especifico
	HANDLE cliente = clientes[(int)param];


	JogoCliente *jog;
	jog = malloc(sizeof(JogoCliente));




	do {



		ret = ReadFile(cliente, jog, sizeof(JogoCliente), &nlidos, NULL);

		if (!ret || !nlidos)
			break;


		if (jog->comando == 1)
		{

			if (JOGO_ONLINE == FALSE && JogoCliente_COMECOU == FALSE)
			{


				criaJogo(jogo);

				criaJogador(jogo, jog);

				atualizaJogadorServidor(jogo, jog);

				atualizaMapaServidor(jogo, jog, jogo->jogadores[jogo->jogadoresLigados].posicao.x,
					jogo->jogadores[jogo->jogadoresLigados].posicao.y);

				atualizaMapaCliente(jogo, jog,
					jogo->jogadores[jogo->jogadoresLigados].posicao.x - (MAXVISX/2),
					jogo->jogadores[jogo->jogadoresLigados].posicao.y - (MAXVISY/2)
					);



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

			WaitForSingleObject(servidorMutex, INFINITE);



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

			atualizaMapaCliente(jogo, jog, x - (MAXVISX/2), y - (MAXVISY/2));

			jog->respostaComando = 1;

			ReleaseMutex(servidorMutex);

		}
		else if (jog->comando == 3)
		{
			if (JOGO_ONLINE == TRUE && JogoCliente_COMECOU == 1) {



				criaJogador(jogo, jog);

				
				atualizaMapaServidor(jogo, jog,
					jogo->jogadores[jogo->jogadoresLigados].posicao.x,
					jogo->jogadores[jogo->jogadoresLigados].posicao.y);

				atualizaMapaCliente(jogo, jog,
					jogo->jogadores[jogo->jogadoresLigados].posicao.x - (MAXVISX/2),
					jogo->jogadores[jogo->jogadoresLigados].posicao.y - (MAXVISY/2)
					);



				jogo->jogadoresLigados++;

				jog->respostaComando = 1;

			}
			else if (jog->comando == 6)
			{

				int x = 0;
				int y = 0;
				int r = 0;

				int ox = 0;
				int oy = 0;


				x = jog->jogador.posicao.x;
				y = jog->jogador.posicao.y;

				ox = x;
				oy = y;
				

				atualizaMapaServidor(jogo, jog, ox, oy);
				atualizaMapaCliente(jogo, jog, x - (MAXVISX/2), y - (MAXVISY/2));


				jog->respostaComando = 1;
			}

			else
				jog->respostaComando = 0;
		}




		escrevePipeJogoCliente(cliente, jog);

		WaitForSingleObject(servidorMutex, INFINITE);

		for (i = 0; i < jogo->jogadoresLigados; i++)
		{


			if (
				jogo->jogoClientes[i].pidCliente != jog->pidCliente
				&& jogo->jogadores[i].pidJogador != 0 && jogo->jogoClientes[i].pidCliente != 0)
			{

				JogoCliente *tmp = &(jogo->jogoClientes[i]);


				escrevePipeJogoCliente(clientes_atualizar[i], tmp);

			

			}


		}

		ReleaseMutex(servidorMutex);




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



