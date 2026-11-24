#ifndef PLUGIN_H
#define PLUGIN_H

#define PLUGIN_ABI_VERSION 1
#define PLUGIN_ENTRY_SYMBOL "plugin_entry"

/*服务:插件对外暴露的能力*/
typedef void *(*service_func_t)(void *arg);

typedef struct {
  const char *name;
  service_func_t func;
  void *data;
} service_t;

/*命令:出现在菜单里的可选项*/
typedef int (*cmd_func_t)(const char *path);

typedef struct {
  const char *name; /*菜单显示名*/
  const char *cmd;  /*关键字*/
  cmd_func_t func;
} cmd_plugin_t;

/*每个so文件必须导出的入口结构*/
typedef struct {
  int abi_version;
  const char *name;
  void (*on_load)(void);
  void (*on_unload)(void);
  const cmd_plugin_t *cmds; /*以{Null,Null,Null}结尾*/
} plugin_desc_t;

/*主体提供给插件的API*/
typedef struct {
  int (*register_service)(const char *name, service_func_t f, void *data);
  service_func_t (*get_service)(const char *name, void **data);
  void (*log)(const char *fmt, ...);
} plugin_api_t;

/*插件侧使用这个全局变量调用主体API*/
extern plugin_api_t plugin_api;

#endif
