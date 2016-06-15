// ClienteGui.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ClienteGui.h"
#include <windows.h>
#include <CommCtrl.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include "resource.h"
#include "gui.h"
#include "mvc.h"

#define _CRT_SECURE_NO_WARNINGS


#include "..\Controlador\uteis.h"
#include "..\Controlador\constantes.h"
#include "..\Controlador\jogo.h"
#include "..\Controlador\pipes.h"
#include "..\Controlador\servidorinfo.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;
int logado = 0;

WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HANDLE servHandler;

HDC DBuf;
HWND vista;

HBITMAP initialImage;
HDC memdc;
HBITMAP hbit;
HWND hWnd;
HWND actualhWnd;

JogoCliente *jogo;
ServidorInfo *info;


HANDLE pipeAtualizaCliente;
HANDLE hPipe = INVALID_HANDLE_VALUE;
DWORD tot;
HANDLE hMutex;

HBITMAP hOrigVista;

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

	if (!RegistaClasse(hInstance, TEXT("Dungeon RPG")))
		return	0;

	vista = CriarVista(hInstance, TEXT("Dungeon RPG"));

	ShowWindow(vista, nCmdShow);
	UpdateWindow(vista);

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

	SelectObject(DBuf, hOrigVista);	//  repoe conteudo original
	DeleteObject(DBuf);

	return (int)msg.wParam;
}


ATOM	RegistaClasse(HINSTANCE hThisInst, TCHAR	* szWinName) {

	WNDCLASSEX wcl;

	wcl.cbSize = sizeof(WNDCLASSEX);
	wcl.hInstance = hThisInst;
	wcl.lpszClassName = szWinName;
	wcl.lpfnWndProc = WndProc;
	wcl.style = CS_HREDRAW;
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcl.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcl.lpszMenuName = MAKEINTRESOURCEW(IDC_CLIENTEGUI);
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hbrBackground = NULL;

	return RegisterClassEx(&wcl);

}




INT_PTR CALLBACK PedeNomeDLG(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	UNREFERENCED_PARAMETER(lParam);

	DWORD dwAddr;

	LPTSTR szText = new TCHAR[256];


	switch (message)
	{
	case WM_CREATE:

		break;

	case WM_INITDIALOG:


		swprintf(szText, 10, TEXT("Jogador "));
		SetDlgItemText(hDlg, IDC_ENOME, szText);

		CenterWindow(hDlg);

		break;
	case WM_COMMAND:

		if (LOWORD(wParam) == OKNOME) {

			GetDlgItemText(hDlg, IDC_ENOME, jogo->jogador.nome, 50);
			EndDialog(hDlg, 0);

		}


		break;
	case WM_CLOSE:

		GetDlgItemText(hDlg, IDC_ENOME, jogo->jogador.nome, 50);
		EndDialog(hDlg, 0);

		break;
	}
	//EndDialog(hDlg, 0);
	return (INT_PTR)FALSE;

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
	case WM_CREATE:

		break;

	case WM_INITDIALOG:


		swprintf(szText, 50, TEXT("127.0.0.1"));
		SetDlgItemText(hDlg, IDC_IPSERVIDOR, szText);


		CenterWindow(hDlg);

		break;
	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_BTLIGAR) {

			GetDlgItemText(hDlg, IDC_IPSERVIDOR, szText, 50);

			if ((servHandler = ligarServidor(szText)) != INVALID_HANDLE_VALUE) {


				EndDialog(hDlg, 0);

				DialogBox(hInst, MAKEINTRESOURCE(IDD_SERVIDORINFO), hWnd, ServidorInfoDLG);

			}
			else {

				MessageBox(NULL, _T("N�o me Consegui Ligar ao servidor!"), _T("Dungeon RPG"), MB_OK);
				//EndDialog(hDlg, 0);
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
	HWND hWndListHist;

	HWND btcriar;
	HWND btjuntar;
	HWND bthist;

	HWND selmp;
	HWND selma;


	LPTSTR szText = new TCHAR[254];
	TCHAR szText2[MAX_PATH];

	LVCOLUMN lvc; LVITEM lvi;

	int res = 0;
	int i = 0;

	info = NULL;


	switch (message)
	{
	case WM_CREATE:

		break;

	case WM_INITDIALOG:



		info = ObterInfoServidor(servHandler, jogo);

		hWndListHist = GetDlgItem(hDlg, IDC_LISTHIST);

		if (info == NULL) {

			MessageBox(NULL, _T("N�o Consegui obter hist�rico do servidor!"), _T("Dungeon RPG"), MB_OK);
			//EndDialog(hDlg, 0);
			//return (INT_PTR)FALSE;
		}
		else
		{
			ZeroMemory(&lvc, sizeof(lvc));


			lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			lvc.iSubItem = 0; swprintf(szText, 50, TEXT("hist")); lvc.pszText = szText; lvc.cx = 5; lvc.fmt = LVCFMT_LEFT;
			SendMessage(hWndListHist, LVM_INSERTCOLUMN, 0, (LPARAM)&lvc);


			lvc.iSubItem = 1; swprintf(szText, 10, TEXT("Jogador")); lvc.pszText = szText; lvc.cx = 100; lvc.fmt = LVCFMT_LEFT;
			SendMessage(hWndListHist, LVM_INSERTCOLUMN, 1, (LPARAM)&lvc);

			lvc.iSubItem = 1; swprintf(szText, 10, TEXT("Vit.")); lvc.pszText = szText; lvc.cx = 40; lvc.fmt = LVCFMT_RIGHT;
			SendMessage(hWndListHist, LVM_INSERTCOLUMN, 2, (LPARAM)&lvc);

			lvc.iSubItem = 2; swprintf(szText, 10, TEXT("Derr.")); lvc.pszText = szText; lvc.cx = 40; lvc.fmt = LVCFMT_LEFT;
			SendMessage(hWndListHist, LVM_INSERTCOLUMN, 3, (LPARAM)&lvc);

			lvc.iSubItem = 3; swprintf(szText, 10, TEXT("Des.")); lvc.pszText = szText; lvc.cx = 40; lvc.fmt = LVCFMT_RIGHT;
			SendMessage(hWndListHist, LVM_INSERTCOLUMN, 4, (LPARAM)&lvc);

			SendMessage(hWndListHist, LVM_DELETECOLUMN, 0, 0);

			SendMessage(hWndListHist, LVM_DELETEALLITEMS, 0, 0);


			for (int i = 0; i < 5; i++)
			{
				ZeroMemory(&lvi, sizeof(lvi));
				lvi.mask = LVIF_TEXT;
				lvi.iItem = 0;
				lvi.iSubItem = i;
				swprintf(szText, 100, info->hist.registo[i].nome);
				lvi.pszText = szText;
				SendMessage(hWndListHist, LVM_INSERTITEMW, i, (LPARAM)&lvi);
				/*
				ZeroMemory(&lvi, sizeof(lvi));
				lvi.mask = LVIF_TEXT;
				lvi.iItem = 0;
				lvi.iSubItem = 1;
				swprintf(szText, 10,TEXT("%d"), info->hist.registo[i].vitoria);
				lvi.pszText = szText;
				SendMessage(hWndListHist, LVM_INSERTITEMW, 1, (LPARAM)&lvi);

				ZeroMemory(&lvi, sizeof(lvi));
				lvi.mask = LVIF_TEXT;
				lvi.iItem = i;
				lvi.iSubItem = 0;
				swprintf(szText, 10, TEXT("%d"), info->hist.registo[i].derrota);
				lvi.pszText = szText;
				SendMessage(hWndListHist, LVM_INSERTITEMW, 2, (LPARAM)&lvi);

				ZeroMemory(&lvi, sizeof(lvi));
				lvi.mask = LVIF_TEXT;
				lvi.iItem = i;
				lvi.iSubItem = 0;
				swprintf(szText, 10, TEXT("%d"), info->hist.registo[i].desistencia);
				lvi.pszText = szText;
				SendMessage(hWndListHist, LVM_INSERTITEMW, 3, (LPARAM)&lvi);
				*/

			}
		}


		if (info==NULL)
		{ }
		else {

			if (info->jogadoresOnline > 0)
			{

				hWndList = GetDlgItem(hDlg, IDC_LISTJOGADORES2);
				btjuntar = GetDlgItem(hDlg, IDC_BTJUNTAR);

				EnableWindow(btjuntar, TRUE);

				SendMessage(hWndList, LB_RESETCONTENT, 0, 0);
				for (int i = 0; i < info->jogadoresOnline; i++)
					SendMessage(hWndList, LB_ADDSTRING, 0, (LPARAM)info->jogadores[i].nome);


			}
			else
			{
				btcriar = GetDlgItem(hDlg, IDC_BTCRIAR);
				EnableWindow(btcriar, TRUE);

				selma = GetDlgItem(hDlg, IDC_SCMP);
				EnableWindow(selma, TRUE);

				selmp = GetDlgItem(hDlg, IDC_SCPD);
				EnableWindow(selmp, TRUE);

			}
		}


		CenterWindow(hDlg);

		break;
	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_BTCRIAR) {

			if (IsDlgButtonChecked(hDlg, IDC_SCMP)) {

				jogo->labDefeito = 1;

			}
			else if (IsDlgButtonChecked(hDlg, IDC_SCPD)) {

				jogo->labDefeito = 0;
			}
			else
			{
				MessageBox(NULL, _T("Escolha um tipo de Mapa!"), _T("Dungeon RPG"), MB_OK);
				return (INT_PTR)FALSE;
			}


			swprintf(jogo->jogador.nome, 256, TEXT(""));

			DialogBox(hInst, MAKEINTRESOURCE(IDD_PEDENOME), hWnd, PedeNomeDLG);


			if (_tcsclen(jogo->jogador.nome) <= 5) {

				MessageBox(NULL, _T("Nome Incorreto!"), _T("Dungeon RPG"), MB_OK);
				return (INT_PTR)FALSE;

			}
			else
			{

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




LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int maxX, maxY;
	int wmId;
	actualhWnd = hWnd;

	TCHAR buf[256];
	DWORD n;

	RECT rc;
	int xPos = 0;
	int yPos = 0;

	PAINTSTRUCT ps;
	HDC hdc;
	RECT area;

	int i, tam, raio;

	switch (message)
	{

	case WM_CREATE:

		hdc = GetDC(hWnd);

		GetWindowRect(hWnd, &rc);

		xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
		yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;

		SetWindowPos(hWnd, HWND_TOP, xPos, yPos, 800, 600, SWP_SHOWWINDOW);

		initialImage = (HBITMAP)LoadImage(NULL, TEXT("wallpaper.bmp"), IMAGE_BITMAP, 800, 600, LR_LOADFROMFILE);

		if (initialImage == NULL) {
			exit(1);
		}

		DBuf = NULL;

		break;

	case WM_INITDIALOG:


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

		hdc = BeginPaint(hWnd, &ps);

		//if (DBuf == NULL) {

		//	DBuf = CreateCompatibleDC(hdc);
		//	hbitmap = CreateCompatibleBitmap(hdc, area.right, area.right);
		//	SelectObject(DBuffer, hbitmap);

		//}


		//if (andar == 1)
		//	FillRect(DBuf, &area, hbAndar);
		//else
		//	FillRect(DBuf, &area, hbParado);



		//for (i = MAXELEM - 1; i >= 0; i--) {

		//	tam = tipos[elementos[i].tipo].tam;
		//	raio = tam / 2;
		//	BitBlt(DBuf,
		//		elementos[i].posx - raio,
		//		elementos[i].posy - raio, tam,
		//		tam,
		//		ElemMemDC[elementos[i].tipo],
		//		0, 0,
		//		SRCAND);
		//}

		//BitBlt(hdc, 0, 0, area.right, area.bottom, DBuf, 0, 0, SRCCOPY);

		//EndPaint(hWnd, &ps);



		memdc = CreateCompatibleDC(hdc);

		SelectObject(memdc, initialImage);

		BitBlt(hdc, 0, 0, 800, 600, memdc, 0, 0, SRCCOPY);



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

	case WM_LBUTTONUP:

		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
