/*
 * @Author: victor victor@example.com
 * @Date: 2023-09-19 18:53:33
 * @LastEditors: victor victor@example.com
 * @LastEditTime: 2023-09-21 14:05:37
 * @FilePath: \work\stage5\game-project\the-gobang-game-of-cc-md-fk\src\client.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include "server.h"

int main()
{
    //创建套接字
    int client_socket;
    struct sockaddr_in server_address;

    // 创建客户端套接字
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) 
    {
        perror("socket");
        return 1;
    }

    // 设置服务器地址
    server_address.sin_family = AF_INET;
    // server_address.sin_addr.s_addr = inet_addr("192.168.13.64");
    if (inet_pton(AF_INET, "192.168.13.64", &(server_address.sin_addr)) <= 0)
    {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return 1;
    }
    server_address.sin_port = htons(11451);

    // 连接到服务器
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) 
    {
        perror("connect");
        return 1;
    }

    //标志，0为注册，1为登录
    int login_flag;
    std::cin>>login_flag;
     //发送客户端名称
    Send_info(client_socket,login_flag);

    close(client_socket);
    return 0;
}
//登录或注册发送
bool Send_info(int client_socket,int a)
{
    std::string client_info;
    client_info.clear();
    std::string client_name;
    std::cin >> client_name;
    std::string client_password;
    std::cin >> client_password;
    if(a==1)
    {
        client_info = "way:login,account:"+ client_name + "|password:" + client_password;
    }
    else if(a==0)
    {
        client_info = "way:register,account:"+ client_name + "|password:" + client_password;
    }
    std::cout<<client_info<<" "<<client_info.length()<< std::endl;
    if(send(client_socket, client_info.c_str(), client_info.length(), 0)<0)
    {
        std::cout<<"登录信息发送失败"<<std::endl;
        return false;
    }
    return true;
}
