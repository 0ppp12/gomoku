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
    char name[50];
    char password[50];
    int sockfd;
    char play_score[50];
    char message[BUFFER_SIZE];
} Player;

typedef struct 
{
    Player people[2];
    int sign;
    int num;
}Room;


bool File_read(std::string filename,Player *buff);
bool File_write(std::string filename,std::string info);
Player  way_choose(char *recvbuffer,Player *buff);
void Get_NameAndPassword(char *recvbuffer);