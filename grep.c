#include "grep.h"

int main(int argc, char **argv) {
  char buf[PAT_COUNT] = {0};
  flag flags = {0};
  if (argc > 2) {
    if (arguments(argc, argv, buf, &flags) == 0)
      open_file(argc, argv, buf, flags);
  }
  return 0;
}

int arguments(int argc, char **argv, char *buf, flag *flags) {
  int opt = 0;
  int fail = 0;

  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1) {
    switch (opt) {
    case 'e':
      flags->e++;
      grep_e(buf, *flags);
      break;
    case 'i':
      flags->i = 1;
      break;
    case 'v':
      flags->v = 1;
      break;
    case 'c':
      flags->c = 1;
      break;
    case 'l':
      flags->l = 1;
      break;
    case 'n':
      flags->n = 1;
      break;
    case 'h':
      flags->h = 1;
      break;
    case 's':
      flags->s = 1;
      break;
    case 'f':
      flags->f++;
      grep_f(buf, argv, &fail, *flags);
      break;
    case 'o':
      flags->o = 1;
      break;
    default:
      fail = 1;
      break;
    }
  }
  if (!flags->e && !flags->f)
    strcat(buf, argv[optind++]);
  return fail;
}

void open_file(int argc, char *argv[], char *buf, flag flags) {
  int count_file = argc - optind;
  FILE *fp = NULL;
  for (; optind < argc; optind++) {
    fp = fopen(argv[optind], "r");
    if (fp == NULL) {
      if (!flags.s)
        fprintf(stderr, "%s: %s: No such file or derectory", argv[0],
                argv[optind]);
      continue;
    }
    workspase(fp, buf, argv, &count_file, flags);
  }
}

void grep_e(char *buf, flag flags) {
  if (flags.e > 1)
    strcat(buf, "|");
  strcat(buf, optarg);
}

void grep_f(char *buf, char *argv[], int *fail, flag flags) {
  FILE *open_file = NULL;
  open_file = fopen(optarg, "r");
  int now_line;
  char f_str[2] = {0};
  if (open_file != NULL) {
    while ((now_line = fgetc(open_file)) != EOF) {
      if (now_line == '\n') {
        strcat(buf, "|");
        continue;
      } else {
        f_str[0] = now_line;
        strcat(buf, f_str);
      }
    }
    if (now_line == EOF) {
      strcat(buf, "|");
    }
    fclose(open_file);
  } else if (flags.f < 2) {
    fprintf(stderr, "%s: %s: No such file or derectory", argv[0], argv[optind]);
    *fail = 1;
  }
}

void workspase(FILE *fp, char *buf, char *argv[], int *count_file, flag flags) {
  int new_line = 0;
  int line_n = 1;
  int line_c = 0;
  int counter = 0;
  regex_t regex;
  char str[STR_COUNT] = {0};
  int reg_i = 0;
  if (flags.f)
    buf[strlen(buf) - 1] = '\0';
  while (fgets(str, STR_COUNT, fp) != NULL) {
    if (flags.i) {
      reg_i = REG_ICASE;
    }
    regcomp(&regex, buf, REG_EXTENDED | reg_i);
    work_flag(&regex, str, &line_n, &line_c, &counter, count_file, argv, fp,
              &new_line, flags);
    line_n++;
  }
  if (flags.c) {
    if (*count_file > 1)
      printf("%s:", argv[optind]);
    if (flags.l && line_c > 0)
      printf("1\n");
    else
      printf("%d\n", line_c);
  }

  if (flags.l && counter > 0)
    printf("%s\n", argv[optind]);
  fclose(fp);
}

void work_flag(regex_t *regex, char *str, int *line_n, int *line_c,
               int *counter, int *count_file, char *argv[], FILE *fp,
               int *new_line, flag flags) {
  regmatch_t matchptr[1] = {0};
  size_t nmatch = 1;

  int regular = regexec(regex, str, nmatch, matchptr, 0);

  if (flags.v) {
    if (regular == REG_NOMATCH)
      regular = 0;
    else
      regular = REG_NOMATCH;
  }

  if (regular == 0) {
    if (flags.o && !flags.c)
      grep_o(matchptr, str, argv, line_n, count_file, regex, new_line, nmatch);
    if (!flags.l && !flags.o && !flags.c) {
      if ((*count_file > 1) && !flags.h) {
        printf("%s:", argv[optind]);
      }
      if (flags.n) {
        printf("%d:", *line_n);
      }

      if (feof(fp)) {
        printf("%s\n", str);
      } else
        printf("%s", str);
    }
    (*line_c)++;
    (*counter)++;
  }
  regfree(regex);
}

void grep_o(regmatch_t matchptr[1], char *str, char *argv[], int *line_n,
            int *count_file, regex_t *regex, int *new_line, size_t nmatch) {
  char *s = str;
  while (*s != '\0') {
    if (regexec(regex, s, nmatch, matchptr, 0) == 0) {
      if ((*count_file > 1) && (*new_line != *line_n)) {
        printf("%s:", argv[optind]);
      }

      for (int i = matchptr[0].rm_so; i < matchptr[0].rm_eo; i++) {
        printf("%c", s[i]);
      }
      printf("\n");

      s += matchptr[0].rm_eo;
      *new_line = *line_n;
    } else
      s++;
  }
}