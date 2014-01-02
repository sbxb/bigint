#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "bigint.h"

bigint* str2bigint(const char* str) {
    bigint* temp = malloc(sizeof(bigint));
    
    if (temp == NULL) {
        fputs("ERROR: cannot allocate enough memory\n", stderr);
        return NULL;
    }
    
    if (str == NULL || strlen(str) == 0) {
        fputs("ERROR: cannot convert empty string\n", stderr);
        return NULL;
    }
    
    if (strlen(str) == 1 && !isdigit(str[0])) {
        fputs("ERROR: cannot convert non-digit characters\n", stderr);
        return NULL;
    }
    
    
    const char* p = str;
    bool format_error = false;
    if (str[0] == '-' || str[0] == '+')
        p++;
    
    for ( ; *p != '\0'; p++) {
        if (!isdigit(*p)) {
            format_error = true;
            break;
        }
    }
    
    if (format_error) {
        fputs("ERROR: cannot convert non-digit characters\n", stderr);
        return NULL;
    }
    
    signed short sign = 0;
    
    if (str[0] == '-') {
        sign = -1;
        str++;
    } else if (str[0] == '+') {
        str++;
    }
    
    while (*str == '0' && *str != '\0') // skipping leading zeros
        str++;
    
    if (strlen(str) == 0) { // 0 as a number
        temp->data = NULL; // no need to allocate memory, 0 is stored as a sign)
        temp->length = 1;
        temp->sign = 0;
    } else { // some other number except 0
        temp->length = strlen(str);
        temp->data = malloc(sizeof(char) * temp->length);
        if (temp->data == NULL) {
            fputs("ERROR: cannot allocate enough memory\n", stderr);
            return NULL;
        }
        temp->sign = (sign == -1) ? -1 : 1;
        unsigned char* q = temp->data;
        for( ; *str != '\0'; str++, ++q) {
            *q = (unsigned char) (*str - '0');
            //printf("DEBUG: %d\n", *temp->data);
        }

    }
    
    return temp;

}

char* bigint2str(const bigint* number) {
    if (number == NULL)
        return NULL;

    int length = number->length + 1;
    if (number->sign == -1)
        length++;
    
    char* str = malloc(sizeof(char) * length);
    if (str == NULL) {
        fputs("ERROR: cannot allocate enough memory\n", stderr);
        return NULL;
    }
    
    char* p = str;
    if (number->sign == -1) {
        *p = '-';
        p++;
    }

    if (number->sign == 0) {
        p[0] = '0';
        p[1] = '\0';
    } else {
        unsigned int i;
        for (i = 0; i < number->length; i++) {
            p[i] = '0' + number->data[i];
        }
        p[i] = '\0';
    }

    
    return str;
    
}
