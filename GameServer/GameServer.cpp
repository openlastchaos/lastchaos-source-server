#include "stdhdrs.h"
#include "Log.h"
#include "Server.h"

void at_exit_exec (void)
{
#ifdef USING_GMIP_TABLE
	gserver.DisconnectDB(false);
#endif
	gserver.DisconnectDB(true);
	GAMELOG << flush;

#ifdef USING_NPROTECT
	CleanupGameguardAuth();
#endif // USING_NPROTECT
}

int main(int argc, char* argv[], char* envp[])
{
	gThreadIDGameThread = LC_CURRENT_THREAD_ID;
	gThreadIDDBThread = (LC_THREAD_T)-1;

	int nRetCode = 1004;

#ifdef SIGPIPE
	signal (SIGPIPE, SIG_IGN);
#endif

	if (!gserver.LoadSettingFile())
	{
		puts("Setting Error!!");
		exit(0);
	}

	int i;
	for (i = 0; i < atoi(gserver.m_config.Find("Zones", "Count")); i++)
	{
		CLCString tmp(20);
		tmp.Format("Zone_%d", i);
		if (strcmp(gserver.m_config.Find(tmp, "Remote"), "FALSE") == 0)
			break;
	}
	if (i == atoi(gserver.m_config.Find("Zones", "Count")))
		i = -1;
	else
	{
		CLCString tmp(20);
		tmp.Format("Zone_%d", i);
		i = atoi(gserver.m_config.Find(tmp, "No"));
	}
	g_log.InitLogFile(gserver.m_serverno, gserver.m_subno, i, false, &g_gamelogbuffer);
	g_logDB.InitLogFile(gserver.m_serverno, gserver.m_subno, i, true, &g_dblogbuffer);

	if (!gserver.ConnectDB())
	{
		GAMELOG << init("Cannot Connect DB!!") << end;
		puts("Cannot Connect DB!!");
		exit(0);
	}

	if (!gserver.LoadSettings())
	{
		GAMELOG << init("Setting Error!!") << end;
		puts("Setting Error!!");
		exit(0);
	}

	if (!gserver.CreateDBThread())	return 1;

	if (!gserver.InitGame())		return 1;

#ifndef USING_GMIP_TABLE
	gserver.DisconnectDB(false);
#endif

	puts("SETTINGS");

	if (gserver.m_bOnlyLocal)
		puts(" : Only Local IP");

#ifdef LC_BIG_ENDIAN
	puts(" : Big Endian");
#else
	puts(" : Little Endian");
#endif

#ifdef FORCE_START_ZONE
	puts("Force start zone 0");
#endif

	strcpy (g_buf, "LastChaos Running...");
	puts (g_buf);

	atexit (at_exit_exec);

	gserver.Run();
	gserver.Close();

#ifdef USING_NPROTECT
	CleanupGameguardAuth();
#endif // USING_NPROTECT

	if (gserver.m_bShutdown)
	{
		FILE *fp = fopen (".shutdown", "w");
		fclose (fp);
	}

	GAMELOG << init("SYSTEM")
			<< "End!"
			<< end;

	return nRetCode;
}
