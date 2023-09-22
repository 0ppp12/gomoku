#pragma once

#define N 15 //棋盘规模

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
    int checkWin(char flagWin);

    //落子合法判断：返回值：0为合法、1和2为不合法
    int isDropLegal();
    
    //接收服务器落子信息
    //int receiv();

    //向服务器发送落子信息
    //int send();

    //显示键盘

    //显示棋子

    int x;              //当前落子位置x
    int y;              //当前落子位置y

private:

    char gomoku[N][N];  //棋盘数组
    char flagWin;       //黑棋手、白棋手标志位'0'白棋--'*'黑棋
};