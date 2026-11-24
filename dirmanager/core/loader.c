#include "plugin.h"
#include <dirent.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_PLUGINS 32
#define MAX_SERVICES 64

struct loaded_plugin {
  void *handle;
  const plugin_desc_t *desc;
  char path[512];
}; // loaded_plugin结构体公有

static struct loaded_plugin plugins[MAX_PLUGINS]; // static私有
static int plugin_count = 0;

static struct {
  const char *name;
  service_func_t func;
  void *data;
} services[MAX_SERVICES];
static int service_count = 0;

/*----主体API实现----*/

static int api_register_service(const char *name, service_func_t f,
                                void *data) {
  if (service_count >= MAX_SERVICES)
    return -1;
  for (int i = 0; i < service_count; i++) {
    if (!strcmp(services[i].name, name)) {
      services[i].func = f;
      services[i].data = data;
      return 0;
    }
  }
  services[service_count].name = name;
  services[service_count].func = f;
  services[service_count].data = data;
  service_count++;
  return 0;
}

static service_func_t api_get_service(const char *name, void **data) {
  for (int i = 0; i <= service_count; i++) {
    if (!strcmp(services[i].name, name)) {
      if (data)
        *data = services[i].data;
      return services[i].func;
    }
  }
  return NULL;
}

static void api_log(const char *fmt, ...) //...不定参数
{
  va_list ap;
  va_start(ap, fmt);
  fprintf(stderr, "[plugin] ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
}

/*全局API,插件通过dlopen时,解析这个符号*/
plugin_api_t plugin_api = {
    .register_service = api_register_service,
    .get_service = api_get_service,
    .log = api_log,
};

/*----加载流程----*/

int loader_load_dir(const char *dir) {
  DIR *d = opendir(dir);
  if (!d) {
    perror("opendir");
    return -1;
  }

  struct dirent *e;
  while ((e = readdir(d))) {
    size_t len = strlen(e->d_name);
    if (len < 4 || strcmp(e->d_name + len - 3, ".so")) {
      continue;
    }
    if (plugin_count >= MAX_PLUGINS) {
      break;
    }

    char path[512];
    snprintf(path, sizeof(path), "%s/%s", dir, e->d_name);

    void *handle = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    if (!handle) {
      fprintf(stderr, "dlopen %s 失败:%s\n", path, dlerror());
      continue;
    }

    const plugin_desc_t *desc =
        (const plugin_desc_t *)dlsym(handle, PLUGIN_ENTRY_SYMBOL);
    if (!desc) {
      fprintf(stderr, "dlsym %s 失败:%s\n", path, dlerror());
      dlclose(handle);
      continue;
    }

    if (desc->abi_version != PLUGIN_ABI_VERSION) {
      fprintf(stderr, "%s ABI版本不匹配(插件%d 主体%d)\n", path,
              desc->abi_version, PLUGIN_ABI_VERSION);
      dlclose(handle);
      continue;
    }

    plugins[plugin_count].handle = handle;
    plugins[plugin_count].desc = desc;
    snprintf(plugins[plugin_count].path, sizeof(plugins[plugin_count].path),
             "%s", path);
    plugin_count++;

    if (desc->on_load) {
      desc->on_load();
    }
    printf("已加载插件:%s(%s)\n", desc->name, path);
  }
  closedir(d);
  return plugin_count;
}

void loader_unload_all(void) {
  for (int i = plugin_count - 1; i >= 0; i--) {
    if (plugins[i].desc->on_unload) {
      plugins[i].desc->on_unload();
    }
    dlclose(plugins[i].handle);
  }
  plugin_count = 0;
  service_count = 0;
}

void loader_foreach_cmd(void (*cb)(const cmd_plugin_t *p, int idx, void *arg),
                        void *arg) {
  int idx = 1;
  for (int i = 0; i < plugin_count; i++) {
    const cmd_plugin_t *c = plugins[i].desc->cmds;
    if (!c) {
      continue;
    }
    for (; c->name; c++) {
      cb(c, idx++, arg);
    }
  }
}
