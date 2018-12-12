#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

int randint(int minvalue, int maxvalue)
{
    //Source: https://stackoverflow.com/questions/2509679/how-to-generate-a-random-integer-number-from-within-a-range
    
    int r;
    const unsigned int range = 1 + maxvalue - minvalue;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    do
    {
        r = rand();
    } while (r >= limit);

    return minvalue + (r / buckets);
}

int main(int argc, char* argv[])
{
    int i;
    //Seed the RNG
    char *username;
    #if (defined (_WIN32) || defined (_WIN64))
        username = calloc(strlen(getenv("USERNAME")), sizeof(char));
        username = getenv("USERNAME");
    #elif (defined (LINUX) || defined (__linux__))
        username = calloc(strlen(getenv("USER")), sizeof(char));
        username = getenv("USER");
    #endif

    int *tmp = calloc(strlen(username), sizeof(int));

    for (i = 0; i < strlen(username); i++)
    {
        tmp[i] = username[i] - '0';
    }

    unsigned int hash = 0;
    for (i = 0; i < strlen(username); i++)
    {
        hash += tmp[i];
    }

    unsigned int seed = time(NULL);

    if ((seed % 3 == 0) || (seed % 7 == 0))
        seed += (time(NULL) * hash) * 10/100;
    else
    {
        seed -= (time(NULL) * hash) * ((seed%3)*10)/100;
        if ((seed % 3 == 0) || (seed % 7 == 0))
            seed += (time(NULL) * hash) * 10/100;
    }

    free(tmp);
    username, tmp = NULL;
    srand(seed);

    if ((argc < 2) || (argv[1][0] == '?'))
    {
        printf("Usage: rtd <x>d<y>\n");
        printf("    <x>: Number of dice to roll\n");
        printf("    <y>: Number of faces on each dice\n\n");
        printf("You can roll up to 20 dice per throw. (e.g.: 20d20, or 10d10 + 5d6 + 5d4)\n");
    }
    else	//Enter main loop
    {
    	//Check if dice expression is ok
    	for (i = 1; i < argc; i++)
    	{
    		int dchar = 0, signchar = 0;
    		char *buf_dices, *buf_faces, *buf_mod;
    		
    		if (strchr(argv[i], 'd') != NULL)
    			dchar = (int)(strchr(argv[i], 'd') - argv[i]);
    		else	//Not a dice
    			continue;
    		
    		//Do we have a modifier to this roll ?
    		if (strchr(argv[i], '+') != NULL)
    			signchar = (int)(strchr(argv[i], '+') - argv[i]);
    		else if (strchr(argv[i], '-') != NULL)
    			signchar = (int)(strchr(argv[i], '-') - argv[i]);
    		else	//No modifier
    			signchar = 0;
    		

    		buf_dices = calloc(dchar + 1, sizeof(char));
    		memset(buf_dices, '\0', dchar);

    		if (signchar == 0)	//No modifier
    		{
    			buf_faces = calloc(strlen(argv[i]) - dchar + 1, sizeof(char));
    			memset(buf_faces, '\0', strlen(argv[i]) - dchar + 1);
    		}
    		else
    		{
    			buf_faces = calloc(signchar - dchar + 1, sizeof(char));
    			memset(buf_faces, '\0', signchar - dchar + 1);
    		}
    		
    		if (dchar == 0)
    			buf_dices[0] = '1';
    		else
    			strncpy(buf_dices, argv[i], dchar);
    		
    		if (signchar == 0)	//No modifier
    		{
    			strncpy(buf_faces, &argv[i][dchar+1], strlen(argv[i]) - dchar - 1);
    			//printf("%s %s\n", buf_dices, buf_faces);
    		}
    		else
    		{
    			strncpy(buf_faces, &argv[i][dchar+1], signchar - dchar - 1);
    			
    			buf_mod = calloc(strlen(argv[i]) - signchar + 1, sizeof(char));
    			memset(buf_mod, '\0', strlen(argv[i]) - signchar + 1);
    			strncpy(buf_mod, &argv[i][signchar+1], strlen(argv[i]) - signchar + 1);
    			
    			//printf("%s %s %c%s\n", buf_dices, buf_faces, argv[i][signchar], buf_mod);
    		}
    		
    		int dices, faces, modifier;
    		
    		sscanf(buf_dices, "%d", &dices);
    		sscanf(buf_faces, "%d", &faces);
    		
    		if (signchar != 0)
    			sscanf(buf_mod, "%d", &modifier);
    		
    		if ((dices == 0) || (faces == 0))
    		{
    			continue;
    		}
    		else
    		{
    			//Proceed to actually roll
    			int result = 0;
    			
    			if (dices == 1)
    			{
    				printf("%dd%d: ", dices, faces);
    				
    				result = randint(1, faces);
    				
    				if (result == 1)
    				{
    					#if defined (WIN32)
    						if (modifier == 0)
    							printf("CRITICAL FAILURE !! You rolled a 1...\n");
    						else
    							printf("CRITICAL FAILURE !! You rolled a natural 1...\n");
    					#elif defined (__linux__)
    						if (modifier == 0)
    							printf("\e[1;91mCRITICAL FAILURE ! \e[21;91mYou rolled a 1...\e[0m");
    						else
    							printf("\e[1;91mCRITICAL FAILURE ! \e[21;91mYou rolled a natural 1...\e[0m");
    					#endif
    				}
    				else if (result == faces)
    				{
    					#if defined (WIN32)
    						if (modifier == 0)
    							printf("CRITICAL SUCCESS !! You rolled a %d !!\n", result);
    						else
    							printf("CRITICAL SUCCESS !! You rolled a natural %d !!\n", result);
    					#elif defined (__linux__)
    						if (modifier == 0)
    							printf("\e[1;92mCRTITICAL SUCCESS !! \e[21;92mYou rolled a %d !\e[0m", result);
    						else
    							printf("\e[1;92mCRTITICAL SUCCESS !! \e[21;92mYou rolled a natural %d !\e[0m", result);
    					#endif
    				}
    				else
    				{
    					if ((signchar != 0) && (modifier != 0))
    					{
    						if (argv[i][signchar] == '+')
    						{
    							printf("You rolled a %d ! (%d + %d)", result + modifier, result, modifier);
    						}
    						else if (argv[i][signchar] == '-')
    						{
    							if (result - modifier < 1)
    							{
    								printf("You rolled a 1 ! (%d + %d)", result, modifier);
    							}
    							else
    							{
    								printf("You rolled a %d ! (%d - %d)", result - modifier, result, modifier);
    							}
    						}
    					}
    					else
    					{
    						printf("You rolled a %d !", result);
    					}
    				}
    				printf("\n");
    			}
    			else
    			{
    				printf("%dd%d: ", dices, faces);
    				int *rolls = calloc(faces, sizeof(int));
    				
    				int j;
    				for (j = 0; j < dices; j++)
    				{
    					rolls[j] = randint(1, faces);
    				}
    				
    				for (j = 0; j < dices; j++)
    					result += rolls[j];
    				
    				if (result == dices)
    				{
    					#if defined (WIN32)
    						if (modifier == 0)
    							printf("CRITICAL FAILURE !! You rolled a 1...\n");
    						else
    							printf("CRITICAL FAILURE !! You rolled a natural 1...\n");
    					#elif defined (__linux__)
    						if (modifier == 0)
    							printf("\e[1;91mCRITICAL FAILURE ! \e[21;91mYou rolled a 1...\e[0m");
    						else
    							printf("\e[1;91mCRITICAL FAILURE ! \e[21;91mYou rolled a natural 1...\e[0m");
    					#endif
    				}
    				else if (result == dices * faces)
    				{
    					#if defined (WIN32)
    						if (modifier == 0)
    							printf("CRITICAL SUCCESS !! You rolled a %d !!\n", result);
    						else
    							printf("CRITICAL SUCCESS !! You rolled a natural %d !!\n", result);
    					#elif defined (__linux__)
    						if (modifier == 0)
    							printf("\e[1;92mCRTITICAL SUCCESS !! \e[21;92mYou rolled a %d !\e[0m", result);
    						else
    							printf("\e[1;92mCRTITICAL SUCCESS !! \e[21;92mYou rolled a natural %d !\e[0m", result);
    					#endif
    				}
    				else
    				{
    					if ((signchar != 0) && (modifier != 0))
    					{
    						if (argv[i][signchar] == '+')
    						{
    							printf("You rolled a %d ! (", result + modifier);
    							
    							int k;
    							for (k = 0; k < dices; k++)
    							{
    								#if defined (WIN32)
    									if (k != dices-1)
    										printf("%d, ", rolls[k]);
    									else
    										printf("%d ", rolls[k]);
    								#elif defined (__linux__)
    									if (k != dices-1)
    									{
    										if (rolls[k] == 1)
    											printf("\e[1;91m%d\e[0m, ", rolls[k]);
    										else if (rolls[k] == faces)
    											printf("\e[1;92m%d\e[0m, ", rolls[k]);
    										else
    											printf("%d, ", rolls[k]);
    									}
    									else
    									{
    										printf("%d ", rolls[k]);
    									}
    								#endif
    							}
    							
    							printf("+ %d)", modifier);
    						}
    						else if (argv[i][signchar] == '-')
    						{
    							if (result - modifier < 1)
    							{
    								printf("You rolled a 1 ! (");
    							
    								int k;
    								for (k = 0; k < dices; k++)
    								{
    									#if defined (WIN32)
    										if (k != dices-1)
    											printf("%d, ", rolls[k]);
    										else
    											printf("%d ", rolls[k]);
    									#elif defined (__linux__)
    										if (k != dices-1)
    										{
    											if (rolls[k] == 1)
    												printf("\e[1;91m%d\e[0m, ", rolls[k]);
    											else if (rolls[k] == faces)
    												printf("\e[1;92m%d\e[0m, ", rolls[k]);
    											else
    												printf("%d, ", rolls[k]);
    										}
    										else
    										{
    											printf("%d ", rolls[k]);
    										}
    									#endif
    								}
    								
    								printf("- %d)", modifier);
    							}
    							else
    							{
    								printf("You rolled a %d ! (", result - modifier);
    							
    								int k;
    								for (k = 0; k < dices; k++)
    								{
    									#if defined (WIN32)
    										if (k != dices-1)
    											printf("%d, ", rolls[k]);
    										else
    											printf("%d ", rolls[k]);
    									#elif defined (__linux__)
    										if (k != dices-1)
    										{
    											if (rolls[k] == 1)
    												printf("\e[1;91m%d\e[0m, ", rolls[k]);
    											else if (rolls[k] == faces)
    												printf("\e[1;92m%d\e[0m, ", rolls[k]);
    											else
    												printf("%d, ", rolls[k]);
    										}
    										else
    										{
    											printf("%d ", rolls[k]);
    										}
    									#endif
    								}
    								
    								printf("- %d)", modifier);
    							}
    						}
    					}
    					else
    					{
    						printf("You rolled a %d ! (", result - modifier);
    							
    						int k;
    						for (k = 0; k < dices; k++)
    						{
    							#if defined (WIN32)
    								if (k != dices-1)
    									printf("%d, ", rolls[k]);
    								else
    									printf("%d ", rolls[k]);
    							#elif defined (__linux__)
    								if (k != dices-1)
    								{
    									if (rolls[k] == 1)
    										printf("\e[1;91m%d\e[0m, ", rolls[k]);
    									else if (rolls[k] == faces)
    										printf("\e[1;92m%d\e[0m, ", rolls[k]);
    									else
    										printf("%d, ", rolls[k]);
    								}
    								else
    								{
    									printf("%d)", rolls[k]);
    								}
    							#endif
    						}
    					}
    				}
    				
    				printf("\n");
    			}
    		}
    	}
    }
    
    return 0;
}
