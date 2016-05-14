#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DLL_IMP_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DLL_IMP_API functions as being imported from a DLL, whereas this DLL sees symbols

#include <windows.h>
#include <tchar.h>
#include <string>


//Definir uma constante para facilitar a leitura do prot�tipo da fun��o
//Este .h deve ser inclu�do no projecto que o vai usar (modo impl�cito)
#define TAM 256

//Esta macro � definida pelo sistema caso estejamos na DLL (<DLL_IMP>_EXPORTS definida)
//ou na app (<DLL_IMP>_EXPORTS n�o definida) onde DLL_IMP � o nome deste projecto
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
//	//Vari�vel global da DLL
//	extern DLL_IMP_API int nDLL;
//
//	//Fun��es a serem exportadas/importadas
//	DLL_IMP_API int UmaString();
//}