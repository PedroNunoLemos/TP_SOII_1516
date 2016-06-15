#include "stdafx.h"
#include <Windows.h>
#include "mvc.h"



void notificaVista(HWND vista) {
	InvalidateRect(vista, NULL, FALSE);
}


HWND CriarVista(HINSTANCE hThisInst, TCHAR	*  szWinName) {

	return CreateWindow(
		szWinName,
		TEXT("DUNGEON RPG"),
		WS_BORDER,	// WS_OVERLAPPEDWINDOW,	// estilo da janela	= normal
		CW_USEDEFAULT,	//  coordenada x	= escolhida  pelo windows
		CW_USEDEFAULT,	//  coordenada y	= escolhida  pelo windows
		800 + 16,	//  center com a borda e o titulo
		600 + 32,	//
		HWND_DESKTOP,	// sem janela pai
		NULL,	// sem menu
		hThisInst,	//  handle para esta instancia do programa
		NULL	// sem argumentos	(informacao)  adicional
		);

}