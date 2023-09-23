/*
g++ 服务端-转发- 落子数据.cpp&&./a.out
g++ 棋手-收发落子情况.cpp&&./a.out
g++ 棋手-收发落子情况.cpp&&./a.out
*/
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<cstring>
#include<unistd.h>
using namespace std;
#define PORT 9985
int getServerSockfd();
int sendGetColorRequest(int serverSockfd);
int recvSendDropRequest(int serverSockfd,char color);
int main(){
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
    int serverSockfd=getServerSockfd();//连接服务端  发送先后手请求  
    char color=sendGetColorRequest(serverSockfd);//接收身份颜色 
    recvSendDropRequest(serverSockfd,color);
}
int recvSendDropRequest(int serverSockfd,char color){
    char recvbuf[128];
    char sendbuf[128];
    int x,y;
    if(color=='*'){
        memset(recvbuf,0,sizeof(recvbuf));
        read(serverSockfd,recvbuf,128);// 接收落子通知
        memset(sendbuf,0,sizeof(sendbuf));
        printf("请输入棋子坐标(x,y)\n");
        scanf("%d,%d",&x,&y);
        //将x,y转化为网络字节序
        x=htons(x);
        y=htons(y);
        sprintf(sendbuf,"{way:down,local:(%d,%d),color:black}",x,y);
        write(serverSockfd,sendbuf,strlen(sendbuf));//发送落子请求
    }
    while(1){
        memset(recvbuf,0,sizeof(recvbuf));
        read(serverSockfd,recvbuf,38);//接收对手落子情况,(之所以设置为38是因为要联系读2次，读多了下面的read就没的读了)
        sscanf(recvbuf,"{way:down,local:(%d,%d)",&x,&y);
        //将x,y转化为本地字节序
        x=ntohs(x);
        y=ntohs(y);
        printf("%d,%d\n",x,y);
        memset(recvbuf,0,sizeof(recvbuf));
        read(serverSockfd,recvbuf,128);//接收落子通知
        memset(sendbuf,0,sizeof(sendbuf));
        printf("请输入棋子坐标(x,y)\n");
        scanf("%d,%d",&x,&y);
        //将x,y转化为网络字节序
        x=htons(x);
        y=htons(y);
        if(color=='0'){
            sprintf(sendbuf,"{way:down,local:(%d,%d),color:white}",x,y);
        } else if(color=='*'){
            sprintf(sendbuf,"{way:down,local:(%d,%d),color:black}",x,y);
        }
        write(serverSockfd,sendbuf,strlen(sendbuf));//发送落子请求
    }
}
int sendGetColorRequest(int serverSockfd){
    char buffer[128]={0};
    strcpy(buffer,"请求告知先后手身份");
    send(serverSockfd,buffer,sizeof(buffer),0);//!发送 请求
    char color;
    read(serverSockfd,&color,1);//!接收 请求处理结果
    if(color=='*'){
        printf("我是先手 color=*\n");
    } else if(color=='0'){
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
    seraddr.sin_port=htons(PORT);//端口号
    seraddr.sin_addr.s_addr=inet_addr("127.0.0.1");//ip地址
    int ret=connect(sockfd,(struct sockaddr*)&seraddr,sizeof(seraddr));
    if(ret<0){
        perror("connect");
        return -1;
    }
    return sockfd;
}