#pragma once

#include <windows.h>
#include <tchar.h>
#include "jogo.h"
#include "servidorinfo.h"

#define PIPE_ENVIO TEXT("\\\\.\\pipe\\servidor")
#define PIPE_RECECAO TEXT("\\\\.\\pipe\\clientes")




#ifdef DLL_EXPORTS
#define DLL_IMP_API __declspec(dllexport)
#else
#define DLL_IMP_API __declspec(dllimport)
#endif

#ifdef UNICODE
#define tstring wstring
#else
#define tstring string
#endif



#ifdef __cplusplus
extern "C" {  // only need to export C interface if
			  // used by C++ source code
#endif


			  // pipes Cliente
	__declspec(dllexport) HANDLE ligarServidor(TCHAR  servidor[]);
	__declspec(dllexport) int escrevePipeJogoCliente(HANDLE hPipe, JogoCliente *jogo);
	__declspec(dllexport) int lePipeJogoCliente(HANDLE hPipe, JogoCliente *jogo);
	__declspec(dllexport) int lePipeJogoClienteComRetVal(HANDLE hPipe, JogoCliente *jogo);
	__declspec(dllexport) HANDLE pipeRececaoCliente();


	//comandos

	_declspec(dllexport)	int criaIniciaJogo(HANDLE hPipe, JogoCliente *jogo);
	_declspec(dllexport)	int juntarJogo(HANDLE hPipe, JogoCliente *jogo);
	_declspec(dllexport)	int registarJogador(TCHAR nome[], HANDLE hPipe, JogoCliente *jogo);
	_declspec(dllexport)	void moverJogador(HANDLE hPipe, JogoCliente *jogo, int dir);
	_declspec(dllexport)	void usaPedra(HANDLE hPipe, JogoCliente *jogo, int usar);
	_declspec(dllexport)	ServidorInfo *ObterInfoServidor(HANDLE hPipe, JogoCliente *jogo);
	
#ifdef __cplusplus
}
#endif

