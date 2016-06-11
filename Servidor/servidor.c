#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <strsafe.h>

#include "..\Controlador\pipes.h"
#include "..\Controlador\constantes.h"
#include "..\Controlador\memoria.h"

#include "JogoServidor.h"
#include "servidor.h"
#include "motorjogo.h"

#define _CRT_SECURE_NO_WARNINGS

BOOL JOGO_ONLINE = FALSE, JogoCliente_COMECOU = FALSE;


HANDLE servidorMutex;

TCHAR nomeMemoria[] = TEXT("MapaGlobal");

DWORD total = 0;

TCHAR procNome[256];

STARTUPINFO si;
PROCESS_INFORMATION pi;

JogoServidor *jogo;
MemoriaPartilhada *jogomem;

HANDLE memoria;
HANDLE ticker;


//int _tmain(int argc, LPTSTR argv[]) {
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow) {

	HANDLE hThread = NULL;
	HANDLE hThreadRec = NULL;

	HANDLE hThreadTime = NULL;

	BOOL pLigado = FALSE;
	BOOL pLigadoRec = FALSE;
	DWORD dwThreadID = 0;
	HANDLE hPipe = INVALID_HANDLE_VALUE;
	HANDLE hPipeRec = INVALID_HANDLE_VALUE;

	MemoriaPartilhada *tmppj;


	int i;


	HANDLE hMutex = CreateMutex(NULL, TRUE, TEXT("SERVIDORDUNGEON"));
	servidorMutex = CreateMutex(NULL, FALSE, TEXT("_servidorMutex"));


	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{


		char ch = getchar();

		CloseHandle(hMutex);
		return -1;

	}


	/////////////////////////Mapeamento de memória/////////////////////////////////////////////

	memoria = CreateFileMapping(NULL, NULL, PAGE_READWRITE, 0, sizeof(MemoriaPartilhada), nomeMemoria);

	if (memoria == NULL) {
		_tprintf(TEXT("[ERRO] Criação de handle de memória - %d"), GetLastError());
		return -1;
	}

	jogomem = (MemoriaPartilhada*)MapViewOfFile(memoria, FILE_MAP_WRITE, 0, 0, sizeof(MemoriaPartilhada));

	if (jogomem == NULL) {
		return -1;
	}

	tmppj = malloc(sizeof(MemoriaPartilhada));

	CopyMemory(jogomem, tmppj, sizeof(MemoriaPartilhada));

	free(tmppj);



	//////////////////////////////////////////////////////////////////////

	jogo = malloc(sizeof(JogoServidor));
	jogo->jogadoresLigados = 0;
	jogo->totalLigacoes = 0;
	jogo->instantes = 0;

	jogomem->mapa = jogo->mapa;

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

	UnmapViewOfFile(jogomem);
	CloseHandle(memoria);

	free(jogo);
	return 0;
}




DWORD WINAPI AtendeCliente(LPVOID param) {


	int i;
	int x = 0;
	int y = 0;
	int r = 0;

	int ox = 0;
	int oy = 0;


	DWORD nlidos;
	DWORD retwait;

	BOOL ret = FALSE;

	//trabalhar somente com 1 cliente especifico
	int id = (int)param;


	HANDLE cliente = jogo->clientes[id].ligacao;
	HANDLE lnt_thr;


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



				//lancaMonstros();
				


				jog->id = id;

				criaJogador(jogo, jog);

				jogo->clientes[id].jogo = *jog;

				CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BonusCafeina, (LPVOID)id, 0, NULL);
				CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Temporizador, (LPVOID)id, 0, NULL);

				atualizaJogadorCliente(jogo, jog);

				atualizaMapaServidor(jogo, jog, jogo->clientes[id].jogo.jogador.posicao.x,
					jogo->clientes[id].jogo.jogador.posicao.y);

				atualizaPosicao(jogo, jog, jogo->clientes[id].jogo.jogador.posicao.x,
					jogo->clientes[id].jogo.jogador.posicao.y);

				if (jog->jogador.efeitoCafeina == 1)
					CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BonusCafeina, (LPVOID)id, 0, NULL);

				atualizaMapaCliente(jogo, jog,
					jogo->clientes[id].jogo.jogador.posicao.x - (MAXVISX / 2),
					jogo->clientes[id].jogo.jogador.posicao.y - (MAXVISY / 2)
					);

				jogo->jogadoresLigados++;

				jog->respostaComando = 1;
				jog->pode = 1;

				JOGO_ONLINE = TRUE;

				JogoCliente_COMECOU = 1;

			}
			else
				jog->respostaComando = 0;
		}

		else if (jog->comando == 5)	
		{

			WaitForSingleObject(servidorMutex, INFINITE);


			atualizaJogadorCliente(jogo, jog);

			forcaDadosServidor(jogo, jog);


	
			if ((jogo->clientes[id].jogo.pode == 1))
			{

				x = jogo->clientes[id].jogo.jogador.posicao.x;
				y = jogo->clientes[id].jogo.jogador.posicao.y;

				ox = x;
				oy = y;

				if (jog->moveuDirecao == 1) if (validaMovimentoJogador(jogo, jog, x, y - 1)) y--; //Mover Para Cima
				if (jog->moveuDirecao == 2) if (validaMovimentoJogador(jogo, jog, x, y + 1)) y++; //Mover Para Baixo
				if (jog->moveuDirecao == 3) if (validaMovimentoJogador(jogo, jog, x + 1, y))  x++; //Mover Para Esquerda
				if (jog->moveuDirecao == 4) if (validaMovimentoJogador(jogo, jog, x - 1, y))  x--; //Mover Para  Direita


				jog->jogador.posicao.x = x;
				jog->jogador.posicao.y = y;

				atualizaMapaServidor(jogo, jog, ox, oy);


				atualizaPosicao(jogo, jog, x, y);

				if (jog->jogador.efeitoCafeina == 1)
					CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BonusCafeina, (LPVOID)id, 0, NULL);

				atualizaMapaCliente(jogo, jog, x - (MAXVISX / 2), y - (MAXVISX / 2));

				jog->pode = 0;

			}

			jog->respostaComando = 51;

			ReleaseMutex(servidorMutex);



		}
		else if (jog->comando == 4) //escolher usar pedra / n Pedra
		{

			WaitForSingleObject(servidorMutex, INFINITE);

			atualizaJogadorCliente(jogo, jog);
			forcaDadosServidor(jogo, jog);

			ReleaseMutex(servidorMutex);

			jog->respostaComando = 1;


		}
		else if (jog->comando == 3)
		{
			if (JOGO_ONLINE == TRUE && JogoCliente_COMECOU == 1) {

				WaitForSingleObject(servidorMutex, INFINITE);

				jog->id = id;

				criaJogador(jogo, jog);

				jogo->clientes[id].jogo = *jog;

				CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BonusCafeina, (LPVOID)id, 0, NULL);
				CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Temporizador, (LPVOID)id, 0, NULL);

				atualizaJogadorCliente(jogo, jog);

				atualizaMapaServidor(jogo, jog,
					jogo->clientes[id].jogo.jogador.posicao.x,
					jogo->clientes[id].jogo.jogador.posicao.y);


				atualizaPosicao(jogo, jog, jogo->clientes[id].jogo.jogador.posicao.x,
					jogo->clientes[id].jogo.jogador.posicao.y);

				if (jog->jogador.efeitoCafeina == 1)
					CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BonusCafeina, (LPVOID)id, 0, NULL);

				atualizaMapaCliente(jogo, jog,
					jogo->clientes[id].jogo.jogador.posicao.x - (MAXVISX / 2),
					jogo->clientes[id].jogo.jogador.posicao.y - (MAXVISY / 2)
					);

				jogo->jogadoresLigados++;

				ReleaseMutex(servidorMutex);

				jog->respostaComando = 1;

				jog->pode = 1;

			}

			else
				jog->respostaComando = 0;
		}


		WaitForSingleObject(servidorMutex, INFINITE);

		jogo->clientes[id].jogo = *jog;

		escrevePipeJogoCliente(cliente, jog);


		for (i = 0; i < jogo->jogadoresLigados; i++)
		{


			if (
				jogo->clientes[i].jogo.pidCliente != jog->pidCliente &&
				jogo->clientes[i].jogo.pidCliente != 0)
			{



				JogoCliente *tmp = &(jogo->clientes[i].jogo);

				tmp->respostaComando = 51;


				atualizaMapaCliente(jogo, tmp,
					tmp->jogador.posicao.x - (MAXVISX / 2),
					tmp->jogador.posicao.y - (MAXVISY / 2)
					);


				atualizaMapaEntreClientes(&(jogo->clientes[i].jogo), tmp);

				escrevePipeJogoCliente(jogo->clientes_atualizar[i], tmp);


			}


		}


		ReleaseMutex(servidorMutex);



	} while (1);





	if (!DisconnectNamedPipe(cliente)) {
		_tperror(TEXT("Erro ao desligar o pipe!"));
		exit(-1);
	}

	desligaJogador(id);

	if (JOGO_ONLINE == TRUE && jogo->jogadoresLigados == 0)
	{
		free(jog);
		exit(-1);
	}



	return 0;
}


DWORD WINAPI Temporizador(LPVOID param) {

	int id = (int)param;

	int lent = LENTIDAO_JOG_INI;
	float temp = 0.000;

	while (1)
	{

		if (jogo->clientes[id].jogo.pidCliente != 0)
		{


			lent = jogo->clientes[id].jogo.jogador.lentidao;

			temp = (1.00 / 15.00);
			temp = temp*10.00 / lent;

			Sleep(temp);

			WaitForSingleObject(servidorMutex, INFINITE);

			jogo->clientes[id].jogo.pode = 1;

			jogo->clientes[id].jogo.respostaComando = 72;


			ReleaseMutex(servidorMutex);




		}

	}
	return 0;

}

DWORD WINAPI BonusCafeina(LPVOID param) {

	int id = (int)param;

	//repor a lentidao do utilizador
	if (jogo->clientes[id].jogo.pidCliente != 0)
	{

		Sleep(EFEITO_CAFEINA);

		WaitForSingleObject(servidorMutex, INFINITE);

		jogo->clientes[id].jogo.jogador.lentidao += 2;

		if (jogo->clientes[id].jogo.jogador.lentidao >= LENTIDAO_JOG_INI)
			jogo->clientes[id].jogo.jogador.lentidao = LENTIDAO_JOG_INI;

		jogo->clientes[id].jogo.jogador.efeitoCafeina = 0;

		jogo->clientes[id].jogo.respostaComando = 71;

		
		escrevePipeJogoCliente(jogo->clientes_atualizar[id],
			&(jogo->clientes[id].jogo)
			);

		ReleaseMutex(servidorMutex);


	}



	return 0;
}


void desligaJogador(int id) {

	int i;
	int del;

	del = 0;

	WaitForSingleObject(servidorMutex, INFINITE);


	for (i = 0; i < jogo->jogadoresLigados; i++) {
		if (i != id) {

			_stprintf(jogo->clientes[i].jogo.mensagem,
				TEXT("Utilizador %s Saiu!"), jogo->clientes[id].jogo.jogador.nome);

			escrevePipeJogoCliente(jogo->clientes_atualizar[i],
				&(jogo->clientes[i].jogo)
				);

		}
	}

	for (i = 0; i < jogo->jogadoresLigados; i++) {

		if (i == id) {

			if (jogo->jogadoresLigados > 1) {

				CloseHandle(jogo->clientes_atualizar[i]);
				CloseHandle(jogo->clientes[i].ligacao);


				jogo->clientes[i] = jogo->clientes[i + 1];
				jogo->clientes_atualizar[i] = jogo->clientes_atualizar[i + 1];

				del = 1;
			}
			continue;
		}

		if (del) {

			jogo->clientes[i] = jogo->clientes[i + 1];
			jogo->clientes_atualizar[i] = jogo->clientes_atualizar[i + 1];

		}

	}

	jogo->jogadoresLigados--;

	ReleaseMutex(servidorMutex);
}


void  lancaMonstros() {

	int tr = 0;

	for (int i = 0; i < MAXINIMIGOS/2; i++) {


		_stprintf_s(procNome, 256,
			TEXT("%s %d %d %d %d %d"),
			TEXT("Monstro"), //0
			jogo->monstros[i].tipo, //1 
			jogo->monstros[i].posicao.x, //2
			jogo->monstros[i].posicao.y, //3
			i,//4
			jogo->monstros[i].energia); //5

		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);


		CreateProcess(NULL, procNome, NULL, NULL, 0, 0, NULL, NULL, &si, &pi);

	}
}


