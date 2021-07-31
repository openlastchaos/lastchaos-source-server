// NewEarthServer.cpp : Defines the entry point for the console application.
//

#include "stdhdrs.h"
#include "Log.h"
#include "Server.h"

FILE *game_log = NULL;

void at_exit_exec (void)
{
	mysql_close(&gserver.m_dbuser);
	mysql_close(&gserver.m_dbauth);
#if defined (EVENT_2PAN4PAN_GOODS) || defined (ATTENDANCE_EVENT) || defined ( ATTENDANCE_EVENT_REWARD ) || defined ( IRIS_POINT ) || defined (EVENT_NOM_MOVIE)
	mysql_close(&gserver.m_dbevent);
#endif // #if defined (EVENT_2PAN4PAN_GOODS) || defined (ATTENDANCE_EVENT) || defined ( ATTENDANCE_EVENT_REWARD ) || defined ( IRIS_POINT )
#if defined ( CHAR_LOG ) && defined ( LC_KOR )
	mysql_close(&gserver.m_dblog);
#endif // #if defined ( CHAR_LOG ) && defined ( LC_KOR )

	if (game_log) fclose (game_log);
}

int main(int argc, char* argv[], char* envp[])
{
	int nRetCode = 1004;

#ifdef SIGPIPE
	signal (SIGPIPE, SIG_IGN);
#endif

	if (!gserver.LoadSettingFile())
	{
		GAMELOG << init("SYS_ERR") << "Load Setting File" << end;
		exit(0);
	}

	sprintf(g_buf, "CS%02d", gserver.m_serverno);
	g_log.InitLogFile(false, &g_gamelogbuffer, g_buf);

	if (!gserver.ConnectDB())
	{
		GAMELOG << init("SYS_ERR") << "Connect DB" << end;
		exit(0);
	}

	if (!gserver.InitGame())
	{
		GAMELOG << init("SYS_ERR") << "Initialize Connector" << end;
		return 1;
	}

	strcpy (g_buf, "LastChaos Connector Running... (Language:");
	strcat (g_buf, "Korean)");
	puts (g_buf);

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
