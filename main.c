#include <stdio.h>
#include <stdlib.h>
#include "bigint.h"

int main() {
    // Creating two numbers (char* to bigint* conversion)
    bigint* m = str2bigint("123456789123456789123456789123456789123456789123456789123456789");
    bigint* n = str2bigint("-97531975319753197531975319753197531975319753197531975319753197531");
    
    // Checking clone function
    bigint* mm = clone(m);
    bigint* nn = clone(n);

    // Checking release function
    release(m);
    release(n);
    
    // Printing numbers (bigint* to char* conversion)
    printf("%s\n", bigint2str(mm));
    printf("%s\n", bigint2str(nn));
    
    // Doing calculations
    printf("+ : %s\n", bigint2str(add(mm, nn)));
    printf("- : %s\n", bigint2str(subtract(mm, nn)));
    printf("* : %s\n", bigint2str(multiply(mm, nn)));
    return (EXIT_SUCCESS);
}
