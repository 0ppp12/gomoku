#include "../head/checkerboard.h"
#include <iostream>
#include<vector>
#include <thread>

extern "C"
{
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>	
}
using namespace std;
void Checkerboard::gomoku_show(){
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
Checkerboard::Checkerboard(){
	//初始化棋盘为'_'
	memset(gomoku,'_',sizeof(gomoku));
}
/*****************************9月26日修改***新增参数 char color ********************************/
//落子合法判断：返回值：非0为合法、0不合法
int Checkerboard::isDropLegal(int x,int y,char color ){
	std::cout<<"isDropLegal:("<<x<<","<<y<<")"<<std::endl;
	if(x<0||x>=N||y<0||y>=N){
		printf("%d,%d落子位置非法，合法坐标范围为（0-12）\n",x,y);
		return 0;
	}
	if(gomoku[x][y]!='_'){
		printf("%d,%d已被%c占\n",x,y,gomoku[x][y]);
		return 0;
	}
	gomoku[x][y] = color;
	return 1;
}

int Checkerboard::checkWin(int x,int y,char color){
	int i,j,cou1=0;

	for(i=y+1;i<N;i++)//'*'落子位置正下方
		if(gomoku[x][i]==color)
			cou1++;
		else break;
	for(i=y-1;i>=0;i--)//正上方
		if(gomoku[x][i]==color)
			cou1++;
		else break;
	if(cou1>=4) return 1;//竖
	cou1=0;

	for(i=x+1;i<N;i++)//正右方
		if(gomoku[i][y]==color)
			cou1++;
		else break;
	for(i=x-1;i>=0;i--)//正左方
		if(gomoku[i][y]==color)
			cou1++;
		else break;
	if(cou1>=4) return 1;//横5
	cou1=0;

	for(i=x+1,j=y+1;i<N,y<N;i++,j++)//右下
		if(gomoku[i][j]==color)
			cou1++;
		else break;
	for(i=x-1,j=y-1;i>=0,j>0;i--,j--)//左上
		if(gomoku[i][j]==color)
			cou1++;
		else break;
	if(cou1>=4) return 1;//  \5
	cou1=0;

	for(i=x-1,j=y+1;i>=0,j<N;i--,j++)//左下
		if(gomoku[i][j]==color)
			cou1++;
		else break;
	for(i=x+1,j=y-1;i<N,j>0;i++,j--)//右上
		if(gomoku[i][j]==color)
			cou1++;
		else break;
	if(cou1>=4) return 1;// /5
	return 0;//胜负未定
}
