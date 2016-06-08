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

		jogo->clientes[jogo->totalLigacoes].ligacao = hPipe;
		jogo->clientes_atualizar[jogo->totalLigacoes] = hPipeRec;

		pLigado = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);


		if (pLigado) {




			hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtendeCliente, (LPVOID)jogo->totalLigacoes, 0, NULL);

			jogo->totalLigacoes++;

			if (hThread == NULL) {


				CloseHandle(jogo->clientes[jogo->totalLigacoes].ligacao);
				CloseHandle(jogo->clientes_atualizar[jogo->totalLigacoes]);


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
			CloseHandle(jogo->clientes[jogo->totalLigacoes].ligacao);
			CloseHandle(jogo->clientes_atualizar[jogo->totalLigacoes]);
		}

	}



	free(jogo);
	return 0;
}



DWORD WINAPI AtendeCliente(LPVOID param) {


	int i;
	DWORD nlidos;

	BOOL ret = FALSE;

	//trabalhar somente com 1 cliente especifico
	int id = (int)param;


	HANDLE cliente = jogo->clientes[id].ligacao;


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

				jogo->clientes[id].jogo = *jog;

				atualizaMapaServidor(jogo, jog, jogo->clientes[id].jogo.jogador.posicao.x,
					jogo->clientes[id].jogo.jogador.posicao.y);

				atualizaMapaCliente(jogo, jog,
					jogo->clientes[id].jogo.jogador.posicao.x - (MAXVISX/2),
					jogo->clientes[id].jogo.jogador.posicao.y - (MAXVISY/2)
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

			jogo->clientes[id].jogo = *jog;


			int x = 0;
			int y = 0;
			int r = 0;

			int ox = 0;
			int oy = 0;


			x = jogo->clientes[id].jogo.jogador.posicao.x;
			y = jogo->clientes[id].jogo.jogador.posicao.y;

			ox = x;
			oy = y;

			if (jog->moveuDirecao == 1) if (validaMovimentoBase(jogo->mapa, x, y - 1)) y--; //Mover Para Cima
			if (jog->moveuDirecao == 2) if (validaMovimentoBase(jogo->mapa, x, y + 1)) y++; //Mover Para Baixo
			if (jog->moveuDirecao == 3) if (validaMovimentoBase(jogo->mapa, x + 1, y))  x++; //Mover Para Esquerda
			if (jog->moveuDirecao == 4) if (validaMovimentoBase(jogo->mapa, x - 1, y))  x--; //Mover Para  Direita



			jog->jogador.posicao.x = x;
			jog->jogador.posicao.y = y;

			atualizaMapaServidor(jogo, jog, ox, oy);

			atualizaMapaCliente(jogo, jog, x - (MAXVISX/2), y - (MAXVISX/2));

			jog->respostaComando = 1;

			ReleaseMutex(servidorMutex);

		}
		else if (jog->comando == 3)
		{
			if (JOGO_ONLINE == TRUE && JogoCliente_COMECOU == 1) {



				criaJogador(jogo, jog);

				jogo->clientes[id].jogo = *jog;

				atualizaMapaServidor(jogo, jog,
					jogo->clientes[id].jogo.jogador.posicao.x,
					jogo->clientes[id].jogo.jogador.posicao.y);

				atualizaMapaCliente(jogo, jog,
					jogo->clientes[id].jogo.jogador.posicao.x - (MAXVISX/2),
					jogo->clientes[id].jogo.jogador.posicao.y - (MAXVISY/2)
					);



				jogo->jogadoresLigados++;

				jog->respostaComando = 1;

			}

			else
				jog->respostaComando = 0;
		}


		jogo->clientes[id].jogo = *jog;

		escrevePipeJogoCliente(cliente, jog);

		WaitForSingleObject(servidorMutex, INFINITE);

		for (i = 0; i < jogo->jogadoresLigados; i++)
		{


			if (
				jogo->clientes[i].jogo.pidCliente != jog->pidCliente && jogo->clientes[i].jogo.pidCliente != 0)
			{

				JogoCliente *tmp = &(jogo->clientes[i].jogo);

				atualizaMapaCliente(jogo, tmp,
					jogo->clientes[id].jogo.jogador.posicao.x - (MAXVISX / 2),
					jogo->clientes[id].jogo.jogador.posicao.y - (MAXVISY / 2)
				);

				jogo->clientes[i].jogo.mapa = tmp->mapa;

				escrevePipeJogoCliente(jogo->clientes_atualizar[i], tmp);



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
		jogo->clientes[id].jogo.jogador.pidJogador = 0;
		jogo->clientes[id].jogo.jogador.posicao.x = -1;
		jogo->clientes[id].jogo.jogador.posicao.y = -1;
		jogo->clientes[id].jogo.jogador.saude = 10;
		jogo->clientes[id].jogo.jogador.lentidao = 5;

		jogo->clientes[id].jogo.jogador.qtdOranges = 0;
		jogo->clientes[id].jogo.jogador.qtdCafeinas = 0;
		jogo->clientes[id].jogo.jogador.qtdPedras = 0;
		jogo->clientes[id].jogo.jogador.qtdVitaminas = 0;


		jogo->jogadoresLigados--;

	}

	if (JOGO_ONLINE == TRUE && jogo->jogadoresLigados == 0)
	{
		free(jog);
		exit(-1);
	}



	return 0;
}



