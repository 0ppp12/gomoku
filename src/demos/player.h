#include<iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<cstring>
#include<unistd.h>
#define PORT 1888
using namespace std;
class Player{
    public:
    char name[20];
    char color;
    int serverSockfd;

    int recvSendDropRequest(int serverSockfd);//收发落子请求（落子情况角落）
    int sendGetColorRequest(int serverSockfd);//发送获取颜色请求
    int getServerSockfd();//获取服务端fd
    int sendRetractRequest(int serverSockfd);//发送悔棋请求
    int signup();//注册
    int signin();//登录
};
