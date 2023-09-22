#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<cstring>
#include<unistd.h>
using namespace std;
int sendRetractRequest(int serversockfd);
int getServerSockfd();
int main(){
    int serverSockfd=getServerSockfd();
    sendRetractRequest(serverSockfd);
    sleep(3);
    sendRetractRequest(serverSockfd);
    sleep(3);
    sendRetractRequest(serverSockfd);
    close(serverSockfd);
}
int sendRetractRequest(int serverSockfd){
    char buffer[128]={0};
    strcpy(buffer,"I want retract");
    printf("I want retract %d\n",send(serverSockfd,buffer,sizeof(buffer),0));//!发送悔棋请求
    //发送悔棋请求
    char c;
    read(serverSockfd,&c,1);//!接收悔棋请求处理结果
    if(c=='1'){
        cout<<"对方同意悔棋请求"<<endl;
    }else{
        cout<<"对方不同意悔棋请求"<<endl;
    }    
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
    seraddr.sin_port=htons(9997);//端口号
    seraddr.sin_addr.s_addr=inet_addr("127.0.0.1");//ip地址
    int ret=connect(sockfd,(struct sockaddr*)&seraddr,sizeof(seraddr));
    if(ret<0){
        perror("connect");
        return -1;
    }
    return sockfd;
}
