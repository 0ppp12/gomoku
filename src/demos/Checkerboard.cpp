#include "Checkerboard.h"
#include <iostream>
#include<vector>

int Checkerboard::checkWin(char flagWin){
	int i,j,cou1=0;
	for(i=y+1;i<N;i++)//'*'落子位置正下方
		if(gomoku[x][i]==flagWin)
			cou1++;
		else break;
	for(i=y-1;i>0;i--)//正上方
		if(gomoku[x][i]==flagWin)
			cou1++;
		else break;
	if(cou1>=4) return 1;//竖
	cou1=0;

	for(i=x+1;i<N;i++)//正右方
		if(gomoku[i][y]==flagWin)
			cou1++;
		else break;
	for(i=x-1;i>0;i--)//正左方
		if(gomoku[i][y]==flagWin)
			cou1++;
		else break;
	if(cou1>=4) return 1;//横5
	cou1=0;

	for(i=x+1,j=y+1;i<N,y<N;i++,j++)//右下
		if(gomoku[i][j]==flagWin)
			cou1++;
		else break;
	for(i=x-1,j=y-1;i>0,j>0;i--,j--)//左上
		if(gomoku[i][j]==flagWin)
			cou1++;
		else break;
	if(cou1>=4) return 1;//  \5
	cou1=0;

	for(i=x-1,j=y+1;i>0,j<N;i--,j++)//左下
		if(gomoku[i][j]==flagWin)
			cou1++;
		else break;
	for(i=x+1,j=y-1;i<N,j>0;i++,j--)//右上
		if(gomoku[i][j]==flagWin)
			cou1++;
		else break;
	if(cou1>=4) return 1;// /5
	return 0;//胜负未定
}

//落子合法判断：返回值：0为合法、1和2为不合法
int Checkerboard::isDropLegal(){
    if(x<1||x>N-1||y<1||y>N-1){
        return 1;
    }
    if(gomoku[x][y]!='_'){
        return 2;
    }
    return 0;
}