#pragma once 
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>
#include <signal.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // 包含网络相关函数的头文件
#include <sys/epoll.h>

#define BUFFER_SIZE 1024
typedef struct 
{
    int client_socket; //套接字描述符
    char client_name[50];//客户名字
} Client;

typedef struct 
{
    char name[50];
    int is_correct;
    char play_time[50];
} Player;

bool Send_info(int client_socket,int a);
bool way_choose(char *recvbuffer,std::string *buff);