#pragma once

#include "resource.h"

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    LigaServidorDLG(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ServidorInfoDLG(HWND, UINT, WPARAM, LPARAM);
