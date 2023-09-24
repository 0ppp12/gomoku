/*** 
 * @Author: PENG
 * @Date: 2023-09-22 15:45:02
 * @LastEditTime: 2023-09-24 14:54:53
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
    /*初始化TCP连接，参数1为服务器IP，参数2为端口号，返回值为client_socket*/
    int INIT_SOCKET(const char* SERVER_IP,int PORT);
    /*发送账号密码，参数1为client_socket，参数2：0为注册1为登录，参数3为用户名，参数4为密码
    返回值为字符串"登录失败,账户或密码错误"或"账户已存在"*/
    char * Send_NameAndPassword(int client_socket,int flag,string name,string password);
    /*发送整个棋盘信息到服务器*/
    bool Send_Checkerboard_Info(int client_socket,char a[13][13]);
    /*接收服务器反馈信息*/
    bool Rev_info(int client_socket,char * recvbuffer);
    
    std::string client_name;
    std::string client_password;
};
