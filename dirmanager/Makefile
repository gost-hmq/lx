CC := $(shell command -v gcc 2>/dev/null || command -v clang 2>/dev/null)
CFLAGS  = -Wall -Wextra -std=c11 -Iinclude
TARGET  = dirmanager

PLUGIN_SRCS = $(wildcard src/plugins/*.c)
PLUGIN_SO   = $(patsubst src/plugins/%.c,plugins/%.so,$(PLUGIN_SRCS))

all: $(TARGET) plugins

$(TARGET): core/main.c core/loader.c
		$(CC) $(CFLAGS) -rdynamic -o $@ $^ -ldl

plugins: $(PLUGIN_SO)

plugins/%.so: src/plugins/%.c include/plugin.h
		@mkdir -p plugins
			$(CC) $(CFLAGS) -fPIC -shared -o $@ $<

clean:
		rm -f $(TARGET) plugins/*.so

print-cc:
		@echo "CC = $(CC)"
			@echo "插件源码: $(PLUGIN_SRCS)"
				@echo "插件输出: $(PLUGIN_SO)"

.PHONY: all plugins clean print-cc

