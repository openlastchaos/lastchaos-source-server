#include "stdhdrs.h"
#include "Exp.h"
#include "Server.h"
#include "DBCmd.h"

static LONGLONG gLevelupExp[MAX_LEVEL];

#ifdef LC_KOR_EXP_DOWN2
void InitExp()
{
	// 경험치 테이블

	LONGLONG i = 0, j = 0;
	LONGLONG sum = 0;
	LONGLONG after90level_seed  = LONGLONG( 100000 ) * LONGLONG(100000);
	LONGLONG after100level_seed = LONGLONG( 1100000 ) * LONGLONG(100000);
	LONGLONG after110level_seed = LONGLONG( 1300000 ) * LONGLONG(100000);
	LONGLONG after130level_seed = LONGLONG( 1600000 ) * LONGLONG(100000);

	// 1 ~ 9레벨 필요경험치
	gLevelupExp[0] = 36;
	gLevelupExp[1] = 130;
	gLevelupExp[2] = 415;
	gLevelupExp[3] = 1161;
	gLevelupExp[4] = 2787;
	gLevelupExp[5] = 5853;
	gLevelupExp[6] = 10535;
	gLevelupExp[7] = 16855;
	gLevelupExp[8] = 23597;

	// 89레벨까지 공식 적용
	for (i = 9; i < 89 && i < MAX_LEVEL; i++)
	{
		j = i + 1;
		gLevelupExp[i] = (j * j * j * j / 8 + 1999) * (j * j) / 10;
		if (gLevelupExp[i] < gLevelupExp[i - 1])
		{
			puts("Exp Error");
			exit(0);
		}
		sum += gLevelupExp[i];
	}

	// 90~ 100레벨까지
	int level_seed =1;

	for(level_seed=1; i < 100 && i < MAX_LEVEL; i++, level_seed++ )
	{
		gLevelupExp[i] = after90level_seed * level_seed;
	}

	for(level_seed=1; i < 110 && i < MAX_LEVEL; i++, level_seed++)
	{
		gLevelupExp[i] = after100level_seed + ( LONGLONG(2000000000) * level_seed ) ;
	}

	for(level_seed=1; i < 130 && i < MAX_LEVEL; i++, level_seed++)
	{
		gLevelupExp[i] = after110level_seed + ( LONGLONG(1500000000) * level_seed ) ;
	}

	for(level_seed=1; i < 140 && i < MAX_LEVEL; i++, level_seed++)
	{
		gLevelupExp[i] = after130level_seed + ( LONGLONG(500000) * LONGLONG(10000)  * level_seed ) ;
	}
}

//LC_KOR_EXP_DOWN2
#elif defined(LC_KOR_EXP_LOAD_DB)

void InitExp()
{
	CDBCmd dbexp;
	dbexp.Init(&gserver.m_dbdata);
	memset( g_buf, 0, MAX_STRING_LENGTH );
	sprintf( g_buf, " SELECT a_level, a_exp FROM t_exp WHERE a_level <= %d order by a_level ", MAX_LEVEL );
	dbexp.SetQuery(g_buf);
	if( !dbexp.Open() )
	{
		puts("t_exp Error");
		exit(0);
	}

	while (dbexp.MoveNext())
	{
		int level = 0;
		dbexp.GetRec("a_level", level );
		if( level > 0 )
			dbexp.GetRec("a_exp", gLevelupExp[level-1] );
		else
		{
			puts("Exp Error");
			exit(0);
		}
	}
}

#else 
void InitExp()
{
	// 경험치 테이블

	LONGLONG i = 0, j = 0;
	LONGLONG sum = 0;
	LONGLONG after90level_seed = 1000000000;
	LONGLONG after100level_seed = 1100000000;
	LONGLONG after110level_seed = 1300000000;
	LONGLONG after120level_seed = 1600000000;

	// 1 ~ 9레벨 필요경험치
	gLevelupExp[0] = 36;
	gLevelupExp[1] = 130;
	gLevelupExp[2] = 415;
	gLevelupExp[3] = 1161;
	gLevelupExp[4] = 2787;
	gLevelupExp[5] = 5853;
	gLevelupExp[6] = 10535;
	gLevelupExp[7] = 16855;
	gLevelupExp[8] = 23597;

	// 89레벨까지 공식 적용
	for (i = 9; i < 89 && i < MAX_LEVEL; i++)
	{
		j = i + 1;
		gLevelupExp[i] = (j * j * j * j / 8 + 1999) * (j * j) / 10;
		if (gLevelupExp[i] < gLevelupExp[i - 1])
		{
			puts("Exp Error");
			exit(0);
		}
		sum += gLevelupExp[i];
	}
	// 94레벨까지
	for(; i < 94 && i < MAX_LEVEL; i++)
	{
		gLevelupExp[i] = after90level_seed * 50;
	}

	for(; i < 99 && i < MAX_LEVEL; i++)
	{
		gLevelupExp[i] = after90level_seed * 100;
	}

	// 110 레벨 까지
	for(; i < 110 && i < MAX_LEVEL; i++)
	{
		gLevelupExp[i] = after100level_seed * 100;
	}

	for(; i < 120 && i < MAX_LEVEL; i++)
	{
		gLevelupExp[i] = after110level_seed * 100;
	}
	for(; i < 130 && i < MAX_LEVEL; i++)
	{
		gLevelupExp[i] = after120level_seed * 100;
	}
		/*
	// 99레벨 이후
	for (i = 99; i < MAX_LEVEL; i++)
	{
		gLevelupExp[i] = sum;
		if (gLevelupExp[i] < gLevelupExp[i - 1])
		{
			puts("Exp Error");
			exit(0);
		}
		sum += sum;
	}
	*/
#ifdef LC_JPN_EXP_DOWN
	for (i = 0; i < MAX_LEVEL; i++)
	{
		if (i < 15)
			continue ;
		else if (i < 19)
			gLevelupExp[i] -= gLevelupExp[i] / 10;
		else if (i < 29)
			gLevelupExp[i] -= gLevelupExp[i] / 4;
		else if (i < 39)
			gLevelupExp[i] -= gLevelupExp[i] * 3 / 20;
		else if (i < 45)
			gLevelupExp[i] -= gLevelupExp[i] / 10;
		else if (i < 50)
			gLevelupExp[i] -= gLevelupExp[i] / 20;
		else if (i < 55)
			gLevelupExp[i] -= gLevelupExp[i] / 50;
		else
			continue ;
	}
#endif // LC_JPN_EXP_DOWN

#ifdef LC_KOR_EXP_DOWN
	for (i = 19; i < MAX_LEVEL; i++)
	{
		gLevelupExp[i] = gLevelupExp[i] * 4 / 5;
		if (gLevelupExp[i] < gLevelupExp[i - 1])
		{
			puts("Exp Error");
			exit(0);
		}
	}
#endif // LC_KOR_EXP_DOWN
}
#endif


LONGLONG GetLevelupExp(int level)
{
	level--;
	if (level < 0)
		return 0;
	else if (level >= MAX_LEVEL)
		level = MAX_LEVEL - 1;

	return gLevelupExp[level];
}
