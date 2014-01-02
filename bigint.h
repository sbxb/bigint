#ifndef BIGINT_H
#define	BIGINT_H

typedef struct {
    unsigned char* data;
    unsigned int length;
    signed short sign;
} bigint;

bigint* str2bigint(const char* str);
char* bigint2str(const bigint* number);

#endif	/* BIGINT_H */
