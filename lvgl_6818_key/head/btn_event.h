#pragma once 
#include <iostream>
using namespace std;

#include "../head/client.h"
#include "../lvgl/lvgl.h"



#define DISP_BUF_SIZE (128 * 800)   //显示缓冲区
#define BLACKGRAND_PATH "S:/lzktemp/blackgrand.jpg"  //开始界面背景
#define BLACKGRAND_PATH2 "S:/lzktemp/bg.jpg"        //登陆注册界面背景
#define KETPAN_PATH "S:/lzktemp/Pan.jpg" //下棋界面棋盘
#define KETPAN_BG_PATH "S:/lzktemp/Pan_bg.jpg" //下棋界面棋盘背景
#define PLAYER1_PATH "S:/lzktemp/player1.jpg" //下棋选手_1_
#define PLAYER2_PATH "S:/lzktemp/player2.jpg" //下棋选手_2_
#define LOGIN 1     //登陆
#define REGISTER 0  //注册


 /*按钮start 触发事件声明*/
void start_game(lv_event_t * e);  

/*按钮exit 触发事件声明*/
void exit_game(lv_event_t * e); 

/*按钮注册 触发事件声明 */     
void register_funtion(lv_event_t * e);  

/*按钮登陆 触发事件声明*/
void login_funtion(lv_event_t * e);  

/*按钮“勾”，触发事件声明*/
void register_funtion_deal(lv_event_t * e); 

/*按钮单机模式 触发事件声明*/
void mode_one(lv_event_t *e);         

/*按钮联网多人模式 触发事件声明*/
void mode_more(lv_event_t *e);   

/*文本点击，触发键盘输入函数声明*/
void ta_event_cb(lv_event_t * e);  

/**/
