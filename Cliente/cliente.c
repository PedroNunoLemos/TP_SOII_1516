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



int _tmain(int argc, LPTSTR argv[]) {


	TCHAR opcao;
	TCHAR ch;

#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif


	cursorVisible(0);

	caixa(25, 5, 60, 15, 0, 0);
	GoToXY(38, 8);
	_tprintf(TEXT("DUNGEON RPG SO"));
	GoToXY(27, 10);
	_tprintf(TEXT("Pressione QQ Tecla Para Continuar."));

	ch = _gettch();

	limpaArea(25, 5, 60, 15);

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

	return 0;

}

void jogar() {

	TCHAR message[256];
	HANDLE hPipe = INVALID_HANDLE_VALUE;

	TCHAR ch;

	JogoCliente *jogo;



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



	jogo = malloc(sizeof(JogoCliente));
	jogo->pidCliente = GetCurrentProcessId();

	if (criaIniciaJogo(hPipe, jogo))//ssucesso 
	{

		limpaArea(0, 0, 70, 20);
		GoToXY(0, 3);

		mostraJogo(jogo);

	}
	else if (juntarJogo(hPipe, jogo)) //ssucesso 
	{


		_tcscpy_s(message, 256, TEXT("Ligando a jogo existente"));

		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtualizaCliente, 0, 0, NULL);

		mostraJogo(jogo);

	}
	else
	{

		_tcscpy_s(message, 256, TEXT("Não me consegui ligar a um jogo."));

		free(jogo);

		CloseHandle(hPipe);

		return;

	}

	limpaArea(0, 0, 70, 20);

	caixa(20, 7, 60, 9, 0, 0);

	GoToXY(26, 8);

	_tprintf(TEXT("[CLIENTE] %s\n"), TEXT("Servidor Offline."));


	limpaArea(0, 0, 70, 20);

	GoToXY(0, 1);
	setForeGroundAndBackGroundColor(Color_White, Color_Black);

	free(jogo);

	CloseHandle(hPipe);

}

void mostraJogo(JogoCliente *jogo) {

	TCHAR ch=TEXT("");

	limpaArea(0, 0, 70, 25);
	caixa(5, 1, 26, 19, 0, 0);
	caixa(28, 1, 65, 19, 0, 0);
	caixa(5, 20, 65, 25, 0, 0);

	/*while (ch != key_ESCAPE)
	{*/
		

		imprimeLabirinto(33, 3, *jogo);

		ch = _gettch();


	/*}*/

	
}

DWORD WINAPI AtualizaCliente(LPVOID param) {

	HANDLE hPipe = INVALID_HANDLE_VALUE;
	JogoCliente jogo;

	atualizaJogoCliente(hPipe, &jogo);

	return 0;

}

