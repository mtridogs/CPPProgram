#define DLL_IMPLEMENT  
#pragma warning(disable:4996)
#include "SimpleDLL.h"  

int DLL_API MessageBoxGetProcessId(HANDLE hMoudle, DWORD reason, LPVOID lpReserved)
{
	char *szProcessId = (char *)malloc(10 * sizeof(char));
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
	{
		_itoa(GetCurrentProcessId(), szProcessId, 10);
		MessageBox(NULL,(LPCWSTR)szProcessId, (LPCWSTR)"RemoteDLL", MB_OK);
		break;
	}
	default:
		break;
		return 0;
	}


}