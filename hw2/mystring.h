#ifndef _CSTRING_H
#define _CSTRING_H
typedef unsigned long size_t;

void *my_memccpy(void *, const void *, int, size_t);
void *my_memchr(const void *, int, size_t);
int my_memcmp(const void *, const void *, size_t);
void *my_memcpy(void *, const void *, size_t);
void *my_memmove(void *, const void *, size_t);
void *my_memset(void *, int, size_t);
char *my_stpcpy(char *, const char *);
char *my_stpncpy(char *, const char *, size_t);
char *my_strcat(char *, const char *);
char *my_strchr(const char *, int);
int my_strcmp(const char *, const char *);
char *my_strcpy(char *, const char *);
size_t my_strcspn(const char *, const char *);
char *my_strdup(const char *);
size_t my_strlen(const char *);
char *my_strncat(char *, const char *, size_t);
int my_strncmp(const char *, const char *, size_t);
char *my_strncpy(char *, const char *, size_t);
char *my_strndup(const char *, size_t);
size_t my_strnlen(const char *, size_t);
char *my_strpbrk(const char *, const char *);
char *my_strrchr(const char *, int);
size_t my_strspn(const char *, const char *);
char *my_strstr(const char *, const char *);
char *my_strtok(char *, const char *);
char *strtok_r(char *, const char *, char **);
char *my_strtoupr(char*, const char*);
char *my_strtolwr(char*, const char*);
#endif
