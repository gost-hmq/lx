#include "plugin.h"
#include <stdio.h>

static void print_file(FILE *fp) {
  int ch;
  while ((ch = fgetc(fp)) != EOF)
    putchar(ch);
}

/*通过服务表拿时间，不直接依赖time_ops*/
static const char *now_str(void) {
  void *data;
  service_func_t f = plugin_api.get_service("time.now", &data);
  return f ? (const char *)f(NULL) : "unknown-time";
}

static void show_content(const char *path) {
  printf("=========内容=========\n");
  FILE *fp = fopen(path, "r");
  if (fp) {
    print_file(fp);
    fclose(fp);
  }
}

static int cmd_clear(const char *path) {
  FILE *fp = fopen(path, "w");
  if (!fp) {
    perror("fopen");
    return -1;
  }
  fprintf(fp, "time:%s\tpath:%s\n", now_str(), path);
  fclose(fp);
  show_content(path);
  return 0;
}

static int cmd_append(const char *path) {
  FILE *fp = fopen(path, "a");
  if (!fp) {
    perror("fopen");
    return -1;
  }
  fprintf(fp, "\ntime:%s\tpath:%s\n", now_str(), path);
  fclose(fp);
  show_content(path);
  return 0;
}

static int cmd_read(const char *path) {
  FILE *fp = fopen(path, "r");
  if (!fp) {
    perror("fopen");
    return -1;
  }
  show_content(path);
  fclose(fp);
  return 0;
}

static const cmd_plugin_t cmds[] = {{"清空写", "clear", cmd_clear},
                                    {"追加写", "append", cmd_append},
                                    {"查看内容", "read", cmd_read},
                                    {NULL, NULL, NULL}};

static void on_load(void) { plugin_api.log("file_ops已加载"); }

const plugin_desc_t plugin_entrytry = {
    .abi_version = PLUGIN_ABI_VERSION,
    .name = "file_ops",
    .on_load = on_load,
    .on_unload = NULL,
    .cmds = cmds,
};
