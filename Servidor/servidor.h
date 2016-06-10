#pragma once

#include <Windows.h>

DWORD WINAPI AtendeCliente(LPVOID param);
DWORD WINAPI BonusCafeina(LPVOID param);
DWORD WINAPI Temporizador(LPVOID param);

void desligaJogador(int id);
void  lancaMonstros();

