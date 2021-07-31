// NewEarthServer.cpp : Defines the entry point for the console application.
//

#include "stdhdrs.h"
#include "Log.h"
#include "Server.h"

void at_exit_exec (void)
{
	gserver.m_dbconn.Close();
//	OleUninitialize();
	CoUninitialize();
}

int main(int argc, char* argv[], char* envp[])
{
	int nRetCode = 1004;

#ifdef SIGPIPE
	signal (SIGPIPE, SIG_IGN);
#endif

	if (!gserver.LoadSettingFile())
	{
		puts("Setting Error!!");
		exit(0);
	}

	g_log.InitLogFile(false, &g_gamelogbuffer, "BS");

	if (!InitCOM())					return 1;

	if (!gserver.ConnectDB())		return 1;

#ifndef LC_TLD
	gserver.ReadSectionList();
#endif //#ifndef LC_TLD

	if (!gserver.InitGame())		return 1;

	puts("Billing Server Running...");

	atexit (at_exit_exec);

	gserver.Run();
	gserver.Close();

	if (gserver.m_breboot)
	{
		FILE *fp = fopen (".shutdown", "w");
		fclose (fp);
	}

	GAMELOG << init("SYSTEM") << "End!" << end;

	return nRetCode;
}
