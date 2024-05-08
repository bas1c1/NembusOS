#ifndef STR_H
#define STR_H

#include "stddef.h"

size_t strlen (char *str);
char* strcat(char* destination, const char* source);
char* strcpy(char* destination, const char* source);
int strcmp(const char *X, const char *Y);
unsigned int digit_count(int num);
void itoa(int num, char *number);
bool is_delim(char c, char *delim);
char *strtok(char *s, char *delim);
char *strchr(char *s, char c);
long atoi(const char* S);
char *hextostr(uint32_t num);

#endif