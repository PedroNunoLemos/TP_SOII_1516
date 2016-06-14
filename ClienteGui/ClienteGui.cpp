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

HANDLE servHandler;

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

INT_PTR CALLBACK LigaServidorDLG(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	UNREFERENCED_PARAMETER(lParam);

	TCHAR buf[256];
	TCHAR namePlayer[256];
	DWORD n;
	actualhWnd = hDlg;

	DWORD dwAddr;

	LPTSTR szText = new TCHAR[50];


	switch (message)
	{

	case WM_INITDIALOG:

		swprintf(szText, 50, TEXT("127.0.0.1"));
		SetDlgItemText(hDlg, IDC_IPSERVIDOR, szText);

		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BTLIGAR) {

			GetDlgItemText(hDlg, IDC_IPSERVIDOR, szText, 50);

			if ((servHandler = ligarServidor(szText)) != INVALID_HANDLE_VALUE) {

				//MessageBox(NULL, _T("Hello server!"), _T("Dungeon RPG"), MB_OK);

				EndDialog(hDlg, 0);

				DialogBox(hInst, MAKEINTRESOURCE(IDD_SERVIDORINFO), hWnd, ServidorInfoDLG);

			}
			else {

				MessageBox(NULL, _T("N�o me Consegui Ligar ao servidor!"), _T("Dungeon RPG"), MB_OK);
				EndDialog(hDlg, 0);
			}
		}

		break;

	case WM_CLOSE:
		EndDialog(hDlg, 0);
		break;
	}
	//EndDialog(hDlg, 0);
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK ServidorInfoDLG(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	UNREFERENCED_PARAMETER(lParam);

	TCHAR buf[256];
	TCHAR namePlayer[256];
	DWORD n;
	actualhWnd = hDlg;

	DWORD dwAddr;
	HWND hWndList;

	int i = 0;

	switch (message)
	{

	case WM_INITDIALOG:

		hWndList = GetDlgItem(hDlg, IDC_LISTJOGADORES2);
		
		
		SendMessage(hWndList, LB_ADDSTRING, 0, (LPARAM)TEXT("Jogador"));

		break;
	case WM_COMMAND:
		//if (LOWORD(wParam) == IDC_BTLIGAR) {


		//}

		break;


	case WM_CLOSE:
		EndDialog(hDlg, 0);
		break;
	}
	
	//EndDialog(hDlg, 0);
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
			DialogBox(hInst, MAKEINTRESOURCE(IDD_LIGASERVIDOR), hWnd, LigaServidorDLG);
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
