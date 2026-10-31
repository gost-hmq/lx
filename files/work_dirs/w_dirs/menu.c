#include "menu.h"
#include "dir_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_INPUT 64
static const char *DIR_PATH = "./test";
static const char *INDEX_FILE = "./test/.index.txt";
static void print_menu(void) // static表明函数私有
{
  printf("\n===== 目录工具 =====\n");
  printf("1.扫描目录并生成索引\n");
  printf("2.查看索引文件\n");
  printf("3.退出\n");
  printf("====================\n");
}
static void do_scan(void) {
  if (ensure_dir(DIR_PATH) != 0)
    return;
  FILE *fp = fopen(INDEX_FILE, "w");
  if (!fp) {
    perror("fopen failed");
    return;
  }
  printf("正在扫描 %s ...\n", DIR_PATH);
  scan_dir(DIR_PATH, fp);
  fclose(fp);
  printf("索引已写入: %s\n", INDEX_FILE);
}
static void do_view(void) {
  FILE *fp = fopen(INDEX_FILE, "r");
  if (!fp) {
    printf("索引文件不存在,请先执行扫描\n");
    return;
  }
  printf("\n--- 索引内容 ---\n");
  char line[512];
  while (fgets(line, sizeof(line), fp)) {
    printf("%s", line);
  }
  printf("--- 结束 ---\n");
  fclose(fp);
}
int menu_run(void) {
  char input[MAX_INPUT];
  while (1) {
    print_menu();
    if (!fgets(input, sizeof(input), stdin))
      break;
    int choice = atoi(input); // 将数字字符转成int
    switch (choice) {
    case 1:
      do_scan();
      break;
    case 2:
      do_view();
      break;
    case 3:
      printf("再见\n");
      return 0;
    default:
      printf("无效输入,请重新输入\n");
    }
  }
  return 0;
}
