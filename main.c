#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "lib/rtd.h"

int main(int argc, char* argv[])
{
	int color = 0;
	
	if (argc >= 2)
	{
		int i;
		//Check for color arg first
		for (i = 1; i < argc; i++)
		{
			if ( (strcmp(argv[i], "--color") == 0) || (strcmp(argv[i], "-c") == 0) )
				color = 1;
			else if ( (strcmp(argv[i], "--nocolor") == 0) || (strcmp(argv[i], "-n") == 0) )
				color = 0;
		}
		
		// Seed RNG
		srand(time(NULL));
		
		//Actually look for a dice string
		for (i = 1; i < argc; i++)
		{
			if ( (strcmp(argv[i], "--color") == 0) || (strcmp(argv[i], "-c") == 0) )
				continue;
			else if (strcmp(argv[i], "--nocolor") == 0)
				continue;
			else
			{			
				char *msg = roll(argv[i], color);
				
				if ((msg[0] != 0) && (msg[1] != '\0'))
				{
					if (i != argc - 1)
						printf("%s\n", msg);
					else
						printf("%s", msg);
				}
				else
					perror("Library call error");
			}
		}
	}
	else
		printf("%s", help());
	
	#if (defined (LINUX) || defined (__linux__))
		printf("\n");
	#endif
	
	return 0;
}