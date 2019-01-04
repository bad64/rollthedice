#include "rtd.h"

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#if (defined (_WIN32) || defined (_WIN64))
	char __declspec(dllexport) *help()
#elif (defined (LINUX) || defined (__linux__))
	extern char *help()
#endif
{
	#if (defined (_WIN32) || defined (_WIN64))
		return "Usage: rtd <x>d<y>\n\t<x>: Number of dice to roll\n\t<y>: Number of faces on each dice\n\t<+/-z>: Optional modifier, positive or negative";
	#elif (defined (LINUX) || defined (__linux__))
		return "Usage: rtd [options] <x>d<y>\n    Options:\n\t-c/--color: Enables colorized output\n\t-n/--nocolor: Disables colorized output\n    Dice formatting:\n\t<x>: Number of dice to roll\n\t<y>: Number of faces on each dice\n\t<+/-z>: Optional modifier, positive or negative";
	#endif
	
}

#if (defined (_WIN32) || defined (_WIN64))
	int __declspec(dllexport) randint(int minvalue, int maxvalue)
#elif (defined (LINUX) || defined (__linux__))
	extern int randint(int minvalue, int maxvalue)
#endif
{
	//Source: https://stackoverflow.com/questions/2509679/how-to-generate-a-random-integer-number-from-within-a-range
	
	int r;
	const unsigned int range = 1 + maxvalue - minvalue;
	const unsigned int buckets = RAND_MAX / range;
	const unsigned int limit = buckets * range;

	#if (defined (_WIN32) || defined (_WIN64))
		do
		{
			r = rand();
		} while (r >= limit);
	#elif (defined (LINUX) || defined (__linux__))
		do
		{
			r = random();
		} while (r >= limit);
	#endif
		
	return minvalue + (r / buckets);
}

#if (defined (_WIN32) || defined (_WIN64))
	char __declspec(dllexport) *roll(char *dice, int color)
#elif (defined (LINUX) || defined (__linux__))
	extern char *roll(char *dice, int color)
#endif
{
	if (dice[0] == '?')
		return help();
	else	//Enter main loop
	{
		//Check if dice expression is ok
		int dchar = 0, signchar = 0;
		char *buf_dices, *buf_faces, *buf_mod;
		
		if (strchr(dice, 'd') != NULL)
			dchar = (int)(strchr(dice, 'd') - dice);
		else	//Not a dice
			return help();
		
		//Do we have a modifier to this roll ?
		if (strchr(dice, '+') != NULL)
			signchar = (int)(strchr(dice, '+') - dice);
		else if (strchr(dice, '-') != NULL)
			signchar = (int)(strchr(dice, '-') - dice);
		else	//No modifier
			signchar = 0;
		

		buf_dices = calloc(dchar + 1, sizeof(char));
		memset(buf_dices, '\0', dchar);

		if (signchar == 0)	//No modifier
		{
			buf_faces = calloc(strlen(dice) - dchar + 1, sizeof(char));
			memset(buf_faces, '\0', strlen(dice) - dchar + 1);
		}
		else
		{
			buf_faces = calloc(signchar - dchar + 1, sizeof(char));
			memset(buf_faces, '\0', signchar - dchar + 1);
		}
		
		if (dchar == 0)
			buf_dices[0] = '1';
		else
			strncpy(buf_dices, dice, dchar);
		
		if (signchar == 0)	//No modifier
		{
			strncpy(buf_faces, &dice[dchar+1], strlen(dice) - dchar - 1);
			//printf("%s %s\n", buf_dices, buf_faces);
		}
		else
		{
			strncpy(buf_faces, &dice[dchar+1], signchar - dchar - 1);
			
			buf_mod = (char *)calloc(strlen(dice) - signchar + 1, sizeof(char));
			memset(buf_mod, '\0', strlen(dice) - signchar + 1);
			strncpy(buf_mod, &dice[signchar+1], strlen(dice) - signchar + 1);
			
			//printf("%s %s %c%s\n", buf_dices, buf_faces, dice[signchar], buf_mod);
		}
		
		int dices, faces, modifier;
		
		sscanf(buf_dices, "%d", &dices);
		sscanf(buf_faces, "%d", &faces);
		
		if (signchar != 0)
			sscanf(buf_mod, "%d", &modifier);
		else
		{
			buf_mod = (char *)calloc(2, sizeof(char));
			memset(buf_mod, '\0', 2);
			buf_mod[0] = '0';
		}
		
		if ((dices == 0) || (faces == 0))
		{
			return help();
		}
		else
		{
			//Proceed to actually roll
			int result = 0;
			
			if (dices == 1)
			{
				result = randint(1, faces);
				
				if (result == 1)
				{
					#if defined (WIN32)
						if (modifier == 0)
						{							
							size_t s_msg = strlen(dice) + strlen(": CRITICAL FAILURE !! You rolled a 1...") + 1;
							char *msg = (char *)calloc(s_msg, sizeof(char));
							memset(msg, '\0', s_msg);
							sprintf(msg, "%s: CRITICAL FAILURE !! You rolled a 1...", dice);
							
							free(buf_dices);
							free(buf_faces);
							free(buf_mod);
							
							return msg;
						}
						else
						{							
							size_t s_msg = strlen(dice) + strlen(": CRITICAL FAILURE !! You rolled a natural 1...") + 1;
							char *msg = (char *)calloc(s_msg, sizeof(char));
							memset(msg, '\0', s_msg);
							sprintf(msg, "%s: CRITICAL FAILURE !! You rolled a natural 1...", dice);
							
							free(buf_dices);
							free(buf_faces);
							free(buf_mod);
							
							return msg;
						}
					#elif defined (__linux__)
						if (modifier == 0)
						{
							if (color)
							{
								size_t s_msg = strlen(dice) + strlen(": \e[1;91mCRITICAL FAILURE !! \e[21;91mYou rolled a 1...\e[0m") + 1;
								char *msg = (char *)calloc(s_msg, sizeof(char));
								memset(msg, '\0', s_msg);
								sprintf(msg, "%s: \e[1;91mCRITICAL FAILURE ! \e[21;91mYou rolled a 1...\e[0m", dice);
								
								free(buf_dices);
								free(buf_faces);
								free(buf_mod);
								
								return msg;
							}
							else
							{
								size_t s_msg = strlen(dice) + strlen(": CRITICAL FAILURE !! You rolled a 1...") + 1;
								char *msg = (char *)calloc(s_msg, sizeof(char));
								memset(msg, '\0', s_msg);
								sprintf(msg, "%s: CRITICAL FAILURE !! You rolled a 1...", dice);
								
								free(buf_dices);
								free(buf_faces);
								free(buf_mod);
								
								return msg;
							}
						}
						else
						{
							if (color)
							{
								size_t s_msg = strlen(dice) + strlen(": \e[1;91mCRITICAL FAILURE !! \e[21;91mYou rolled a natural 1...\e[0m") + 1;
								char *msg = (char *)calloc(s_msg, sizeof(char));
								memset(msg, '\0', s_msg);
								sprintf(msg, "%s: \e[1;91mCRITICAL FAILURE ! \e[21;91mYou rolled a natural 1...\e[0m", dice);
								
								free(buf_dices);
								free(buf_faces);
								free(buf_mod);
								
								return msg;
							}
							else
							{
								size_t s_msg = strlen(dice) + strlen(": CRITICAL FAILURE !! You rolled a natural 1...") + 1;
								char *msg = (char *)calloc(s_msg, sizeof(char));
								memset(msg, '\0', s_msg);
								sprintf(msg, "%s: CRITICAL FAILURE !! You rolled a natural 1...", dice);
								
								free(buf_dices);
								free(buf_faces);
								free(buf_mod);
								
								return msg;
							}
						}
					#endif
				}
				else if (result == faces)
				{
					#if defined (WIN32)
						if (modifier == 0)
						{							
							size_t s_msg = strlen(dice) + strlen(": CRITICAL SUCCESS !! You rolled a ") + strlen(buf_faces) + strlen(" !!") + 1;
							char *msg = (char *)calloc(s_msg, sizeof(char));
							memset(msg, '\0', s_msg);
							sprintf(msg, "%s: CRITICAL SUCCESS !! You rolled a %s !!", dice, buf_faces);
							
							free(buf_dices);
							free(buf_faces);
							free(buf_mod);
							
							return msg;
						}
						else
						{							
							size_t s_msg = strlen(dice) + strlen(": CRITICAL SUCCESS !! You rolled a natural ") + strlen(buf_faces) + strlen(" !!") + 1;
							char *msg = (char *)calloc(s_msg, sizeof(char));
							memset(msg, '\0', s_msg);
							sprintf(msg, "%s: CRITICAL SUCCESS !! You rolled a natural %s !!", dice, buf_faces);
							
							free(buf_dices);
							free(buf_faces);
							free(buf_mod);
							
							return msg;
						}
					#elif defined (__linux__)
						if (modifier == 0)
						{
							if (color)
							{
								size_t s_msg = strlen(dice) + strlen(": \e[1;92mCRTITICAL SUCCESS !! \e[21;92mYou rolled a ") + strlen(buf_faces) + strlen(" !!\e[0m") + 1;
								char *msg = (char *)calloc(s_msg, sizeof(char));
								memset(msg, '\0', s_msg);
								sprintf(msg, "%s: \e[1;92mCRTITICAL SUCCESS !! \e[21;92mYou rolled a %s !!\e[0m", dice, buf_faces);
								
								free(buf_dices);
								free(buf_faces);
								free(buf_mod);
								
								return msg;
							}
							else
							{
								size_t s_msg = strlen(dice) + strlen(": CRITICAL SUCCESS !! You rolled a ") + strlen(buf_faces) + strlen(" !!") + 1;
								char *msg = (char *)calloc(s_msg, sizeof(char));
								memset(msg, '\0', s_msg);
								sprintf(msg, "%s: CRITICAL SUCCESS !! You rolled a %s !!", dice, buf_faces);
								
								free(buf_dices);
								free(buf_faces);
								free(buf_mod);
								
								return msg;
							}
						}
						else
						{
							if (color)
							{
								size_t s_msg = strlen(dice) + strlen(": \e[1;92mCRTITICAL SUCCESS !! \e[21;92mYou rolled a natural ") + strlen(buf_faces) + strlen(" !!\e[0m") + 1;
								char *msg = (char *)calloc(s_msg, sizeof(char));
								memset(msg, '\0', s_msg);
								sprintf(msg, "%s: \e[1;92mCRTITICAL SUCCESS !! \e[21;92mYou rolled a natural %s !!\e[0m", dice, buf_faces);
								
								free(buf_dices);
								free(buf_faces);
								free(buf_mod);
								
								return msg;
							}
							else
							{
								size_t s_msg = strlen(dice) + strlen(": CRITICAL SUCCESS !! You rolled a natural ") + strlen(buf_faces) + strlen(" !!") + 1;
								char *msg = (char *)calloc(s_msg, sizeof(char));
								memset(msg, '\0', s_msg);
								sprintf(msg, "%s: CRITICAL SUCCESS !! You rolled a natural %s !!", dice, buf_faces);
								
								free(buf_dices);
								free(buf_faces);
								free(buf_mod);
								
								return msg;
							}
						}
					#endif
				}
				else
				{
					if ((signchar != 0) && (modifier != 0))
					{
						if (dice[signchar] == '+')
						{			
							char *buf_result = (char *)calloc(12, sizeof(char));
							memset (buf_result, '\0', 12);
							sprintf(buf_result, "%d", result);
							
							char *buf_reswithmodifier = (char *)calloc(12, sizeof(char));
							memset (buf_reswithmodifier, '\0', 12);
							sprintf(buf_reswithmodifier, "%d", result + modifier);
							
							size_t s_msg = strlen(buf_dices) + 1 + strlen(buf_faces) + 2 + strlen("You rolled a ") + strlen(buf_reswithmodifier) + strlen(" ! (") + strlen(buf_result) + strlen(" + ") + strlen(buf_mod) + 2;
							char *msg = (char *)calloc(s_msg, sizeof(char));
							memset(msg, '\0', s_msg);
							sprintf(msg, "%sd%s: You rolled a %d ! (%d + %d)", buf_dices, buf_faces, result + modifier, result, modifier);
							
							free(buf_dices);
							free(buf_faces);
							free(buf_mod);
							free(buf_result);
							free(buf_reswithmodifier);
							
							return msg;
						}
						else if (dice[signchar] == '-')
						{
							if (result - modifier < 1)
							{
								char *buf_result = (char *)calloc(12, sizeof(char));
								memset (buf_result, '\0', 12);
								sprintf(buf_result, "%d", result);
								
								size_t s_msg = strlen(buf_dices) + 1 + strlen(buf_faces) + 2 + strlen("You rolled a 1 ! (") + strlen(buf_result) + strlen(" - ") + strlen(buf_mod) + 2;
								char *msg = (char *)calloc(s_msg, sizeof(char));
								memset(msg, '\0', s_msg);
								sprintf(msg, "%sd%s: You rolled a 1 ! (%d + %d)", buf_dices, buf_faces, result, modifier);
								
								free(buf_dices);
								free(buf_faces);
								free(buf_mod);
								free(buf_result);
								
								return msg;
							}
							else
							{
								char *buf_result = (char *)calloc(12, sizeof(char));
								memset (buf_result, '\0', 12);
								sprintf(buf_result, "%d", result);
						
								char *buf_reswithmodifier = (char *)calloc(12, sizeof(char));
								memset (buf_reswithmodifier, '\0', 12);
								sprintf(buf_reswithmodifier, "%d", result - modifier);
								
								size_t s_msg = strlen(buf_dices) + 1 + strlen(buf_faces) + 2 + strlen("You rolled a ") + strlen(buf_reswithmodifier) + strlen(" ! (") + strlen(buf_result) + strlen(" + ") + strlen(buf_mod) + 2;
								char *msg = (char *)calloc(s_msg, sizeof(char));
								memset(msg, '\0', s_msg);
								sprintf(msg, "%sd%s: You rolled a %d ! (%d - %d)", buf_dices, buf_faces, result + modifier, result, modifier);
								
								free(buf_dices);
								free(buf_faces);
								free(buf_mod);
								free(buf_result);
								free(buf_reswithmodifier);
								
								return msg;
							}
						}
						else
						{
							return "Error";
						}
					}
					else
					{				
						char *buf_result = (char *) malloc(12 * sizeof(char));
						memset (buf_result, '\0', 12);
						sprintf(buf_result, "%d", result);
						
						size_t s_msg = strlen(dice) + strlen(": You rolled a ") + strlen(buf_result) + strlen(" !") + 1;
						char *msg = (char *) malloc(s_msg * sizeof(char));
						memset(msg, '\0', s_msg);
						sprintf(msg, "%s: You rolled a %s !", dice, buf_result);
						
						free(buf_dices);
						free(buf_faces);
						free(buf_mod);
						free(buf_result);
						
						return msg;
					}
				}
			}
			else
			{
				int *rolls = (int *)calloc(dices, sizeof(int));
				
				int i;
				for (i = 0; i < dices; i++)
				{
					rolls[i] = randint(1, faces);
				}
				
				for (i = 0; i < dices; i++)
					result += rolls[i];
				
				if (result == dices)
				{
					#if defined (WIN32)
						if (modifier == 0)
						{							
							size_t s_msg = strlen(dice) + strlen(": CRITICAL FAILURE !! You rolled a ") + strlen(buf_dices) + strlen("...") + 1;
							char *msg = (char *)calloc(s_msg, sizeof(char));
							memset(msg, '\0', s_msg);
							sprintf(msg, "%s: CRITICAL FAILURE !! You rolled a %s...", dice, buf_dices);

							free(buf_dices);
							free(buf_faces);
							free(buf_mod);
							
							return msg;
						}
						else
						{							
							size_t s_msg = strlen(dice) + strlen(": CRITICAL FAILURE !! You rolled a natural ") + strlen(buf_dices) + strlen("...") + 1;
							char *msg = (char *)calloc(s_msg, sizeof(char));
							memset(msg, '\0', s_msg);
							sprintf(msg, "%s: CRITICAL FAILURE !! You rolled a natural %s...", dice, buf_dices);
							
							free(buf_dices);
							free(buf_faces);
							free(buf_mod);
							
							return msg;
						}
					#elif defined (__linux__)
						if (modifier == 0)
						{
							if (color)
							{
								size_t s_msg = strlen(dice) + strlen(": \e[1;91mCRITICAL FAILURE !! \e[21;91mYou rolled a ") + strlen(buf_dices) + strlen("...\e[0m") + 1;
								char *msg = (char *)calloc(s_msg, sizeof(char));
								memset(msg, '\0', s_msg);
								sprintf(msg, "%s: \e[1;91mCRITICAL FAILURE !! \e[21;91mYou rolled a %s...\e[0m", dice, buf_dices);
								
								free(buf_dices);
								free(buf_faces);
								free(buf_mod);
								
								return msg;
							}
							else
							{
								size_t s_msg = strlen(dice) + strlen(": CRITICAL FAILURE !! You rolled a ") + strlen(buf_dices) + strlen("...") + 1;
								char *msg = (char *)calloc(s_msg, sizeof(char));
								memset(msg, '\0', s_msg);
								sprintf(msg, "%s: CRITICAL FAILURE !! You rolled a %s...", dice, buf_dices);

								free(buf_dices);
								free(buf_faces);
								free(buf_mod);
								
								return msg;
							}
						}
						else
						{
							if (color)
							{
								size_t s_msg = strlen(dice) + strlen(": \e[1;91mCRITICAL FAILURE !! \e[21;91mYou rolled a natural") + strlen(buf_dices) + strlen("...\e[0m") + 1;
								char *msg = (char *)calloc(s_msg, sizeof(char));
								memset(msg, '\0', s_msg);
								sprintf(msg, "%s: \e[1;91mCRITICAL FAILURE !! \e[21;91mYou rolled a natural %s...\e[0m", dice, buf_dices);
								
								free(buf_dices);
								free(buf_faces);
								free(buf_mod);
								
								return msg;
							}
							else
							{
								size_t s_msg = strlen(dice) + strlen(": CRITICAL FAILURE !! You rolled a natural ") + strlen(buf_dices) + strlen("...") + 1;
								char *msg = (char *)calloc(s_msg, sizeof(char));
								memset(msg, '\0', s_msg);
								sprintf(msg, "%s: CRITICAL FAILURE !! You rolled a natural %s...", dice, buf_dices);
								
								free(buf_dices);
								free(buf_faces);
								free(buf_mod);
								
								return msg;
							}
						}
					#endif
				}
				else if (result == dices * faces)
				{
					#if defined (WIN32)
						if (modifier == 0)
						{
							char *buf_result = (char *) malloc(12 * sizeof(char));
							memset (buf_result, '\0', 12);
							sprintf(buf_result, "%d", result);
							
							size_t s_msg = strlen(dice) + strlen(": CRITICAL SUCCESS !! You rolled a ") + strlen(buf_result) + strlen(" !!") + 1;
							char *msg = (char *)calloc(s_msg, sizeof(char));
							memset(msg, '\0', s_msg);
							sprintf(msg, "%s: CRITICAL SUCCESS !! You rolled a %s !!", dice, buf_result);
							
							free(buf_dices);
							free(buf_faces);
							free(buf_mod);
							free(buf_result);
							
							return msg;
						}
						else
						{
							char *buf_result = (char *) malloc(12 * sizeof(char));
							memset (buf_result, '\0', 12);
							sprintf(buf_result, "%d", result);
						
							size_t s_msg = strlen(dice) + strlen(": CRITICAL SUCCESS !! You rolled a natural ") + strlen(buf_result) + strlen(" !!") + 1;
							char *msg = (char *)calloc(s_msg, sizeof(char));
							memset(msg, '\0', s_msg);
							sprintf(msg, "%s: CRITICAL SUCCESS !! You rolled a natural %s !!", dice, buf_result);

							free(buf_dices);
							free(buf_faces);
							free(buf_mod);
							free(buf_result);
							
							return msg;
						}
					#elif defined (__linux__)
						if (modifier == 0)
						{
							if (color)
							{
								char *buf_result = (char *) malloc(12 * sizeof(char));
								memset (buf_result, '\0', 12);
								sprintf(buf_result, "%d", result);
								
								size_t s_msg = strlen(dice) + strlen(": \e[1;92mCRTITICAL SUCCESS !! \e[21;92mYou rolled a ") + strlen(buf_result) + strlen(" !!\e[0m") + 1;
								char *msg = (char *)calloc(s_msg, sizeof(char));
								memset(msg, '\0', s_msg);
								sprintf(msg, "%s: \e[1;92mCRTITICAL SUCCESS !! \e[21;92mYou rolled a %s !!\e[0m", dice, buf_result);
								
								free(buf_dices);
								free(buf_faces);
								free(buf_mod);
								free(buf_result);
								
								return msg;
							}
							else
							{
								char *buf_result = (char *) malloc(12 * sizeof(char));
								memset (buf_result, '\0', 12);
								sprintf(buf_result, "%d", result);
								
								size_t s_msg = strlen(dice) + strlen(": CRITICAL SUCCESS !! You rolled a ") + strlen(buf_result) + strlen(" !!") + 1;
								char *msg = (char *)calloc(s_msg, sizeof(char));
								memset(msg, '\0', s_msg);
								sprintf(msg, "%s: CRITICAL SUCCESS !! You rolled a %s !!", dice, buf_result);
								
								free(buf_dices);
								free(buf_faces);
								free(buf_mod);
								free(buf_result);
								
								return msg;
							}
						}
						else
						{
							if (color)
							{
								char *buf_result = (char *) malloc(12 * sizeof(char));
								memset (buf_result, '\0', 12);
								sprintf(buf_result, "%d", result);
								
								size_t s_msg = strlen(dice) + strlen(": \e[1;92mCRTITICAL SUCCESS !! \e[21;92mYou rolled a natural ") + strlen(buf_result) + strlen(" !!\e[0m") + 1;
								char *msg = (char *)calloc(s_msg, sizeof(char));
								memset(msg, '\0', s_msg);
								sprintf(msg, "%s: \e[1;92mCRTITICAL SUCCESS !! \e[21;92mYou rolled a natural %s !!\e[0m", dice, buf_result);
								
								free(buf_dices);
								free(buf_faces);
								free(buf_mod);
								free(buf_result);
								
								return msg;
							}
							else
							{
								char *buf_result = (char *) malloc(12 * sizeof(char));
								memset (buf_result, '\0', 12);
								sprintf(buf_result, "%d", result);
							
								size_t s_msg = strlen(dice) + strlen(": CRITICAL SUCCESS !! You rolled a natural ") + strlen(buf_result) + strlen(" !!") + 1;
								char *msg = (char *)calloc(s_msg, sizeof(char));
								memset(msg, '\0', s_msg);
								sprintf(msg, "%s: CRITICAL SUCCESS !! You rolled a natural %s !!", dice, buf_result);

								free(buf_dices);
								free(buf_faces);
								free(buf_mod);
								free(buf_result);
								
								return msg;
							}
						}
					#endif
				}
				else
				{
					size_t s_msg = strlen(buf_dices) + strlen("d") + strlen(buf_faces) + strlen(": You rolled a ");
					
					char *buf_result = (char *) malloc(12 * sizeof(char));
					memset (buf_result, '\0', 12);				
					
					if ((signchar != 0) && (modifier != 0))
					{
						if (dice[signchar] == '+')
						{
							sprintf(buf_result, "%d", result + modifier);
						}
						else if (dice[signchar] == '-')
						{
							if (result - modifier < 1)
								sprintf(buf_result, "%d", 1);
							else
								sprintf(buf_result, "%d", result - modifier);
						}
					}
					else
					{
						sprintf(buf_result, "%d", result);
					}
					
					s_msg += strlen(buf_result);
					s_msg += strlen(" ! (");
					
					int j;
					for (j = 0; j < dices; j++)
					{
						char *buf_roll = (char *) malloc(12 * sizeof(char));
						memset (buf_roll, '\0', 12);
						sprintf(buf_roll, "%d", rolls[j]);
						s_msg += strlen(buf_roll);
						
						if (j != dices - 1)
							s_msg += strlen(", ");
						
						free(buf_roll);
					}
					
					if ((signchar != 0) && (modifier != 0))
					{
						s_msg += strlen(" + ") + strlen(buf_mod);
					}
					
					s_msg += strlen(")") + 1;
					
					char *msg = (char *)calloc(s_msg, sizeof(char));
					memset(msg, '\0', s_msg);
					sprintf(msg, "%sd%s: You rolled a %s ! (", buf_dices, buf_faces, buf_result);
					
					for (j = 0; j < dices; j++)
					{
						char *buf_roll = (char *) malloc(12 * sizeof(char));
						memset (buf_roll, '\0', 12);
						sprintf(buf_roll, "%d", rolls[j]);
						
						strcat(msg, buf_roll);
						
						if (j != dices -1)
							strcat(msg, ", ");
						
						free(buf_roll);
					}
					
					strcat(msg, ")");
					
					if ((signchar != 0) && (modifier != 0))
					{
						if (dice[signchar] == '+')
							strcat(msg, " + ");
						else if (dice[signchar] == '-')
							strcat(msg, " - ");
						
						strcat (msg, buf_mod);
					}
					
					free(buf_dices);
					free(buf_faces);
					free(buf_mod);
					free(buf_result);
					
					return msg;
				}
			}
		}
	}
}