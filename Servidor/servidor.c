#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <strsafe.h>

#include "..\Controlador\pipes.h"
#include "..\Controlador\constantes.h"
#include "..\Controlador\memoria.h"
#include "..\Controlador\servidorinfo.h"


#include "JogoServidor.h"
#include "servidor.h"
#include "motorjogo.h"
#include "hist.h"

#define _CRT_SECURE_NO_WARNINGS

BOOL JOGO_ONLINE = FALSE, JogoCliente_COMECOU = FALSE;


HANDLE servidorMutex;

TCHAR nomeMemoria[] = TEXT("MapaGlobal");

DWORD total = 0;

TCHAR procNome[256];

STARTUPINFO si;
PROCESS_INFORMATION pi;
HANDLE processos[MAXINIMIGOS / 2];

JogoServidor *jogo;

HANDLE memoria;
HANDLE ticker;
HANDLE fich;

int desligados = 0;


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
	JogoServidor *tmppj;

	int i = 0;

	HANDLE hMutex = CreateMutex(NULL, TRUE, TEXT("SERVIDORDUNGEON"));
	servidorMutex = CreateMutex(NULL, FALSE, TEXT("ServidorMutex"));


	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{


		char ch = getchar();

		CloseHandle(hMutex);
		return -1;

	}


	/////////////////////////Mapeamento de memória/////////////////////////////////////////////

	memoria = CreateFileMapping(NULL, NULL, PAGE_READWRITE, 0, sizeof(JogoServidor), nomeMemoria);

	if (memoria == NULL) {
		_tprintf(TEXT("[ERRO] Criação de handle de memória - %d"), GetLastError());
		return -1;
	}

	jogo = (JogoServidor*)MapViewOfFile(memoria, FILE_MAP_WRITE, 0, 0, sizeof(JogoServidor));

	if (jogo == NULL) {
		return -1;
	}

	tmppj = malloc(sizeof(JogoServidor));

	CopyMemory(jogo, tmppj, sizeof(JogoServidor));

	free(tmppj);



	//////////////////////////////////////////////////////////////////////

	//jogo = malloc(sizeof(JogoServidor));
	jogo->jogadoresLigados = 0;
	jogo->totalLigacoes = 0;
	jogo->JogoTerminado = 0;


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

	UnmapViewOfFile(jogo);
	CloseHandle(memoria);

	for (int i = 0; i < MAXINIMIGOS / 2; i++)
	{
		CloseHandle(processos[i]);
		TerminateProcess(processos[i], 0);
	}

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

	DWORD n;
	int tn = 0;
	ServidorInfo serv;

	Historico *hist;
	hist = malloc(sizeof(Historico));


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

				if (jog->labDefeito == 0)
					criaJogo(jogo);
				else
					carregaLabirintoDefeito(jogo);

				jogo->monstrosCriados = 0;

				CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Temporizador, (LPVOID)NULL, 0, NULL);


				lancaMonstros();

				jog->id = id;

				criaJogador(jogo, jog);

				jogo->clientes[id].jogo = *jog;

				CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BonusCafeina, (LPVOID)id, 0, NULL);

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

			if (jog->jogador.podeMovimentar != 1) {
				jog->respostaComando = 99;
			}


			if (jogo->clientes[id].jogo.jogador.contadorMovimento == 0)
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

				jog->respostaComando = 51;

				jog->jogador.contadorMovimento = jog->jogador.lentidao;

			}


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

			}

			else
				jog->respostaComando = 0;
		}


		WaitForSingleObject(servidorMutex, INFINITE);

		jogo->clientes[id].jogo = *jog;

		if (jog->comando == 66) {


			AtualizaHistorico(hist);

			serv.hist = *hist;

			tn = 0;

			for (int k = 0; k < jogo->jogadoresLigados; k++)
				if (jogo->clientes[k].jogo.id >= 0)
				{
					serv.jogadores[k] = jogo->clientes[k].jogo.jogador;
					tn++;
				}

			serv.jogoIniciado = (int)JOGO_ONLINE;
			serv.jogadoresOnline = tn;

			WriteFile(cliente, &serv, sizeof(ServidorInfo), &n, NULL);

			free(hist);

		}


		escrevePipeJogoCliente(cliente, jog);




		for (i = 0; i < jogo->jogadoresLigados; i++)
		{

			if (jogo->clientes[i].jogo.id >= 0)
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
		}


		ReleaseMutex(servidorMutex);



	} while (1);





	if (!DisconnectNamedPipe(cliente)) {
		_tperror(TEXT("Erro ao desligar o pipe!"));
		exit(-1);
	}

	WaitForSingleObject(servidorMutex, INFINITE);




	desligaJogador(id);

	UtilizadorHist ut;

	swprintf(ut.nome, 50, jog->jogador.nome);

	ut.derrota = (JogadoresOnline(jogo) == 1) ? 0 : 1;
	ut.vitoria = !ut.derrota;
	ut.desistencia = (jog->jogador.saude == 0) ? 0 : 1;

	AdicionaHist(ut);



	ReleaseMutex(servidorMutex);

	if (JOGO_ONLINE == TRUE && jogo->JogoTerminado == 1)
	{

		free(jog);

		exit(-1);

	}



	return 0;
}


DWORD WINAPI Temporizador(LPVOID param) {

	//int id = (int)param;

	int i = 0;
	float temp = 0.000;


	temp = (1.00 / 15.00);
	temp = temp * 100;


	while (1)
	{


		WaitForSingleObject(servidorMutex, INFINITE);


		for (i = 0; i < jogo->jogadoresLigados; i++)
		{
			if (jogo->clientes[i].jogo.id >= 0)
			{

				if (jogo->clientes[i].jogo.jogador.saude <= 0)
				{
					jogo->clientes[i].jogo.jogador.saude = 0;
					jogo->clientes[i].jogo.jogador.podeMovimentar = 0;
				}

				jogo->clientes[i].jogo.jogador.contadorMovimento--;

				if (jogo->clientes[i].jogo.jogador.contadorMovimento <= 0)
					jogo->clientes[i].jogo.jogador.contadorMovimento = 0;
			}
		}

		for (i = 0; i < jogo->monstrosCriados; i++)
		{

			jogo->monstros[i].contadorMovimento--;

			if (jogo->monstros[i].contadorMovimento <= 0)
				jogo->monstros[i].contadorMovimento = 0;
		}



		for (i = 0; i < jogo->jogadoresLigados; i++)
		{

			if (jogo->clientes[i].jogo.id >= 0)
			{




				JogoCliente *tmp = &(jogo->clientes[i].jogo);

				tmp->respostaComando = 61;


				atualizaMapaEntreClientes(&(jogo->clientes[i].jogo), tmp);

				escrevePipeJogoCliente(jogo->clientes_atualizar[i], tmp);

			}
		}



		ReleaseMutex(servidorMutex);

		Sleep(temp);

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
	int oid = -1;

	WaitForSingleObject(servidorMutex, INFINITE);


	for (i = 0; i < jogo->jogadoresLigados; i++) {
		if (jogo->clientes[i].jogo.id >= 0)
		{
			if (i != id) {

				_stprintf(jogo->clientes[i].jogo.mensagem,
					TEXT("Utilizador %s Saiu!"), jogo->clientes[id].jogo.jogador.nome);

				escrevePipeJogoCliente(jogo->clientes_atualizar[i],
					&(jogo->clientes[i].jogo)
					);

			}
		}
	}

	for (i = 0; i < jogo->jogadoresLigados; i++) {
		if (jogo->clientes[i].jogo.id >= 0)
		{

			if (i == id) {

				if (jogo->jogadoresLigados >= 1) {

					CloseHandle(jogo->clientes_atualizar[i]);
					CloseHandle(jogo->clientes[i].ligacao);


					jogo->clientes[i].jogo.id = -1;

					desligados++;
				}continue;
			}
		}

	}

	//jogo->jogadoresLigados--;

	ReleaseMutex(servidorMutex);


	if (desligados == jogo->jogadoresLigados)
	{
		WaitForSingleObject(servidorMutex, INFINITE);

		jogo->JogoTerminado = 1;

		ReleaseMutex(servidorMutex);

		Sleep(1000);


	}
}


void  lancaMonstros() {

	int tr = 0;
	int tipo = 0;

	jogo->monstrosCriados = 0;

	for (int i = 0; i < MAXINIMIGOS / 2; i++) {


		tr = aleatorio(1, N_CASAS, i);

		tipo = ((i + 1) % 2);

		_stprintf_s(procNome, 256,
			TEXT("%s %d %d %d %d"),
			TEXT("Monstro"), //0
			tipo,// tipo Monstro
			tipo == 0 ? SAUDE_MONSTRO_DIST : SAUDE_MONSTRO_BULLY, //Energia
			tr, // valor de N,
			-1 // se é criado de raiz ou Dup 
			); //2

		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);

		CreateProcess(NULL, procNome, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		processos[i] = pi.hProcess;
		//CREATE_NO_WINDOW
	}
}


