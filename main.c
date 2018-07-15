#include "dice.h"

int randint(int minvalue, int maxvalue)
{
	return rand() % (maxvalue+1 - minvalue) + minvalue;
}

int main(int argc, char* argv[])
{
	//Seed the RNG
    char username[30];
    strcpy(username, getenv("USERNAME"));
    char *ptr;
    unsigned int hash = strtoul(username, &ptr, 36);
    unsigned int seed = time(NULL);

    if ((seed % 3 == 0) || (seed % 7 == 0))
        seed += (time(NULL) * hash) * 10/100;
    else
    {
        seed -= (time(NULL) * hash) * ((seed%3)*10)/100;
        if ((seed % 3 == 0) || (seed % 7 == 0))
            seed += (time(NULL) * hash) * 10/100;
    }

    srand(seed);

    if ((argc < 2) || (argv[1][0] == '?'))
    {
        printf("Usage: roll <x>d<y>\n");
        printf("    <x>: Number of dice to roll\n");
        printf("    <y>: Number of faces on each dice\n\n");
        printf("You can roll up to 20 dice per throw. (e.g.: 20d20, or 10d10 + 5d6 + 5d4)\n");
    }
    else
    {
		char buffer_faces[10], buffer_dices[10];
		int individual_dice[21];
		int faces, dices;
		int i, j, k, l, m, result;

        for (i = 1; i < argc; i++)
		{
			result = 0;
			individual_dice[0] = '\0';
			buffer_faces[0] = '\0';
			buffer_dices[0] = '\0';
			faces = 0;
			dices = 0;

			if (argv[i][0] == '+')
            {
                continue;
            }

			for (j = 0; j < strlen(argv[i]); j++)
			{
				if (argv[i][j] != 'd')
                {
                    buffer_dices[j] = argv[i][j];
                }
				else
				{
					buffer_dices[j] = '\0';

					for (k = j+1; k < strlen(argv[i]); k++)
					{
						buffer_faces[k-j-1] = argv[i][k];
					}
					buffer_faces[k-j-1] = '\0';

					break;
				}
			}

			sscanf(buffer_faces, "%d", &faces);
			sscanf(buffer_dices, "%d", &dices);

			if ((dices == 0) || (faces < 4))
            {
                continue;
            }

			for (l = 0; l < dices; l++)
			{
			    int buffer = randint(1, faces);
				result += buffer;
				individual_dice[l] = buffer;
			}
			individual_dice[l] = 0;

			if ((faces == 6) && (dices == 1))
            {
                d6(result);

                if (result == dices)
                    printf("CRITICAL FAILURE ! You rolled a %d...", result);
                else if (result == dices * faces)
                    printf("CRTITICAL SUCCESS !! You rolled a %d !", result);
                else
                    printf("You rolled a %d !", result);
            }
			else if ((faces == 20) && (dices == 1))
            {
                d20(result);

                if (result == dices)
                    printf("CRITICAL FAILURE ! You rolled a %d...", result);
                else if (result == dices * faces)
                    printf("CRTITICAL SUCCESS !! You rolled a %d !", result);
                else
                    printf("You rolled a %d !", result);
            }
            else
            {
                printf("%s: ", argv[i]);

                if (result == dices)
                    printf("CRITICAL FAILURE ! You rolled a %d...", result);
                else if (result == dices * faces)
                    printf("CRTITICAL SUCCESS !! You rolled a %d !", result);
                else
                    printf("You rolled a %d !", result);

                if (dices >= 2)
                {
                    printf(" (");

                    for (m = 0; m < dices; m++)
                    {
                        if (m != dices-1)
                            printf("%d, ", individual_dice[m]);
                        else
                            printf("%d)", individual_dice[m]);
                    }
                }
            }

            printf("\n");
		}
    }
    return 0;
}
