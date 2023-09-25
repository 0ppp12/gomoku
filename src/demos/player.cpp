#include "player.h"
#include "checkerboard.h"
#include "touchscreen.h"

int Player::recvSendDropRequest(int serverSockfd){
    Checkerboard b;//棋盘类对象
    TsDevice& TS = TsDevice::TsInstance();
    //创建触摸屏线程


    char recvbuf[128];
    char sendbuf[128];
    char colorname[128];
    char opponentcolor;
    int x,y;
    int winflag=0;
    if(this->color=='B'){
        memset(recvbuf,0,sizeof(recvbuf));
        read(serverSockfd,recvbuf,128);// 接收落子通知
        memset(sendbuf,0,sizeof(sendbuf));
        while(1){
            printf("请输入棋子坐标(x,y)\n");
            scanf("%d,%d",&x,&y);
            if(b.isDropLegal(x,y)){
                b.gomoku[x][y]=this->color;
                if(b.checkWin(x,y,this->color)){
                    printf("我赢了\n");
                }
                break;
            }
        }b.gomoku_show();
        //将x,y转化为网络字节序
        x=htons(x);
        y=htons(y);
        sprintf(sendbuf,"{way:down,local:(%d,%d),color:black}",x,y);
        write(serverSockfd,sendbuf,strlen(sendbuf));//发送落子请求
    }
    while(1){
        memset(recvbuf,0,sizeof(recvbuf));
        memset(colorname,0,sizeof(colorname));
        read(serverSockfd,recvbuf,38);//接收对手落子情况,(之所以设置为38是因为要联系读2次，读多了下面的read就没的读了)
        sscanf(recvbuf,"{way:down,local:(%d,%d),color:%[^}]}",&x,&y,colorname);
        if(strstr(colorname,"white")){
            opponentcolor='W';
        }
        else if(strstr(colorname,"black")){
            opponentcolor='B';
        }
        //将x,y转化为本地字节序
        x=ntohs(x);
        y=ntohs(y);
        // printf("%d,%d\n",x,y);
        b.gomoku[x][y]=opponentcolor;//画对手棋子
        if(b.checkWin(x,y,opponentcolor)){
            printf("我输了\n");
            break;
        }b.gomoku_show();
        memset(recvbuf,0,sizeof(recvbuf));
        read(serverSockfd,recvbuf,128);//接收落子通知
        memset(sendbuf,0,sizeof(sendbuf));
        while(1){
            printf("请输入棋子坐标(x,y)\n");
            scanf("%d,%d",&x,&y);
            if(b.isDropLegal(x,y)){
                b.gomoku[x][y]=this->color;
                if(b.checkWin(x,y,this->color)){//{object:xxx,result:win/lose}
                    printf("我赢了\n");
                    winflag=1;
                    /***************9月25日14:47修改***增加向服务器发送胜利信息************/
                    sprintf(sendbuf,"{object:%s,result:win}",this->color);
                    write(serverSockfd,sendbuf,strlen(sendbuf));//发送落子请求
                    memset(sendbuf,0,sizeof(sendbuf));
                }
                break;
            }
        }b.gomoku_show();

        if(winflag != 1){
            //将x,y转化为网络字节序
            x=htons(x);
            y=htons(y);
            if(this->color=='W'){
                sprintf(sendbuf,"{way:down,local:(%d,%d),color:white}",x,y);
            } else if(this->color=='B'){
                sprintf(sendbuf,"{way:down,local:(%d,%d),color:black}",x,y);
            }
            write(serverSockfd,sendbuf,strlen(sendbuf));//发送落子请求  
        }
        //if(winflag)break;//如果我赢了也结束
    }
}
int Player::sendGetColorRequest(int serverSockfd){
    char buffer[128]={0};
    strcpy(buffer,"请求告知先后手身份");
    send(serverSockfd,buffer,sizeof(buffer),0);//!发送 请求
    // char color;
    read(serverSockfd,&this->color,1);//!接收 请求处理结果
    if(this->color=='B'){
        printf("我是先手 color=B\n");
        this->color='B';
    } else if(color=='W'){
        printf("我是后手 color=W\n");
        this->color='W';
    }
    return color;
}
int Player::getServerSockfd(){
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
    seraddr.sin_addr.s_addr=inet_addr(SERVERIP);//ip地址
    int ret=connect(sockfd,(struct sockaddr*)&seraddr,sizeof(seraddr));
    if(ret<0){
        perror("connect");
        return -1;
    }
    this->serverSockfd=sockfd;
    return sockfd;
}
int Player::sendRetractRequest(int serverSockfd){
    char buffer[128]={0};
    strcpy(buffer,"I want retract");
    printf("I want retract %d\n",send(serverSockfd,buffer,sizeof(buffer),0));//!发送悔棋请求
    char c;
    read(serverSockfd,&c,1);//!接收悔棋请求处理结果
    if(c=='1'){
        cout<<"对方同意悔棋请求"<<endl;
    } else{
        cout<<"对方不同意悔棋请求"<<endl;
    }
}
int Player::signup(){//注册
    //提示用户填写注册信息
    //提交注册信息
    //接收服务端审核注册结果
    //提示用户注册结果
};
int Player::signin(){//登陆

};


void ts_geting(){

}