#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include "pipes.h"
#include "jogo.h"
#include "constantes.h"

TCHAR pipeEnvio[256];
TCHAR pipeRececao[256];


HANDLE criaPipeEscutaCliente(TCHAR  servidor[])
{

	HANDLE hPipe;


	//_tcscpy_s(server, 256, TEXT("nenhuma opcao valida"));

	swprintf(pipeEnvio, 256, TEXT("\\\\%s\\pipe\\servidor"), servidor);
	swprintf(pipeRececao, 256, TEXT("\\\\%s\\pipe\\clientes"), servidor);

	//_tprintf(TEXT("[CLIENTE]Esperar pelo pipe '%s'(WaitNamedPipe)\n"), PIPE_ENVIO);
	if (!WaitNamedPipe(pipeEnvio, NMPWAIT_WAIT_FOREVER)) {
		//_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'... (WaitNamedPipe)\n"), PIPE_ENVIO);
		return INVALID_HANDLE_VALUE;
	}

	//_tprintf(TEXT("[CLIENTE] Ligação ao servidor... (CreateFile)\n"));
	//Alterar sentido da comunicação
	hPipe = CreateFile(pipeEnvio, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hPipe == NULL) {
		//_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'... (CreateFile)\n"), PIPE_ENVIO);
		return INVALID_HANDLE_VALUE;
	}

	//_tprintf(TEXT("[CLIENTE]Liguei-me...\n"));

	return hPipe;
}

int escrevePipeJogoCliente(HANDLE hPipe, JogoCliente *jogo) {

	BOOL ret;
	DWORD n;


	WriteFile(hPipe, jogo, sizeof(JogoCliente), &n, NULL);

	if (n > 0)
		return 1; else return 0;

}

int lePipeJogoCliente(HANDLE hPipe, JogoCliente *jogo) {

	BOOL ret;
	DWORD n;

	ret = ReadFile(hPipe, jogo, sizeof(JogoCliente), &n, NULL);

	if (n > 0)
		return 1; else return 0;

}

void atualizaJogoCliente(HANDLE hPipe, JogoCliente *jogo) {

	DWORD n;
	BOOL ret = FALSE;

	if (!WaitNamedPipe(pipeRececao, NMPWAIT_WAIT_FOREVER)) {
		_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'... (WaitNamedPipe)\n"), pipeRececao);
		hPipe = INVALID_HANDLE_VALUE;
	}

	hPipe = CreateFile(pipeRececao, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	while (1) {

		ret = ReadFile(hPipe, jogo, sizeof(JogoCliente), &n, NULL);

		if (!ret || !n)
			break;

		//se chegar ate aqui enviou 

	}

}


//----------------- Pipes Servidor -------------------------------


HANDLE criaPipeEscutaServidor() {

	HANDLE hPipe;

	//_tprintf(TEXT("[SERVIDOR] Vou criar o pipe '%s' de recepcao... (CreateNamedPipe)\n"), PIPE_ENVIO);

	hPipe = CreateNamedPipe(PIPE_ENVIO, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_WAIT | PIPE_TYPE_MESSAGE
		| PIPE_READMODE_MESSAGE, MAXJOGADORES, sizeof(JogoCliente), sizeof(JogoCliente),
		1000, NULL);

	if (hPipe == INVALID_HANDLE_VALUE) {
		//_tprintf(TEXT("CreateNamedPipe falhou, erro=%d\n"), GetLastError());
		return INVALID_HANDLE_VALUE;
	}

	/*hPipeA[i] = CreateNamedPipe(PIPE_RECECAO, PIPE_ACCESS_OUTBOUND|FILE_FLAG_OVERLAPPED, PIPE_WAIT | PIPE_TYPE_MESSAGE
	| PIPE_READMODE_MESSAGE, N, sizeof(JogoCliente), 0,1000,NULL);
	total++;*/

	return hPipe;

}

HANDLE criaPipeCliente() {

	HANDLE Hpipe;

	Hpipe = CreateNamedPipe(PIPE_RECECAO, PIPE_ACCESS_OUTBOUND | FILE_FLAG_OVERLAPPED, PIPE_WAIT | PIPE_TYPE_MESSAGE
		| PIPE_READMODE_MESSAGE, MAXJOGADORES, sizeof(JogoCliente), 0, 1000, NULL);


	return Hpipe;

}




