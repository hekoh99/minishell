/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yubchoi <yubchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 18:01:46 by yubchoi           #+#    #+#             */
/*   Updated: 2022/07/23 15:08:26 by yubchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
char **ft_split(char *s, char c);
size_t ft_strlcpy(char *dst, const char *src, size_t dstsize);
char *ft_strjoin(char const *s1, char const *s2);
int ft_atoi(const char *str);
long long ft_atoll(const char *str);

void print_error(char *msg, int status);
void print_error2(char *function, char *msg, int status);
void error_exit(char *msg, int status);
void free_matrix(char **matrix);
char *ft_strjoin_free_s1(char *s1, char *s2);

int ft_isdigit(int c);
int ft_isalpha(int c);
char *ft_itoa(int n);

// system call
void *ft_malloc(size_t size);
void ft_free(void *str);
int ft_open(char *file, int o_flag, int mode);
void ft_close(int fd);
void ft_dup2(int fd, int fd2);
void ft_execve(char *file, char **argv, char **envp);

// builtin utils
int is_invalid_key(char *function, char *key);
int select_longer(char *str1, char *str2);

#endif