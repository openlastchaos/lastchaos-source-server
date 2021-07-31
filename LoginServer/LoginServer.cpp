// LoginServer.cpp : Defines the entry point for the console application.
//

#include "stdhdrs.h"
#include "Log.h"
#include "Server.h"
#include "DBCmd.h"

void at_exit_exec (void)
{
	gserver.DisconnectDB();
}

int main(int argc, char* argv[], char* envp[])
{
	int nRetCode = 1004;

#ifdef SIGPIPE
	signal (SIGPIPE, SIG_IGN);
#endif

	if (!gserver.LoadSettingFile())
	{
		puts("Load Setting File Error!!");
		exit(0);
	}

	sprintf(g_buf, "LS%02d", gserver.m_subno);
	g_log.InitLogFile(false, &g_gamelogbuffer, g_buf);

	if (!gserver.ConnectDB())
	{
		puts("Connect DB Error!!");
		exit(0);
	}

	if (!gserver.InitGame())		return 1;

	mysql_close(&gserver.m_dbdata);

	puts("Login Server Running...");

	atexit (at_exit_exec);

	gserver.Run();
	gserver.Close();

	gserver.DisconnectDB();

	if (gserver.m_breboot)
	{
		remove(LOGIN_SERVER_OPEN_CHECK_FILENAME);
		FILE *fp = fopen (".shutdown", "w");
		fclose (fp);
	}

	GAMELOG << init("SYSTEM") << "End!" << end;

	return nRetCode;
}