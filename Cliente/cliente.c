#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

#define _CRT_SECURE_NO_WARNINGS

#include "tui.h"
#include "cliente.h"
#include "..\Controlador\uteis.h"
#include "..\Controlador\constantes.h"
#include "..\Controlador\jogo.h"
#include "..\Controlador\pipes.h"


TCHAR servnome[256];


JogoCliente *jogo;
HANDLE pipeAtualizaCliente;
HANDLE hPipe = INVALID_HANDLE_VALUE;
DWORD tot;
HANDLE hMutex;

int _tmain(int argc, LPTSTR argv[]) {


	TCHAR opcao;
	TCHAR ch;

#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif


	jogo = malloc(sizeof(JogoCliente));
	jogo->pidCliente = GetCurrentProcessId();
	tot = 0;

	hMutex = CreateMutex(NULL, FALSE, TEXT("server_update"));

	cursorVisible(0);

	caixa(25, 5, 63, 15, 0, 0);
	GoToXY(38, 8);
	_tprintf(TEXT("DUNGEON RPG SO"));
	GoToXY(28, 10);
	_tprintf(TEXT("Pressione QQ Tecla Para Continuar."));

	ch = _gettch();

	limpaArea(25, 5, 63, 15);

	caixa(5, 5, 40, 15, 0, 0);

	GoToXY(7, 8);
	_tprintf(TEXT("Escolha uma das seguintes opções"));
	GoToXY(7, 10);	_tprintf(TEXT(" 1-Jogar "));
	GoToXY(7, 11);	_tprintf(TEXT(" 2-Historico de Jogos "));
	GoToXY(7, 12);	_tprintf(TEXT(" 3-Sair "));
	GoToXY(7, 13);

	do {

		opcao = _gettch();

		switch (opcao) {
		case '1':
			jogar();
			break;
		case '2':
			//historico();
			break;
		case '3':
			ExitProcess(0);
			break;
		}

	} while (opcao < '1' || opcao > '3');

	cursorVisible(1);

	free(jogo);


	return 0;

}

void jogar() {

	TCHAR message[256];
	TCHAR ch;


	limpaArea(5, 5, 40, 15);

	caixa(20, 7, 70, 10, 0, 0);

	GoToXY(22, 8);
	_tprintf(TEXT("Introduza o ip servidor [ENTER] para localhost."));
	GoToXY(22, 9);

	cursorVisible(1);

	_fgetts(servnome, 256, stdin);

	cursorVisible(0);

	if (_tcscmp(servnome, TEXT("\n")) == 0) {
		_tcscpy_s(servnome, 256, TEXT("127.0.0.1"));
	}

	limpaArea(0, 0, 70, 20);
	GoToXY(0, 1);


	hPipe = ligarServidor(servnome);
	pipeAtualizaCliente = pipeRececaoCliente();

	if (hPipe == INVALID_HANDLE_VALUE && pipeAtualizaCliente == INVALID_HANDLE_VALUE)
	{



		limpaArea(0, 0, 70, 20);

		caixa(20, 7, 60, 9, 0, 0);

		GoToXY(26, 8);

		_tprintf(TEXT("[CLIENTE] %s\n"), TEXT("Servidor Offline."));

		ch = _gettch();

		return;

	}




	if (criaIniciaJogo(hPipe, jogo))//ssucesso 
	{



		limpaArea(0, 0, 70, 20);
		GoToXY(0, 3);

		mostraJogo(hPipe, jogo);

		CloseHandle(hPipe);

		return;

	}
	else
	{

		if (juntarJogo(hPipe, jogo)) {


			limpaArea(0, 0, 70, 20);
			GoToXY(0, 3);

			mostraJogo(hPipe, jogo);

			CloseHandle(hPipe);

			return;
		}
		else
		{

			_tcscpy_s(message, 256, TEXT("Não me consegui ligar a um jogo."));

			CloseHandle(hPipe);
			return;

		}

	}


	limpaArea(0, 0, 70, 20);

	caixa(20, 7, 60, 9, 0, 0);

	GoToXY(26, 8);

	_tprintf(TEXT("[CLIENTE] %s\n"), TEXT("Servidor Offline."));


	limpaArea(0, 0, 70, 20);

	GoToXY(0, 1);
	setForeGroundAndBackGroundColor(Color_White, Color_Black);

	CloseHandle(hPipe);

}

void impDados(JogoCliente *jogo) {


	setcolor(Color_BrightWhite);

	GoToXY(8, 2);
	_tprintf(jogo->jogador.nome);


	GoToXY(8, 4);
	_tprintf(TEXT("Saude :   "));

	GoToXY(8, 4);
	_tprintf(TEXT("Saude : %d"), jogo->jogador.saude);

	GoToXY(8, 6);
	_tprintf(TEXT("Lentidao : %d"), jogo->jogador.lentidao);

	GoToXY(8, 8);
	_tprintf(TEXT("Cafeinas : %d"), jogo->jogador.efeitoCafeina);


	GoToXY(8, 9);
	_tprintf(TEXT("Contador : %d"), jogo->jogador.contadorMovimento);
	
	/*

			GoToXY(8, 10);
			_tprintf(TEXT("Cafeina : %d"), jogo->jogador.qtdCafeinas);

			*/

	GoToXY(8, 11);
	_tprintf(TEXT("Pedras : %d"), jogo->jogador.qtdPedras);

	//GoToXY(8, 21);
	//_tprintf(jogo->mensagem);

	imprimeLabirinto(38, 3, jogo);

}

void mostraJogo(HANDLE Hpipe, JogoCliente *jogo) {

	int res = 0;

	HANDLE cmdThread;

	int ch;
	ch = '\0';


	limpaArea(0, 0, 75, 25);
	caixa(5, 1, 26, 19, 0, 0);
	caixa(28, 1, 65, 19, 0, 0);
	caixa(5, 20, 65, 25, 0, 0);


	setcolor(Color_BrightWhite);

	GoToXY(8, 21);
	_tprintf(TEXT("Pressione as teclas de direção se movimentar"));

	GoToXY(8, 23);
	_tprintf(TEXT("Pressione ESC para sair"));

	//_stprintf(jogo->mensagem,TEXT(" "));

	impDados(jogo);

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtualizaCliente, 0, 0, NULL);
	cmdThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ComandosCliente, (LPVOID)Hpipe, 0, NULL);

	WaitForSingleObject(cmdThread, INFINITE);


	setForeGroundAndBackGroundColor(Color_White, Color_Black);

	limpaArea(0, 0, 75, 25);

	caixa(20, 7, 60, 9, 0, 0);

	GoToXY(31, 8);

	_tprintf(TEXT("Jogo Terminado."));

	GoToXY(0, 1);


	ch = _gettch();
}

DWORD WINAPI ComandosCliente(LPVOID param) {

	int ch;

	HANDLE Hpipe = (HANDLE)param;


	ch = '\0';

	while (ch != key_ESCAPE)
	{


		if (jogo->jogador.podeMovimentar == 0)
		{
			break;
		}

		ch = _gettch();

		if (ch == key_DOWN) moverJogador(Hpipe, jogo, 1); //Dir 1
		if (ch == key_UP) moverJogador(Hpipe, jogo, 2); //Dir 2 
		if (ch == key_LEFT) moverJogador(Hpipe, jogo, 3); //Dir 3
		if (ch == key_RIGHT) moverJogador(Hpipe, jogo, 4); //Dir 4


		impDados(jogo);


	}

	return 0;

}

DWORD WINAPI AtualizaCliente(LPVOID param) {


	JogoCliente *m;
	BOOL ret = FALSE;

	m = malloc(sizeof(JogoCliente));

	while (1) {



		ret = lePipeJogoClienteComRetVal(pipeAtualizaCliente, m);

		if (ret == 1)
			break;


		if (m->pidCliente == jogo->pidCliente)
		{
			if (m->respostaComando  == 51) {

				WaitForSingleObject(hMutex, INFINITE);

				jogo = m;

				setcolor(Color_BrightWhite);


				impDados(jogo);

				ReleaseMutex(hMutex);

			}


			if (m->respostaComando == 71)
			{
				WaitForSingleObject(hMutex, INFINITE);
				jogo->jogador.efeitoCafeina = m->jogador.efeitoCafeina;
				jogo->jogador.lentidao = m->jogador.lentidao;
				ReleaseMutex(hMutex);
			}


			if (m->respostaComando == 61)
			{
				WaitForSingleObject(hMutex, INFINITE);
				jogo->jogador.efeitoCafeina = m->jogador.efeitoCafeina;
				jogo->jogador.lentidao = m->jogador.lentidao;
				ReleaseMutex(hMutex);
			}

		}

	}





	return 0;

}

