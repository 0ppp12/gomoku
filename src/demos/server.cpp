#include "./server.h"
int Server::forwardHeiBaiRequest(int heifd,int baifd){
    char sendbuf[128];
    memset(sendbuf,0,128);
    strcpy(sendbuf,"该你落子了");
    write(heifd,sendbuf,strlen(sendbuf));//!发送先手落子通知给黑子
    char recvbuf[128];
    memset(recvbuf,0,128);
    int r=read(heifd,recvbuf,sizeof(recvbuf));//!接收黑子落子请求
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
        write(baifd,sendbuf,strlen(sendbuf));//!发送落子通知给白子 
        memset(recvbuf,0,128);
        read(baifd,recvbuf,128);//!接收白子落子请求
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
        read(heifd,recvbuf,128);//!接收黑子落子请求
        sscanf(recvbuf,"{way:down,local:(%d,%d)",&x,&y);
        //将x,y转化为本地字节序
        x=ntohs(x);
        y=ntohs(y);
        printf("%d,%d\n",x,y);
    }
}
pair<int,int> Server::sendSetColor(){
    int sockfd=tcpInit();
    int cfd=Server::tcpAcceptCfd(sockfd);//!接收2玩家连接 
    int cfd2=Server::tcpAcceptCfd(sockfd);
    char recvbuffer[128]={0};
    char recvbuffer2[128]={0};
    int request1=read(cfd,recvbuffer,128);
    int request2=read(cfd2,recvbuffer,128);
    int heifd,baifd;
    if(request1>0&&request1>0){//!接收2个先后手身份请求 
        if(random()%2){//!随机先2玩家发送身份   
            char color='W';
            write(cfd,&color,1);
            baifd=cfd;
            color='B';
            write(cfd2,&color,1);
            heifd=cfd2;
        } else{
            char color='B';
            write(cfd,&color,1);
            heifd=cfd;
            color='W';
            write(cfd2,&color,1);
            baifd=cfd2;
        }
    }
    return make_pair(heifd,baifd);
}
int Server::tcpAcceptCfd(int sockfd){
    //4.接受连接（默认是阻塞）
    struct sockaddr_in clientaddr;
    socklen_t len=sizeof(clientaddr);
    int clientfd=accept(sockfd,(struct sockaddr *)(&clientaddr),&len);
    if(clientfd<0){
        perror("accept");
        return -1;
    }
    printf("客户端IP:%s\n",inet_ntoa(clientaddr.sin_addr));
    return clientfd;
}
int Server::tcpInit(){
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
    this->sockfd=sockfd;
    return sockfd;
}