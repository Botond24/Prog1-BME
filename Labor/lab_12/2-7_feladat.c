#include <stdio.h>
#include <stdlib.h>
 
typedef struct BiFa {
    int ertek;
    struct BiFa *bal, *jobb;
} BiFa;
 
BiFa *beszur(BiFa *gyoker, int ertek) {
    if (gyoker == NULL) {
        BiFa *uj = (BiFa*) malloc(sizeof(BiFa));
        uj->ertek = ertek;
        uj->bal = uj->jobb = NULL;
        return uj;
    }
    if (ertek < gyoker->ertek) {        /* balra szur */
        gyoker->bal = beszur(gyoker->bal, ertek);
    }
    else if (ertek > gyoker->ertek) {   /* jobbra szur */
        gyoker->jobb = beszur(gyoker->jobb, ertek);
    }
    else {
        /* mar benne van */
    }
    return gyoker;
}

void print(BiFa *gyoker) {
    if (gyoker == NULL)
        return;
    print(gyoker->bal);
    printf("%d ", gyoker->ertek);
    print(gyoker->jobb);
    return;
}

void delete(BiFa *gyoker) {
    if (gyoker == NULL)
        return;
    delete(gyoker->bal);
    delete(gyoker->jobb);
    free(gyoker);
}

int count(BiFa *gyoker) {
    if (gyoker == NULL)
        return 0;
    return 1 + count(gyoker->bal) + count(gyoker->jobb);
}

int sum(BiFa *gyoker) {
    if (gyoker == NULL)
        return 0;
    return gyoker->ertek + sum(gyoker->bal) + sum(gyoker->jobb);
}

BiFa *find(BiFa *gyoker, int ertek) {
    if (gyoker == NULL)
        return NULL;
    if (gyoker->ertek == ertek)
        return gyoker;
    if (ertek < gyoker->ertek)
        return find(gyoker->bal, ertek);
    else
        return find(gyoker->jobb, ertek);
}

void negate(BiFa *gyoker) {
    if (gyoker == NULL)
        return;
    negate(gyoker->bal);
    negate(gyoker->jobb);
    gyoker->ertek *= -1;
}

BiFa *find_n(BiFa *gyoker, int ertek) {
    if (gyoker == NULL)
        return NULL;
    if (gyoker->ertek == ertek)
        return gyoker;
    if (ertek > gyoker->ertek)
        return find_n(gyoker->bal, ertek);
    else
        return find_n(gyoker->jobb, ertek);
}

void mirror(BiFa *gyoker) {
    if (gyoker == NULL)
        return;
    mirror(gyoker->bal);
    mirror(gyoker->jobb);
    BiFa *uj = gyoker->bal;
    gyoker->bal = gyoker->jobb;
    gyoker->jobb = uj;
}
 
int main(void) {
    int minta[] = {15, 96, 34, 12, 14, 56, 21, 11, 10, 9, 78, 43, 0};
    BiFa *gyoker = NULL;
    for (int i = 0; minta[i] > 0; i++)
        gyoker = beszur(gyoker, minta[i]);
    
    int sum_m = 0;
    int count_m;
    for (count_m = 0; minta[count_m] > 0; count_m++)
    {
        sum_m += minta[count_m];
    }
    /* Ide tedd be a kipróbálandó függvények hívásait! */
    print(gyoker);
    printf("\ntree count: %d\n", count(gyoker));
    printf("list count: %d\n", count_m);
    printf("tree sum: %d\n", sum(gyoker));
    printf("list sum: %d\n", sum_m);
    negate(gyoker);
    print(gyoker);
    printf("\nnegated find: %d\n", find_n(gyoker, -56)->ertek);
    mirror(gyoker);
    print(gyoker);
    printf("\nmirrored find: %d\n", find(gyoker, -56)->ertek);
    delete(gyoker);
    return 0;
}