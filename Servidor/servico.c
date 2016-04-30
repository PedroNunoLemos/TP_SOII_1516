// MySrv.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#define NOME_DO_SERVICO TEXT("ServidorDungeon")
#define TAM 256

SERVICE_STATUS          MyServiceStatus;
SERVICE_STATUS_HANDLE   MyServiceStatusHandle;
BOOL flag;

int status;

VOID  WINAPI MyServiceStart(DWORD argc, LPTSTR *argv); //Faz actualmente o vosso servidor
VOID  WINAPI MyServiceCtrlHandler(DWORD opcode);

void MostrarErro(TCHAR *str, DWORD val);


void _tmain()
{
	SERVICE_TABLE_ENTRY   DispatchTable[] =
	{
		{ NOME_DO_SERVICO, MyServiceStart },
		{ NULL,              NULL }
	};

	if (!StartServiceCtrlDispatcher(DispatchTable))
	{
		MostrarErro(TEXT(" [ServidorDungeon] StartServiceCtrlDispatcher error = %d\n"), GetLastError());
	}
}

void WINAPI MyServiceStart(DWORD argc, LPTSTR *argv)
{
	HANDLE File;
	TCHAR buffer[TAM];
	int n1, n2;
	DWORD nrBytes;

	flag = TRUE;

	MyServiceStatus.dwServiceType = SERVICE_WIN32;
	MyServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	MyServiceStatus.dwControlsAccepted = 0;
	MyServiceStatus.dwWin32ExitCode = 0;
	MyServiceStatus.dwServiceSpecificExitCode = 0;
	MyServiceStatus.dwCheckPoint = 0;
	MyServiceStatus.dwWaitHint = 0;

	MyServiceStatusHandle = RegisterServiceCtrlHandler(
		NOME_DO_SERVICO,
		MyServiceCtrlHandler);

	if (MyServiceStatusHandle == (SERVICE_STATUS_HANDLE)0)
	{
		MostrarErro(TEXT(" [ServidorDungeon] RegisterServiceCtrlHandler failed %d\n"), GetLastError());
		return;
	}

	if (!SetServiceStatus(MyServiceStatusHandle, &MyServiceStatus))
	{
		status = GetLastError();
		MostrarErro(TEXT(" [ServidorDungeon] SetServiceStatus error %ld\n"), status);
		return;
	}

	//TODO: ...

	// Initialization complete - report running status. 
	MyServiceStatus.dwCurrentState = SERVICE_RUNNING;
	MyServiceStatus.dwCheckPoint = 0;
	MyServiceStatus.dwWaitHint = 0;
	MyServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;

	if (!SetServiceStatus(MyServiceStatusHandle, &MyServiceStatus))
	{
		status = GetLastError();
		MostrarErro(TEXT(" [ServidorDungeon] SetServiceStatus error %ld\n"), status);
		return;
	}

	// This is where the service does its work. 
	MostrarErro(TEXT(" [ServidorDungeon] Vou iniciar o processamento \n"), 0);

	while (1) {

		Sleep(1000);


	}

	MyServiceStatus.dwCurrentState = SERVICE_STOPPED;
	MyServiceStatus.dwCheckPoint = 0;
	MyServiceStatus.dwWaitHint = 0;

	if (!SetServiceStatus(MyServiceStatusHandle, &MyServiceStatus))
	{
		status = GetLastError();
		MostrarErro(TEXT(" [ServidorDungeon] SetServiceStatus error %ld\n"), status);
	}

	return;
}

VOID WINAPI MyServiceCtrlHandler(DWORD Opcode)
{
	switch (Opcode)
	{
	case SERVICE_CONTROL_PAUSE:
		// Do whatever it takes to pause here. 
		MyServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;

	case SERVICE_CONTROL_CONTINUE:
		// Do whatever it takes to continue here. 
		MyServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;

	case SERVICE_CONTROL_STOP:
		// Do whatever it takes to stop here. 
		MyServiceStatus.dwWin32ExitCode = 0;
		MyServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		MyServiceStatus.dwCheckPoint = 0;
		MyServiceStatus.dwWaitHint = 0;

		if (!SetServiceStatus(MyServiceStatusHandle,
			&MyServiceStatus))
		{
			status = GetLastError();
			MostrarErro(TEXT(" [ServidorDungeon] SetServiceStatus error %ld\n"), status);
		}
		flag = FALSE;

		MostrarErro(TEXT(" [ServidorDungeon] Leaving MyService \n"), 0);
		return;

	case SERVICE_CONTROL_INTERROGATE:
		// Fall through to send current status. 
		break;

	default:
		MostrarErro(TEXT(" [ServidorDungeon] Unrecognized opcode %ld\n"),
			Opcode);
		break;
	}

	// Send current status. 
	if (!SetServiceStatus(MyServiceStatusHandle, &MyServiceStatus))
	{
		status = GetLastError();
		MostrarErro(TEXT(" [ServidorDungeon] SetServiceStatus error %ld\n"), status);
	}
	return;
}

HANDLE hEventSource;

void MostrarErro(TCHAR *str, DWORD val)
{
	TCHAR buf[TAM * 24];
	TCHAR *msg[2] = { buf,NULL };

	_stprintf_s(buf, sizeof(buf), str, val);

	if (!hEventSource) {
		hEventSource = RegisterEventSource(NULL,            // local machine
			NOME_DO_SERVICO); // source name
	}

	if (hEventSource) {
		ReportEvent(hEventSource,
			EVENTLOG_INFORMATION_TYPE,
			0,
			0,
			NULL,   // sid
			1,
			0,
			(const TCHAR **)msg,
			NULL);
	}

}
