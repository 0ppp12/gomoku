#include <iostream>
using namespace std;

#include "../lvgl/lvgl.h"
//#include "lvgl/demos/lv_demos.h"
#include "../lv_drivers/display/fbdev.h"
#include "../lv_drivers/indev/evdev.h"
#include "../head/client.h"   //连接头文件
//#include "../head/btn_event.h"
#include "../head/display_key.h"
extern "C"
{
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
}
#define DISP_BUF_SIZE (128 * 800)   //显示缓冲区

Info_SendAndRev info;              //创建连接类对象
int client_socket;                 //用户链接后返回的套接字描述符


int main(int argc, char const *argv[])
{
    /*初始化lvgl*/
    lv_init();

    /*初始化设备，包含lcd ，映射*/
    fbdev_init();

    /*记录绘制屏幕的一段小缓冲区*/
    static lv_color_t buf[DISP_BUF_SIZE];

    /*初始化设备描述符*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    /*初始化lcd参数，并注册（记录）到lvgl内置结构体*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = fbdev_flush;
    disp_drv.hor_res    = 800;
    disp_drv.ver_res    = 480;
    lv_disp_drv_register(&disp_drv);

    evdev_init(); //触摸屏的初始化
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;

    /*This function will be called periodically (by the library) to get the mouse position and state*/
    indev_drv_1.read_cb = evdev_read;
    lv_indev_drv_register(&indev_drv_1);

    contanier( disp_drv);

      /*与服务端进行连接*/
    const char *IP_4 = argv[1];
    int Port =stoi(argv[2]);
    do
    {
        client_socket = info.INIT_SOCKET(IP_4,Port);
    }
    while(client_socket == 1); 
    cout<<"连接成功"<<endl;

    /*回调处理按键事件*/
    while(1) {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}

uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}
