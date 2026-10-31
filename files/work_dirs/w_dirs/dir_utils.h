#ifndef DIR_UTILS_H
#define DIR_UTILS_H
#include <stdio.h>
int ensure_dir(const char *path);
void scan_dir(const char *path, FILE *fp);
#endif
