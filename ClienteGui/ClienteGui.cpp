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

#define _CRT_SECURE_NO_WARNINGS


#include "..\Controlador\uteis.h"
#include "..\Controlador\constantes.h"
#include "..\Controlador\jogo.h"
#include "..\Controlador\pipes.h"
#include "..\Controlador\servidorinfo.h"

#define MAX_LOADSTRING 100


#include	<math.h>
#include <iostream>

HANDLE servHandler;

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






#define GLOBALSPEED	3	
#define MAXTIPOS 11

int conetado = 0;

JogoCliente *jogo;
ServidorInfo *info;

HWND actualhWnd;
HANDLE pipeAtualizaCliente;
HANDLE hPipe = INVALID_HANDLE_VALUE;
DWORD tot;
HANDLE hMutex;
HINSTANCE hInst;
HWND hWnd;
HDC memdc;
HBITMAP initialImage;


#define MAXELEM	11


#define MAXX	600
#define MAXY	600

#define CIMA	1
#define BAIXO	2
#define ESQ	3
#define DIR	4

int TeclaJog = 0;
int andar = 1;
int MContinua = 1;

HBITMAP ElemBMP[MAXTIPOS];
HDC ElemMemDC[MAXTIPOS];
HBITMAP OrigDCBMP[MAXTIPOS];

HDC DBuffer;
HBITMAP hbitmap;
HBITMAP hOrigDBBmp;

HWND vista;

HBRUSH hbAndar = (HBRUSH)GetStockObject(WHITE_BRUSH);   //fundo jogo a decorrer
HBRUSH hbParado = (HBRUSH)GetStockObject(DKGRAY_BRUSH);	 // fundo jogo parou


void notificaVista() {
	InvalidateRect(vista, NULL, FALSE);
}


HBITMAP LoadImagemDisco(TCHAR	* nome) {

	HBITMAP aux;

	aux = (HBITMAP)LoadImage(NULL, nome,
		IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

	if (aux == NULL) {
	}

	return aux;
}

void  loadAllBitmaps() {

	int i;

	for (i = 0; i < MAXTIPOS; i++)
		ElemBMP[i] = LoadImagemDisco(imagens[i]);
}

void CreateAndSelectAllDC(HDC orig) {

	int i;

	for (i = 0; i < MAXTIPOS; i++) {

		ElemMemDC[i] = CreateCompatibleDC(orig);

		OrigDCBMP[i] = (HBITMAP)SelectObject(ElemMemDC[i], ElemBMP[i]);

	}
}

int	WINAPI	_tWinMain(HINSTANCE  hThisInst, HINSTANCE hPrevInst, TCHAR *lpszCmdLine, int nwinMode) {

	UNREFERENCED_PARAMETER(hPrevInst);
	UNREFERENCED_PARAMETER(lpszCmdLine);

	TCHAR 	JanelaPrinc[] = TEXT("DUNGEON RPG");

	MSG msg;
	HANDLE hThread;
	DWORD dwThreadId;

	int i;

	jogo = (JogoCliente*)malloc(sizeof(JogoCliente));
	jogo->pidCliente = GetCurrentProcessId();


	loadAllBitmaps();


	if (!RegistaClasses(hThisInst, JanelaPrinc))
		return	0;

	vista = CriarJanela(hThisInst, JanelaPrinc);

	ShowWindow(vista, nwinMode);
	UpdateWindow(vista);

	while (GetMessage(&msg, NULL, 0, 0)) {

		TranslateMessage(&msg);	/* trad cod.  teclado esp	*/
		DispatchMessage(&msg);	/* reencaminha para janela alvo*/

	}

	MContinua = 0;

	for (i = 0; i < MAXTIPOS; i++) {

		if (ElemMemDC[i] != NULL)
			SelectObject(ElemMemDC[i], OrigDCBMP[i]);

		DeleteDC(ElemMemDC[i]);

		if (ElemBMP[i] != NULL)
			DeleteObject(ElemBMP[i]);

	}


	SelectObject(DBuffer, hOrigDBBmp);	//  repoe conteudo original
	DeleteObject(DBuffer);
	return msg.wParam;

}




ATOM	RegistaClasses(HINSTANCE hThisInst, TCHAR	* szWinName) {

	WNDCLASSEX wcl;

	wcl.cbSize = sizeof(WNDCLASSEX);
	wcl.hInstance = hThisInst;
	wcl.lpszClassName = szWinName;
	wcl.lpfnWndProc = WindowFunc;
	wcl.style = CS_HREDRAW;
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcl.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcl.lpszMenuName = wcl.lpszMenuName = MAKEINTRESOURCEW(IDC_CLIENTEGUI);
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hbrBackground = NULL;

	return RegisterClassEx(&wcl);

}

HWND CriarJanela(HINSTANCE hThisInst, TCHAR	*  szWinName) {

	return CreateWindow(
		szWinName,
		TEXT("Dungeon RPG"),	// titulo
		WS_BORDER,	// WS_OVERLAPPEDWINDOW,	// estilo da janela	= normal
		CW_USEDEFAULT,	//  coordenada x	= escolhida  pelo windows
		CW_USEDEFAULT,	//  coordenada y	= escolhida  pelo windows
		MAXX + 16,	//  center com a borda e o titulo
		MAXY + 32,	//
		HWND_DESKTOP,	// sem janela pai
		NULL,	// sem menu
		hThisInst,	//  handle para esta instancia do programa
		NULL	// sem argumentos	(informacao)  adicional
		);

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
						notificaVista();

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
					notificaVista();
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



LRESULT CALLBACK WindowFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	PAINTSTRUCT ps;
	HDC hdc;
	int wmId;

	int i, tam, raio;	// tamd2;
	int ix = 0;
	int iy = 0;

	int k = 0;
	char c = ' ';


	RECT area;

	switch (message) {

	case	WM_CREATE:


		initialImage = (HBITMAP)LoadImage(NULL, TEXT("wallpaper.bmp"), IMAGE_BITMAP, 800, 600, LR_LOADFROMFILE);

		if (initialImage == NULL) {
			exit(1);
		}


		hdc = GetDC(hwnd);
		CreateAndSelectAllDC(hdc);
		ReleaseDC(hwnd, hdc);
		DBuffer = NULL;

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


	case	WM_DESTROY:	// Encerra a janela
		PostQuitMessage(0);	//	0 é a exit-code
		break;

	case	WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);

		// prepara dbbuf
		GetClientRect(hwnd, &area); 	// pq.  area nao definida em WM_CREATE

		if (DBuffer == NULL) {

			DBuffer = CreateCompatibleDC(hdc);
			hbitmap = CreateCompatibleBitmap(hdc, area.right, area.right);
			SelectObject(DBuffer, hbitmap);

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

							BitBlt(DBuffer, ix * 32 + 1, iy * 32 + 1,
								32, 32, ElemMemDC[IM_chao], 0, 0, SRCAND);

							switch (jogo->mapa[ix][iy].objeto)
							{
							case Tipo_Pedra:

								BitBlt(DBuffer, ix * 32 + 1, iy * 32 + 1,
									32, 32, ElemMemDC[IM_pedra], 0, 0, SRCAND);

								break;

							case Tipo_SacoPedra:


								BitBlt(DBuffer, ix * 32 + 1, iy * 32 + 1,
									32, 32, ElemMemDC[IM_pedra], 0, 0, SRCAND);

								break;

							case Tipo_Pocao:


								BitBlt(DBuffer, ix * 32 + 1, iy * 32 + 1,
									32, 32, ElemMemDC[IM_vitamina], 0, 0, SRCAND);

								break;

							case Tipo_Vitamina:


								BitBlt(DBuffer, ix * 32 + 1, iy * 32 + 1,
									32, 32, ElemMemDC[IM_vitamina], 0, 0, SRCAND);

								break;

							case Tipo_Cafeina:

								BitBlt(DBuffer, ix * 32 + 1, iy * 32 + 1,
									32, 32, ElemMemDC[IM_cafeina], 0, 0, SRCAND);

								break;

							case Tipo_OrangeBull:

								BitBlt(DBuffer, ix * 32 + 1, iy * 32 + 1,
									32, 32, ElemMemDC[IM_orange], 0, 0, SRCAND);

								break;


							default:
								break;

							}


							if (jogo->mapa[ix][iy].jogador != 0 &&
								jogo->mapa[ix][iy].jogador != jogo->jogador.pidJogador)
							{
								BitBlt(DBuffer, ix * 32 + 1, iy * 32 + 1,
									32, 32, ElemMemDC[IM_ini], 0, 0, SRCAND);

							}


							if (jogo->mapa[ix][iy].monstro != -1 &&
								jogo->mapa[ix][iy].monstro >= 0
								)
							{

								memdc = CreateCompatibleDC(hdc);

								if (jogo->mapa[ix][iy].tipoMonstro == 0)
									BitBlt(DBuffer, ix * 32 + 1, iy * 32 + 1,
										32, 32, ElemMemDC[IM_dist], 0, 0, SRCAND);
								else
									BitBlt(DBuffer, ix * 32 + 1, iy * 32 + 1,
										32, 32, ElemMemDC[IM_bully], 0, 0, SRCAND);



							}

							break;

						case TipoCelula_PAREDE:
							BitBlt(DBuffer, ix * 32 + 1, iy * 32 + 1,
								32, 32, ElemMemDC[IM_parede], 0, 0, SRCAND);
							break;
						default:
							BitBlt(DBuffer, ix * 32 + 1, iy * 32 + 1,
								32, 32, ElemMemDC[IM_vazio], 0, 0, SRCAND);

							break;

						}


					}
				}


				BitBlt(DBuffer, 100 * 32 * 7 + 1, 100 * 32 + 1* 7,
					32, 32, ElemMemDC[IM_jog], 0, 0, SRCAND);



				BitBlt(hdc, 0, 0, area.right, area.bottom, DBuffer, 0, 0, SRCCOPY);


			}
		}


		//if (andar == 1)
		FillRect(DBuffer, &area, hbAndar);
		/*else
			FillRect(DBuffer, &area, hbParado);*/


		EndPaint(hwnd, &ps);

		break;

	case WM_ERASEBKGND:
		break;



	case	WM_KEYDOWN:
		switch (wParam) {


		case VK_LEFT:
			moverJogador(servHandler, jogo, 3);
			notificaVista();
			break;

		case VK_RIGHT:
			moverJogador(servHandler, jogo, 4);
			notificaVista();
			break;

		case VK_UP:

			moverJogador(servHandler, jogo, 2);
			notificaVista();
			break;

		case VK_DOWN:
			moverJogador(servHandler, jogo, 1);
			notificaVista();
			break;

		case VK_ESCAPE:
			PostQuitMessage(0);
			break;

		default:
			break;
		}

		break;
	case	WM_KEYUP:
		break;

	case	WM_CHAR:

		if (wParam == ' ') TeclaJog = 0;

		if (wParam == '5') andar = 1;

		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);

		return	0;

	}
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
			notificaVista();
		}
	}

	return 0;
}