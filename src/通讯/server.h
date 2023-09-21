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
#include <arpa/inet.h>  // ����������غ�����ͷ�ļ�
#include <sys/epoll.h>

#define BUFFER_SIZE 1024
typedef struct 
{
    int client_socket; //�׽���������
    char client_name[50];//�ͻ�����
} Client;

typedef struct 
{
    char name[50];
    int is_correct;
    char play_time[50];
} Player;

bool Send_info(int client_socket,int a);
bool way_choose(char *recvbuffer,std::string *buff);