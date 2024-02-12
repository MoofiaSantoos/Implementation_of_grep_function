#ifndef GREP_H_
#define GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAT_COUNT 2048
#define STR_COUNT 2048
#define NEW_COUNT 2048

typedef struct flag {
  int e, i, v, c, l, n, h, s, f, o;
} flag;

int arguments(int argc, char *argv[], char *buf, flag *flags);
void open_file(int argc, char *argv[], char *buf, flag flags);
void grep_e(char *buf, flag flags);
void grep_f(char *buf, char *argv[], int *fail, flag flags);
void workspase(FILE *fp, char *buf, char *argv[], int *count_file, flag flags);
void work_flag(regex_t *regex, char *str, int *line_n, int *line_c,
               int *counter, int *count_file, char *argv[], FILE *fp,
               int *new_line, flag flags);
void grep_o(regmatch_t matchptr[1], char *str, char *argv[], int *line_n,
            int *count_file, regex_t *regex, int *new_line, size_t nmatch);

#endif