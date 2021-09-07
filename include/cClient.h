#ifndef cClient__h
#define cClient__h

#define MAX_MESSAGE_SIZE 1024
#define EPOLL_SIZE 5000
#define BUF_SIZE 1000
#define SEND_ERROR -1
#define PARM_ERROR -1
#define CONNECT_ERROR -1
#define MAX_RC4KEY_SIZE 256
#define MYRC4_KEY "123456"
#define EXIT "EXIT"
#define LOGOUT "logout"
#define CLEAN_FALG '0'
#define LOGIN_FALG '1'
#define TALK_FALG '2'
#define LOGOUT_FALT '3'
struct json_info {
  char username[20];
  char code;
  char err_code;
  char msg[1024];
  char status[1024];
  char type[20];
  char to[20];
  char from[20];
  char talk_time[1024];
};
/**
 * @param[out] outstring 使用RC4加密后的输出字符串
 * @param[in] instring 未加密的字符串
 * @param[in] rc4_key rc4加密用的密钥
 */
int RC4_encode(char* outstring, const char* instring, const char* rc4_key);
/**
 * @param[out] outstring 使用RC4解密后的输出字符串
 * @param[in] instring 使用RC4加密的字符串
 * @param[in] rc4_key rc4加密用的密钥
 */
int RC4_decode(char* outstring, const char* instring, const char* rc4_key);
/**
 * @param[out] send_message json字符串
 * @param[in] type 通信类型，包括login,talk等类型
 * @param[in] msg 发送的消息
 */
char* generate_sendjson(struct json_info* input_json);
int parsing_getjson(struct json_info* output_json, char* get_message);

void ShowHelpInfo();
void Input_ServerIpPort(char* output_ip, int* output_port);
int Connect_ServerIpPort(struct sockaddr_in* output_addr,
                         struct hostent* output_host, int* output_fd,
                         char* input_ip, int* input_port);
int Set_jsoninfo(struct json_info* outputjson, char* input_msg, char code);
int show_jsoninfo(struct json_info* inputjson);
int Send_message(int* input_fd, int code);
int Talk_Server(int* input_fd);
#endif
