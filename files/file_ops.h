#ifndef FILE_OPS_H
#define FILE_OPS_H
#include <stdio.h>
void print_file(FILE *fp);
int write_clear(const char *name);
int write_append(const char *name);
void get_current_time(char *buf, int len);
#endif
