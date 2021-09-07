#include <getopt.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>

#include "cClient.h"
#include "cJSON.h"
int main(int argc, char* argv[]) {
  char server_ip[256] = "";
  int server_port = 0;
  char parm = 0;
  int option_index = 0;
  int client_fd = 0;
  int connect_flag = 0;
  int recv_flag = 0;
  pid_t pid;
  struct sockaddr_in server_addr;
  struct hostent* server_host;
  /**
   *  定义命令行参数列表，option结构的含义如下（详见 man 3 getopt）：
   *  struct option {
   *      const char *name;      //参数的完整名称，对应命令中的 --xxx
   *      int	 has_arg;   //该参数是否带有一个值，如 –config xxx.conf
   *      int	*flag;      //一般设置为NULL
   *      int	 val;
   * //解析到该参数后getopt_long函数的返回值，为了方便维护，一般对应getopt_long调用时第三个参数
   *  };
   */

  static struct option arg_options[] = {
      {"talk", 0, 0, 't'}, {"help", 0, 0, 'h'}, {0, 0, 0, 0}};
  while ((parm = getopt_long(argc, argv, "th", arg_options, &option_index)) !=
         PARM_ERROR) {
    switch (parm) {
      case 't':
        Input_ServerIpPort(server_ip, &server_port);
        Connect_ServerIpPort(&server_addr, server_host, &client_fd, server_ip,
                             &server_port);
        connect_flag = connect(client_fd, (struct sockaddr*)&server_addr,
                               sizeof(struct sockaddr));
        if (CONNECT_ERROR == connect_flag)  //总共等了75s后仍未收到响应则返回本错误
        {
          printf("fail to connect!!");
          goto err;
        }
        Talk_Server(&client_fd);
        break;
      case 'h':
        printf("hhhhhh\n");
        ShowHelpInfo();
        return 0;
      default:
        fprintf(stderr, "Unknown option: %c\n", parm);
        printf("404!404!404!!!!\n");
        ShowHelpInfo();
        return -1;
    }
  }
err:
  close(client_fd);
  return 0;
}

