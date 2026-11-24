#include "plugin.h"
#include <plugin-api.h>
#include <stdio.h>
#include <time.h>

static void *svc_now(void *arg) {
  (void)arg;
  static char buf[64];
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
           tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour,
           tm->tm_min, tm->tm_sec);
  /*
   * tm_year年
   * tm_mon月
   * tm_mday日
   * tm_hour时
   * tm_min分
   * tm_sec秒
   */
  return buf;
}

static int cmd_show_time(const char *path) {
  (void)path;
  printf("当前时间:%s\n", (char *)svc_now(NULL));
  return 0;
}

static const cmd_plugin_t cmds[] = {{"显示时间", "time", cmd_show_time},
                                    {NULL, NULL, NULL}};

static void on_load(void) {
  plugin_api.register_service("time.now", svc_now, NULL);
  plugin_api.log("time_ops已加载,注册time.now");
}

const plugin_desc_t plugin_entry = {
    .abi_version = PLUGIN_ABI_VERSION,
    .name = "time_ops",
    .on_load = on_load,
    .on_unload = NULL,
    .cmds = cmds,
};
