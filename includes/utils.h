#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

size_t ft_strlen(const char *str);
char *ft_strdup(const char *src);
char *ft_substr(char const *s, unsigned int start, size_t len);
char *ft_strchr(const char *s, int c);
int ft_strncmp(char *s1, char *s2, int n);
int ft_strcmp(char *s1, char *s2);
size_t ft_strlcpy(char *dst, const char *src, size_t dstsize);
char *ft_strjoin(char const *s1, char const *s2);
int ft_atoi(const char *str);
long long ft_atoll(const char *str);
void error_exit(char *msg, int status);

int ft_isdigit(int c);
int ft_isalpha(int c);
char *ft_itoa(int n);

// system call
void *ft_malloc(size_t size);
void ft_free(void *str);

// test
char *get_next_line(int fd);
// builtin utils
void printf_invalid_identifier(char *key, int *status);
int is_invalid_key(char *key, int *exit_status);
int select_longer(char *str1, char *str2);

#endif