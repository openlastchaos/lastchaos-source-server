#ifndef __EXP_H__
#define __EXP_H__

#if defined (LC_MAL)
#define MAX_LEVEL 120
#elif defined (NIGHT_SHADOW) 
#define MAX_LEVEL 165
#elif defined ( LC_BRZ )
#define MAX_LEVEL 130
#elif defined (LC_USA)
#define MAX_LEVEL 140
#elif defined (LC_HBK)
#define MAX_LEVEL 110
#elif defined (LC_GER)
#define MAX_LEVEL 120
#elif defined (LC_SPN) || defined (LC_FRC) || defined (LC_PLD)
#define MAX_LEVEL 70
#elif defined (LC_TLD)
#define MAX_LEVEL 115
#elif defined(LC_JPN)
#define MAX_LEVEL 140
#elif defined(LC_RUS)
#define MAX_LEVEL 40
#elif defined (LC_KOR) 
#define MAX_LEVEL 145
#else
#define MAX_LEVEL 99
#endif

void InitExp();
LONGLONG GetLevelupExp(int level);

#endif //__EXP_H__
