// ClienteGui.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ClienteGui.h"
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include "resource.h"

#define _CRT_SECURE_NO_WARNINGS


#include "..\Controlador\uteis.h"
#include "..\Controlador\constantes.h"
#include "..\Controlador\jogo.h"
#include "..\Controlador\pipes.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;
int logado = 0;

WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HBITMAP initialImage;
HDC memdc;
HBITMAP hbit;
HWND hWnd;
HWND actualhWnd;

//DWORD WINAPI enviaServidor(LPVOID param);
//DWORD WINAPI LeServidor(LPVOID param);

JogoCliente *jogo;
HANDLE pipeAtualizaCliente;
HANDLE hPipe = INVALID_HANDLE_VALUE;
DWORD tot;
HANDLE hMutex;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int WINAPI _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASSEX wcApp;	// Estrutura que define a classe da janela

	TCHAR buf[256];
	int i = 0;
	BOOL ret;
	DWORD n, dwMode;


	jogo = (JogoCliente*)malloc(sizeof(JogoCliente));
	jogo->pidCliente = GetCurrentProcessId();


	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CLIENTEGUI, szWindowClass, MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance,
		MAKEINTRESOURCE(IDC_CLIENTEGUI)); //this is for accelerator keys

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

// le servidor
DWORD WINAPI readFromServer(LPVOID param) {

	HANDLE hPipeReadFromServer = (HANDLE)param;


	//int i, j;

	//while (1) {


	//	if () {
	//		}
	//	}
	//	else {
	//		if (_tcscmp() == 0) {

	//			Button_Enable(GetDlgItem(actualhWnd, IDREGISTER), false);
	//			GetDlgItemText(actualhWnd, IDC_EDITLog, log, kBufferSize);
	//			//_tcscat(log, TEXT("\n"));
	//			//_tcscat(log, answerFromServer.answer);
	//			SetDlgItemText(actualhWnd, IDC_EDITLog, log);
	//		}
	//		else if (_tcsstr(answerFromServer.answer, TEXT("NEW GAME"))) {
	//			//EndDialog(actualhWnd, 0);
	//		}
	//		else {
	//			GetDlgItemText(actualhWnd, IDC_EDITLog, log, kBufferSize);
	//			_tcscat(log, TEXT("\n"));
	//			_tcscat(log, answerFromServer.answer);
	//			SetDlgItemText(actualhWnd, IDC_EDITLog, log);
	//		}
	//	}

	//}


	//desliga pipe

	return 0;

}

// regista classe
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENTEGUI));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CLIENTEGUI);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//inicia instancia
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindowW(
		szWindowClass,
		TEXT("DUNGEON RPG"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

INT_PTR CALLBACK iniciaJogo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	UNREFERENCED_PARAMETER(lParam);

	TCHAR buf[256];
	TCHAR namePlayer[256];
	DWORD n;
	actualhWnd = hDlg;
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		//
		//if (LOWORD(wParam) == IDREGISTER && !loggedIn)
		//{
		//	_tcscpy(buf, TEXT("REGISTER "));
		//	GetDlgItemText(hDlg, IDC_NAMEBOX, namePlayer, kBufferSize);
		//	_tcscat(buf, namePlayer);
		//	WriteFile(hPipeWriteServerConnection, buf, _tcslen(buf) * sizeof(TCHAR), &n, NULL);
		//	return (INT_PTR)TRUE;
		//}
		/*else if (LOWORD(wParam) == IDCANCEL) {
		EndDialog(hDlg, LOWORD(wParam));
		return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDNEWGAME) {
		_tcscpy(buf, TEXT("NEW GAME"));
		WriteFile(hPipeWriteServerConnection, buf, _tcslen(buf) * sizeof(TCHAR), &n, NULL);
		return (INT_PTR)TRUE;
		}*/
		break;
	}
	//EndDialog(hDlg, 0);
	return (INT_PTR)FALSE;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int maxX, maxY;
	int wmId;
	actualhWnd = hWnd;

	TCHAR buf[256];
	DWORD n;

	switch (message)
	{

	case WM_CREATE:
		maxX = GetSystemMetrics(SM_CXSCREEN);// Screen Max Size
		maxY = GetSystemMetrics(SM_CYSCREEN);

		SetWindowPos(hWnd, HWND_TOP, 0, 0, maxX, maxY, SWP_SHOWWINDOW);

		initialImage = (HBITMAP)LoadImage(NULL, TEXT("wallpaper.bmp"), IMAGE_BITMAP, maxX, maxY, LR_LOADFROMFILE);

		if (initialImage == NULL) {
			exit(1);
		}

		break;

	case WM_COMMAND:
	{
		wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_JOGO_NOVO:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_JOGOSERVIDOR), hWnd, About);
			break;

		case ID_JOGO_SAIR:
			DestroyWindow(hWnd);
			ExitProcess(0);

			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		maxX = GetSystemMetrics(SM_CXSCREEN);// Screen Max Size
		maxY = GetSystemMetrics(SM_CYSCREEN);

		hdc = GetDC(hWnd);

		memdc = CreateCompatibleDC(hdc);// Criar janela virtual
										//initialImage = CreateCompatibleBitmap(hdc, maxX, maxY);// Criar janela virtual

		SelectObject(memdc, initialImage);

		BitBlt(hdc, 0, 0, maxX, maxY, memdc, 0, 0, SRCCOPY);



		ReleaseDC(hWnd, hdc);


		EndPaint(hWnd, &ps);
	}
	break;

	case WM_KEYDOWN:

		switch ((char)wParam) {
		case VK_DOWN:				// Seta para baixo

			InvalidateRect(hWnd, NULL, 1);
			break;
		case VK_LEFT:				// Seta esquerda

			InvalidateRect(hWnd, NULL, 1);
			break;
		case VK_RIGHT:				// Seta direita

			InvalidateRect(hWnd, NULL, 1);
			break;
		case VK_UP:				// Seta cima

			InvalidateRect(hWnd, NULL, 1);
			break;
		default:
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
