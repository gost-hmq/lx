#include <stdio.h>
#include <string.h>
#define LEN 256
int main() {
  FILE *fp = NULL;
  char name[LEN] = {0};
  scanf("%255s", name);
  fp = fopen(name, "r");
  if (fp) {
    fclose(fp);
    int n = 1;
    while (n) {
      printf("======================\n");
      printf("1.清空写\n");
      printf("2.追加写\n");
      printf("======================\n");
      printf("请输入你的选择:");
      scanf("%d", &n);
      if (n == 1) {
        fp = fopen(name, "w");
        fprintf(fp, "name:%s\n", name);
        fclose(fp);
        break;
      } else if (n == 2) {
        fp = fopen(name, "a");
        fprintf(fp, "\nname:%s\n", name);
        fclose(fp);
        break;
      } else
        printf("输入错误\n");
    }
  } else {
    fp = fopen(name, "w");
    fprintf(fp, "name:%s", name);
    fclose(fp);
  }
  return 0;
}
