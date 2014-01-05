#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "bigint.h"

// TODO make swap to guarantee (max, min) inside _abs_add, there is no need to make it multiple times before add() call

bigint* str2bigint(const char* str) {

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
    
    bigint* temp = NULL;
    if (strlen(str) == 0) { // 0 as a number
        temp = _init_bigint(0);
        temp->sign = 0;
    } else { // some other number except 0
        temp = _init_bigint(strlen(str));
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

    if (number->sign == 0)
        return "0";
    
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

    unsigned int i;
    for (i = number->length; i > 0; i--) {
        *p++ = '0' + number->data[i];
    }
    *p = '\0';

    return str;
}

bigint* add(const bigint* x, const bigint* y) {
    if (x == NULL || y == NULL)
        return NULL;
    
    if (x->sign == 0)
        return clone(y);
    else if (y->sign == 0)
        return clone(x);
    
    bigint* result = NULL;
    
    if (x->sign * y->sign == 1) {
        result = _abs_add(x, y);
        if (result != NULL)
            result->sign = x->sign;
    } else if (x->sign * y->sign == -1) {
        result = _abs_subtract(x, y);
        if (result != NULL)
            result->sign = (abs_compare(x, y) >= 0) ? x->sign : y->sign;
    }

    return result;
}

bigint* subtract(const bigint* x, const bigint* y) {
    
    if (x == NULL || y == NULL)
        return NULL;
    
    bigint* result = NULL;
    
    if (x->sign == 0) {
        result = clone(y);
        if (result != NULL)
            result->sign = y->sign * -1;
        return result;
    } else if (y->sign == 0) {
        return clone(x);
    }
    
    if (abs_compare(x, y) == 0 && x->sign == y->sign) {
        result = _init_bigint(0);
        if (result != NULL)
            result->sign = 0;
        return result;
    }
    
    if (x->sign * y->sign == -1) {
        result = _abs_add(x, y);
        if (result != NULL)
            result->sign = x->sign;
        return result;
    } else if (x->sign * y->sign == 1) {
        result = _abs_subtract(x, y);
        if (result != NULL)
            result->sign = (abs_compare(x, y) > 0) ? x->sign : x->sign * -1;
    }
    
    return result;
}

bigint* clone(const bigint* number) {
    if (number == NULL)
        return NULL;
    
    bigint* copy = _init_bigint(number->length);
    if (copy == NULL)
        return NULL;
    
    copy->sign = number->sign;
    if (number->data != NULL && copy->data != NULL) {
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

// returns |x| + |y|
bigint* _abs_add(const bigint* x, const bigint* y) {
    if (abs_compare(x, y) < 0) {
        const bigint * temp = x;
        x = y;
        y = temp;
    }
    
    bigint* result = _init_bigint(x->length);
    if (result == NULL)
        return result;
    
    unsigned char carry = 0;
    for (unsigned int i = 1; i < (x->length + 1); i++) {
        unsigned char t = carry + x->data[i];
        if (y->length >= i)
            t += y->data[i];
        result->data[i] = t % 10;
        carry = t / 10;
    }
    
    if (carry > 0) {
        unsigned char* old_data = result->data;
        result->length++;
        result->data = malloc(sizeof(char) * (result->length + 1));
        if (result->data == NULL) {
            fputs("ERROR: cannot allocate enough memory\n", stderr);
            return NULL;
        }
        memcpy(result->data, old_data, sizeof(char) * (result->length));
        *(result->data + result->length) = carry; // last element, i.e. of the highest order
        free(old_data);
    }
    
    return result;
}

// returns ||x| - |y||
bigint* _abs_subtract(const bigint* x, const bigint* y) {
    // process equal ?
    if (abs_compare(x, y) < 0) {
        const bigint * temp = x;
        x = y;
        y = temp;
    }
    
    bigint* result = _init_bigint(x->length);
    if (result == NULL)
        return result;
    
    unsigned char carry = 0;
    for (unsigned int i = 1; i < (x->length + 1); i++) {
        if (y->length >= i) {
            if (x->data[i] - carry >= y->data[i]) {
                result->data[i] = x->data[i] - carry - y->data[i];
                carry = 0;
            } else {
                result->data[i] = 10 + x->data[i] - carry - y->data[i];
                carry = 1;
            }
        } else {
            if (x->data[i] - carry >= 0) {
                result->data[i] = x->data[i] - carry;
                carry = 0;
            } else {
                result->data[i] = 10 + x->data[i] - carry;
                carry = 1;
            }
        }
    }
    
    unsigned char * p = result->data + result->length;
    while (*p == 0)
        p--; //skipping leading zeroes
    if (p - result->data < result->length) {
        unsigned char* old_data = result->data;
        result->length = p - result->data;
        result->data = malloc(sizeof(char) * (result->length + 1));
        if (result->data == NULL) {
            fputs("ERROR: cannot allocate enough memory\n", stderr);
            return NULL;
        }
        memcpy(result->data, old_data, sizeof(char) * (result->length + 1));
        free(old_data);
    }
    
    return result;    
}

// returns |x| * |y|
bigint* _abs_multiply(const bigint* x, const bigint* y) {
    if (abs_compare(x, y) < 0) {
        const bigint * temp = x;
        x = y;
        y = temp;
    }
    
    bigint* result = _init_bigint(x->length + y->length);
    if (result == NULL)
        return NULL;
    
    bigint* tbuffer = _init_bigint(x->length + 1);
    if (tbuffer == NULL)
        return NULL;
    tbuffer->sign = 1;
    unsigned char carry;
    unsigned int k;
    unsigned int rshift = 1;
    // TODO skip multiplying by zero, but agjust rshift!
    for (unsigned int i = 1; i < (y->length + 1); i++) {
        carry = 0;
        k = 1;
        for (unsigned int j = 1; j < (x->length + 1); j++) {
            unsigned char t = x->data[j] * y->data[i] + carry;
            tbuffer->data[k++] = t % 10;
            carry = t / 10;
        }
        if (carry > 0) {
            tbuffer->data[k] = carry;
            carry = 0;
        }
printf("%s\n", bigint2str(tbuffer));
        // add buffer to result, keep in mind shift
        carry = 0;
        for (unsigned int i = 1; i < (tbuffer->length + 1); i++) {
            unsigned char t = carry + tbuffer->data[i] + result->data[i+rshift];
            result->data[i+rshift] = t % 10;
            carry = t / 10;
        }
        if (carry > 0) {
            result->data[i+rshift] = carry;
            carry = 0;
        }
        rshift++;
        memset(tbuffer->data, 0, sizeof (char) * (tbuffer->length + 1));
    }
    return result;
}

bigint* _init_bigint(unsigned int length) {
    bigint* number = malloc(sizeof(bigint));
    if (number == NULL) {
        fputs("ERROR: cannot allocate enough memory\n", stderr);
        return NULL;
    }
    number->length = length;
    if (length > 0) {
        number->data = malloc(sizeof(char) * (number->length + 1));
        if (number->data == NULL) {
            fputs("ERROR: cannot allocate enough memory\n", stderr);
            return NULL;
        }
        memset(number->data, 0, sizeof(char) * (number->length + 1));
    } else {
        number->data = NULL; // no need to allocate memory, 0 is stored as a sign
    }
    return number;
}
