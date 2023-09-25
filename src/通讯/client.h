/*** 
 * @Author: PENG
 * @Date: 2023-09-22 15:45:02
 * @LastEditTime: 2023-09-25 20:10:12
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
#include <string>
#include <signal.h>
#include <stdlib.h>
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
    /*发送整个棋盘信息到服务器,参数1为client_socket，参数2为棋盘信息*/
    bool Send_Checkerboard_Info(int client_socket,int a[13][13]);
    /*接收整个棋盘信息，参数1为client_socket，返回值为a[13][13]*/
    int **Recv_Checkerboard_Info(int client_socket);
    /*反回落子信息,参数1为client_socket，参数2为x,参数3为y,参数4为color*/
    void Send_position(int client_socket,int x,int y,int color);
    /*接收落子信息，参数1为client_socket，返回值num[0]为x，num[1]为y，num[2]为color*/
    int *Recv_position(int client_socket);

    /*发送登录或者观战信息*/
    bool Send_start_and_watch_Info(int client_socket,int flag);
    
    std::string client_name;
    std::string client_password;
};
