

#include "stdafx.h"
#include	<Windows.h>
#include	<tchar.h>
#include	<math.h>
#include <iostream>


LRESULT CALLBACK WindowFunc(HWND, UINT, WPARAM, LPARAM);

ATOM  RegistaClasse(HINSTANCE  hThislnst, TCHAR 	*  szWinName);

HWND CriarJanela(HINSTANCE hThisInst, TCHAR	*  szWinName);

void notificaVistas();


#define GLOBALSPEED	3	// velocidade global	(quantos pontos anda de  cada vez)

//
// 	tipos --- 
//-#define THALF 0 
//-#define TLENT 1 
#define MAXTIPOS 6


typedef struct {

	int tam;
	int lent;

} TTipo;

// definicao de tipos de imagens e de  "lentidao"
TTipo tipos[MAXTIPOS] = {
	// tamanho,  "lentidao"
	{ 48,	4 },	// o jogador e sempre deste tipo
	{ 48,	3 },	//
	{ 48,	5 },
	{ 64,	7 },	//
	{ 64,	9 },	//
	{ 32,	1 }
}
;

typedef struct {

	int tipo;
	int cont;
	int posx, posy; int dx, dy;

} TElemento;


#define MAXELEM	9
TElemento elementos[MAXELEM] =
{
	// tipo,  cont,  x,  y,  dx,  dy
	{ 0,	1,	200,	200,	0,	0 },	// o jogador esta sempre nesta posicao
	{ 1,	1,	430,	520,	+1,	+1 },	// as doffs Oltimos nOmeros descreve
	{ 2,	1,	200,	250,	-1,	-1 },	// o tamanho do "passo" dada	(direcc5o em x	e	y
	{ 3,	1,	90,	100,	-1,	+2 },	// e quantos pontos de  cada vez)
	{ 4,	1,	300,	20,	+1,	-1 },	// em rigor da verdade,  os valores
	{ 1,	1,	20,	200,	-1,	-1 },	// deveriam estar entre	-1 e	1,  para cumprir
	{ 2,	1,	200,	100,	+1,	-1 },	// a velocidade tal coma descrita
	{ 5,	1,	100,	150,	+2,	-1 },	// no a factor de lentidao
	{ 5,	1,	300,	350,	-1,	-1 }
};

#define MAXX	600
#define MAXY	600

#define CIMA	1
#define BAIXO	2
#define ESQ	3
#define DIR	4

int TeclaJog = 0;
int andar = 1;

int	moveCima() {

	if (elementos[0].posy > 0) {
		elementos[0].posy -= GLOBALSPEED;
		return	1;	// indica que hove uma	alteracao
	}
	return	0;	// indica nenhuma alteracao
}

int	moveBaixo() {

	if (elementos[0].posy < MAXY - 1) {
		elementos[0].posy += GLOBALSPEED;
		return	1;
	}
	return	0;
}


int	moveEsq() {

	if (elementos[0].posx > 0) {
		elementos[0].posx -= GLOBALSPEED;
		return	1;
	}
	return	0;
}

int	moveDir() {
	if (elementos[0].posx < MAXX - 1) {
		elementos[0].posx += GLOBALSPEED;
	}
	return	0;
}


int	movimentaJog() {

	int res = 0;

	if (andar != 1)
		return	0;

	if (elementos[0].cont > 0)	// repete verificac5o
		return	0;

	switch (TeclaJog) {
	case CIMA:
		res = moveCima();
		break;	//  break redundante devido ao return
	case BAIXO:
		res = moveBaixo(); break;
	case ESQ:
		res = moveEsq(); break;
	case DIR:
		res = moveDir(); break;
	default:
		res = 0;
		break;
	}

	if (res > 0)	elementos[0].cont = tipos[0].lent;	//  repoe contador
	return res;	// indica  se houve alteracao

}

int movimentaElemento(int elem) {

	if (elem < 0 || elem >= MAXELEM)
		return	0;	// nao a suposto acontecer

	if (elementos[elem].cont > 0)
		return	0;	// tem que esperar mais	(nao a suposto acontecer)

	elementos[elem].posx += elementos[elem].dx	* GLOBALSPEED;

	if (elementos[elem].posx < 0) {
		elementos[elem].posx = 0;
		elementos[elem].dx = -elementos[elem].dx;	//  inverte mov em x
	}
	else
		if (elementos[elem].posx >= MAXX) {
			elementos[elem].posx = MAXX - 1;
			elementos[elem].dx = -elementos[elem].dx;	//  inverte mov em x
		}
	// y

	elementos[elem].posy += elementos[elem].dy	* GLOBALSPEED;


	if (elementos[elem].posy < 0) {
		elementos[elem].posy = 0;
		elementos[elem].dy = -elementos[elem].dy;	//  inverte mov em y
	}
	else
		if (elementos[elem].posy >= MAXY) {
			elementos[elem].posy = MAXY - 1;
			elementos[elem].dy = -elementos[elem].dy;	//  inverte may em y
		}
	//  repoe contador

	elementos[elem].cont = tipos[elementos[elem].tipo].lent;

	return	1;	// assinala alteracao


}


void verificaEncontros() {

	int i, distx, disty;
	double dist, mindist;

	for (i = 1; i < MAXELEM; i++)
	{
		distx = elementos[i].posx - elementos[0].posx;
		disty = elementos[i].posy - elementos[0].posy;

		dist = sqrt(distx*distx + disty*disty);

		mindist = tipos[elementos[i].tipo].tam / 2;
		mindist = sqrt(2 * (mindist*mindist));

		if (dist < mindist) andar = 0;
	}
}

void moveTudo() {

	int i, alter;
	alter = 0;
	// jogador
	elementos[0].cont--;

	if (elementos[0].cont <= 0) alter += movimentaJog();

	//  restantes elementos
	for (i = 1; i < MAXELEM; i++) {
		elementos[i].cont--;
		if (elementos[i].cont <= 0) alter += movimentaElemento(i);
	}

	verificaEncontros();
	if (alter > 0) notificaVistas();

}

int MContinua = 1;

DWORD WINAPI Motor(LPVOID lpvParam) {

	while (MContinua == 1) {

		Sleep(5);

		if (andar) moveTudo();
	}

	return	0;

}

HBITMAP ElemBMP[MAXTIPOS];
HDC ElemMemDC[MAXTIPOS];
HBITMAP OrigDCBMP[MAXTIPOS];

TCHAR	*  ElemImagens[MAXTIPOS] = {
	TEXT("elem0.bmp"),
	TEXT("eleml.bmp"),
	TEXT("elem2.bmp"),
	TEXT("elem3.bmp"),
	TEXT("elem4.bmp"),
	TEXT("elem5.bmp")
};

HDC DBuf;
HBITMAP hbitmap;
HBITMAP hOrigDBBmp;

HWND vista;

HBRUSH hbAndar = (HBRUSH)GetStockObject(WHITE_BRUSH);   //fundo jogo a decorrer
HBRUSH hbParado = (HBRUSH)GetStockObject(DKGRAY_BRUSH);	 // fundo jogo parou


void notificaVistas() {
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
		ElemBMP[i] = LoadImagemDisco(ElemImagens[i]);
}

void CreateAndSelectAllDC(HDC orig) {

	int i;

	for (i = 0; i < MAXTIPOS; i++) {

		ElemMemDC[i] = CreateCompatibleDC(orig);

		OrigDCBMP[i] = (HBITMAP)SelectObject(ElemMemDC[i], ElemBMP[i]);

	}
}

int	WINAPI	_tAWinMain(HINSTANCE  hThisInst, HINSTANCE hPrevInst, TCHAR *lpszCmdLine, int nwinMode) {

	UNREFERENCED_PARAMETER(hPrevInst);
	UNREFERENCED_PARAMETER(lpszCmdLine);

	TCHAR 	JanelaPrinc[] = TEXT("Bitmaps animação");

	MSG msg;
	HANDLE hThread;
	DWORD dwThreadId;

	int i;

	loadAllBitmaps();


	if (!RegistaClasse(hThisInst, JanelaPrinc))
		return	0;

	vista = CriarJanela(hThisInst, JanelaPrinc);

	ShowWindow(vista, nwinMode);
	UpdateWindow(vista);

	//tratar movimentos thread


	//	-  lanca thread

	hThread = CreateThread(NULL, 0, Motor, (LPVOID)NULL, 0, &dwThreadId);

	if (hThread == NULL) {}

	while (GetMessage(&msg, NULL, 0, 0)) {

		TranslateMessage(&msg);	/* trad cod.  teclado esp	*/
		DispatchMessage(&msg);	/* reencaminha para janela alvo*/

	}

	MContinua = 0;

	WaitForSingleObject(hThread, 10000);

	for (i = 0; i < MAXTIPOS; i++) {

		if (ElemMemDC[i] != NULL)
			SelectObject(ElemMemDC[i], OrigDCBMP[i]);

		DeleteDC(ElemMemDC[i]);

		if (ElemBMP[i] != NULL)
			DeleteObject(ElemBMP[i]);

	}


	SelectObject(DBuf, hOrigDBBmp);	//  repoe conteudo original
	DeleteObject(DBuf);
	return msg.wParam;

}




ATOM	RegistaClasse(HINSTANCE hThisInst, TCHAR	* szWinName) {

	WNDCLASSEX wcl;

	wcl.cbSize = sizeof(WNDCLASSEX);
	wcl.hInstance = hThisInst;
	wcl.lpszClassName = szWinName;
	wcl.lpfnWndProc = WindowFunc;
	wcl.style = CS_HREDRAW;
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcl.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcl.lpszMenuName = NULL;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hbrBackground = NULL;

	return RegisterClassEx(&wcl);

}

HWND CriarJanela(HINSTANCE hThisInst, TCHAR	*  szWinName) {

	return CreateWindow(
		szWinName,
		TEXT("502	-  exempla Win32	-  Bitmaps  "),	// titulo
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

LRESULT CALLBACK WindowFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	PAINTSTRUCT ps;
	HDC hdc;

	int i, tam, raio;	// tamd2;

	RECT area;

	switch (message) {

	case	WM_CREATE:

		hdc = GetDC(hwnd);
		CreateAndSelectAllDC(hdc);
		ReleaseDC(hwnd, hdc);
		DBuf = NULL;

		break;

	case	WM_DESTROY:	// Encerra a janela
		PostQuitMessage(0);	//	0 é a exit-code
		break;

	case	WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);

		// prepara dbbuf
		GetClientRect(hwnd, &area); 	// pq.  area nao definida em WM_CREATE

		if (DBuf == NULL) {

			DBuf = CreateCompatibleDC(hdc);
			hbitmap = CreateCompatibleBitmap(hdc, area.right, area.right);
			SelectObject(DBuf, hbitmap);

		}


		if (andar == 1)
			FillRect(DBuf, &area, hbAndar);
		else
			FillRect(DBuf, &area, hbParado);



		for (i = MAXELEM - 1; i >= 0; i--) {

			tam = tipos[elementos[i].tipo].tam;
			raio = tam / 2;
			BitBlt(DBuf,
				elementos[i].posx - raio,
				elementos[i].posy - raio, tam,
				tam,
				ElemMemDC[elementos[i].tipo],
				0, 0,
				SRCAND);
		}

		BitBlt(hdc, 0, 0, area.right, area.bottom, DBuf, 0, 0, SRCCOPY);

		EndPaint(hwnd, &ps);

		break;

	case WM_ERASEBKGND:
		break;



	case	WM_KEYDOWN:
		switch (wParam) {


		case VK_LEFT:

			TeclaJog = ESQ;
			break;

		case VK_RIGHT:
			TeclaJog = DIR;
			break;

		case VK_UP:

			TeclaJog = CIMA;
			break;

		case VK_DOWN:

			TeclaJog = BAIXO;
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