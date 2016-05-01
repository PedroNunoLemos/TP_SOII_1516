


#include <Windows.h>
#include <stdio.h>
#include <strsafe.h>


/*
** MACROS
*/
# define MY_SERVICE_NAME  TEXT("DungeonServer")
# define MY_SERVICE_DESCRIPTOR  TEXT("Servidor Jogo Dungeon SO2")
# define MY_SERVICE_BIN_NAME  TEXT("servidor.exe")

#define SVC_ERROR                        ((DWORD)0xC0020001L)

/*
** GLOBALS
*/
SERVICE_STATUS g_ServiceStatus;
SERVICE_STATUS_HANDLE g_ServiceStatusHandle;

/*
** INSTALL THE SERVICE
*/
BOOL InstallMyService()
{

	SC_HANDLE schSCManager;
	SC_HANDLE schService;
	TCHAR szPath[MAX_PATH];

	if (!GetModuleFileName(NULL, szPath, MAX_PATH))
	{
		printf("Cannot install service (%d)\n", GetLastError());
		return (FALSE);
	}

	// Get a handle to the SCM database. 

	schSCManager = OpenSCManager(
		NULL,                    // local computer
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (NULL == schSCManager)
	{
		printf("OpenSCManager failed (%d)\n", GetLastError());
		return (FALSE);
	}

	// Create the service

	schService = CreateService(
		schSCManager,              // SCM database 
		MY_SERVICE_NAME,                   // name of service 
		MY_SERVICE_DESCRIPTOR,                   // service name to display 
		SERVICE_ALL_ACCESS,        // desired access 
		SERVICE_WIN32_OWN_PROCESS, // service type 
		SERVICE_DEMAND_START,      // start type 
		SERVICE_ERROR_NORMAL,      // error control type 
		szPath,                    // path to service's binary 
		NULL,                      // no load ordering group 
		NULL,                      // no tag identifier 
		NULL,                      // no dependencies 
		NULL,                      // LocalSystem account 
		NULL);                     // no password 

	if (schService == NULL)
	{
		printf("CreateService failed (%d)\n", GetLastError());
		CloseServiceHandle(schSCManager);
		return (FALSE);
	}
	else printf("Service installed successfully\n");

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	return (TRUE);
}

/*
** DELETE THE SERVICE
*/
BOOL DeleteMyService()
{
	SC_HANDLE schSCManager;
	SC_HANDLE hService;

	if ((schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
		return (FALSE);
	if ((hService = OpenService(schSCManager, MY_SERVICE_NAME, SERVICE_ALL_ACCESS)) == NULL)
		return (FALSE);
	if (!DeleteService(hService))
		return (FALSE);
	if (!CloseServiceHandle(hService))
		return (FALSE);
	return (TRUE);


}


VOID SvcReportEvent(LPTSTR szFunction)
{
	HANDLE hEventSource;
	LPCTSTR lpszStrings[2];
	TCHAR Buffer[80];

	hEventSource = RegisterEventSource(NULL, MY_SERVICE_NAME);

	if (NULL != hEventSource)
	{
		StringCchPrintf(Buffer, 80, TEXT("%s failed with %d"), szFunction, GetLastError());
		(Buffer, 80, TEXT("%s failed with %d"), szFunction, GetLastError());

		lpszStrings[0] = MY_SERVICE_NAME;
		lpszStrings[1] = Buffer;

		ReportEvent(hEventSource,        // event log handle
			EVENTLOG_ERROR_TYPE, // event type
			0,                   // event category
			SVC_ERROR,           // event identifier
			NULL,                // no security identifier
			2,                   // size of lpszStrings array
			0,                   // no binary data
			lpszStrings,         // array of strings
			NULL);               // no binary data

		DeregisterEventSource(hEventSource);
	}
}


/*
** SERVICE EVENT HANDLER
*/
void WINAPI ServiceCtrlHandler(DWORD Opcode)
{
	switch (Opcode)
	{
	case SERVICE_CONTROL_PAUSE:
		g_ServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE:
		g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	case SERVICE_CONTROL_STOP:
		g_ServiceStatus.dwWin32ExitCode = 0;
		g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		g_ServiceStatus.dwCheckPoint = 0;
		g_ServiceStatus.dwWaitHint = 0;
		SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	default:
		break;
	}
}

/*
** SERVICE MAIN ENTRY
*/
void WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{

	HANDLE stopServiceEvent = 0;

	g_ServiceStatus.dwServiceType = SERVICE_WIN32;
	g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwServiceSpecificExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 0;
	g_ServiceStatus.dwWaitHint = 0;
	g_ServiceStatusHandle = RegisterServiceCtrlHandler(MY_SERVICE_NAME, ServiceCtrlHandler);
	
	if (g_ServiceStatusHandle == (SERVICE_STATUS_HANDLE)0)
		return;

	g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	g_ServiceStatus.dwCheckPoint = 0;
	g_ServiceStatus.dwWaitHint = 0;
	SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);

	//do the job :P
	// do initialisation here

	stopServiceEvent = CreateEvent(0, FALSE, FALSE, 0);

	do
	{
		Beep(1000, 100);

	} while (WaitForSingleObject(stopServiceEvent, 5000) == WAIT_TIMEOUT);


}

/*
** MAIN ENTRY
*/
int main(int argc, char* argv[])
{
	if (argc > 1)
	{

		if (!strcmp(argv[1], "-i"))
			InstallMyService();

		else if (!strcmp(argv[1], "-d"))
			if (DeleteMyService())
				printf("\nService correctly deleted\n");
			else
				printf("\nService uncorrectly deleted\n");
	}
	else
	{
		
		SERVICE_TABLE_ENTRY DispatchTable[] = { { MY_SERVICE_NAME, ServiceMain },{ NULL, NULL } };


		if (!StartServiceCtrlDispatcher(DispatchTable)) { SvcReportEvent(TEXT("StartServiceCtrlDispatcher")); }


	}
	return (EXIT_SUCCESS);

}



