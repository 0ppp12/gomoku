#include "Checkerboard.h"
#include <iostream>
#include<vector>

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
void Checkerboard::gomoku_show(){
	int i,j;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			printf("%c ",gomoku[i][j]);
		}
		printf("\n");
	}printf("\n");
}
Checkerboard::Checkerboard(){
	//初始化棋盘为'_'
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			gomoku[i][j]='_';
		}
	}
}
//落子合法判断：返回值：非0为合法、0不合法
int Checkerboard::isDropLegal(int x,int y){
	if(x<0||x>=N||y<0||y>=N){
		printf("%d,%d落子位置非法，合法坐标范围为（0-12）\n",x,y);
		return 0;
	}
	if(gomoku[x][y]!='_'){
		printf("%d,%d已被%c占\n",x,y,gomoku[x][y]);
		return 0;
	}
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




//发送输赢信息{object:xxx,result:win/lose}
// int Checkerboard::Sendwin(){
// 	if(flagWin[0]=='W')		//确认棋手身份
// 	{
// 		player_id="white";
// 	} else if(flagWin[0]=='B'){
// 		player_id="black";
// 	}
// 	std::string temp1="{object:";
// 	std::string temp2=",result:win}";
// 	winbuffer_send=temp1+player_id+temp2;//打包协议：{object:xxx,result:win}
// 	send(sockfd,winbuffer_send.c_str(),winbuffer_send.size(),0);
// }

// //接收本地玩家落子信息
// int Checkerboard::Localplayer(int _x,int _y){
// 	x = _x;
// 	y = _y;
// }
/* //接收玩家落子信息	{way:down,local:(x,y),color:black}
int Checkerboard::Receive(){
	//接收数据
	int ret=read(sockfd,(void*)recvbuffer.c_str(),128);
	if(ret<=0){
		perror("read");
	}
	//解析接收到的数据
	sscanf(recvbuffer.c_str(),"{way:down,local:(%d,%d),color:%s}",&opponent_x,&opponent_y,opponent_id.c_str());
	std::cout<<"local:("<<opponent_x<<','<<opponent_y<<')'<<std::endl;
	//将数据更新到棋盘数组
	if(opponent_id.c_str()=="black"){
		gomoku[opponent_x][opponent_y]='B';
	} else if(opponent_id.c_str()=="white"){
		gomoku[opponent_x][opponent_y]='W';
	}
	//打印接收到的数据
	std::cout<<"recver :  "<<recvbuffer.c_str()<<std::endl;
} */