#pragma once
#include <windows.h>
#include <tchar.h>


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


#ifdef __cplusplus
extern "C" {  // only need to export C interface if
			  // used by C++ source code
#endif

	__declspec(dllexport)  int aleatorio(int min_num, int max_num, int seed);

#ifdef __cplusplus
}
#endif