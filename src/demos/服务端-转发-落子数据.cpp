#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<cstring>
#include<unistd.h>
using namespace std;
#define PORT 9984
int tcpInit();
pair<int,int> tcpAcceptCfd(int sockfd);
pair<int,int> sendSetColor();
int forwardHeiBaiRequest(int heifd,int baifd);
int main(){
    pair<int,int> fds=sendSetColor();
    int heifd=fds.first;
    int baifd=fds.second;
    forwardHeiBaiRequest(heifd,baifd);
    return 0;
}
int forwardHeiBaiRequest(int heifd,int baifd){
    //服务端                    先手黑子        后手白纸
    //接收2玩家连接             连接服务端      连接服务端
    //接收2个先后手身份请求      发送先后手请求  发送先后手请求
    //随机先2玩家发送身份       接收身份颜色    接收身份颜色
    //发送先手落子通知给黑子     接收落子通知
    //接收黑子落子请求          发送落子请求
// while(1){
    //发送黑子落子情况给白子                    接收对手落子情况
    //发送落子通知给白子                        接收落子通知
    //接收白子落子请求                           发送落子请求
    //发送白子落子情况给黑子    接收对手落子情况
    //发送落子通知给黑子        接收落子通知
    //接收黑子落子请求          发送落子请求
// }
    char sendbuf[128];
    memset(sendbuf,0,128);
    strcpy(sendbuf,"该你落子了");
    write(heifd,sendbuf,strlen(sendbuf));//!发送先手落子通知给黑子
    char recvbuf[128];
    memset(recvbuf,0,128);
    read(heifd,recvbuf,sizeof(recvbuf));//!接收黑子落子请求
    int x,y;
    sscanf(recvbuf,"{way:down,local:(%d,%d)",&x,&y);
    //将x,y转化为本地字节序
    x=ntohs(x);
    y=ntohs(y);
    printf("%d,%d\n",x,y);
    sscanf(recvbuf,"way:down,local:(%d,%d)",&x,&y);
    while(1){
        write(baifd,recvbuf,strlen(recvbuf));//!发送黑子落子情况给白子
        memset(sendbuf,0,128);
        strcpy(sendbuf,"该你落子了");
        // printf("%d--baifd:%d\n",write(baifd,sendbuf,strlen(sendbuf)),baifd);//!发送落子通知给白子 
        // printf("%d--baifd:%d\n",write(baifd,sendbuf,strlen(sendbuf)),baifd);//!发送落子通知给白子 
        memset(recvbuf,0,128);
        printf("1\n");
        read(baifd,recvbuf,128);//!接收白子落子请求
        printf("2\n");
        sscanf(recvbuf,"{way:down,local:(%d,%d)",&x,&y);
        //将x,y转化为本地字节序
        x=ntohs(x);
        y=ntohs(y);
        printf("%d,%d\n",x,y);
        write(heifd,recvbuf,strlen(recvbuf));//!发送白子落子情况给黑子
        memset(sendbuf,0,128);
        strcpy(sendbuf,"该你落子了");
        write(heifd,sendbuf,strlen(sendbuf));//!发送落子通知给黑子
        memset(recvbuf,0,128);
        printf("3\n");
        read(heifd,recvbuf,128);//!接收黑子落子请求
        printf("4\n");
        sscanf(recvbuf,"{way:down,local:(%d,%d)",&x,&y);
        //将x,y转化为本地字节序
        x=ntohs(x);
        y=ntohs(y);
        printf("%d,%d\n",x,y);
    }
}
pair<int,int> sendSetColor(){
    int sockfd=tcpInit();
    pair<int,int> cfdSfd=tcpAcceptCfd(sockfd);//!接收2玩家连接 
    pair<int,int> cfdSfd2=tcpAcceptCfd(sockfd);
    char recvbuffer[128]={0};
    char recvbuffer2[128]={0};
    int request1=read(cfdSfd.first,recvbuffer,128);
    int request2=read(cfdSfd2.first,recvbuffer,128);
    int heifd,baifd;
    if(request1>0&&request1>0){//!接收2个先后手身份请求 
        if(random()%2){//!随机先2玩家发送身份   
            char color='0';
            write(cfdSfd.first,&color,1);
            baifd=cfdSfd.first;
            color='*';
            write(cfdSfd2.first,&color,1);
            heifd=cfdSfd2.first;
        } else{
            char color='*';
            write(cfdSfd.first,&color,1);
            heifd=cfdSfd.first;
            color='0';
            write(cfdSfd2.first,&color,1);
            baifd=cfdSfd2.first;
        }
    }
    return make_pair(heifd,baifd);
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