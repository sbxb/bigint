#include <stdio.h>
#include <stdlib.h>
#include "bigint.h"

int main() {
    bigint* m = str2bigint("-242424242424242424242424242424242424242424242424242424242424242424242424242424242424242");
    bigint* mm = clone(m);
    
    bigint* n = str2bigint("-959595959595959595959595959595959595959595959595959595959595959595959595959595");
    bigint* nn = clone(n);
    
    release(m);
    release(n);
    
    printf("%s\n", bigint2str(mm));
    printf("%s\n", bigint2str(nn));
    printf("%s\n", bigint2str(add(mm, nn)));
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
