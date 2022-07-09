#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <stdlib.h>

size_t ft_strlen(const char *str);
char *ft_strdup(const char *src);
void *ft_malloc(size_t size);
char *ft_substr(char const *s, unsigned int start, size_t len);
char *ft_strchr(const char *s, int c);
int ft_strncmp(const char *s1, const char *s2, size_t n);
size_t ft_strlcpy(char *dst, const char *src, size_t dstsize);
char *ft_strjoin(char const *s1, char const *s2);

#endif