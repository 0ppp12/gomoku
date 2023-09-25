#pragma once

const int BOARD_SIZE = 13; //棋盘大小13*13

class SCOREMAP{

public:
    int socreCur;//当前位置的得分
    int rowPos, colPos;
    int bestPoint[32][3];
    int scoreMap[BOARD_SIZE][BOARD_SIZE];
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
    
    void calculateScore(int chessboard[BOARD_SIZE][BOARD_SIZE], SCOREMAP & sm, int playerChess, int aiChess);//对棋盘各点位评分，生成评分表
    
    void scoremapClear(int socremap[BOARD_SIZE][BOARD_SIZE]);//清空评分表

    void chessboard_copy(int chessboard_c[BOARD_SIZE][BOARD_SIZE],int chessboard[BOARD_SIZE][BOARD_SIZE]);//复制当前棋盘，用于AI测试落子

    COORDINATE deepSearch(int chessboard[BOARD_SIZE][BOARD_SIZE],int playerChess, int aiChess,int n,int row,int col);

    int deepSecond(SCOREMAP sm, int chessmapCopied[BOARD_SIZE][BOARD_SIZE], int row, int col, int playerChess, int aiChess);

    //获取AI坐标接口,参数列表(棋盘二位数组， 玩家棋型， AI棋型， 0, 玩家上一步棋的X坐标， 玩家上一步棋的y坐标)
    COORDINATE chessdownbyAI(int chessboard[BOARD_SIZE][BOARD_SIZE],int playerChess, int aiChess,int n,int row,int col);

};