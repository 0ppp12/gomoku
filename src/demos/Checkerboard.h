#pragma once
#include <iostream>
#define N 13 //棋盘规模

class Checkerboard{
public:

    Checkerboard(){
        //初始化棋盘
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                gomoku[i][j]='_';
            }
        }
    }

    //胜负判断胜利返回1，未胜利返回0
    int checkWin();

    //落子合法判断：返回值：0为合法、1和2为不合法
    int isDropLegal();
    
    //接收本地玩家落子信息
    int Localplayer(int _x,int _y);

    //接收对方玩家落子信息
    int Receive();

    //向服务器发送输赢信息
    int Sendwin();

    //显示棋盘

    //显示棋子

public:
    int  sockfd;                    //服务器套接字
    char gomoku[N][N];              //棋盘数组
private:    

    std::string player_id;          //本机棋手身份   black  white  
    std::string opponent_id;        //对手身份
    std::string winbuffer_send;          //用于发送输赢协议字符串
    std::string winbuffer_recv;          //用于接收输赢协议字符串
    std::string recvbuffer;         //用于接收对手落子字信息字符串
    std::string flagWin;            //黑棋手、白棋手标志位'0'白棋--'*'黑棋

    int x;                  //本人落子位置x
    int y;                  //本人落子位置y

    int opponent_x = 0;     //对手落子位置
	int opponent_y = 0;     //对手落子位置
};