/*** 
 * @Author: PENG
 * @Date: 2023-09-22 15:45:02
 * @LastEditTime: 2023-09-23 10:04:05
 * @LastEditors: PENG
 * @Description: 
 * @FilePath: \c++\c++ 阶段项目\Gobang\src\通讯\client.h
 * @Copyright(c)
 */
#pragma once 
#include <iostream>
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

using namespace std;
class Info_SendAndRev{
public:
    int INIT_SOCKET(const char* SERVER_IP,int PORT);
    bool Send_NameAndPassword(int client_socket,int flag,string name,string password);
    bool Send_Checkerboard_Info(int client_socket);
    bool Rev_info(int client_socket,char * recvbuffer);
    
    std::string client_name;
    std::string client_password;
};
