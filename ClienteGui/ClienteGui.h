#pragma once

#include "resource.h"


#define IM_pedra 0
#define IM_vitamina 1
#define IM_orange 2
#define IM_cafeina 3
#define IM_jog 4
#define IM_ini 5
#define IM_bully 6
#define IM_dist 7
#define IM_vazio 8
#define IM_chao 9
#define IM_parede 10

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WindowFunc(HWND, UINT, WPARAM, LPARAM);

ATOM  RegistaClasses(HINSTANCE  hThislnst, TCHAR 	*  szWinName);

HWND CriarJanela(HINSTANCE hThisInst, TCHAR	*  szWinName);


LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    LigaServidorDLG(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ServidorInfoDLG(HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI AtualizaCliente(LPVOID param);
void notificaVista();