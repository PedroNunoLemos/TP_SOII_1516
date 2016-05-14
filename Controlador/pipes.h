#pragma once

#include <windows.h>
#include <tchar.h>
#include "jogo.h"

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
	__declspec(dllexport) HANDLE criaPipeEscutaCliente(TCHAR  servidor[]);
	__declspec(dllexport) int escrevePipeJogoCliente(HANDLE hPipe, JogoCliente *jogo);
	__declspec(dllexport) int lePipeJogoCliente(HANDLE hPipe, JogoCliente *jogo);
	__declspec(dllexport) void atualizaJogoCliente(HANDLE hPipe, JogoCliente *jogo);

	// pipes servidor 

	__declspec(dllexport) HANDLE criaPipeEscutaServidor();
	_declspec(dllexport) HANDLE criaPipeCliente();

#ifdef __cplusplus
}
#endif

