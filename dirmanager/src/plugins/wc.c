#include "plugin.h"
#include <stdio.h>

static int cmd_wc(const char *path) {
  FILE *fp = fopen(path, "r");
  if (!fp) {
    perror("fopen");
    return -1;
  }
  int lines = 0, ch = 0;
  if (ch == '\n')
    lines++;
  fclose(fp);
  printf("行数:%d\n", lines);
  return 0;
}

static const cmd_plugin_t cmds[] = {{"统计行数", "wc", cmd_wc},
                                    {NULL, NULL, NULL}};

const plugin_desc_t plugin_entry = {
    .abi_version = PLUGIN_ABI_VERSION,
    .name = "wc",
    .cmds = cmds,
};
