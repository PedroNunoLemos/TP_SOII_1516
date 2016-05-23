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

	HANDLE hPipe = INVALID_HANDLE_VALUE;

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

	if (hPipe == INVALID_HANDLE_VALUE)
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

		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtualizaCliente, (LPVOID)hPipe, 0, NULL);

		mostraJogo(hPipe, jogo);


		return;

	}
	else
	{

		if (juntarJogo(hPipe, jogo)) {
			_tcscpy_s(message, 256, TEXT("Ligando a jogo existente"));

			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtualizaCliente, (LPVOID)hPipe, 0, NULL);

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

void mostraJogo(HANDLE Hpipe, JogoCliente *jogo) {

	TCHAR ch = TEXT("");

	limpaArea(0, 0, 75, 25);
	caixa(5, 1, 26, 19, 0, 0);
	caixa(28, 1, 65, 19, 0, 0);
	caixa(5, 20, 65, 25, 0, 0);


	setcolor(Color_BrightWhite);

	GoToXY(8, 21);
	_tprintf(TEXT("Pressione as teclas de direção se movimentar"));

	GoToXY(8, 23);
	_tprintf(TEXT("Pressione ESC para sair"));



	while (ch != key_ESCAPE)
	{
		setcolor(Color_BrightWhite);

		GoToXY(8, 2);
		_tprintf(jogo->jogador.nome);

		GoToXY(8, 4);
		_tprintf(TEXT("Saude : %d"), jogo->jogador.saude);

		GoToXY(8, 6);
		_tprintf(TEXT("Lentidao : %d"), jogo->jogador.lentidao);

		GoToXY(8, 8);
		_tprintf(TEXT("Vitaminas : %d"), jogo->jogador.qtdVitaminas);


		GoToXY(8, 9);
		_tprintf(TEXT("OrangeBull : %d"), jogo->jogador.qtdOranges);


		GoToXY(8, 10);
		_tprintf(TEXT("Cafeina : %d"), jogo->jogador.qtdCafeinas);

		GoToXY(8, 11);
		_tprintf(TEXT("Pedras : %d"), jogo->jogador.qtdPedras);



		imprimeLabirinto(38, 3, *jogo);

		ch = _gettch();


		jogo->respostaComando = 0;

		if (ch == key_DOWN) moverJogador(Hpipe, jogo, 1); //Dir 1
		if (ch == key_UP) moverJogador(Hpipe, jogo, 2); //Dir 2 
		if (ch == key_LEFT) moverJogador(Hpipe, jogo, 3); //Dir 3
		if (ch == key_RIGHT) moverJogador(Hpipe, jogo, 4); //Dir 4


	}

	setForeGroundAndBackGroundColor(Color_White, Color_Black);

	limpaArea(0, 0, 75, 25);

	caixa(20, 7, 60, 9, 0, 0);

	GoToXY(31, 8);

	_tprintf(TEXT("Jogo Terminado."));

	GoToXY(0, 1);


	ch = _gettch();
}

DWORD WINAPI AtualizaCliente(LPVOID param) {

	int res = 0;


	HANDLE Hpipe = (HANDLE)param;
	JogoCliente *tmp;

	tmp = malloc(sizeof(JogoCliente));

	atualizaJogoCliente(Hpipe, tmp, &res);

	if (tmp->respostaComando == 1) { jogo = tmp; }

	return 0;

}

