#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DLL_IMP_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DLL_IMP_API functions as being imported from a DLL, whereas this DLL sees symbols

#include <windows.h>
#include <tchar.h>
#include <string>


//Definir uma constante para facilitar a leitura do protótipo da função
//Este .h deve ser incluído no projecto que o vai usar (modo implícito)
#define TAM 256

//Esta macro é definida pelo sistema caso estejamos na DLL (<DLL_IMP>_EXPORTS definida)
//ou na app (<DLL_IMP>_EXPORTS não definida) onde DLL_IMP é o nome deste projecto
#ifdef DLL_EXPORTS
#define DLL_IMP_API __declspec(dllexport)
#else
#define DLL_IMP_API __declspec(dllimport)
#endif

#ifdef UNICODE
#define tstring wstring
#else
#define tstring string
#endif
//
//extern "C"
//{
//	//Variável global da DLL
//	extern DLL_IMP_API int nDLL;
//
//	//Funções a serem exportadas/importadas
//	DLL_IMP_API int UmaString();
//}