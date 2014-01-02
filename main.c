#include <stdio.h>
#include <stdlib.h>
#include "bigint.h"

int main(int argc, char** argv) {

    return (EXIT_SUCCESS);
}

/*
bigint* create() {
    bigint* number = malloc(sizeof(bigint));
    if (number == NULL) {
        printf("ERROR: Cannot create number (malloc failed)");
        return NULL;
    }
    number->data = NULL;
    number->sign = 0;
    return number;
}

bigint* reset(bigint* number) {
    if (number == NULL)
        return number;
    if (number->data != NULL) {
        free(number->data);
        number->data = NULL;
    }
    number->sign = 0;
    return number;
}

bigint* fill(bigint* number, const char* str) {
    
    return number;
}
*/
