#include "Function.h"
#include "ProgressPre.h"

int Change_Process_authority(const char * name)
{
	HANDLE this_pro_Token;
	TOKEN_PRIVILEGES Token_pri;
	LUID luid;
	OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&this_pro_Token);
	LookupPrivilegeValue(NULL,name,&luid);

	/*******************************/
	Token_pri.PrivilegeCount=1;
	Token_pri.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
	Token_pri.Privileges[0].Luid=luid;
	/******************************/
	AdjustTokenPrivileges(this_pro_Token,0,&Token_pri,sizeof(TOKEN_PRIVILEGES),NULL,NULL);
	return 0;
}


int Inject_Process_Name(const char *Dllpath,const DWORD dwRemoteProcessId)
{
	HANDLE hRemoteProcess;
	Change_Process_authority(SE_DEBUG_NAME);//进程提权到指定权限se_debyg_name 是debug权限

	hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwRemoteProcessId);
	char *pszLibFileRemote;
	pszLibFileRemote = (char *)VirtualAllocEx(hRemoteProcess,NULL,strlen(Dllpath)+1,MEM_COMMIT,PAGE_READWRITE);
	WriteProcessMemory(hRemoteProcess,pszLibFileRemote,(void*)Dllpath,strlen(Dllpath)+1,NULL);

	PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE) GetProcAddress(GetModuleHandle(TEXT("Kernel32.dll")),"LoadLibraryA");
	HANDLE hRemoteThread;
	if((hRemoteThread=CreateRemoteThread(hRemoteProcess,NULL,0,pfnStartAddr,pszLibFileRemote,0,NULL))==NULL)
	{
	printf("Inject in Thread Fail!\n");
	return FALSE;
	}

	CloseHandle(hRemoteProcess);
	CloseHandle(hRemoteThread);

	return TRUE;
}


DWORD SelectProcessId()
{
DWORD pid =-1;
HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

PROCESSENTRY32 iPrs;
ZeroMemory(&iPrs,sizeof(iPrs));
iPrs.dwSize = sizeof(iPrs);
char *targetFile="SERVICES.EXE";//目标进程的PID
Process32First(hSnap,&iPrs);
if(strstr(targetFile,iPrs.szExeFile))
{
	pid=iPrs.th32ProcessID;
	return pid;
}
while(TRUE)
{
	ZeroMemory(&iPrs,sizeof(iPrs));
	iPrs.dwSize = sizeof(iPrs);
	if(!Process32Next(hSnap,&iPrs))
		{
			pid=-1;
			break;
		}
	if(strstr(targetFile,iPrs.szExeFile))
		{
		pid=iPrs.th32ProcessID;
		break;
		}
}
return pid;

}