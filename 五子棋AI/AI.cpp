#include <iostream>
#include <ctime>
#include <cstdlib>
#include "AI.h"

using namespace std;

//全局搜索
COORDINATE AI::deepSearch(char chessboard[BOARD_SIZE][BOARD_SIZE],char playerChess, char aiChess,int n,int row,int col)
{
	COORDINATE  cor;//用于接受AI计算后返回的坐标
    
    SCOREMAP scoreMap;//用于计算棋盘各点位权重

    AI ai;

    int score_max=0, score=0,score_min=10000000,score_2=0;
	int m=0;
	
    char chessmapCopied[BOARD_SIZE][BOARD_SIZE] = { 0 };
	
    ai.chessboard_copy(chessmapCopied, chessboard);
	
    SCOREMAP scTemp;//第一层棋盘权重表
    ai.calculateScore(chessmapCopied, scTemp, playerChess, aiChess);//计算第一层权重
	
    //由上一步为中心向四周发散搜索，寻找权重最高值
	for (int i = row; i < BOARD_SIZE; i++) {
		for (int j = col; j < BOARD_SIZE; j++) {
			if (chessmapCopied[i][j] == 0)
			{
				score = scTemp.scoreMap[i][j];
				scoreMap.scoreMap[i][j] = score;
				if (score > score_max) {
					score_max = score;
				}
			}
			else {
				scoreMap.scoreMap[i][j] =chessmapCopied[i][j];
			}
		}
	}  //右上
	for (int i = row; i < BOARD_SIZE; i++) {
		for (int j = col; j >= 0; j--) {
			if (chessmapCopied[i][j] == 0)
			{
				score = scTemp.scoreMap[i][j];
				scoreMap.scoreMap[i][j] = score;
				if (score > score_max) {
					score_max = score;
				}
			}
			else {
				scoreMap.scoreMap[i][j] = chessmapCopied[i][j];
			}
		}
	}
	for (int i = row; i >= 0; i--) {
		for (int j = col; j >= 0; j--) {
			if (chessmapCopied[i][j] == 0)
			{
				score = scTemp.scoreMap[i][j];
				scoreMap.scoreMap[i][j] = score;
				if (score > score_max) {
					score_max = score;
				}
			}
			else {
				scoreMap.scoreMap[i][j] = chessmapCopied[i][j];
			}
		}
	}
	for (int i = row; i >= 0; i--) {
		for (int j = col; j < BOARD_SIZE; j++) {
			if (chessmapCopied[i][j] == 0)
			{
				score = scTemp.scoreMap[i][j];
				scoreMap.scoreMap[i][j] = score;
				if (score > score_max) {
					score_max = score;
				}
			}
			else {
				scoreMap.scoreMap[i][j] = chessmapCopied[i][j];
			}
		}
	}
	
	//将搜索评价表的最大值放入到该棋盘结构体中的best数组中，用于后面的深层搜索
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (scTemp.scoreMap[i][j] == score_max) {
				scoreMap.bestPoint[m][0] = i;
				scoreMap.bestPoint[m][1] = j;
				scoreMap.bestPoint[m][2] = score_max;
				m++;
			}
		}
	}
	m--;
	//判断二层搜索结果，将第一层相等的点位对第二层进行搜索，然后取第二层的min值。
	while (m!=-1) {
		score_2=ai.deepSecond(scoreMap,chessmapCopied,scoreMap.bestPoint[m][0], scoreMap.bestPoint[m][1], playerChess, aiChess);
		if (score_2 < score_min) {
			score_min = score_2;
			scoreMap.rowPos = scoreMap.bestPoint[m][0];
			scoreMap.colPos = scoreMap.bestPoint[m][1];
		}
		m--;
	}
	cor.row = scoreMap.rowPos;
    cor.col = scoreMap.colPos;

    return cor;
}

int AI::deepSecond(SCOREMAP sm, char chessmapCopied[BOARD_SIZE][BOARD_SIZE], int row, int col, char playerChess, char aiChess)
{
    int m = 0;
	
    AI ai;//用于调用AI里面的方法

    SCOREMAP second;
    second.next = new SCOREMAP;//第二次权值计算的评分表
    
    char chessMap[BOARD_SIZE][BOARD_SIZE] = { 0 };
    ai.chessboard_copy(chessMap, chessmapCopied); //再次复制AI用于试下子的棋盘
	
    chessMap[row][col] = aiChess;//将AI目标子下到试用棋盘
	
    ai.calculateScore(chessMap, second, playerChess, aiChess);//再次计算权值
	
	second.next->socreCur = 0;//先清空本次计算的权值

    //开始对比第一次计算权值的表，若第二次高于第一次则将第一次的值赋予到第一次的表中
    for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			second.next->scoreMap[i][j] = sm.scoreMap[i][j];
			if (sm.scoreMap[i][j] > second.next->socreCur) {
				second.next->socreCur = sm.scoreMap[i][j];
			}
		}
	}
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (sm.scoreMap[i][j] == second.next->socreCur) {
				second.next->bestPoint[m][0] = i;
				second.next->bestPoint[m][1] = j;
				second.next->bestPoint[m][2] = second.next->socreCur;
				m++;
			}
		}
	}
	return second.next->socreCur;
}

void AI::chessboard_copy(char chessboard_c[BOARD_SIZE][BOARD_SIZE],char chessboard[BOARD_SIZE][BOARD_SIZE])
{	
	//复制棋盘，用于试下，不破坏原有棋盘
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			chessboard_c[i][j] = chessboard[i][j];
		}
	}
}

//清空权重表
void AI::scoremapClear(char socremap[BOARD_SIZE][BOARD_SIZE])
{
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			socremap[row][col] = 0;
		}
	}
}

//评分计算各个点的价值
void AI::calculateScore(char chessboard[BOARD_SIZE][BOARD_SIZE], SCOREMAP & sm, char playerChess, char aiChess)
{
	int personNum = 0; //玩家方（黑棋）多少个连续的棋子
	int aiNum = 0; //系统方（白棋）连续有多少个连续的棋子
	int emptyNum = 0; // 该方向上空白位的个数
 
    AI ai;
    
	// 评分数组清零 
	ai.scoremapClear(sm.scoreMap);
 
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {                            
			//对每个点进行计算
			if (chessboard[row][col]) { continue; } //避开已经下过的点 -1、1都是true
 
			//控制方向 一共4个正方向：y不变 x+、y- x-、y- x不变、y- x+
			for (int y = -1; y <= 0; y++) {        //Y的范围是-1,0
				for (int x = -1; x <= 1; x++) {    //X的范围是 -1,0,1
					if (y == 0 && x == 0) { continue; }
					if (y == 0 && x != 1) { continue; } //当y=0时，仅允许x=1
 
					personNum = 0;
					aiNum = 0;
					emptyNum = 0;
 
					// 假设黑棋在该位置落子，会构成什么棋型
					for (int i = 1; i <= 4; i++) {
						int curRow = row + i * y;
						int curCol = col + i * x;
 
						if (curRow >= 0 && curRow < BOARD_SIZE &&
							curCol >= 0 && curCol < BOARD_SIZE &&
							chessboard[curRow][curCol] == playerChess) {
							personNum++;
						}
						else if (curRow >= 0 && curRow < BOARD_SIZE &&
							curCol >= 0 && curCol < BOARD_SIZE &&
							chessboard[curRow][curCol] == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}
 
					// 反向继续计算
					for (int i = 1; i <= 4; i++) {
						int curRow = row - i * y;
						int curCol = col - i * x;
 
						if (curRow >= 0 && curRow < BOARD_SIZE &&
							curCol >= 0 && curCol < BOARD_SIZE &&
							chessboard[curRow][curCol] == playerChess) {
							personNum++;
						}
						else if (curRow >= 0 && curRow < BOARD_SIZE &&
							curCol >= 0 && curCol < BOARD_SIZE &&
							chessboard[curRow][curCol] == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}
 
					if (personNum == 1) { //连2
						sm.scoreMap[row][col] += 10;
					}
					else if (personNum == 2) {
						if (emptyNum == 1) { //死3
							sm.scoreMap[row][col] += 30;
						}
						else if (emptyNum == 2) { //活3
							sm.scoreMap[row][col] += 40;
						}
					}
					else if (personNum == 3) {
						if (emptyNum == 1) { //死4
							sm.scoreMap[row][col] = 60;
						}
						else if (emptyNum == 2) { //活4
							sm.scoreMap[row][col] = 5000;
						}
					}
					else if (personNum == 4) { //连5
						sm.scoreMap[row][col] = 20000;
					}
 
                    
                    emptyNum = 0;
					// 假设白棋在该位置落子，会构成什么棋型 
					for (int i = 1; i <= 4; i++) {
						int curRow = row + i * y;
						int curCol = col + i * x;
 
						if (curRow >= 0 && curRow < BOARD_SIZE &&
							curCol >= 0 && curCol < BOARD_SIZE &&
							chessboard[curRow][curCol] == aiChess) {
							aiNum++;
						}
						else if (curRow >= 0 && curRow < BOARD_SIZE &&
							curCol >= 0 && curCol < BOARD_SIZE &&
							chessboard[curRow][curCol] == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}
 
					for (int i = 1; i <= 4; i++) {
						int curRow = row - i * y;
						int curCol = col - i * x;
 
						if (curRow >= 0 && curRow < BOARD_SIZE &&
							curCol >= 0 && curCol < BOARD_SIZE &&
							chessboard[curRow][curCol] == aiChess) {
							aiNum++;
						}
						else if (curRow >= 0 && curRow < BOARD_SIZE &&
							curCol >= 0 && curCol < BOARD_SIZE &&
							chessboard[curRow][curCol] == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}
 
					if (aiNum == 0) {
						sm.scoreMap[row][col] += 5;
					}
					else if (aiNum == 1) { //连2
						sm.scoreMap[row][col] += 10;
					}
					else if (aiNum == 2) {
						if (emptyNum == 1) { //死3
							sm.scoreMap[row][col] += 25;
						}
						else if (emptyNum == 2) { //活3
							sm.scoreMap[row][col] += 50;
						}
					}
					else if (aiNum == 3) {
						if (emptyNum == 1) { //死4
							sm.scoreMap[row][col] += 55;
						}
						else if (emptyNum == 2) { //活4
							sm.scoreMap[row][col] += 10000;
						}
					}
					else if (aiNum >= 4) { //连5
						sm.scoreMap[row][col] += 30000;
					}
				}
			}
		}
	}
}

void showmap(char map[13][13])
{
    cout<<"\t";
    for (int i = 0; i < BOARD_SIZE; i++)
        cout<<i<<"\t";
    cout<<endl;
    
    for (int row = 0; row < BOARD_SIZE; row++)
    {   
        cout<<row<<"\t";
        for (int col = 0; col < BOARD_SIZE; col++)            
            cout<<map[row][col]<<"\t";
        cout<<endl;
    }
    cout<<"-------------"<<endl;
}

COORDINATE AI::chessdownbyAI(char chessboard[BOARD_SIZE][BOARD_SIZE],char playerChess, char aiChess,int n,int row,int col)
{
    COORDINATE coordinate;
    srand(time(0));
    for (int row = 0; row < BOARD_SIZE; row++)
        for (int col = 0; col < BOARD_SIZE; col++)
            if (chessboard[row][col] != 0 )
                coordinate  = deepSearch(chessboard,playerChess, aiChess, n, row, col);
            else
            {
                coordinate.row = rand()%12;
                coordinate.col = rand()%12;
            }
    return coordinate;  
}


int main(void)
{
    int x = 0,y = 0;
    
    char map[13][13] = { 0 };

    AI ai;

    COORDINATE cor;

    while (1)
    {
        
        scanf("%d,%d", &x, &y);
        map[x][y] = 1;
        cor = ai.deepSearch(map, 1, -1, 0, x, y);
        cout<<"*******"<<endl;
        cout<<cor.row<<","<<cor.col<<endl;
        map[cor.row][cor.col] = -1;
        cout<<"*******"<<endl;
        showmap(map);
    }

    return 0;
}