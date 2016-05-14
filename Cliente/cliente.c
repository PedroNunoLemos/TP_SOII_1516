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





int _tmain(int argc, LPTSTR argv[]) {

	TCHAR buf[256];
	HANDLE hPipe;
	BOOL ret;
	DWORD n;

	JogoCliente *jogo;

#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	jogo = malloc(sizeof(JogoCliente));
	jogo->pidCliente = GetCurrentProcessId();


	hPipe = criaPipeEscutaCliente(TEXT("127.0.0.1"));


	while (1) {

		_tprintf(TEXT("digite uma opção \n"));
		_fgetts(buf, 256, stdin);


		if (_tcscmp(buf, TEXT("1\n")) == 0) {

			jogo->comando = 1;

			escrevePipeJogoCliente(hPipe, jogo);

			////Recebe resposta

			lePipeJogoCliente(hPipe, jogo);

			if (jogo->respostaComando == 1)//ssucesso 
			{
				_tprintf(TEXT("[CLIENTE] %s\n"), TEXT("Jogo Criado."));
				CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtualizaCliente, 0, 0, NULL);
			}
			else
				_tprintf(TEXT("[CLIENTE] %s\n"), TEXT("Não consegui criar o jogo"));
		}
		else if (_tcscmp(buf, TEXT("2\n")) == 0) {

			jogo->comando = 2;

			escrevePipeJogoCliente(hPipe, jogo);

			////Recebe resposta

			lePipeJogoCliente(hPipe, jogo);


			if (jogo->respostaComando == 1) //sucesso 
				_tprintf(TEXT("[CLIENTE] %s\n"), TEXT("Jogo Iniciado."));
			else
				_tprintf(TEXT("[CLIENTE] %s\n"), TEXT("Nao pude iniciar o jogo"));

		}
		else if (_tcscmp(buf, TEXT("3\n")) == 0)
		{

			jogo->comando = 3;

			escrevePipeJogoCliente(hPipe, jogo);

			////Recebe resposta

			lePipeJogoCliente(hPipe, jogo);

			if (jogo->respostaComando == 1) //ssucesso 
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

	Sleep(200);
	return 0;

}

DWORD WINAPI AtualizaCliente(LPVOID param) {

	HANDLE hPipe = INVALID_HANDLE_VALUE;
	DWORD n;
	JogoCliente jogo;
	BOOL ret = FALSE;

	_tprintf(TEXT("Actualizando JogoCliente\n"));

	atualizaJogoCliente(hPipe, &jogo);

	return 0;

}
