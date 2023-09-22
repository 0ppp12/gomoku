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

Checkerboard::Checkerboard(){
	//初始化棋盘为'_'
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			gomoku[i][j]='_';
		}
	}
}

int Checkerboard::checkWin(){
	int i,j,cou1=0;
	for(i=y+1;i<N;i++)//'*'落子位置正下方
		if(gomoku[x][i]==flagWin[0])
			cou1++;
		else break;
	for(i=y-1;i>0;i--)//正上方
		if(gomoku[x][i]==flagWin[0])
			cou1++;
		else break;
	if(cou1>=4) return 1;//竖
	cou1=0;

	for(i=x+1;i<N;i++)//正右方
		if(gomoku[i][y]==flagWin[0])
			cou1++;
		else break;
	for(i=x-1;i>0;i--)//正左方
		if(gomoku[i][y]==flagWin[0])
			cou1++;
		else break;
	if(cou1>=4) return 1;//横5
	cou1=0;

	for(i=x+1,j=y+1;i<N,y<N;i++,j++)//右下
		if(gomoku[i][j]==flagWin[0])
			cou1++;
		else break;
	for(i=x-1,j=y-1;i>0,j>0;i--,j--)//左上
		if(gomoku[i][j]==flagWin[0])
			cou1++;
		else break;
	if(cou1>=4) return 1;//  \5
	cou1=0;

	for(i=x-1,j=y+1;i>0,j<N;i--,j++)//左下
		if(gomoku[i][j]==flagWin[0])
			cou1++;
		else break;
	for(i=x+1,j=y-1;i<N,j>0;i++,j--)//右上
		if(gomoku[i][j]==flagWin[0])
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

//接收本地玩家落子信息
int Checkerboard::Localplayer(int _x,int _y){
	x = _x;
	y = _y;
}

//发送输赢信息{object:xxx,result:win/lose}
int Checkerboard::Sendwin(){    

	if (flagWin[0] == '*')		//确认棋手身份
	{
		player_id = "white";
	}
	else if (flagWin[0] == '0')
	{
		player_id = "black";
	}
	
	std::string temp1 = "{object:";

	std::string temp2 = ",result:win}";

	winbuffer_send = temp1 + player_id + temp2;//打包协议：{object:xxx,result:win}

	send(sockfd, winbuffer_send.c_str(), winbuffer_send.size(), 0);
}

//接收玩家落子信息	{way:down,local:(x,y),color:black}
int Checkerboard::Receive(){
    
	//接收数据
    int ret = read(sockfd, (void*)recvbuffer.c_str(), 128);
    if(ret <= 0)
    {
        perror("read");
    }

	//解析接收到的数据
	sscanf(recvbuffer.c_str(), "{way:down,local:(%d,%d),color:black}",&opponent_x, &opponent_y);
	std::cout<< "local:("<< opponent_x << ',' << opponent_y << ')' <<std::endl;

	//打印接收到的数据
	std::cout<< recvbuffer.c_str() <<std::endl;
}


//触摸屏的初始化
TsDevice::TsDevice(std::string devname)
{
	//1.打开触摸屏并保存文件描述符   
	this->ts_fd = open(devname.c_str(),O_RDWR);
	if(-1 == this->ts_fd)
	{
		perror("open touch screen error");
		exit(0);
	}

	printf("touch screen init is OK!\n");
}

//单例
TsDevice& TsDevice::TsInstance(){
    static TsDevice Ts;
    return Ts;
}

//获取一次手指坐标
int TsDevice::TS_GetVal()
{
	//获取输入事件的信息并存储到指定类型的空间
	struct input_event ts_event;
	
	while(1)
	{
		read(ts_fd,&ts_event,sizeof(ts_event));

		//分析获取到的输入设备的参数信息：类型、编码、数值
		if (ts_event.type == EV_ABS && ts_event.code == ABS_X)
		{
			
			ts_x = ts_event.value*800/1024; //存储X轴坐标，该句需要选择性修改，新版触摸屏必须转换	
		}

		if (ts_event.type == EV_ABS && ts_event.code == ABS_Y)
		{
			
			ts_y = ts_event.value*480/600; //存储Y轴坐标，该句需要选择性修改，新版触摸屏必须转换	
		}

		//当用户的手指离开触摸屏，则把坐标返回
		if(ts_event.type == EV_KEY && ts_event.code == BTN_TOUCH && ts_event.value == 0) 
		{
			printf("x=%d,y=%d\n",ts_x,ts_y);
			break;
		}
	}
	
	return 0;
}
