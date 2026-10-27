/*
 * 1.输入文件名
 * 2.用fopen(r)模式判断该文件是否存在
 * 3.if(fp)判断
 * 4.存在打印菜单switch选择
 * 5.然后调用print_file打印文件内容
 */
#include "file_ops.h"
#include <stdio.h>
#define LEN 256
int main() {
  char name[LEN] = {0};
  FILE *fp = NULL;
  printf("请输入文件名:");
  scanf("%255s", name);
  fp = fopen(name, "r");
  if (!fp) {
    write_clear(name);
    printf("=====文件内容=====\n");
    fp = fopen(name, "r");
    if (fp) {
      print_file(fp);
      fclose(fp);
    }
    return 0;
  }
  fclose(fp);
  while (1) {
    printf("====================\n");
    printf("1.清空写\n");
    printf("2.追加写\n");
    printf("0.退出\n");
    printf("====================\n");
    int n = 0;
    printf("请输入你的选择:");
    scanf("%d", &n);
    switch (n) {
    case 0:
      return 0;
    case 1: {
      if (write_clear(name)) {
        printf("=====文件内容=====\n");
        fp = fopen(name, "r");
        if (fp) {
          print_file(fp);
          fclose(fp);
        }
      } else {
        printf("写入失败\n");
      }
      break;
    }
    case 2: {
      if (write_append(name)) {
        printf("=====文件内容=====\n");
        fp = fopen(name, "r");
        if (fp) {
          print_file(fp);
          fclose(fp);
        }
      } else {
        printf("写入失败\n");
      }
      break;
    }
    default: {
      printf("输入错误,请重新输入\n");
      break;
    }
    }
  }
  return 0;
}
