#pragma once

#include "resource.h"

// Forward declarations of functions included in this code module:
ATOM RegistaClasse(HINSTANCE hThisInst, TCHAR	* szWinName);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    LigaServidorDLG(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ServidorInfoDLG(HWND, UINT, WPARAM, LPARAM);
