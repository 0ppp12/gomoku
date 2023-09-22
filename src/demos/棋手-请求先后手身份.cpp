#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<cstring>
#include<unistd.h>
int getServerSockfd();
int sendGetColorRequest(int serverSockfd);
using namespace std;
int main(){
    int serverSockfd=getServerSockfd();
    char color=sendGetColorRequest(serverSockfd);
}
int sendGetColorRequest(int serverSockfd){
    char buffer[128]={0};
    strcpy(buffer,"请求告知先后手身份");
    printf("请求告知先后手身份 %d\n",send(serverSockfd,buffer,sizeof(buffer),0));//!发送 请求
    char color;
    read(serverSockfd,&color,1);//!接收 请求处理结果
    if(color=='*'){
        printf("我是先手 color=*\n");
    }else if(color=='0'){
        printf("我是后手 color=0\n");
    }
    return color;
}
int getServerSockfd(){
    //1.创建套接字
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        perror("socket");
        return -1;
    }
    //2.连接服务器
    struct sockaddr_in seraddr;
    memset(&seraddr,0,sizeof(seraddr));
    seraddr.sin_family=AF_INET;//地址族
    seraddr.sin_port=htons(9996);//端口号
    seraddr.sin_addr.s_addr=inet_addr("127.0.0.1");//ip地址
    int ret=connect(sockfd,(struct sockaddr*)&seraddr,sizeof(seraddr));
    if(ret<0){
        perror("connect");
        return -1;
    }
    return sockfd;
}