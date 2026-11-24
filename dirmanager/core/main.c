#include "plugin.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PLUGIN_DIR "plugins"
#define MAX_PATH 512

extern int loader_load_dir(const char *dir);
extern void loader_unload_all(void);
extern void loader_foreach_cmd(void (*cb)(const cmd_plugin_t *, int, void *),
                               void *);

/*检查插件目录，不存在则创建*/
static int ensure_plugin_dir(const char *dir) {
  struct stat st;
  if (!stat(dir, &st)) {
    if (S_ISDIR(st.st_mode)) {
      printf("插件目录已存在:%s\n", dir);
      return 0;
    }
    fprintf(stderr, "%s存在但不是目录\n", dir);
    return -1;
  }
  if (!mkdir(dir, 0755)) {
    printf("插件目录已创建:%s\n", dir);
    return 0;
  }
  perror("mkdir");
  return -1;
}

/*----目录管理主循环----*/

static char cwd[MAX_PATH] = ".";

static void list_dir(void) {
  DIR *d = opendir(cwd);
  if (!d) {
    perror("opendir");
    return;
  }
  printf("\n========%s========\n", cwd);
  struct dirent *e;
  int idx = 0;
  while ((e = readdir(d))) {
    if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, "..")) {
      continue;
    }
    char full[MAX_PATH];
    snprintf(full, sizeof(full), "%s/%s", cwd, e->d_name);
    struct stat st;
    stat(full, &st);
    printf("[%d] %s %s\n", ++idx, S_ISDIR(st.st_mode) ? "[D]" : "[F]",
           e->d_name);
  }
  closedir(d);
}

/*
 * 返回-1=退出,
 * 0=返回上级目录,
 * 1=进入目录,
 * 2=选中文件
 */
static int select_entry(char *out, size_t outlen) {
  list_dir();
  printf("选择(0=上级,q=退出):");
  char buf[32];
  if (!fgets(buf, sizeof(buf), stdin)) {
    return -1;
  }
  if (buf[0] == 'q' || buf[0] == 'Q') {
    return -1;
  }
  int n = atoi(buf);
  if (n == 0) {
    char *p = strrchr(cwd, '/'); /*67*/
    if (p && p != cwd) {
      *p = '\0';
    } else {
      strcpy(cwd, "/");
    }
    return 0;
  }
  DIR *d = opendir(cwd);
  if (!d) {
    return -1;
  }
  struct dirent *e;
  int idx = 0, ret = -1;
  while ((e = readdir(d))) {
    if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, "..")) {
      continue;
    }
    if (++idx == n) {
      char full[MAX_PATH];
      snprintf(full, sizeof(full), "%s/%s", cwd, e->d_name);
      struct stat st;
      stat(full, &st);
      if (S_ISDIR(st.st_mode)) {
        snprintf(cwd, sizeof(cwd), "%s", full);
        ret = 1;
      } else {
        snprintf(out, outlen, "%s", full);
        ret = 2;
      }
      break;
    }
  }
  closedir(d);
  if (ret == -1)
    printf("无效选择\n");
  return ret;
}

static void print_cmd(const cmd_plugin_t *p, int idx, void *arg) {
  (void)arg;
  printf("%d. %s (%s)\n", idx, p->name, p->cmd);
}

struct find_cmd {
  int target;
  const cmd_plugin_t *result;
};

static void find_cmd(const cmd_plugin_t *p, int idx, void *arg) {
  struct find_cmd *fc = arg;
  if (idx == fc->target)
    fc->result = p;
}

static void file_menu(const char *filepath) {
  while (1) {
    printf("\n----文件:%s----\n", filepath);
    loader_foreach_cmd(print_cmd, NULL);
    printf("0.返回目录\n选择:");
    char buf[32];
    if (!fgets(buf, sizeof(buf), stdin))
      break;
    int n = atoi(buf);
    if (n == 0)
      break;
    struct find_cmd fc = {
        .target = n,
        .result = NULL,
    };
    loader_foreach_cmd(find_cmd, &fc);
    if (fc.result)
      fc.result->func(filepath);
    else
      printf("无效选择\n");
  }
}

int main(void) {
  /*检查/创建插件目录*/
  if (ensure_plugin_dir(PLUGIN_DIR) != 0) {
    fprintf(stderr, "插件目录准备失败,已退出\n");
    return 1;
  }

  /*加载插件*/
  int n = loader_load_dir(PLUGIN_DIR);
  if (n == 0)
    printf("警告,没有检测到任何插件,文件操作不可用\n");

  /*主循环*/
  char filepath[MAX_PATH];
  while (1) {
    int r = select_entry(filepath, sizeof(filepath));
    if (r == -1)
      break;
    if (r == 2)
      file_menu(filepath);
  }

  loader_unload_all();
  printf("已退出\n");
  return 0;
}
