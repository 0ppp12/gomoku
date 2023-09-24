/* 
g++ 服务端-分配先后手身份.cpp&&./a.out
g++ 棋手-请求先后手身份.cpp&&./a.out
 */
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<cstring>
#include<unistd.h>
using namespace std;
#define PORT 8888
int tcpInit();
pair<int,int> tcpAcceptCfd(int sockfd);
int main(){
    int sockfd=tcpInit();
    pair<int,int> cfdSfd=tcpAcceptCfd(sockfd);
    pair<int,int> cfdSfd2=tcpAcceptCfd(sockfd);
    char recvbuffer[128]={0};
    char recvbuffer2[128]={0};
    int request1=read(cfdSfd.first,recvbuffer,128);
    int request2=read(cfdSfd2.first,recvbuffer,128);
    if(request1>0&&request1>0){//!接收2个先后手身份请求
        if(random()%2){
            char color='0';
            write(cfdSfd.first,&color,1);//!向申请的棋手玩家发送
            color='*';
            write(cfdSfd2.first,&color,1);//!向申请的棋手玩家的对手发送发送
        } else{
            char color='*';
            write(cfdSfd.first,&color,1);//!向申请的棋手玩家发送
            color='0';
            write(cfdSfd2.first,&color,1);//!向申请的棋手玩家的对手发送发送
        }
    }
    return 0;
}
pair<int,int> tcpAcceptCfd(int sockfd){
    //4.接受连接（默认是阻塞）
    struct sockaddr_in clientaddr;
    socklen_t len=sizeof(clientaddr);
    int clientfd=accept(sockfd,(struct sockaddr *)(&clientaddr),&len);
    if(clientfd<0){
        perror("accept");
        return make_pair(-1,-1);
    }
    printf("客户端IP:%s\n",inet_ntoa(clientaddr.sin_addr));
    pair<int,int> cfdSfd=make_pair(clientfd,sockfd);
    return cfdSfd;
}
int tcpInit(){
    //1.创建套接字
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        perror("socket");
        return -1;
    }
    //2.绑定
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr=INADDR_ANY;
    int ret=bind(sockfd,(struct sockaddr*)(&addr),sizeof(addr));
    if(ret<0){
        perror("bind");
        return -1;
    }
    //3.监听
    ret=listen(sockfd,5);
    if(ret<0){
        perror("listen");
        return -1;
    }
    return sockfd;
}