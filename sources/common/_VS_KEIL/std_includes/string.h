#pragma once


char   *strcat(char *, const char *);
char   *strcpy(void *dest, const char *src);
char   *strlcat(char *dest, const char *src, unsigned int size);
int     strcmp(const void *, const void *);
float   atof(char *);
void    memset(void *, int value, unsigned int size);
void    memcpy(void *dest, const void *src, unsigned int);
void   *memmove(void *destptr, const void *srcptr, size_t num);
int     memcmp(const void *, const void *, unsigned int);
char   *strchr(const char *str, int c);
int     toupper(int);
int     sscanf(void *, void *, ...);
unsigned int  strlen(const char *);
