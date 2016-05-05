#include <windows.h>
#include <tchar.h>
#include "labirinto.h"




Labirinto lab;

LRESULT CALLBACK WindProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX  WndCls;
	static char szAppName[] = "ExoBrush";
	MSG         Msg;


	lab.tamx = 70;
	lab.tamy = 70;

	lab = CriaLabirinto(lab, 70, 70, 5);


	WndCls.cbSize = sizeof(WndCls);
	WndCls.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	WndCls.lpfnWndProc = WindProcedure;
	WndCls.cbClsExtra = 0;
	WndCls.cbWndExtra = 0;
	WndCls.hInstance = hInstance;
	WndCls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndCls.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndCls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndCls.lpszMenuName = NULL;
	WndCls.lpszClassName = szAppName;
	WndCls.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);

	RegisterClassEx(&WndCls);

	CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
		szAppName, TEXT("GDI Brushes Fundamentals"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 420, 340,
		NULL, NULL, hInstance, NULL);

	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return (int)(Msg.wParam);
}

LRESULT CALLBACK WindProcedure(HWND hWnd, UINT Msg,
	WPARAM wParam, LPARAM lParam)
{
	HDC         hDC;
	PAINTSTRUCT Ps;
	HBRUSH      NewBrush;



	switch (Msg)
	{
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &Ps);


		NewBrush = CreateSolidBrush(RGB(250, 25, 5));

		SelectObject(hDC, NewBrush);


		for (int k = 0; k < lab.tamsalas; k++) {
			
			Sala sal = lab.salas[k];

			Rectangle(hDC, sal.x, sal.y, sal.x + sal.w, sal.y + sal.h);

		}

		for (int y = 0; y < lab.tamy; y++) {
			for (int x = 0; x < lab.tamx; x++)
			{

				/*	Celula tile = lab.celula[x][y];

					if (tile.tipo == TipoCelula_VAZIO) NewBrush = CreateSolidBrush(RGB(250, 25, 5));
					else if (tile.tipo == TipoCelula_PAREDE) NewBrush = CreateSolidBrush(RGB(250, 25, 5));
					else NewBrush = CreateSolidBrush(RGB(250, 25, 5));*/


		}
	}

	//Rectangle(hDC, 20, 20, 250, 125);
	DeleteObject(NewBrush);

	EndPaint(hWnd, &Ps);
	break;
	case WM_DESTROY:
		PostQuitMessage(WM_QUIT);
		break;
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
}
return 0;
}

//
//int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow)
//{
//
//
//
//	Labirinto lab;
//
//	int x = 0;
//	int y = 0;
//
//	HANDLE hMutex = CreateMutex(NULL, TRUE, TEXT("SERVIDORDUNGEON"));
//
//	if (GetLastError() == ERROR_ALREADY_EXISTS)
//	{
//		MessageBox(NULL, TEXT("_"), TEXT("oups"), MB_OK);
//		CloseHandle(hMutex);
//		return -1;
//	}
//	else
//	{
//		MessageBox(NULL, TEXT("_"), TEXT("OK"), MB_OK);
//	}
//
//
//	lab = CriaLabirinto(lab, 70, 70, 10);
//
//	http://jsfiddle.net/loktar/Y9VtP/
//	return 0;
//}
