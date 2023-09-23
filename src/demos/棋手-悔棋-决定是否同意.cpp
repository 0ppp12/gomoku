/*
g++ 棋手-悔棋-决定是否同意.cpp -pthread&&./a.out
g++ 棋手-悔棋-请求同意悔棋.cpp&&./a.out
*/
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<cstring>
#include<unistd.h>
using namespace std;
void* isAgreeRetract(void*arg);
pair<int,int> tcpAcceptCfd();
int main(){
    //单独一个线程等待对方请求悔棋
    pthread_t t1;
    pthread_create(&t1,NULL,isAgreeRetract,NULL);
    printf("做其它正事(下棋)\n");
    pthread_join(t1,NULL);
    printf("正事做完了\n");
    return 0;
}
void* isAgreeRetract(void*arg){
    //假设已建立了tcp连接，获取了对方的fd
    int isAcceptRequest=0;
    char r;
    pair<int,int> cfdSfd=tcpAcceptCfd();
    while(1){//直到棋局结束才不需要悔棋，故需要一直等这类请求
        char recvbuffer[128]={0};
        if(0==isAcceptRequest){
            if(read(cfdSfd.first,recvbuffer,128)>0){//!
                isAcceptRequest=1;
            }//接收对方请求悔棋
        }
        if(isAcceptRequest){
            printf("对方请求悔棋，是否同意?(y/n)\n");
            cin>>r;
            if('y'==r){
                char buf='1';
                write(cfdSfd.first,&buf,1);//!
                printf("已发送同意对方悔棋通知\n");
                isAcceptRequest=0;
            } else if('n'==r){
                char buf='0';
                write(cfdSfd.first,&buf,1);
                printf("已发送不同意对方悔棋通知\n");
                isAcceptRequest=0;
            } else{
                printf("输入有误，请重新输入(y/n)\n");
                isAcceptRequest=1;
            }
        }
    }
    close(cfdSfd.first);
    close(cfdSfd.second);
    pthread_exit(NULL);
}
pair<int,int> tcpAcceptCfd(){
    //1.创建套接字
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        perror("socket");
        return make_pair(-1,-1);
    }
    //2.绑定
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(9997);
    addr.sin_addr.s_addr=INADDR_ANY;
    int ret=bind(sockfd,(struct sockaddr*)(&addr),sizeof(addr));
    if(ret<0){
        perror("bind");
        return make_pair(-1,-1);
    }
    //3.监听
    ret=listen(sockfd,5);
    if(ret<0){
        perror("listen");
        return make_pair(-1,-1);
    }
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