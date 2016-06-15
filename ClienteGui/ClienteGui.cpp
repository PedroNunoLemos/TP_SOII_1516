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

HBITMAP ElemBMP[9];
HDC ElemMemDC[9];
HBITMAP OrigDCBMP[9];

TCHAR	*imagens[11] = {
	TEXT("pedra.bmp"),
	TEXT("vitamina.bmp"),
	TEXT("orange.bmp"),
	TEXT("cafeina.bmp"),
	TEXT("jog.bmp"),
	TEXT("ini.bmp"),
	TEXT("bully.bmp"),
	TEXT("dist.bmp"),
	TEXT("vazio.bmp"),
	TEXT("chao.bmp"),
	TEXT("parede.bmp")
};



HDC memdc;
HBITMAP hbit;
HWND hWnd;
HWND actualhWnd;
int conetado = 0;

JogoCliente *jogo;
ServidorInfo *info;


HANDLE pipeAtualizaCliente;
HANDLE hPipe = INVALID_HANDLE_VALUE;
DWORD tot;
HANDLE hMutex;


void  lerbits() {

	int i;

	for (i = 0; i < 11; i++)
		ElemBMP[i] = LerImagemDisco(imagens[i]);
}


void CreateAndSelectAllDC(HDC orig) {

	int i;

	for (i = 0; i < 11; i++) {

		ElemMemDC[i] = CreateCompatibleDC(orig);

		OrigDCBMP[i] = (HBITMAP)SelectObject(ElemMemDC[i], ElemBMP[i]);

	}
}

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

	lerbits();

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


	for (i = 0; i < 9; i++) {

		if (ElemMemDC[i] != NULL)
			SelectObject(ElemMemDC[i], OrigDCBMP[i]);

		DeleteDC(ElemMemDC[i]);

		if (ElemBMP[i] != NULL)
			DeleteObject(ElemBMP[i]);

	}



	SelectObject(DBuf, vista);	//  repoe conteudo original
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


				info = NULL;


				EndDialog(hDlg, 0);

				DialogBox(hInst, MAKEINTRESOURCE(IDD_SERVIDORINFO), hWnd, ServidorInfoDLG);

			}
			else {

				MessageBox(NULL, _T("Não me Consegui Ligar ao servidor!"), _T("Dungeon RPG"), MB_OK);
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

	int res = 0;
	int i = 0;

	info = NULL;


	switch (message)
	{
	case WM_CREATE:

		break;

	case WM_INITDIALOG:

		//hWndListHist = GetDlgItem(hDlg, IDC_HIST);

		swprintf(jogo->jogador.nome, 10, TEXT("Jogador ---"));

		info = ObterInfoServidor(servHandler, jogo);

		if (info == NULL) {

			MessageBox(NULL, _T("Não Consegui obter histórico do servidor!"), _T("Dungeon RPG"), MB_OK);
			//EndDialog(hDlg, 0);
			//return (INT_PTR)FALSE;
		}
		else
		{

			swprintf(jogo->jogador.nome, 10, TEXT("Jogador ---"));

			info = ObterInfoServidor(servHandler, jogo);

			if (info == NULL) {

				MessageBox(NULL, _T("Não Consegui obter histórico do servidor!"), _T("Dungeon RPG"), MB_OK);
				//EndDialog(hDlg, 0);
				//return (INT_PTR)FALSE;
			}
			else
			{

				hWndListHist = GetDlgItem(hDlg, IDC_LISTHIST);



				SendMessage(hWndListHist, LB_RESETCONTENT, 0, 0);

				for (int i = 0; i < info->hist.totReg; i++)
				{
					swprintf(szText, 80, TEXT("%s vit: %d der: %d des:%d "),
						info->hist.registo[i].nome, info->hist.registo[i].vitoria
						, info->hist.registo[i].derrota, info->hist.registo[i].desistencia
						);

					SendMessage(hWndListHist, LB_ADDSTRING, 0, (LPARAM)szText);
				}


			}

			if (info->jogoIniciado)
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

		if (LOWORD(wParam) == IDC_BTJUNTAR) {

			swprintf(jogo->jogador.nome, 256, TEXT(""));

			DialogBox(hInst, MAKEINTRESOURCE(IDD_PEDENOME), hWnd, PedeNomeDLG);


			if (_tcsclen(jogo->jogador.nome) <= 5) {

				MessageBox(NULL, _T("Nome Incorreto!"), _T("Dungeon RPG"), MB_OK);
				return (INT_PTR)FALSE;

			}
			else
			{
				info = ObterInfoServidor(servHandler, jogo);

				if (info->jogoIniciado) {

					if (juntarJogo(servHandler, jogo)) {

						EndDialog(hDlg, 0);
						notificaVista(vista);

						CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtualizaCliente, 0, 0, NULL);

					}
					else {

						MessageBox(NULL, _T("Não consegui juntar me ao jogo!"), _T("Dungeon RPG"), MB_OK);
						return (INT_PTR)FALSE;
					}
				}
			}

		}

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
				info = ObterInfoServidor(servHandler, jogo);

				if (info->jogoIniciado) {


					MessageBox(NULL, _T("Outro utilizador já criou o jogo! volte a entrar"),
						_T("Dungeon RPG"), MB_OK);

					return (INT_PTR)FALSE;
				}

				if (criaIniciaJogo(servHandler, jogo)) {


					EndDialog(hDlg, 0);
					CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtualizaCliente, 0, 0, NULL);
					notificaVista(vista);
				}
				else {

					MessageBox(NULL, _T("Não consegui criar o jogo!"), _T("Dungeon RPG"), MB_OK);
					return (INT_PTR)FALSE;
				}
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
	HBITMAP hbitmap;

	int i, tam, raio;

	int ix = 0;
	int iy = 0;

	int k = 0;
	char c = ' ';


	switch (message)
	{

	case WM_CLOSE:
		CloseHandle(servHandler);
		break;

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

		CreateAndSelectAllDC(hdc);
		ReleaseDC(hWnd, hdc);


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
			if (!conetado)
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

		GetClientRect(hWnd, &area);

		if (DBuf == NULL) {

			DBuf = CreateCompatibleDC(hdc);
			hbitmap = CreateCompatibleBitmap(hdc, area.right, area.right);
			SelectObject(DBuf, hbitmap);

		}

		info = ObterInfoServidor(servHandler, jogo);

		if (info == NULL) {

			memdc = CreateCompatibleDC(hdc);

			SelectObject(memdc, initialImage);

			BitBlt(hdc, 0, 0, 800, 600, memdc, 0, 0, SRCCOPY);

		}
		else {


			if (info->jogoIniciado == 1)
			{

				for (ix = 0; ix < MAXVISX; ix++)
				{
					for (iy = 0; iy < MAXVISY; iy++) {


						switch (jogo->mapa[ix][iy].tipo)
						{

						case TipoCelula_PORTA:
						case TipoCelula_CHAO:

							BitBlt(DBuf, ix * 32 + 1, iy * 32 + 1,
								32, 32,ElemMemDC[9], 0, 0, SRCAND);

				/*			switch (jogo->mapa[ix][iy].objeto)
							{
							case Tipo_Pedra:

								memdc = CreateCompatibleDC(hdc);
								SelectObject(memdc, pedra);

								BitBlt(hdc, ix * 32 + 1, iy * 32 + 1,
									32, 32, memdc, 0, 0, SRCAND);

								break;

							case Tipo_SacoPedra:


								memdc = CreateCompatibleDC(hdc);
								SelectObject(memdc, pedra);

								BitBlt(hdc, ix * 32 + 1, iy * 32 + 1,
									32, 32, memdc, 0, 0, SRCAND);


								break;

							case Tipo_Pocao:


								memdc = CreateCompatibleDC(hdc);
								SelectObject(memdc, vitamina);

								BitBlt(hdc, ix * 32 + 1, iy * 32 + 1,
									32, 32, memdc, 0, 0, SRCAND);


								break;

							case Tipo_Vitamina:


								memdc = CreateCompatibleDC(hdc);
								SelectObject(memdc, vitamina);

								BitBlt(hdc, ix * 32 + 1, iy * 32 + 1,
									32, 32, memdc, 0, 0, SRCAND);


								break;

							case Tipo_Cafeina:

								memdc = CreateCompatibleDC(hdc);
								SelectObject(memdc, cafeina);

								BitBlt(hdc, ix * 32 + 1, iy * 32 + 1,
									32, 32, memdc, 0, 0, SRCAND);

								break;

							case Tipo_OrangeBull:

								memdc = CreateCompatibleDC(hdc);
								SelectObject(memdc, orange);

								BitBlt(hdc, ix * 32 + 1, iy * 32 + 1,
									32, 32, memdc, 0, 0, SRCAND);

								break;


							default:
								break;

							}

							if (jogo->mapa[ix][iy].jogador != 0 &&
								jogo->mapa[ix][iy].jogador != jogo->jogador.pidJogador)
							{

								memdc = CreateCompatibleDC(hdc);
								SelectObject(memdc, ini);

								BitBlt(hdc, ix * 32 + 1, iy * 32 + 1,
									32, 32, memdc, 0, 0, SRCAND);

							}


							if (jogo->mapa[ix][iy].monstro != -1 &&
								jogo->mapa[ix][iy].monstro >= 0
								)
							{

								memdc = CreateCompatibleDC(hdc);

								if (jogo->mapa[ix][iy].tipoMonstro == 0)
									SelectObject(memdc, dist);
								else
									SelectObject(memdc, bully);

								BitBlt(hdc, ix * 32 + 1, iy * 32 + 1,
									32, 32, memdc, 0, 0, SRCAND);

							}
*/
							break;

						case TipoCelula_PAREDE:
							BitBlt(DBuf, ix * 32 + 1, iy * 32 + 1,
								32, 32, ElemMemDC[3], 0, 0, SRCAND);
							break;
						default:
							//memdc = CreateCompatibleDC(hdc);
							//SelectObject(memdc, vazio);

							//BitBlt(hdc, ix * 32 + 1, iy * 32 + 1,
							//	32, 32, memdc, 0, 0, SRCAND);
							break;

						}


					}
				}


				//GoToXY(x + (MAXVISX / 2), y + (MAXVISY / 2));
				//setForeGroundAndBackGroundColor(Color_Blue, 6);
				//_tprintf(TEXT("@"));

				BitBlt(hdc, 0, 0, area.right, area.bottom, DBuf, 0, 0, SRCCOPY);
				//BitBlt(hdc, 0, 0, 800, 600, memdc, 0, 0, SRCCOPY);

			}
			else
			{
				memdc = CreateCompatibleDC(hdc);

				SelectObject(memdc, initialImage);

				BitBlt(hdc, 0, 0, 800, 600, memdc, 0, 0, SRCCOPY);

			}
		}

		EndPaint(hWnd, &ps);

	}
	break;

	case WM_KEYDOWN:

		switch ((char)wParam) {
		case VK_DOWN:				// Seta para baixo
			moverJogador(servHandler, jogo, 1);
			InvalidateRect(vista, NULL, FALSE);
			break;
		case VK_LEFT:				// Seta esquerda
			moverJogador(servHandler, jogo, 3);
			InvalidateRect(hWnd, NULL, 1);
			break;
		case VK_RIGHT:				// Seta direita
			moverJogador(servHandler, jogo, 4);
			InvalidateRect(hWnd, NULL, 1);
			break;
		case VK_UP:				// Seta cima
			moverJogador(servHandler, jogo, 2);
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



DWORD WINAPI AtualizaCliente(LPVOID param) {


	JogoCliente *m;
	BOOL ret = FALSE;

	m = (JogoCliente*)malloc(sizeof(JogoCliente));

	while (1) {



		ret = lePipeJogoClienteComRetVal(pipeAtualizaCliente, m);

		if (ret == 1)
			break;


		if (m->pidCliente == jogo->pidCliente)
		{
			jogo = m;
			notificaVista(vista);
		}
	}

	return 0;
}