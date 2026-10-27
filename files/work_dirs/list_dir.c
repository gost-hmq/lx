#include "list_dir.h"
#include <dirent.h>
#include <stdio.h>
void list_dir(const char *path, const char *comment, const char *target) {
  DIR *dir = opendir(path);
  if (!dir) // 创建目录失败
  {
    perror("opendir");
    return;
  }
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 ||
        strcmp(entry->d_name, "..") == 0) // 过滤掉.和..
    {
      continue;
    }
    if (comment) {
      if (target == 0) // 批量注释
      {
        printf("注释:%s->文件:%s\n", comment, entry->d_name);
      } else {
        printf("注释:%s\n", comment);
      }
    } else {
      printf("%s\n", entry->d_name);
    }
  }
  closedir(dir);
}
