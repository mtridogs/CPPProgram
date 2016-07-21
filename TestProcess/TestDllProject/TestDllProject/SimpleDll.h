#pragma once

#ifdef DLL_IMPLEMENT  
#define DLL_API __declspec(dllexport)  
#else  
#define DLL_API __declspec(dllimport)  
#endif  
#include <Windows.h>
#include <stdlib.h>
DLL_API int MessageBoxGetProcessId(HANDLE hMoudle,DWORD reason,LPVOID lpReserved); 