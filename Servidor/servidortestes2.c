//#include <windows.h>
//#include <tchar.h>
//#include <io.h>
//#include <fcntl.h>
//#include <stdio.h>
//#include <strsafe.h>
//#include <tchar.h>
//
//#include "labirinto.h"
//#include "..\Controlador\pipes.h"
//#include "..\Controlador\constantes.h"
//#include "JogoServidor.h"
//#include "servidor.h"
//
//JogoServidor *jogo;
//
//DWORD WINAPI testes(LPVOID param);
//int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow)
//{
//
//
//	HANDLE hThread = NULL;
//
//	HANDLE hMutex = CreateMutex(NULL, TRUE, TEXT("SERVIDORDUNGEON"));
//
//	if (GetLastError() == ERROR_ALREADY_EXISTS)
//	{
//		MessageBox(NULL, TEXT("Dungeon"), TEXT("Servidor já a correr."), MB_OK);
//		CloseHandle(hMutex);
//		return -1;
//	}
//
//	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)testes, 0, 0, NULL);
//
//
//	return 0;
//}
//
//
//DWORD WINAPI testes(LPVOID param)
//{
//
//	int i = 0;
//
//	return 0;
//}
//
//DWORD WINAPI EscutaServidor(LPVOID param) {
//
//	HANDLE hThread = NULL;
//	BOOL pLigado = FALSE;
//	DWORD dwThreadID = 0;
//
//	int i = 0;
//
//
//
//	jogo = malloc(sizeof(JogoServidor));
//	jogo->jogadoresLigados = 0;
//	jogo->ClienteLigado = 0;
//	jogo->totalLigacoes = 0;
//
//	jogo->JogoIniciado = 0;
//	jogo->ClienteLigado = 0;
//
//	for (i = 0; i < MAXJOGADORES; i++) {
//
//		//Duplo sentido na comunicação
//
//		jogo->pipeAtual = criaPipeEscutaServidor();
//
//		if (jogo->pipeAtual == INVALID_HANDLE_VALUE) {
//			exit(-1);
//		}
//
//		pLigado = ConnectNamedPipe(jogo->pipeAtual, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
//
//		//Lancar thread para cada novo cliente (hPipe[i] ou i)
//
//		if (pLigado) {
//
//
//			hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtendeCliente, (LPVOID)jogo, 0, NULL);
//
//			if (hThread == NULL) {
//				exit(-1);
//			}
//			else
//				CloseHandle(hThread);
//		}
//		else
//			CloseHandle(jogo->pipeAtual);
//
//	}
//
//
//	free(jogo);
//
//	return 0;
//
//}
//
//
//DWORD WINAPI AtendeCliente(LPVOID param) {
//
//
//
//	DWORD nlidos, nescritos, i;
//	BOOL ret = FALSE;
//
//	JogoCliente jog;
//
//
//	//trabalhar somente com 1 cliente especifico
//	HANDLE cliente = jogo->pipeAtual;
//
//	do {
//
//
//		ret = lePipeJogoCliente(cliente, &jog);
//
//		if (ret != 0)
//		{
//
//			if (jog.comando == 1)
//			{
//
//				if (jogo->JogoIniciado == 0) {
//
//					jogo->hPipeA[jogo->totalLigacoes] = criaPipeCliente();
//
//					jogo->totalLigacoes++;
//
//					jogo->jogadores[jogo->jogadoresLigados].pidJogador = jog.pidCliente;
//
//					jogo->jogadoresLigados++;
//					jog.respostaComando = 1;
//
//					jogo->JogoIniciado = 1;
//
//				}
//				else
//					jog.respostaComando = 0;
//			}
//
//			else if (jog.comando == 2)
//			{
//				if (jogo->JogoIniciado == 1 && jogo->ClienteLigado == 0 &&
//					jog.pidCliente == jogo->jogadores[0].pidJogador) {
//
//					jog.respostaComando = 1;
//
//					jogo->ClienteLigado = 1;
//				}
//				else
//					jog.respostaComando = 0;
//
//			}
//			else if (jog.comando == 3)
//			{
//				if (jogo->JogoIniciado == 1 && jogo->ClienteLigado == 1) {
//
//					jogo->hPipeA[jogo->totalLigacoes] = criaPipeCliente();
//
//					jogo->totalLigacoes++;
//
//					jog.respostaComando = 1;
//
//					jogo->jogadores[jogo->jogadoresLigados].pidJogador = jog.pidCliente;
//
//					jogo->jogadoresLigados++;
//
//				}
//
//				else
//					jog.respostaComando = 0;
//			}
//			else
//				_tcscpy_s(jog.buf, 30, TEXT("nenhuma opcao valida"));
//
//
//			escrevePipeJogoCliente(cliente, &jog);
//
//			for (i = 0; i < jogo->totalLigacoes; i++)
//				escrevePipeJogoCliente(jogo->hPipeA[i], &jog);
//
//		}
//
//	} while (1);
//
//
//
//	if (!DisconnectNamedPipe(cliente)) {
//		_tperror(TEXT("Erro ao desligar o pipe!"));
//	}
//
//	//Sleep(2000);
//	CloseHandle(cliente);
//	jogo->jogadoresLigados--;
//
//	if (jogo->JogoIniciado == 1 && jogo->jogadoresLigados == 0)
//	{
//		MessageBox(NULL, TEXT("Bye Bye .."), TEXT("DUNGEONRPG"), MB_OK);
//
//		exit(-1);
//	}
//
//
//	return 0;
//}
