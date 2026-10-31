#include "dir_utils.h"
#include <dirent.h>
#include <linux/stat.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
int ensure_dir(const char *path) {
  struct stat *sb;
  if (stat(path, sb) == 0 && S_ISDIR(sb->st_mode)) {
    return 0;
  }
  if (mkdir(path, 0755) != 0) {
    perror("mkdir failed");
    return -1;
  }
  return 0;
}
void scan_dir(const char *path, FILE *fp) {
  DIR *dir = opendir(path);
  if (!dir) {
    perror("opendir failed");
    return;
  }
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }
    char full[512];
    snprintf(full, sizeof(full), "%s/%s", path, entry->d_name);
    struct stat sb;
    if (stat(full, &sb) != 0)
      continue;
    if (S_ISDIR(sb.st_mode)) {
      fprintf(fp, "[DIR] %s\n", full);
      printf("[DIR] %s\n", full);
      scan_dir(full, fp);
    } else if (S_ISDIR(sb.st_mode)) {
      fprintf(fp, "[FILE] %s (%ld bytes)\n", full, (long)sb.st_size);
      printf("[FILE] %s (%ld bytes)\n", full, (long)sb.st_size);
    }
  }
  closedir(dir);
}
