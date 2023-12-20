#include <stdio.h>
#include <stdlib.h>
// #include "../debugmalloc.h"
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Hasznalat: %s <bemenet> <kimenet>\n", argv[0]);
        return -1;
    }
    typedef enum LyAllapot
    {
        alap,
        l_volt,
        L_volt,
        ll_volt,
        Ll_volt,
    } LyAllapot;
    LyAllapot all;
    int i = 1, c;
    char *szoveg = (char *)malloc(1);
    szoveg[0] = EOF;
    all = alap;
    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
    {
        fprintf(stderr, "Sikertelen fajlnyitas.\n");
        return -1;
    }
    while ((c = getc(f)) != EOF)
    {
        szoveg = (char *)realloc(szoveg, i + 1);
        szoveg[i - 1] = c;
        szoveg[i] = '\0';
        switch (all)
        {
        case alap:
            if (c == 'l')
                all = l_volt;
            else if (c == 'L')
                all = L_volt;
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
        case L_volt:
            switch (c)
            {
            case 'l':
                all = Ll_volt;
                break;
            case 'y':
                all = alap;
                szoveg = realloc(szoveg, --i);
                szoveg[i - 1] = 'J';
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
        case Ll_volt:
            switch (c)
            {
            case 'l':
                break;
            case 'y':
                all = alap;
                szoveg = realloc(szoveg, --i);
                szoveg[i - 2] = 'J';
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
    fclose(f);
    f = fopen(argv[2], "w");
    fprintf(f, "%s", szoveg);
    free(szoveg);
    return 0;
}