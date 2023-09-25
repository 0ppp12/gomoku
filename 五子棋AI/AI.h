#pragma once

const int BOARD_SIZE = 13; //棋盘大小13*13

class SCOREMAP{

public:
    int socreCur;//当前位置的得分
    int rowPos, colPos;
    int bestPoint[32][3];
    char scoreMap[BOARD_SIZE][BOARD_SIZE];
    SCOREMAP * next;

};

class COORDINATE{

public:
    int row;
    int col;

};

class AI{

public:

    COORDINATE cor;
    
    void calculateScore(char chessboard[BOARD_SIZE][BOARD_SIZE], SCOREMAP & sm, char playerChess, char aiChess);//对棋盘各点位评分，生成评分表
    
    void scoremapClear(char socremap[BOARD_SIZE][BOARD_SIZE]);//清空评分表

    void chessboard_copy(char chessboard_c[BOARD_SIZE][BOARD_SIZE],char chessboard[BOARD_SIZE][BOARD_SIZE]);//复制当前棋盘，用于AI测试落子

    COORDINATE deepSearch(char chessboard[BOARD_SIZE][BOARD_SIZE],char playerChess, char aiChess,int n,int row,int col);

    int deepSecond(SCOREMAP sm, char chessmapCopied[BOARD_SIZE][BOARD_SIZE], int row, int col, char playerChess, char aiChess);

    //获取AI坐标接口,参数列表(棋盘二位数组， 玩家棋型， AI棋型， 0, 玩家上一步棋的X坐标， 玩家上一步棋的y坐标)
    COORDINATE chessdownbyAI(char chessboard[BOARD_SIZE][BOARD_SIZE],char playerChess, char aiChess,int n,int row,int col);

};