#pragma once
#include <Windows.h>


DWORD WINAPI AtualizaCliente(LPVOID param);
DWORD WINAPI ComandosCliente(LPVOID param);

void jogar();
void mostraJogo(HANDLE Hpipe, JogoCliente *jogo);