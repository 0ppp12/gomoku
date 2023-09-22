#pragma once

#define N 15 //棋盘规模

class Checkerboard{
public:
    //胜负判断胜利返回1，未胜利返回0
    int checkWin();

    //接收落子信息

    //发送落子信息

    int x;              //当前落子位置x
    int y;              //当前落子位置y

private:

    char gomoku[N][N];  //棋盘数组
    char flagWin;       //黑棋手、白棋手标志位
    char Playwhite;     //白棋手
    char Playblace;     //黑棋手

};