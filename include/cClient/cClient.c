#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <openssl/rc4.h>
#include "cClient.h"
#include "cJSON.h"
int RC4_encode(char* outstring,const char* instring, const char* rc4key)
{
    RC4_KEY key;
    unsigned char *out_string_tmp;
    RC4_set_key(&key,strlen(rc4key),(unsigned const char*)rc4key);
    RC4(&key,strlen(instring),(unsigned const char*)instring,outstring);
    return 0;
}
int RC4_decode(char* outstring,const char* instring, const char* rc4key)
{
    RC4_KEY key;
    unsigned char *out_string_tmp;
    RC4_set_key(&key,strlen(rc4key),(unsigned const char*)rc4key);
    RC4(&key,strlen(instring),(unsigned const char*)instring,outstring);
    return 0;
}
int show_jsoninfo(struct json_info* inputjson)
{
    printf("user_name:%s\n",inputjson->username);
    printf("code:%c\n",inputjson->code);
    printf("err_code:%c\n",inputjson->err_code);
    printf("msg:%s\n",inputjson->msg);
    printf("takl_status:%s\n",inputjson->status);
    printf("type:%s\n",inputjson->type);
    printf("to:%s\n",inputjson->to);
    printf("from:%s\n",inputjson->from);
    printf("talk_time:%s\n",inputjson->talk_time);
    return 0;
}
int Set_jsoninfo(struct json_info* outputjson, char* input_msg, char code)
{
    switch(code)
    {
    case '0':
        memset(outputjson->username,0,sizeof(outputjson->username));
        memset(outputjson->msg,0,sizeof(outputjson->msg));
        memset(outputjson->status,0,sizeof(outputjson->status));
        memset(outputjson->type,0,sizeof(outputjson->type));
        memset(outputjson->to,0,sizeof(outputjson->to));
        memset(outputjson->from,0,sizeof(outputjson->from));
        memset(outputjson->talk_time,0,sizeof(outputjson->talk_time));
        break;
    case '1':
        strcpy(outputjson->username,"daiqf");
        outputjson->code = '0';
        outputjson->err_code = '0';
        strcpy(outputjson->msg,input_msg);
        strcpy(outputjson->status,"send");
        strcpy(outputjson->type,"login");
        strcpy(outputjson->to,"gly2");
        strcpy(outputjson->from,"daiqf");
        strcpy(outputjson->talk_time,"0");
        break;
    case '2':
        strcpy(outputjson->username,"daiqf");
        outputjson->code = '0';
        outputjson->err_code = '0';
        strcpy(outputjson->msg,input_msg);
        strcpy(outputjson->status,"send");
        strcpy(outputjson->type,"talk");
        strcpy(outputjson->to,"gly2");
        strcpy(outputjson->from,"daiqf");
        strcpy(outputjson->talk_time,"0");
        break;
    }
    return 0;
}
char* generate_sendjson(struct json_info* input_json)
{
    cJSON *tmp_json = NULL;
    char *input_message = NULL;
    input_message = (char *)malloc(BUF_SIZE);
    tmp_json = cJSON_CreateObject();
    cJSON_AddStringToObject(tmp_json,"username",input_json->username);
    cJSON_AddStringToObject(tmp_json,"type",input_json->type);
    cJSON_AddStringToObject(tmp_json,"msg",input_json->msg);
    cJSON_AddStringToObject(tmp_json,"status",input_json->status);
    cJSON_AddStringToObject(tmp_json,"to",input_json->to);
    input_message = cJSON_Print(tmp_json);
    cJSON_Delete(tmp_json);
    return input_message;
}
int parsing_getjson(struct json_info* output_json,char* get_message)//测试
{
    cJSON *tmp_json = NULL;
    tmp_json = cJSON_Parse(get_message);
    cJSON *item = cJSON_GetObjectItem(tmp_json,"msg");
    strcpy(output_json->msg,item->valuestring);
    item = cJSON_GetObjectItem(tmp_json,"username");
    strcpy(output_json->username,item->valuestring);
    item = cJSON_GetObjectItem(tmp_json,"status");
    strcpy(output_json->status,item->valuestring);
    item = cJSON_GetObjectItem(tmp_json,"type");
    strcpy(output_json->type,item->valuestring);
    item = cJSON_GetObjectItem(tmp_json,"to");
    strcpy(output_json->to,item->valuestring);
    item = cJSON_GetObjectItem(tmp_json,"from");
    strcpy(output_json->from,item->valuestring);
    item = cJSON_GetObjectItem(tmp_json,"talk_time");
    strcpy(output_json->talk_time,item->valuestring);
    item = cJSON_GetObjectItem(tmp_json,"code");
    output_json->code,item->valuestring[0];
    item = cJSON_GetObjectItem(tmp_json,"err_code");
    output_json->err_code,item->valuestring[0];
    return 0;
}
void ShowHelpInfo() {
  printf("Usage: Digest [option]...\n\n");
  printf("  -t, --talk   enter communication mode\n");
  printf("  -h, --help   display this help\n");
  printf("\n");
}


int Send_message(int* input_fd, int code) {
  char buf_message[1024] = "";
  char quit_flag[256] = "quit\n";
  int send_bytes = 0;
  fgets(buf_message, sizeof(buf_message), stdin);
  if (0 == strcmp(buf_message, quit_flag)) {
    printf("Quit!\n");
    return -1;
  }
  if ((send_bytes = send(*input_fd, buf_message, strlen(buf_message), 0)) ==
      -1) {
    printf("fail to send!\n");
    goto err;
  }
err:
  return 0;
}
void Input_ServerIpPort(char* output_ip, int* output_port) {
  printf("进入交流模式,请输入服务器IP地址:\n");
  scanf("%s", output_ip);
  printf("%s", output_ip);
  printf("请输入服务器端口号:\n");
  scanf("%d", output_port);
  printf("%d", *output_port);
}
int Connect_ServerIpPort(struct sockaddr_in* output_addr,
                         struct hostent* output_host, int* output_fd,
                         char* input_ip, int* input_port) {
  printf("input_ip:%s\n input_port:%d\n", input_ip, *input_port);
  int connect_flag = 0;
  int err_flag = 0;
  output_host = gethostbyname(input_ip);
  printf("h_length:%d\n", output_host->h_length);
  *output_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == *output_fd) {
    printf("fail to create socket");
    err_flag = 1;
    goto err;
  }
  output_addr->sin_family = AF_INET;
  output_addr->sin_port = htons((uint16_t)*input_port);
  output_addr->sin_addr = *((struct in_addr*)output_host->h_addr);
  // connect_flag = connect(*output_fd,(struct sockaddr
  // *)&output_addr,sizeof(struct sockaddr));
  if (-1 == connect_flag)  //总共等了75s后仍未收到响应则返回本错误
  {
    printf("fail to connect!!");
    err_flag = 2;
    goto err;
  }
err:
  printf("fail to connect!!\n");
  if (err_flag == 0) printf("connect success!\n");
  return err_flag;
}
int Talk_Server(int* input_fd)
{
    char message[BUF_SIZE] = {0};
    char send_message_tmp[BUF_SIZE] = {0};
    char rc4send_message[BUF_SIZE] = {0};
    char *send_message = send_message_tmp;
    char getrc4_message[BUF_SIZE] = {0};
    char get_message[BUF_SIZE] = {0};
    int isClientWork = 1;  //表示 客户端是否正常工作
    struct json_info *send_json;
    send_json = (struct json_info*)malloc(sizeof(struct json_info));
    struct json_info *get_json;
    get_json = (struct json_info*)malloc(sizeof(struct json_info));
    Set_jsoninfo(send_json,NULL,0);
    strcpy(send_json->username,"daiqf");
    strcpy(send_json->type,"login");
    send_message = generate_sendjson(send_json);
    printf("发送登录请求json:%s\n",send_message);
    RC4_encode(rc4send_message,send_message,MYRC4_KEY);
    printf("发送加密后的登录请求json:%s\n",rc4send_message);
    if(send(*input_fd,rc4send_message, strlen(rc4send_message),0) == SEND_ERROR)
    {
        printf("Talk_Server:file to send \n");
        goto err;
    }
    pid_t pid = fork();
    while(isClientWork)
    {
        if(pid > 0)
        {
            fgets(message, BUF_SIZE, stdin);
            if (strncmp(message, EXIT, strlen(EXIT)) == 0) {
                isClientWork = 0;
                goto err;
            }
            Set_jsoninfo(send_json,NULL,0);
            Set_jsoninfo(send_json,message,2);
            memset(send_message,0,sizeof(send_message));
            send_message = generate_sendjson(send_json);
            printf("发送的Json消息:%s\n",send_message);
            memset(rc4send_message,0,sizeof(rc4send_message));
            RC4_encode(rc4send_message,send_message,MYRC4_KEY);
            printf("发送的RC4_Json消息:%s\n",rc4send_message);
            send(*input_fd,rc4send_message, strlen(rc4send_message),0);
            if(send(*input_fd,rc4send_message, strlen(rc4send_message),0) == SEND_ERROR)
            {
                printf("Talk_Serve:file to send,talk \n");
                goto err;
            }
        }
        else if(pid == 0)
        {
            memset(getrc4_message,0,sizeof(getrc4_message));
            int ret = recv(*input_fd, getrc4_message, sizeof(getrc4_message), 0);
            if(recv(*input_fd, getrc4_message, sizeof(getrc4_message), 0) <= 0)
            {
                printf("服务端关闭了连接，fd=%d\n", *input_fd);
                isClientWork = 0;
                goto err;
            }
            memset(get_message,0,sizeof(get_message));
            RC4_decode(get_message,getrc4_message,MYRC4_KEY);
            Set_jsoninfo(get_json,NULL,0);
            parsing_getjson(get_json,get_message);//testhellocali
            show_jsoninfo(get_json);
            if(strncmp(get_json->type, LOGOUT, strlen(LOGOUT)) == 0)
            {
                printf("logout!!!!\n");
                isClientWork = 0;
                goto err;
            }
            printf("接收消息：%s\n", get_message);
        }
        else
        {
            isClientWork = 0;
            printf("Talk_Server:file create fork\n");
            goto err;
        }
    }
err:
    free(send_json);
    send_json = NULL;
    return 0;
}
