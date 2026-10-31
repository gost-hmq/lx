/*
 * 打开目录<dirent.h>
 * DIR* opendir(const char *path)
 * 成功返回当前目录指针,失败则Null
 * path：路径名
 *
 * 读目录<dirent.h>
 * struct dirent* readdir(DIR* dir_handle)
 * 成功返回dirent结构体指针，失败则Null
 * dir_handle:当前目录指针
 *
 * 关闭目录<dirent.h>
 * int closedir(DIR *dirp)
 * 成功返回0，失败-1
 * dirp:当前目录指针
 *
 * mkdir创建目录<sys/stat.h>
 * int mkdir(const char *pathname, mode_t mode)
 * 成功返回0，失败-1
 * pathname:要创建的目录路径
 * mode:权限模式
 * 读:4   写:2    执行:1
 * 用户模型：所有者，同组用户，普通用户(0开头)
 *
 * 删除目录<sys/stat.h>
 * int rmdir(const char *pathname)
 * 成功返回0，失败-1
 * pathname:要删除的目录路径
 *
 * 切换目录<unistd.h>
 * int chdir(const char *path)
 * 成功返回0，失败-1
 * path:目标目录路径
 *
 * 获取当前目录绝对路径<unistd.h>
 * char *getcwd(char *buf,size_t size)
 * 成功返回缓冲区指针即buf指针，失败则Null
 * buf:存放结果的缓冲区
 * size:缓冲区大小
 *
 * 获取当前目录/文件属性<sys/stat.h><unistd.h>
 * int stat(const char *pathname,struct stat *statbuf)
 * 成功返回0，失败-1
 * pathname:查询文件/目录路径
 * statbuf:stat结构体指针
 * stat结构体详细信息到时在查
 */
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
int main() { //
  return menu_run();
}
