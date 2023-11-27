//
// Created by sziha on 25/10/2023.
//
#include <stdio.h>
#include <stdlib.h>

struct test{
    int a;
};
typedef struct{
    int b;
} test_t;

int add(int a, int b){
    return a + b;
}

int main(void){
    test_t *t = malloc(sizeof(test_t));
    int i = 0;
    i++;
    printf("%d", t->b);
    return 0;
}
