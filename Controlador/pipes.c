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

//------------------------ pipes comunicacao ------------------------------------------------------



HANDLE ligarServidor(TCHAR  servidor[])
{

	HANDLE hPipe;

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


	DWORD n;


	WriteFile(hPipe, jogo, sizeof(JogoCliente), &n, NULL);

	if (n > 0)
		return 1; else return 0;

}

int lePipeJogoCliente(HANDLE hPipe, JogoCliente *jogo) {

	BOOL ret;
	DWORD nlidos;

	ret = ReadFile(hPipe, jogo, sizeof(JogoCliente), &nlidos, NULL);


	if (nlidos > 0)
		return 1; else return 0;


}


int lePipeJogoClienteComRetVal(HANDLE hPipe, JogoCliente *jogo) {

	BOOL ret;
	DWORD nlidos;

	ret = ReadFile(hPipe, jogo, sizeof(JogoCliente), &nlidos, NULL);

	if (!ret || !nlidos)
		return 1; else return 0;

}



HANDLE pipeRececaoCliente(JogoCliente *jogo) {
	HANDLE hPipe;
	DWORD n;
	BOOL ret = FALSE;

	if (!WaitNamedPipe(pipeRececao, NMPWAIT_WAIT_FOREVER)) {
		//_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'... (WaitNamedPipe)\n"), pipeRececao);
		hPipe = INVALID_HANDLE_VALUE;
	}

	hPipe = CreateFile(pipeRececao, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	return hPipe;
}





// -----------------------------------  comandos ----------------------------------------



int criaIniciaJogo(HANDLE hPipe, JogoCliente *jogo) {

	//envia pedido registo

	jogo->comando = 1;

	escrevePipeJogoCliente(hPipe, jogo);

	////Recebe resposta

	lePipeJogoCliente(hPipe, jogo);

	return jogo->respostaComando;

}

int juntarJogo(HANDLE hPipe, JogoCliente *jogo) {

	//envia pedido registo

	jogo->comando = 3;

	escrevePipeJogoCliente(hPipe, jogo);

	////Recebe resposta

	lePipeJogoCliente(hPipe, jogo);

	return jogo->respostaComando;

}



int registarJogador(TCHAR nome[], HANDLE hPipe, JogoCliente *jogo) {

	//envia pedido registo

	jogo->comando = 4;

	escrevePipeJogoCliente(hPipe, jogo);

	////Recebe resposta

	lePipeJogoCliente(hPipe, jogo);

	return jogo->respostaComando;

}

int moverJogador(HANDLE hPipe, JogoCliente *jogo, int dir) {

	//envia pedido registo

	jogo->comando = 5;
	jogo->moveuDirecao = dir;

	escrevePipeJogoCliente(hPipe, jogo);

	////Recebe resposta

	lePipeJogoCliente(hPipe, jogo);

	return jogo->respostaComando;

}

int apanharItem(HANDLE hPipe, JogoCliente *jogo) {

	//envia pedido registo

	jogo->comando = 11;

	escrevePipeJogoCliente(hPipe, jogo);

	////Recebe resposta

	lePipeJogoCliente(hPipe, jogo);

	return jogo->respostaComando;

}




