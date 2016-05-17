#pragma once
#include <Windows.h>


DWORD WINAPI AtualizaCliente(LPVOID param);
void jogar();
void mostraJogo(HANDLE Hpipe, JogoCliente *jogo);