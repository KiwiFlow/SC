#ifndef _SCC_H
#define _SCC_H
#define _CRT_SECURE_NO_WARNINGS
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<algorithm>
#include<windows.h>

/*****************************error.h begin***********************************/
/* ���󼶱� */
enum e_ErrorLevel
{
	LEVEL_WARNING,
	LEVEL_ERROR,
};

/* �����׶� */
enum e_WorkStage
{
	STAGE_COMPILE,
	STAGE_LINK,
};
void warning(char *fmt, ...);
void error(char *fmt, ...);
void expect(char *msg);
void skip(int c);
void link_error(char *fmt, ...);
/*****************************error.h end*************************************/
#endif //_SCC_H
