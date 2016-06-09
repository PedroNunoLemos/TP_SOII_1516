#pragma once

#include <Windows.h>

DWORD WINAPI AtendeCliente(LPVOID param);

typedef struct _pipesServer
{
	HANDLE hPipe;
	HANDLE hPipeRec;

} PipesServer;

