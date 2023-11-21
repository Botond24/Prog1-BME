#include <stdlib.h>
#include <stdio.h>
#include "../debugmalloc.h"

typedef struct ListaElem {
    int adat;
    struct ListaElem *kov;
} ListaElem;
 
 
/* létrehoz egy listát, benne egy csomó számmal */
ListaElem *lista_letrehoz(void) {
    int szamok[] = { 8, 14, 13, 17, 1, 19, 16, 5, 3, 11, 2,
                     15, 9, 10, 6, 22, 4, 7, 18, 27, -1 };
    ListaElem *lis = NULL;
 
    for (int i = 0; szamok[i] != -1; ++i) {
        ListaElem *u;
        u = (ListaElem*) malloc(sizeof(ListaElem));
        u->kov = lis;
        u->adat = szamok[i];
        lis = u;
    }
    return lis;
}
int len(const ListaElem *start) {
    int num = 0;
    while (start != NULL) {
        num++;
        start = start->kov;
    }
    return num;
}
void free_list(ListaElem *start) {
    while (start != NULL) {
        ListaElem *curr = start;
        start = start->kov;
        free(curr);
    }
    return;
}
ListaElem *lista_elejere_beszur(ListaElem *start, int uj) {
    ListaElem *u;
    u = (ListaElem*) malloc(sizeof(ListaElem));
    u->kov = start;
    u->adat = uj;
    return u;
}
void lista_vegere_beszur(ListaElem *start, int uj) {
    ListaElem *u;
    u = (ListaElem*) malloc(sizeof(ListaElem));
    ListaElem *temp = start;
    while (temp->kov != NULL)
    {
        temp = temp->kov;
    }
    temp->kov = u;
    u->kov = NULL;
    u->adat = uj;
    return;
}

ListaElem *find(ListaElem *start, int num){
    while (start != NULL)
    {
        start = start->kov;
        if (start->adat == num)
        {
            return start;
        }
    }
    return NULL;
}
 
int main(void) {
    ListaElem *eleje;
    
    eleje = lista_letrehoz();
    eleje = lista_elejere_beszur(eleje, 21);
    lista_vegere_beszur(eleje, 20);
    ListaElem *temp = eleje;
    while (temp != NULL) {
        printf("%d ", temp->adat);
        temp = temp->kov;
    }
    printf("\n%d\n", len(eleje));
    printf("%d\n", find(eleje, 11)->adat);
    free_list(eleje);
    return 0;
}
//fordítva vannak mert a láncolt lista fordítva van létrehozva

