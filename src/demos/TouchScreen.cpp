#include <iostream>
#include "TouchScreen.h"

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
