#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debugmalloc.h"

/*FILE *fptr;

                Konyvek a;
                a.kkov = NULL;
                a.kelozo = NULL;
                fptr = fopen ("konyvek.txt", "wb");
                if ( fptr == NULL)
                {
                    printf("Sikertelen fajlnyitas.\n");
                    exit(-1);
                }

                do
                {
                    printf("\nLeltari szam: "); a.cim = sor_olvasas();
                    printf("Szerzo: ");         scanf("%s", a.szerzo);
                    printf("Cim: ");            scanf("%s", a.cim);
                    printf("Kiadas eve: ");     scanf("%d", &a.ev);
                    printf("Tema: ");           scanf("%s", a.tema);
                    printf("Kolcson van-e adva?(i/n)   "); scanf("%s", a.kolcson);

                    if (a.lszam!=-1)
                        fwrite(&a, sizeof(Konyvek), 1, fptr);

                }while (a.lszam!=-1);*/

typedef struct Konyvek{
        int lszam;
        char *szerzo;
        char *cim;
        int ev;
        char *tema;
        char *kolcson;
        struct Konyvek *k_kov;
        struct Konyvek *k_el;
    } Konyvek;

typedef struct Olvasok{
    int azon;
    char *nev;
    int szev;
    char *hely;
    char *lakcim;
    struct Olvasok *o_kov;
    struct Olvasok *o_el;
} Olvasok;

/*char *sor_olvasas() {
    char *uj_sor = (char*) malloc(sizeof(char));
    uj_sor[0] = '\0';
    char adat;
    int db = 0;
    while (scanf("%c", &adat) == 1 && adat != '\n') {
        char *tomb = (char*) malloc(sizeof(char) * (db+2));
        for (int i = 0; i < db; ++i)
            tomb[i] = uj_sor[i];
        free(uj_sor);
        uj_sor = tomb;
        tomb[db] = adat;
        tomb[db++] = '\0';
        db++;
    }

    return uj_sor;
}*/


/*Konyvek *FileOlvasas(Konyvek **utolso){
    File *fptr;
    Konyvek *uj, *elso=NULL, *utolso=NULL, p;
    fptr= fopen ("proba.txt", "rb");
    if (fptr == NULL)
    {
        printf("Sikertelen fajnyitas.");
        exit(-1);
    }
    printf("Leltari szam     Szerzo           Cim                   Kiadas eve       Tema          Kolcsonadva \n");
    while ( fread(&p, sizeof(Konyvek), 1, fptr)==1)
    {
        uj (char *)malloc((strlen(becim)+1)*sizeof(char));
        uj->lszam = p.lszam;
        strcpy(uj->szerzo, uj.szerzo);
        strcpy(uj->cim, uj.cim);
        uj->ev = p.ev;
        strcpy(uj->tema, uj.tema);
        strcpy(uj->kolcson, uj.kolcson);

        printf("%10d%20s%20s%4d%20s%2s", uj->lszam, uj->szerzo, uj->cim, uj->ev, uj->tema, uj->kolcson);
        uj -> kkov = NULL;
        uj -> kelozo = NULL;
        if (e == NULL)
            e=uj;
        else
        {
            u->kkov = uj;
            uj->kelozo = u;
        }
        u =uj;
    }
    fclose(fptr);
    *ut=u;
    return e;
}*/

void Konyvek_Filebairasa(){
    Konyvek uj;
    FILE *fptr;
    fptr=fopen("konyvek.txt", "w");
    if(fptr == NULL)
    {
        printf("Sikertelen fajlnyitas.");
        exit(-1);
    }
    uj.szerzo = (char*)malloc(131);
    uj.cim = (char *)malloc(131);
    uj.kolcson = (char *)malloc(2);
    uj.tema = (char *)malloc(51);
    do {
        printf("\nLeltari szam: ");
        scanf("%d", &uj.lszam);
        printf("\n%d\n", uj.lszam);
        getchar();
        if (uj.lszam == -1) {
            // Felszabadítom a dinamikusan foglalt memóriát ű
            free(uj.szerzo);
            free(uj.cim);
            free(uj.tema);
            free(uj.kolcson);
            break;
        } 
        printf("Szerzo: ");
        fgets(uj.szerzo, 130, stdin);
        uj.szerzo[strcspn(uj.szerzo, "\n")] = '\0';
        //printf("\n%s\n", uj.szerzo);

        printf("Cim: ");
        fgets(uj.cim, 130, stdin);
        uj.cim[strcspn(uj.cim, "\n")] = '\0';
        //printf("\n%s\n", uj.cim);

        printf("Ev: ");
        scanf("%d", &uj.ev);
        getchar();
        //printf("\n%d\n", uj.ev);

        printf("Tema: ");
        fgets(uj.tema, 50, stdin);
        uj.tema[strcspn(uj.tema, "\n")] = '\0';
        //printf("\n%s\n", uj.tema);

        printf("Kolcson van-e adva? (i/n): ");
        fgets(uj.kolcson, 2, stdin);
        uj.kolcson[strcspn(uj.kolcson, "\n")] = '\0';
        //printf("\n%s\n", uj.kolcson);

        fprintf(fptr, "%d,%s,%s,%d,%s,%s;\n", uj.lszam, uj.szerzo, uj.cim, uj.ev, uj.tema, uj.kolcson);

    } while (uj.lszam != -1);

    fclose(fptr);
}

void Olvasok_Filebairasa(){
    Olvasok uj;
    FILE *fptr;
    fptr=fopen("olvasok.txt", "w");
    if(fptr == NULL)
    {
        printf("Sikertelen fajlnyitas.");
        exit(-1);
    }
    do {
        printf("\nAzonosito: ");
        scanf("%d", &uj.azon);
        //printf("\n%d\n", uj.azon);
        getchar();
        if (uj.azon != -1) {
            printf("Nev: ");
            uj.nev = (char*)malloc(130*sizeof(char)+1);
            fgets(uj.nev, 130, stdin);
            uj.nev[strcspn(uj.nev, "\n")] = '\0';
            //printf("\n%s\n", uj.nev);

            printf("Szuletesi ev: ");
            scanf("%d", &uj.szev);
            getchar();
            //printf("\n%d\n", uj.szev);

            printf("Szuletesi hely: ");
            uj.hely = (char *)malloc(50*sizeof(char));
            fgets(uj.hely, 50, stdin);
            uj.hely[strcspn(uj.hely, "\n")] = '\0';
            //printf("\n%s\n", uj.hely);

            printf("Lakcim (0000 Hely, Valami utca 00.): ");
            uj.lakcim = (char *)malloc(130*sizeof(char));
            fgets(uj.lakcim, 130, stdin);
            uj.lakcim[strcspn(uj.lakcim, "\n")] = '\0';
            //printf("\n%s\n", uj.lakcim);

            fprintf(fptr, "%d,%s,%d,%s,%s;\n", uj.azon, uj.nev, uj.szev, uj.hely, uj.lakcim);

            // Felszabadítom a dinamikusan foglalt memóriát
            free(uj.nev);
            free(uj.hely);
            free(uj.lakcim);
        }
    } while (uj.azon != -1);

    fclose(fptr);
}

int main()
{
    int nagymenu=0;
    char kismenu1;

    do{
    printf ("\n\t\t||Menu||\n");
    printf( "+----------------------------------+\n"
            "| 1. Konyvek kezelese              |\n"
            "| 2. Konyv keresese                |\n"
            "| 3. Olvasok adatai                |\n"
            "| 4. Kolcsonzesek                  |\n"
            "| 5. Kilepes                       |\n"
            "+----------------------------------+");

        printf("\n\n\tValsztott menupont(pl. 3): ");
        scanf("%d", &nagymenu);
        getchar();

    switch (nagymenu){

    case 1:
    do{
        system("cls");
        printf("\nKonyvek kezelese\n");
        printf("a. Hozzaadas\n"
               "b. Modositas\n"
               "c. Torles\n"
               "d. Vissza a fomenube\n");

        printf("\nValasztott menupont (pl. a): ");
        scanf(" %c", &kismenu1);
        getchar();
        //printf("%xd", kismenu1);

            switch (kismenu1){
            case 'a':
                printf("\nKonyv hozzaadasa\nKerem a konyv adatait!");
                Konyvek_Filebairasa();
                break;

            case 'b':
                printf("\nKonyv modositasa\nKerem a konyv adatait!");
                break;

            case 'c':
                printf("\nKonyv torlese\nKerem a konyv leltari szamat!");
                break;

            case 'd':
                system("cls");
                printf("\nVissza a fomenube.\n\n");
                break;

            default:
                printf("\nErvenytelen valasztas.\n");
                break;

            }
        }while (kismenu1 != 'd');
        break;

        case 2:
            do{
                system("cls");
                printf("\nKonyv keresese\n");
                printf("a. Szerzo\n"
                        "b. Cim\n"
                        "c. Kiadasi ev\n"
                        "d. Tema\n"
                        "e. Kulcsszo\n"
                        "f. Vissza a fomenube\n");

                printf("\nValasztott menupont (pl. a): ");
                scanf(" %c", &kismenu1);
                getchar();

                switch (kismenu1){
                case 'a':
                    printf("\nKerem a szerzo nevet: ");
                    break;

                case 'b':
                    printf("\nKerem a konyv cimet: ");
                    break;

                case 'c':
                    printf("\nKerem a konyv kiadasi evet: ");
                    break;

                case 'd':
                    printf("\nKerem a konyv temajat: ");
                    break;

                case 'e':
                    printf("\nKerem a kulcsszot: ");
                    break;

                case 'f':
                    system("cls");
                    printf("\nVissza a fomenube.\n\n");
                    break;

                default:
                    printf("\nErvenytelen valasztas.\n");
                    break;
                }
            }while (kismenu1 != 'f');
            break;

        case 3:
            do{
                system("cls");
                printf("\nOlvasok adatai\n");
                printf("a. Hozzaadas\n"
                        "b. Adatok modositasa\n"
                        "c. Eltavolitas\n"
                        "d. Vissza a fomenube\n");

                printf("\nValasztott menupont (pl. a): ");
                scanf(" %c", &kismenu1);
                getchar();

                switch (kismenu1){
                    case 'a':
                        printf("\nHozzaadas\n");
                        Olvasok_Filebairasa();
                    break;

                    case 'b':
                        printf("\nAdatok modositasa");
                        break;

                    case 'c':
                        printf("\nEltavolitas");
                        break;

                    case 'd':
                        system("cls");
                        printf("\nVissza a fomenube.\n\n");
                        break;

                    default:
                        printf("\nErvenytelen valasztas.\n");
                        break;
                    }
            }while (kismenu1 != 'd');
            break;

        case 4:
            do{
                system("cls");
                printf("\nKolcsonzesek\n");
                printf("a. Konyv kolcsonzese\n"
                        "b. Konyv visszahozva\n"
                        "c. Kolcsonzott konyvek\n"
                        "d. Olvaso keresese\n"
                        "e. Vissza a fomenube\n");

                printf("\nValasztott menupont (pl. a): ");
                scanf(" %c", &kismenu1);
                getchar();

                switch (kismenu1){
                case 'a':
                    printf("\nKonyv kolcsonzese");
                    break;

                case 'b':
                    printf("\nKonyv visszahozva");
                    break;

                case 'c':
                    printf("\nKolcsonzott konyvek");
                    break;

                case 'd':
                    printf("\nOlvaso keresese\n\n");
                    break;

                case 'e':
                    system("cls");
                    printf("\nVissza a fomenube\n\n");
                    break;

                default:
                    printf("\nErvenytelen valasztas.\n");
                    break;
                }
            }while (kismenu1 != 'e');
            break;
        }
    }while (nagymenu != 5);


    return 0;
}
