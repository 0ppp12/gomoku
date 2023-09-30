/*
g++ watchclienttest.cpp checkerboard.cpp&&./a.out
 */
#include "../head/client.h"
#include "../head/checkerboard.h"
#include<queue>
#define SERVER_IP "192.168.45.34"
#define PORT 9999
#define N 13
using namespace std;
void gomoku_show(char gomoku[N][N]){
    cout<<"    ";
    for(int i=0; i<N; i++){
        if(i<10){
            cout<<i<<"  ";
        } else{
            cout<<i<<" ";
        }
    }
    cout<<endl;
    for(int row=0; row<N; row++){
        if(row<10)cout<<" ";
        cout<<row<<"  ";
        for(int col=0; col<N; col++)
            cout<<gomoku[row][col]<<"  ";
        cout<<endl;
    }
}
int main(){
    // int server_socket;
    // struct sockaddr_in server_address;
    // server_socket=socket(AF_INET,SOCK_STREAM,0);
    // if(server_socket==-1){
    //     perror("socket");
    //     return 1;
    // }
    // server_address.sin_family=AF_INET;
    // server_address.sin_addr.s_addr=inet_addr(SERVER_IP);
    // server_address.sin_port=htons(PORT);
    // if(connect(server_socket,(struct sockaddr *)&server_address,sizeof(server_address))==-1){
    //     perror("connect");
    //     return 1;
    // }
    // write(server_socket,"way:login,account:a|password:a",strlen("way:login,account:a|password:a"));//登陆
    write(server_socket,"watch:game",strlen("watch:game"));//申请观战
    usleep(1000*1000*3);

    Checkerboard board;
    // char gomoku2[N][N];//转化后的棋盘
    // memset(gomoku2,'_',sizeof(gomoku2));
    char buf[N*N];//接收棋盘
    memset(board.gomoku,'_',sizeof(board.gomoku));
    queue<pair<int,int>> newPeices;
    int x,y;
    while(1){//接收观战数据
        memset(buf,0,sizeof(buf));
        read(server_socket,buf,N*N);
        if('_'==buf[0]||'B'==buf[0]||'W'==buf[0]){
            for(int i=0;i<N*N;i++){
                if(board.gomoku[i/N][i%N]!=buf[i]){
                    newPeices.push(make_pair(i/N,i%N));//收集新增棋子
                    board.gomoku[i/N][i%N]=buf[i];
                    // printf("push_back: %c\n",buf[i]);
                }
            }
            board.gomoku_show();
            while(!newPeices.empty()){//处理新增棋子
                // printf("已处理（在lcd画出）新棋子：%d,%d\n",newPeices.front().first,newPeices.front().second);
                x=newPeices.front().first;
                y=newPeices.front().second;
                if('B'==board.gomoku[x][y]){//1黑2白
                    cout<<"黑子落子："<<x<<","<<y<<" black"<<endl;
                    if(board.checkWin(x,y,'B')){
                        printf("对方赢了\n");
                    }
                    x=x*34+166;
                    y=y*34+30;
                    // dis_key(x,y,"black");
                } else if('W'==board.gomoku[x][y]){
                    cout<<"白子落子："<<x<<","<<y<<" white"<<endl;
                    // dis_key(x,y,"white");
                    if(board.checkWin(x,y,'W')){
                        printf("白子赢了\n");
                    }
                    x=x*34+166;
                    y=y*34+30;
                }
                newPeices.pop();
            }
        }
    }
    return 0;
}