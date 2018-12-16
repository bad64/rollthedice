#ifndef _RTD_H
#define _RTD_H

#ifdef __cplusplus
extern "C"
{
#endif

#if (defined (_WIN32) || defined (_WIN64))
	char __declspec(dllexport) *help();
	int __declspec(dllexport) randint(int minvalue, int maxvalue);
	char __declspec(dllexport) *roll(char *dice, int color);
#elif (defined (LINUX) || defined (__linux__))
	extern char *help();
	extern int randint(int minvalue, int maxvalue);
	extern char *roll(char *dice, int color);
#endif
	
#ifdef __cplusplus
}
#endif

#endif