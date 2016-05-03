#include <Windows.h>
#include "principal.h"

BOOL WINAPI DllMain(HANDLE hInstance, DWORD razao, LPVOID tipo_declaracao) {

	//switch (razao) {
	//case DLL_PROCESS_ATTACH:
	//	if (tipo_declaracao == NULL)
	//		MessageBox(NULL, TEXT("O processou carregou a biblioteca explicitamente"), TEXT("DLLmain"), MB_OK);
	//	else
	//		MessageBox(NULL, TEXT("O processou carregou a biblioteca implicitamente"), TEXT("DLLmain"), MB_OK);
	//	break;
	//case DLL_PROCESS_DETACH:
	//	if (tipo_declaracao == NULL)
	//		MessageBox(NULL, TEXT("O processou desmapeou a biblioteca explicitamente"), TEXT("DLLmain"), MB_OK);
	//	else
	//		MessageBox(NULL, TEXT("O processou desmapeou a biblioteca implicitamente"), TEXT("DLLmain"), MB_OK);
	//	break;
	//}

	return 1;

}


