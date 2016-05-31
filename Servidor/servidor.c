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

#define MOVE_MUTEX TEXT("moveMutex")

BOOL JOGO_ONLINE = FALSE, JogoCliente_COMECOU = FALSE;

HANDLE hPipeA[MAXJOGADORES];

HANDLE moveMutex;

DWORD total = 0;

JogoServidor *jogo;



//int _tmain(int argc, LPTSTR argv[]) {
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow) {

	HANDLE hThread = NULL;
	HANDLE hThreadRec = NULL;
	BOOL pLigado = FALSE;
	DWORD dwThreadID = 0;
	HANDLE hPipe = INVALID_HANDLE_VALUE;
	HANDLE hPipeRec = INVALID_HANDLE_VALUE;

	PipesServer *pipes;


	int i;


	HANDLE hMutex = CreateMutex(NULL, TRUE, TEXT("SERVIDORDUNGEON"));

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{


		char ch = getchar();

		CloseHandle(hMutex);
		return -1;
	}

	pipes = malloc(sizeof(PipesServer));
	pipes->hPipe = INVALID_HANDLE_VALUE;
	pipes->hPipeRec = INVALID_HANDLE_VALUE;

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

		pLigado = ConnectNamedPipe(hPipe, NULL)  ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

		if (pLigado) {

			jogo->totalLigacoes++;

			pipes->hPipe = hPipe;

			hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtendeCliente, (LPVOID)pipes, 0, NULL);


			if (hThread == NULL ) {

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
			CloseHandle(pipes->hPipe);
			CloseHandle(pipes->hPipeRec);
		}

	}



	free(jogo);
	free(pipes);
	return 0;
}



DWORD WINAPI AtendeCliente(LPVOID param) {


	DWORD i;
	DWORD nlidos;

	BOOL ret = FALSE;

	PipesServer *pipes = param;

	//trabalhar somente com 1 cliente especifico
	HANDLE cliente = (HANDLE)pipes->hPipe;



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

				jogo->jogoClientes[jogo->jogadoresLigados] = *jog;

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



				jogo->jogoClientes[jogo->jogadoresLigados] = *jog;

				jogo->jogadoresLigados++;

				swprintf(jog->mensagem, 256, TEXT("%s Ligou-se"), jog->jogador.nome);

				jog->respostaComando = 1;

			}

			else
				jog->respostaComando = 0;
		}
		else
			_tcscpy_s(jog->mensagem, 30, TEXT("nenhuma opcao valida"));




		escrevePipeJogoCliente(cliente, jog);

		//for (i = 0; i < jogo->jogadoresLigados; i++) {
		//	if (jogo->jogoClientes[i].pidCliente != jog->pidCliente)
		//		escrevePipeJogoCliente(jogo->pipeClientes[i], &(jogo->jogoClientes[i]));
		//}

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



