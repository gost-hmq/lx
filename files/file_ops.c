#include "file_ops.h"
#include <stdio.h>
#include <time.h>
void get_current_time(char *buf, int len) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  snprintf(buf, len, "%04d-%02d-%02d %02d:%02d:%02d\n", t->tm_year + 1900,
           t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
  /*
   * year年+1900
   * mon月+1
   * mday日
   * hour时
   * min分
   * sec秒
   */
}
int write_clear(const char *name) {
  FILE *fp = fopen(name, "w");
  if (!fp)
    return 0;
  char time_buf[64];
  get_current_time(time_buf, sizeof(time_buf));
  fprintf(fp, "time:%s\tname:%s\n", time_buf, name);
  fclose(fp);
  return 1;
}
int write_append(const char *name) {
  FILE *fp = fopen(name, "a");
  if (!fp)
    return 0;
  char time_buf[64];
  get_current_time(time_buf, sizeof(time_buf));
  fprintf(fp, "\ntime:%s\tname:%s\n", time_buf, name);
  fclose(fp);
  return 1;
}
void print_file(FILE *fp) {
  int ch;
  while ((ch = fgetc(fp)) != EOF) {
    putchar(ch);
  }
}
