#ifndef BIGINT_H
#define	BIGINT_H

typedef struct {
    unsigned char* data;
    unsigned int length;
    signed short sign;
} bigint;

bigint* str2bigint(const char* str);
char* bigint2str(const bigint* number);
bigint* add(const bigint* x, const bigint* y);
bigint* subtract(const bigint* x, const bigint* y);
bigint* clone(const bigint* number);
int abs_compare(const bigint* x, const bigint* y);
void release(bigint* number);
bigint* _abs_add(const bigint* x, const bigint* y);
bigint* _abs_subtract(const bigint* x, const bigint* y);
bigint* _abs_multiply(const bigint* x, const bigint* y);
bigint* _init_bigint(unsigned int length);
#endif	/* BIGINT_H */
