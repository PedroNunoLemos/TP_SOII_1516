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


	char opcao;
	char ch;

#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

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
			//sair();
			break;
		}

	} while (opcao < '1' || opcao > '3');


	return 0;

}

DWORD WINAPI AtualizaCliente(LPVOID param) {

	HANDLE hPipe = INVALID_HANDLE_VALUE;
	DWORD n;
	JogoCliente jogo;
	BOOL ret = FALSE;

	//_tprintf(TEXT("Actualizando JogoCliente\n"));

	atualizaJogoCliente(hPipe, &jogo);

	return 0;

}

void jogar() {

	TCHAR buf[256];
	HANDLE hPipe = INVALID_HANDLE_VALUE;
	BOOL ret;
	DWORD n;

	JogoCliente *jogo;


	limpaArea(5, 5, 40, 15);

	caixa(20, 7, 70, 10, 0, 0);

	GoToXY(22, 8);
	_tprintf(TEXT("Introduza o ip servidor enter para localhost."));
	GoToXY(22, 9);

	_fgetts(servnome, 256, stdin);

	if (_tcscmp(servnome, TEXT("\n")) == 0) {
		_tcscpy_s(servnome, 256, TEXT("127.0.0.1"));
	}

	limpaArea(0, 0, 70, 20);
	GoToXY(0, 0);


	hPipe = ligarServidor(servnome);

	if (hPipe == INVALID_HANDLE_VALUE)
	{

		limpaArea(5, 5, 40, 15);

		caixa(20, 7, 60, 9, 0, 0);

		GoToXY(26, 8);

		_tprintf(TEXT("[CLIENTE] %s\n"), TEXT("Servidor Offline."));

		char ch = getch();

		return 0;

	}



	jogo = malloc(sizeof(JogoCliente));
	jogo->pidCliente = GetCurrentProcessId();

	if (criaIniciaJogo(hPipe, jogo))//ssucesso 
	{

		_tprintf(TEXT("1 Cliente Jogo Criado ... \n"));


		imprimeLabirinto(25, 5, *jogo);

	}

	GoToXY(0, 0);
	setForeGroundAndBackGroundColor(Color_White,Color_Black);


	while (1) {

		_tprintf(TEXT("digite uma opção \n"));
		_fgetts(buf, 256, stdin);


		//if (_tcscmp(buf, TEXT("1\n")) == 0) {

		//	if (criaIniciaJogo(hPipe, jogo))//ssucesso 
		//	{
		//		_tprintf(TEXT("[CLIENTE] %s\n"), TEXT("Jogo Criado."));
		//		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtualizaCliente, 0, 0, NULL);
		//	}
		//	else
		//		_tprintf(TEXT("[CLIENTE] %s\n"), TEXT("Não consegui criar o jogo"));
		//}
		//else
		if (_tcscmp(buf, TEXT("2\n")) == 0) {


		}
		else if (_tcscmp(buf, TEXT("3\n")) == 0)
		{

			if (juntarJogo(hPipe, jogo)) //ssucesso 
			{

				_tprintf(TEXT("[CLIENTE] %s\n"), TEXT("Ligado a jogo existente"));

				CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtualizaCliente, 0, 0, NULL);

			}
			else
				_tprintf(TEXT("[CLIENTE] %s\n"), TEXT("Não me consegui ligar a um jogo."));
		}
		else
			jogo->respostaComando = 0;

	}


	CloseHandle(hPipe);

}