/*
 * 1.输入文件名
 * 2.用fopen(r)模式判断该文件是否存在
 * 3.if(fp)判断
 * 4.存在打印菜单switch选择
 * 5.然后调用print_file打印文件内容
 */
#include <stdio.h>
#define LEN 256
void print_file(FILE *fp); // 文件内容打印函数
int main() {
  char name[LEN] = {0};
  FILE *fp = NULL, *fw = NULL; // 文件指针先置空
  // 输入文件名
  printf("请输入文件名:");
  scanf("%255s", name);
  // 判断文件是否存在
  fp = fopen(name, "r");
  if (!fp) // fp=null
  {
    fw = fopen(name, "w");
    fprintf(fw, "name:%s\n", name);
    fclose(fw);
    printf("=====文件内容=====\n");
    fp = fopen(name, "r");
    print_file(fp);
    fclose(fp);
    return 0;
  }
  while (1) // 打印菜单
  {
    printf("====================\n");
    printf("1.清空写\n");
    printf("2.追加写\n");
    printf("0.退出\n");
    printf("====================\n");
    int n = 0;
    printf("请输入你的选择:");
    scanf("%d", &n);
    switch (n) {
    case 1: {
      fw = fopen(name, "w");
      if (fw) {
        fprintf(fw, "name:%s\n", name);
        fclose(fw);
        printf("=====文件内容=====\n");
        fp = fopen(name, "r");
        print_file(fp);
        fclose(fp);
      } else
        printf("写入失败");
      break;
    }
    case 2: {
      fw = fopen(name, "a");
      if (fw) {
        fprintf(fw, "\nname:%s\n", name);
        fclose(fw);

        printf("=====文件内容=====\n");
        fp = fopen(name, "r");
        print_file(fp);
        fclose(fp);
      } else
        printf("写入失败");
      break;
    }
    case 0: {
      return 0; // 关闭程序
      break;
    }
    default: {
      printf("输入错误,请重新输入\n");
      break;
    }
    }
  }
}
void print_file(FILE *fp) {
  int ch = 0; // 因为eof是-1
  while ((ch = fgetc(fp)) != EOF) {
    putchar(ch);
  }
}
