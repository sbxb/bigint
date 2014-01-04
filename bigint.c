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
        temp->data = malloc(sizeof(char) * (temp->length + 1 )); // reserve a place for a left bound element
        if (temp->data == NULL) {
            fputs("ERROR: cannot allocate enough memory\n", stderr);
            return NULL;
        }
        temp->sign = (sign == -1) ? -1 : 1;
        unsigned char* q = temp->data + temp->length; // set pointer to the last element
        for( ; *str != '\0'; str++, --q) {
            *q = (unsigned char) (*str - '0');
            //printf("DEBUG: %d\n", *q);
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
        for (i = number->length; i > 0; i--) {
            *p++ = '0' + number->data[i];
        }
        *p = '\0';
    }

    return str;
}

bigint* add(const bigint* x, const bigint* y) {
    if (x->sign * y->sign == -1) {
        // TODO subtract
        // (bigger - smaller), but remember to adjust the sign
    }
    if (x == NULL || y == NULL)
        return NULL;
    
    if (x->sign == 0)
        return clone(y);
    else if (y->sign == 0)
        return clone(x);
    
    // here comes real addition
    bigint* sum = malloc(sizeof(bigint));
    if (sum == NULL) {
        fputs("ERROR: cannot allocate enough memory\n", stderr);
        return NULL;
    }
    
    sum->sign = x->sign;
    //sum->length = (x->length > y->length) ? x->length + 1 : y->length + 1; // X3
    sum->length = (x->length > y->length) ? x->length : y->length;
    sum->data = malloc(sizeof(char) * (sum->length + 1));
    
    if (sum->data == NULL) {
        fputs("ERROR: cannot allocate enough memory\n", stderr);
        return NULL;
    }
    
    const bigint * first;
    const bigint * second;
    
    if (x->length > y->length) {
        first = x;
        second = y;
    } else {
        first = y;
        second = x;
    }
    
    unsigned char carry = 0;
    
    for (unsigned int i = 1; i < (first->length + 1); i++) {
        unsigned char t = carry + first->data[i];
        if (second->length >= i)
            t += second->data[i];
        sum->data[i] = t % 10;
        carry = t / 10;
    }
    
    if (carry > 0) {
        unsigned char* old_data = sum->data;
        sum->length++;
        sum->data = malloc(sizeof(char) * (sum->length + 1));
        if (sum->data == NULL) {
            fputs("ERROR: cannot allocate enough memory\n", stderr);
            return NULL;
        }
        memcpy(sum->data, old_data, sizeof(char) * (sum->length));
        *(sum->data + sum->length) = carry; // last element, i.e. of highest-order
        free(old_data);
    }
    
    return sum;
}

bigint* subtract(const bigint* x, const bigint* y) {
    if (x->sign * y->sign == -1) {
        // TODO add
        // remember to adjust the sign
    }
    
    return NULL; // TODO remove later
}

bigint* clone(const bigint* number) {
    if (number == NULL)
        return NULL;
    
    bigint* copy = malloc(sizeof(bigint));
    if (copy == NULL)
        return NULL;
    
    copy->length = number->length;
    copy->sign = number->sign;
    if (number->data == NULL)
        copy->data = NULL;
    else {
        copy->data = malloc(sizeof(char) * (copy->length + 1)); // reserve a place for a left bound element
        if (copy->data == NULL) {
            fputs("ERROR: cannot allocate enough memory\n", stderr);
            return NULL;
        }
        memcpy(copy->data, number->data, sizeof(char) * (copy->length + 1));
    }
    
    return copy;
}

int abs_compare(const bigint* x, const bigint* y) { // Only absolute values matter
    
    // Process zero(s)
    if (x->sign == 0 && y->sign == 0)
        return 0;
    else if (x->sign == 0)
        return -1;
    else if (y->sign == 0)
        return 1;
    
    // Process nonzero numbers
    if (x->length > y->length)
        return 1;
    else if (x->length < y->length)
        return -1;
    else {
        int result = 0;
        for(unsigned int i = x->length; i > 0; i--) {
            if (x->data[i] > y->data[i]) {
                result = 1;
                break;
            } else if (x->data[i] < y->data[i]) {
                result = -1;
                break;
            }
        }
        return result;
    }
}

void release(bigint* number) {
    if (number != NULL) {
        if (number->data != NULL)
            free(number->data);
        free(number);
    }
}

