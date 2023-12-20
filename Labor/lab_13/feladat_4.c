#include <stdio.h>
#include <stdlib.h>
// #include "../debugmalloc.h"
int main(void)
{
	typedef enum LyAllapot
	{
		alap,
		l_volt,
		ll_volt
	} LyAllapot;
	LyAllapot all;
	int i = 1, c;
	char *szoveg = (char *)malloc(1);
	szoveg[0] = EOF;
	all = alap;
	while ((c = getchar()) != EOF)
	{
		szoveg = (char *)realloc(szoveg, i + 1);
		szoveg[i - 1] = c;
		szoveg[i] = '\0';
		switch (all)
		{
		case alap:
			if (c == 'l')
				all = l_volt;
			break;
		case l_volt:
			switch (c)
			{
			case 'l':
				all = ll_volt;
				break;
			case 'y':
				all = alap;
				szoveg = realloc(szoveg, --i);
				szoveg[i - 1] = 'j';
				szoveg[i] = '\0';
				break;
			default:
				all = alap;
				break;
			}
			break;

		case ll_volt:
			switch (c)
			{
			case 'l':
				break;
			case 'y':
				all = alap;
				szoveg = realloc(szoveg, --i);
				szoveg[i - 2] = 'j';
				szoveg[i - 1] = 'j';
				szoveg[i] = '\0';
				break;
			default:
				all = alap;
				break;
			}
			break;
		}
		i++;
		// printf("%d %s\n", i,szoveg);
	}

	printf("%s", szoveg);
	free(szoveg);
	return 0;
}