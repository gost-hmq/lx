#include "list_dir.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
int main(int argc, char **argv) {
  const char *path = "."; // 默认当前目录
  const char *comment = NULL;
  const char *target = NULL;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-m") == 0) {
      if (i + 2 < argc) {
        target = argv[i + 1];
        comment = argv[i + 2];
        i += 2;
      }
    } else if (strcmp(argv[i], "-a") == 0) {
      if (i + 1 < argc) {
        comment = argv[i + 1];
        i++;
      }
    } else if (argv[i][0] != '-') {
      path = argv[i];
    }
  }
  list_dir(path, comment, target);
  return 0;
}
