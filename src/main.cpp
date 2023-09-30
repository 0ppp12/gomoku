#include <iostream>
using namespace std;

#include "../lvgl/lvgl.h"
//#include "lvgl/demos/lv_demos.h"
#include "../lv_drivers/display/fbdev.h"
#include "../lv_drivers/indev/evdev.h"
#include "../head/client.h"   //连接头文件
//#include "../head/btn_event.h"
//#include "../head/display_key.h"
#include "../head/touchscreen.h"
#include "../head/checkerboard.h"
#include "../head/AI.h"
#include<queue>
extern "C"
{
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
}


#define DISP_BUF_SIZE (128 * 800)   //显示缓冲区
#define BLACKGRAND_PATH  "S:/lzktemp/blackgrand.jpg"  //开始界面背景
#define BLACKGRAND_PATH2 "S:/lzktemp/bg.jpg"        //登陆注册界面背景
#define KETPAN_PATH      "S:/lzktemp/Pan.jpg" //下棋界面棋盘
#define KETPAN_BG_PATH   "S:/lzktemp/Pan_bg.jpg" //下棋界面棋盘背景
#define PLAYER1_PATH     "S:/lzktemp/player1.jpg" //下棋选手_1_
#define PLAYER2_PATH     "S:/lzktemp/player2.jpg" //下棋选手_2_
#define BLACK_KEY_PATH   "S:/lzktemp/black.jpg" //黑棋
#define WHITE_KEY_PATH   "S:/lzktemp/white.jpg" //白棋

#define LOGIN 1     //登陆
#define REGISTER 0  //注册

lv_obj_t *  contanier_start=NULL; //作为开始界面容器
lv_obj_t *  contanier_login=NULL; //作为登陆界面容器
lv_obj_t *  contanier_register=NULL; //作为注册界面
lv_obj_t *  contanier_mode=NULL; // 模式选择界面
lv_obj_t *  contanier_result=NULL; //进入房间界面
lv_obj_t *  contanier_play=NULL; //下棋界面

lv_obj_t * password;               //创建密码对象
lv_obj_t * user_name;              //创建用户名对象
lv_obj_t * password_rg;            //创建注册密码对象
lv_obj_t * password_rg_tmp;        //创建注册密码对象
lv_obj_t * user_name_rg;           //创建注册用户名对象
lv_obj_t * btn3,*btn4;             //btn3注册按钮，btn登陆按钮，为了使用键盘时隐藏该按钮，设为全局
lv_obj_t * btn_register_yes;       //注册确定按钮


string user_name_check;       //获取textarea的用户名，待发送给服务端
string password_check;        //获取textarea的密码，待发送给服务端

Info_SendAndRev info;              //创建连接类对象
int server_socket;                 //用户链接后返回的套接字描述符
char color;

TsDevice dev;                      //创建屏幕点击类对象
Checkerboard board;                //

pthread_t touch;                  //创建获取触摸屏坐标线程

void start_game(lv_event_t * e);        //按钮start 触发事件声明
void exit_game(lv_event_t * e);         //按钮exit 触发事件声明
void register_funtion(lv_event_t * e);  //按钮注册 触发事件声明
void login_funtion(lv_event_t * e);     //按钮登陆 触发事件声明
void register_funtion_deal(lv_event_t * e); //按钮“勾”，触发事件声明
void mode_one(lv_event_t *e);           //按钮单机模式 触发事件声明
void mode_more(lv_event_t *e);          //按钮联网多人模式 触发事件声明
void ta_event_cb(lv_event_t * e);       //文本点击，触发键盘输入函数声明
void dis_key(int x,int y,const char* b_or_w,lv_obj_t * obj=contanier_play);
void sig_funtion(int);                 //线程接收信号终结函数声明
TsDevice tsDevice;

void get_xy(int &x,int &y);

int main(int argc,char const *argv[]){
    /*LittlevGL init*/
    lv_init();

    /*Linux frame buffer device init*/
    fbdev_init();

    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];

    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf,buf,NULL,DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf=&disp_buf;
    disp_drv.flush_cb=fbdev_flush;
    disp_drv.hor_res=800;
    disp_drv.ver_res=480;
    lv_disp_drv_register(&disp_drv);

    evdev_init();
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type=LV_INDEV_TYPE_POINTER;

    /*This function will be called periodically (by the library) to get the mouse position and state*/
    indev_drv_1.read_cb=evdev_read;
    lv_indev_drv_register(&indev_drv_1);

    /*========================================================================================================*/
    /*创建开始界面容器*/
    contanier_start=lv_obj_create(lv_scr_act());       //基于屏幕创建一个和屏幕大小一样的容器contanier_6818
    lv_obj_set_size(contanier_start,disp_drv.hor_res,disp_drv.ver_res);       //设置到屏幕大小
    lv_obj_center(contanier_start);

    /*放出开始界面*/
    lv_obj_t *img1=lv_img_create(contanier_start);
    lv_obj_set_size(img1,disp_drv.hor_res,disp_drv.ver_res);//设置大小
    lv_obj_center(img1);
    lv_img_set_src(img1,BLACKGRAND_PATH);
    lv_obj_clear_flag(contanier_start,LV_OBJ_FLAG_SCROLLABLE);

    /*Properties to transition*/
    static lv_style_prop_t props[]={
        LV_STYLE_TRANSFORM_WIDTH,LV_STYLE_TRANSFORM_HEIGHT,LV_STYLE_TEXT_LETTER_SPACE,LV_STYLE_PROP_INV
    };

    /*Transition descriptor when going back to the default state.
     *Add some delay to be sure the press transition is visible even if the press was very short*/
    static lv_style_transition_dsc_t transition_dsc_def;
    lv_style_transition_dsc_init(&transition_dsc_def,props,lv_anim_path_overshoot,250,100,NULL);

    /*Transition descriptor when going to pressed state.
     *No delay, go to presses state immediately*/
    static lv_style_transition_dsc_t transition_dsc_pr;
    lv_style_transition_dsc_init(&transition_dsc_pr,props,lv_anim_path_ease_in_out,250,0,NULL);

    /*Add only the new transition to he default state*/
    static lv_style_t style_def;
    lv_style_init(&style_def);
    lv_style_set_transition(&style_def,&transition_dsc_def);

    /*Add the transition and some transformation to the presses state.*/
    static lv_style_t style_pr;
    lv_style_init(&style_pr);
    lv_style_set_transform_width(&style_pr,10);
    lv_style_set_transform_height(&style_pr,-10);
    lv_style_set_text_letter_space(&style_pr,10);
    lv_style_set_transition(&style_pr,&transition_dsc_pr);

    /*设置按钮边界*/
    static lv_style_t style_border;
    lv_style_init(&style_border);
    lv_style_set_border_color(&style_border,lv_color_black()); // 设置边框颜色为黑色
    lv_style_set_border_width(&style_border,2); // 设置边框宽度为2

    /*设置按钮圆形样式*/
    static lv_style_t style_circel;
    lv_style_init(&style_circel);
    lv_style_set_radius(&style_circel,50); // 设置按钮样式圆角弧度


    /*创建开始按钮*/
    lv_obj_t * btn1=lv_btn_create(contanier_start);
    lv_obj_add_style(btn1,&style_pr,LV_STATE_PRESSED);
    lv_obj_add_style(btn1,&style_def,0);
    lv_obj_add_style(btn1,&style_border,0);
    lv_obj_add_style(btn1,&style_circel,0);
    lv_obj_set_size(btn1,200,100);
    lv_style_set_bg_color(&style_pr,lv_palette_darken(LV_PALETTE_DEEP_ORANGE,2));
    lv_style_set_bg_color(&style_def,lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_center(btn1);
    lv_obj_add_event_cb(btn1,start_game,LV_EVENT_ALL,NULL); //设置开始函数

    /*设置开始标签*/
    static lv_style_t style_start_label;
    lv_style_init(&style_start_label);
    lv_obj_t * start_label=lv_label_create(btn1);
    lv_label_set_text(start_label,"S t a r t");
    lv_style_set_text_font(&style_start_label,&lv_font_montserrat_24);
    lv_obj_add_style(start_label,&style_start_label,LV_STATE_DEFAULT);
    lv_obj_center(start_label);

    /*创建退出按钮*/
    lv_obj_t * btn2=lv_btn_create(contanier_start);
    lv_obj_add_style(btn2,&style_pr,LV_STATE_PRESSED);
    lv_obj_add_style(btn2,&style_def,0);
    lv_obj_add_style(btn2,&style_border,0);
    lv_obj_add_style(btn2,&style_circel,0);
    lv_obj_set_size(btn2,200,100);
    lv_style_set_bg_color(&style_pr,lv_palette_darken(LV_PALETTE_DEEP_ORANGE,2));
    lv_style_set_bg_color(&style_def,lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn2,LV_ALIGN_CENTER,0,140);
    lv_obj_add_event_cb(btn2,exit_game,LV_EVENT_ALL,(void *)"start"); //设置退出函数

    /*创建退出标签*/
    lv_obj_t * back_label=lv_label_create(btn2);
    lv_label_set_text(back_label,"EXIT " LV_SYMBOL_CLOSE);
    lv_obj_add_style(back_label,&style_start_label,LV_STATE_DEFAULT);
    lv_obj_center(back_label);
    /*========================================================================================================*/
    /*与服务端进行连接*/
    const char *IP_4=argv[1];
    int Port=stoi(argv[2]);
    do{
        server_socket=info.INIT_SOCKET(IP_4,Port);
    } while(server_socket==1);

    cout<<"连接成功"<<endl;

    /*=========================================================================================================*/
    /*创建登陆界面容器*/
    contanier_login=lv_obj_create(lv_scr_act());       //基于屏幕创建一个和屏幕大小一样的容器contanier_6818
    lv_obj_set_size(contanier_login,disp_drv.hor_res,disp_drv.ver_res);       //设置到屏幕大小
    lv_obj_center(contanier_login);

    lv_obj_add_flag(contanier_login,LV_OBJ_FLAG_HIDDEN); //设置该界面隐藏
    lv_obj_clear_flag(contanier_login,LV_OBJ_FLAG_SCROLLABLE);//设置禁止滑条

    /*放出开始界面*/
    lv_obj_t *img2=lv_img_create(contanier_login);
    lv_obj_set_size(img2,disp_drv.hor_res,disp_drv.ver_res);//设置大小
    lv_obj_center(img2);
    lv_img_set_src(img2,BLACKGRAND_PATH);

    /*放出透明登陆界面*/
    lv_obj_t *img3=lv_img_create(img2);
    lv_obj_set_size(img3,350,250);//设置大小
    lv_obj_align(img3,LV_ALIGN_CENTER,0,-20);
    lv_img_set_src(img3,BLACKGRAND_PATH2);

    /*创建键盘*/
    lv_obj_t * kb=lv_keyboard_create(contanier_login);
    lv_obj_add_flag(kb,LV_OBJ_FLAG_HIDDEN);
    cout<<"key"<<endl;

    /*设置标签样式*/
    static lv_style_t style_text_muted;
    lv_style_init(&style_text_muted);
    lv_color_t Text_color=lv_color_hex(0Xffffff);
    lv_style_set_text_font(&style_text_muted,&lv_font_montserrat_18);
    lv_style_set_text_color(&style_text_muted,Text_color);

    /*创建用户名标签*/
    lv_obj_t * user_name_label=lv_label_create(contanier_login);
    lv_label_set_text(user_name_label,"Username");
    lv_obj_add_style(user_name_label,&style_text_muted,0);
    lv_obj_align(user_name_label,LV_ALIGN_CENTER,-80,-130);

    /*创建用户名文本*/
    user_name=lv_textarea_create(contanier_login);
    lv_textarea_set_one_line(user_name,true);
    lv_obj_align(user_name,LV_ALIGN_CENTER,0,-100);
    lv_textarea_set_placeholder_text(user_name,"Your name");
    lv_obj_add_event_cb(user_name,ta_event_cb,LV_EVENT_ALL,kb);

    /*创建密码标签*/
    lv_obj_t * password_label=lv_label_create(contanier_login);
    lv_label_set_text(password_label,"Password");
    lv_obj_add_style(password_label,&style_text_muted,0);
    lv_obj_align(password_label,LV_ALIGN_CENTER,-80,-50);

    /*创建密码文本*/
    password=lv_textarea_create(contanier_login);
    lv_textarea_set_one_line(password,true);
    lv_textarea_set_password_mode(password,true);
    lv_obj_align(password,LV_ALIGN_CENTER,0,-20);
    lv_textarea_set_placeholder_text(password,"Min. 8 chars.");
    lv_obj_add_event_cb(password,ta_event_cb,LV_EVENT_ALL,kb);

    /*创建注册按钮*/
    btn3=lv_btn_create(contanier_login);
    lv_obj_add_style(btn3,&style_pr,LV_STATE_PRESSED);
    lv_obj_add_style(btn3,&style_def,0);
    lv_obj_add_style(btn3,&style_border,0);
    lv_obj_set_size(btn3,100,50);
    lv_style_set_bg_color(&style_pr,lv_palette_darken(LV_PALETTE_DEEP_ORANGE,2));
    lv_style_set_bg_color(&style_def,lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn3,LV_ALIGN_CENTER,-80,80);
    lv_obj_add_event_cb(btn3,register_funtion,LV_EVENT_ALL,NULL); //设置注册函数

    /*设置注册标签*/
    static lv_style_t style_register_label;
    lv_style_init(&style_register_label);
    lv_obj_t * register_label=lv_label_create(btn3);
    lv_label_set_text(register_label,"Register");
    lv_style_set_text_font(&style_register_label,&lv_font_montserrat_20);
    lv_obj_add_style(register_label,&style_register_label,LV_STATE_DEFAULT);
    lv_obj_center(register_label);

    /*创建登陆按钮*/
    btn4=lv_btn_create(contanier_login);
    lv_obj_add_style(btn4,&style_pr,LV_STATE_PRESSED);
    lv_obj_add_style(btn4,&style_def,0);
    lv_obj_add_style(btn4,&style_border,0);
    lv_obj_set_size(btn4,100,50);
    lv_style_set_bg_color(&style_pr,lv_palette_darken(LV_PALETTE_DEEP_ORANGE,2));
    lv_style_set_bg_color(&style_def,lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn4,LV_ALIGN_CENTER,80,80);
    lv_obj_add_event_cb(btn4,login_funtion,LV_EVENT_ALL,NULL); //设置登陆函数

    /*创建登陆标签*/
    static lv_style_t style_login_label;
    lv_style_init(&style_login_label);
    lv_obj_t * login_label=lv_label_create(btn4);
    lv_label_set_text(login_label,"Login" LV_SYMBOL_OK);
    lv_style_set_text_font(&style_login_label,&lv_font_montserrat_20);
    lv_obj_add_style(login_label,&style_login_label,LV_STATE_DEFAULT);
    lv_obj_center(login_label);

    /*创建返回按钮*/
    lv_obj_t *btn_black_login=lv_btn_create(contanier_login);
    lv_obj_add_style(btn_black_login,&style_pr,LV_STATE_PRESSED);
    lv_obj_add_style(btn_black_login,&style_def,0);
    lv_obj_add_style(btn_black_login,&style_border,0);
    lv_obj_set_size(btn_black_login,100,50);
    lv_style_set_bg_color(&style_pr,lv_palette_darken(LV_PALETTE_DEEP_ORANGE,2));
    lv_style_set_bg_color(&style_def,lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_black_login,LV_ALIGN_TOP_LEFT,0,0);
    lv_obj_add_event_cb(btn_black_login,exit_game,LV_EVENT_ALL,(void *)"login"); //设置登陆函数

    /*创建返回标签*/
    lv_obj_t * login_black=lv_label_create(btn_black_login);
    lv_label_set_text(login_black,"Black" LV_SYMBOL_NEW_LINE);
    lv_obj_add_style(login_black,&style_login_label,LV_STATE_DEFAULT);
    lv_obj_center(login_black);

    /*========================================================================================================*/
    /*创建注册界面容器*/
    contanier_register=lv_obj_create(lv_scr_act());
    lv_obj_set_size(contanier_register,disp_drv.hor_res,disp_drv.ver_res);       //设置到屏幕大小
    lv_obj_center(contanier_register);

    /*放出注册背景界面*/
    lv_obj_t *img_rg=lv_img_create(contanier_register);
    lv_obj_set_size(img_rg,disp_drv.hor_res,disp_drv.ver_res);//设置大小
    lv_obj_center(img_rg);
    lv_img_set_src(img_rg,BLACKGRAND_PATH);

    /*放出注册界面*/
    lv_obj_t *img_rg_m=lv_img_create(img_rg);
    lv_obj_set_size(img_rg_m,350,250);//设置大小
    lv_obj_align(img_rg_m,LV_ALIGN_CENTER,0,-60);
    lv_img_set_src(img_rg_m,BLACKGRAND_PATH2);

    lv_obj_add_flag(contanier_register,LV_OBJ_FLAG_HIDDEN); //设置该界面隐藏
    lv_obj_clear_flag(contanier_register,LV_OBJ_FLAG_SCROLLABLE);//设置禁止滑条

    /*创建键盘*/
    lv_obj_t * kb_rg=lv_keyboard_create(contanier_register);
    lv_obj_add_flag(kb_rg,LV_OBJ_FLAG_HIDDEN);

    /*创建用户名文本*/
    user_name_rg=lv_textarea_create(contanier_register);
    lv_textarea_set_one_line(user_name_rg,true);
    lv_obj_align(user_name_rg,LV_ALIGN_CENTER,0,-160);
    lv_textarea_set_placeholder_text(user_name_rg,"Your name");
    lv_obj_add_event_cb(user_name_rg,ta_event_cb,LV_EVENT_ALL,kb_rg);


    /*创建密码文本*/
    password_rg=lv_textarea_create(contanier_register);
    lv_textarea_set_one_line(password_rg,true);
    lv_textarea_set_password_mode(password_rg,true);
    lv_obj_align(password_rg,LV_ALIGN_CENTER,0,-90);
    lv_textarea_set_placeholder_text(password_rg,"password:Min. 8 chars.");
    lv_obj_add_event_cb(password_rg,ta_event_cb,LV_EVENT_ALL,kb_rg);

    /*创建确定密码文本*/
    password_rg_tmp=lv_textarea_create(contanier_register);
    lv_textarea_set_one_line(password_rg_tmp,true);
    lv_textarea_set_password_mode(password_rg_tmp,true);
    lv_obj_align(password_rg_tmp,LV_ALIGN_CENTER,0,-20);
    lv_textarea_set_placeholder_text(password_rg_tmp,"input again.");
    lv_obj_add_event_cb(password_rg_tmp,ta_event_cb,LV_EVENT_ALL,kb_rg);

    /*创建注册确定按钮*/
    btn_register_yes=lv_btn_create(contanier_register);
    lv_obj_add_style(btn_register_yes,&style_pr,LV_STATE_PRESSED);
    lv_obj_add_style(btn_register_yes,&style_def,0);
    lv_obj_add_style(btn_register_yes,&style_border,0);
    lv_obj_set_size(btn_register_yes,100,50);
    lv_style_set_bg_color(&style_pr,lv_palette_darken(LV_PALETTE_DEEP_ORANGE,2));
    lv_style_set_bg_color(&style_def,lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_register_yes,LV_ALIGN_CENTER,0,30);
    lv_obj_add_event_cb(btn_register_yes,register_funtion_deal,LV_EVENT_ALL,NULL); //设置注册函数

    /*设置注册标签*/
    lv_obj_t * register_label_rg=lv_label_create(btn_register_yes);
    lv_label_set_text(register_label_rg,LV_SYMBOL_OK);
    lv_obj_center(register_label_rg);

    /*创建返回按钮*/
    lv_obj_t *btn_black_register=lv_btn_create(contanier_register);
    lv_obj_add_style(btn_black_register,&style_pr,LV_STATE_PRESSED);
    lv_obj_add_style(btn_black_register,&style_def,0);
    lv_obj_add_style(btn_black_register,&style_border,0);
    lv_obj_set_size(btn_black_register,100,50);
    lv_style_set_bg_color(&style_pr,lv_palette_darken(LV_PALETTE_DEEP_ORANGE,2));
    lv_style_set_bg_color(&style_def,lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_black_register,LV_ALIGN_TOP_LEFT,0,0);
    lv_obj_add_event_cb(btn_black_register,exit_game,LV_EVENT_ALL,(void *)"register"); //设置登陆函数

    /*创建返回标签*/
    lv_obj_t * register_black=lv_label_create(btn_black_register);
    lv_label_set_text(register_black,"Black" LV_SYMBOL_NEW_LINE);
    lv_obj_add_style(register_black,&style_login_label,LV_STATE_DEFAULT);
    lv_obj_center(register_black);



    /*========================================================================================================*/
    /*游戏模式选择界面*/
    contanier_mode=lv_obj_create(lv_scr_act());       //基于屏幕创建一个和屏幕大小一样的容器contanier_6818
    lv_obj_set_size(contanier_mode,disp_drv.hor_res,disp_drv.ver_res);       //设置到屏幕大小
    lv_obj_center(contanier_mode);
    lv_obj_add_flag(contanier_mode,LV_OBJ_FLAG_HIDDEN); //设置该界面隐藏
    lv_obj_clear_flag(contanier_mode,LV_OBJ_FLAG_SCROLLABLE);//设置禁止滑条

    /*放出模式界面*/
    lv_obj_t *img_mode=lv_img_create(contanier_mode);
    lv_obj_set_size(img_mode,disp_drv.hor_res,disp_drv.ver_res);//设置棋盘大小
    lv_obj_center(img_mode);
    lv_img_set_src(img_mode,BLACKGRAND_PATH);

    /*创建观战按钮*/
    lv_obj_t *btn_look=lv_btn_create(contanier_mode);
    lv_obj_add_style(btn_look,&style_pr,LV_STATE_PRESSED);
    lv_obj_add_style(btn_look,&style_def,0);
    lv_obj_add_style(btn_look,&style_border,0);
    lv_obj_add_style(btn_look,&style_circel,0);
    lv_obj_set_size(btn_look,200,100);
    lv_style_set_bg_color(&style_pr,lv_palette_darken(LV_PALETTE_DEEP_ORANGE,2));
    lv_style_set_bg_color(&style_def,lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_look,LV_ALIGN_CENTER,0,-150);
    void mode_watch(lv_event_t*e);
    lv_obj_add_event_cb(btn_look,mode_watch,LV_EVENT_ALL,NULL);


    /*设置单机标签*/
    lv_obj_t * look=lv_label_create(btn_look);
    lv_label_set_text(look,"Watch" LV_SYMBOL_EYE_OPEN);
    lv_obj_center(look);
    lv_obj_add_style(look,&style_start_label,LV_STATE_DEFAULT);

    /*创建单机按钮*/
    lv_obj_t *btn_one=lv_btn_create(contanier_mode);
    lv_obj_add_style(btn_one,&style_pr,LV_STATE_PRESSED);
    lv_obj_add_style(btn_one,&style_def,0);
    lv_obj_add_style(btn_one,&style_border,0);
    lv_obj_add_style(btn_one,&style_circel,0);
    lv_obj_set_size(btn_one,200,100);
    lv_style_set_bg_color(&style_pr,lv_palette_darken(LV_PALETTE_DEEP_ORANGE,2));
    lv_style_set_bg_color(&style_def,lv_palette_main(LV_PALETTE_ORANGE));
    //lv_obj_center(btn_one);
    lv_obj_align(btn_one,LV_ALIGN_CENTER,0,0);
    lv_obj_add_event_cb(btn_one,mode_one,LV_EVENT_ALL,NULL); //设置注册函数

    /*设置单机标签*/
    lv_obj_t * one=lv_label_create(btn_one);
    lv_label_set_text(one,"singel");
    lv_obj_center(one);
    lv_obj_add_style(one,&style_start_label,LV_STATE_DEFAULT);


    /*创建多人按钮*/
    lv_obj_t *btn_more=lv_btn_create(contanier_mode);
    lv_obj_add_style(btn_more,&style_pr,LV_STATE_PRESSED);
    lv_obj_add_style(btn_more,&style_def,0);
    lv_obj_add_style(btn_more,&style_border,0);
    lv_obj_add_style(btn_more,&style_circel,0);
    lv_obj_set_size(btn_more,200,100);
    lv_style_set_bg_color(&style_pr,lv_palette_darken(LV_PALETTE_DEEP_ORANGE,2));
    lv_style_set_bg_color(&style_def,lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_more,LV_ALIGN_CENTER,0,150);
    lv_obj_add_event_cb(btn_more,mode_more,LV_EVENT_ALL,NULL); //设置注册函数

    /*设置多人标签*/
    lv_obj_t * more=lv_label_create(btn_more);
    lv_label_set_text(more,"multiplayer");
    lv_obj_center(more);
    lv_obj_add_style(more,&style_start_label,LV_STATE_DEFAULT);

    /*创建返回按钮*/
    lv_obj_t *btn_black_mode=lv_btn_create(contanier_mode);
    lv_obj_add_style(btn_black_mode,&style_pr,LV_STATE_PRESSED);
    lv_obj_add_style(btn_black_mode,&style_def,0);
    lv_obj_add_style(btn_black_mode,&style_border,0);
    lv_obj_set_size(btn_black_mode,100,50);
    lv_style_set_bg_color(&style_pr,lv_palette_darken(LV_PALETTE_DEEP_ORANGE,2));
    lv_style_set_bg_color(&style_def,lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_black_mode,LV_ALIGN_TOP_LEFT,0,0);
    lv_obj_add_event_cb(btn_black_mode,exit_game,LV_EVENT_ALL,(void *)"mode"); //设置登陆函数

    /*创建返回标签*/
    lv_obj_t * mode_black=lv_label_create(btn_black_mode);
    lv_label_set_text(mode_black,"Black" LV_SYMBOL_NEW_LINE);
    lv_obj_add_style(mode_black,&style_login_label,LV_STATE_DEFAULT);
    lv_obj_center(mode_black);
    /*========================================================================================================*/
    /*创建下棋界面容器*/
    contanier_play=lv_obj_create(lv_scr_act());       //基于屏幕创建一个和屏幕大小一样的容器contanier_6818
    lv_obj_set_size(contanier_play,disp_drv.hor_res,disp_drv.ver_res);       //设置到屏幕大小
    lv_obj_center(contanier_play);
    lv_obj_add_flag(contanier_play,LV_OBJ_FLAG_HIDDEN); //设置该界面隐藏
    lv_obj_clear_flag(contanier_play,LV_OBJ_FLAG_SCROLLABLE);//设置禁止滑条

    /*放出开始界面*/
    lv_obj_t *img_play_bg=lv_img_create(contanier_play);
    lv_obj_set_size(img_play_bg,disp_drv.hor_res,disp_drv.ver_res);//设置棋盘背景大小
    lv_obj_center(img_play_bg);
    lv_img_set_src(img_play_bg,KETPAN_BG_PATH);

    lv_obj_t *img_play=lv_img_create(contanier_play);
    lv_obj_set_size(img_play,disp_drv.hor_res-320,disp_drv.ver_res);//设置棋盘大小
    lv_obj_center(img_play);
    lv_img_set_src(img_play,KETPAN_PATH);

    lv_obj_t *img_player1=lv_img_create(contanier_play);
    lv_obj_align(img_player1,LV_ALIGN_BOTTOM_LEFT,0,0);
    lv_img_set_src(img_player1,PLAYER1_PATH);

    lv_obj_t *img_player2=lv_img_create(contanier_play);
    lv_obj_align(img_player2,LV_ALIGN_TOP_RIGHT,0,0);
    lv_img_set_src(img_player2,PLAYER2_PATH);

    /*创建认输按钮*/
    lv_obj_t *btn_defeat=lv_btn_create(contanier_play);
    lv_obj_add_style(btn_defeat,&style_pr,LV_STATE_PRESSED);
    lv_obj_add_style(btn_defeat,&style_def,0);
    lv_obj_add_style(btn_defeat,&style_border,0);
    lv_obj_set_size(btn_defeat,120,50);
    lv_style_set_bg_color(&style_pr,lv_palette_darken(LV_PALETTE_DEEP_ORANGE,2));
    lv_style_set_bg_color(&style_def,lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_defeat,LV_ALIGN_RIGHT_MID,0,50);
    // void mode_more(lv_event_t *e);
    // lv_obj_add_event_cb(btn_defeat,mode_more,LV_EVENT_ALL,NULL); //设置注册函数

    /*设置认输标签*/
    lv_obj_t * defeat=lv_label_create(btn_defeat);
    lv_label_set_text(defeat,"concede");
    lv_obj_center(defeat);
    lv_obj_add_style(defeat,&style_start_label,LV_STATE_DEFAULT);

    /*创建悔棋按钮*/
    lv_obj_t *btn_regret=lv_btn_create(contanier_play);
    lv_obj_add_style(btn_regret,&style_pr,LV_STATE_PRESSED);
    lv_obj_add_style(btn_regret,&style_def,0);
    lv_obj_add_style(btn_regret,&style_border,0);
    lv_obj_set_size(btn_regret,120,50);
    lv_style_set_bg_color(&style_pr,lv_palette_darken(LV_PALETTE_DEEP_ORANGE,2));
    lv_style_set_bg_color(&style_def,lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_regret,LV_ALIGN_RIGHT_MID,0,150);
    // void mode_more(lv_event_t *e);
    // lv_obj_add_event_cb(btn_defeat,mode_more,LV_EVENT_ALL,NULL); //设置注册函数

    /*设置认输标签*/
    lv_obj_t * regret=lv_label_create(btn_regret);
    lv_label_set_text(regret,"regret");
    lv_obj_center(regret);
    lv_obj_add_style(regret,&style_start_label,LV_STATE_DEFAULT);

    /*创建退出按钮*/
    lv_obj_t *btn_exit_play=lv_btn_create(contanier_play);
    lv_obj_add_style(btn_exit_play,&style_pr,LV_STATE_PRESSED);
    lv_obj_add_style(btn_exit_play,&style_def,0);
    lv_obj_add_style(btn_exit_play,&style_border,0);
    lv_obj_set_size(btn_exit_play,100,50);
    lv_style_set_bg_color(&style_pr,lv_palette_darken(LV_PALETTE_DEEP_ORANGE,2));
    lv_style_set_bg_color(&style_def,lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_exit_play,LV_ALIGN_TOP_LEFT,0,0);
    lv_obj_add_event_cb(btn_exit_play,exit_game,LV_EVENT_ALL,(void *)"exit"); //设置登陆函数

    /*创建退出标签*/
    lv_obj_t * play_exit=lv_label_create(btn_exit_play);
    lv_label_set_text(play_exit,"Exit" LV_SYMBOL_CLOSE);
    lv_obj_add_style(play_exit,&style_login_label,LV_STATE_DEFAULT);
    lv_obj_center(play_exit);

    /*创建返回按钮*/
    lv_obj_t *btn_black_play=lv_btn_create(contanier_play);
    lv_obj_add_style(btn_black_play,&style_pr,LV_STATE_PRESSED);
    lv_obj_add_style(btn_black_play,&style_def,0);
    lv_obj_add_style(btn_black_play,&style_border,0);
    lv_obj_set_size(btn_black_play,100,50);
    lv_style_set_bg_color(&style_pr,lv_palette_darken(LV_PALETTE_DEEP_ORANGE,2));
    lv_style_set_bg_color(&style_def,lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_black_play,LV_ALIGN_TOP_LEFT,0,80);
    lv_obj_add_event_cb(btn_black_play,exit_game,LV_EVENT_ALL,(void *)"play"); //设置登陆函数

    /*创建返回标签*/
    lv_obj_t * play_black=lv_label_create(btn_black_play);
    lv_label_set_text(play_black,"Black" LV_SYMBOL_NEW_LINE);
    lv_obj_add_style(play_black,&style_login_label,LV_STATE_DEFAULT);
    lv_obj_center(play_black);
    /*========================================================================================================*/
    /*创建结果界面容器*/
    contanier_result=lv_obj_create(contanier_play);       //创建一个容器
    lv_obj_set_size(contanier_result,400,300);       //设置到大小
    lv_obj_center(contanier_result);
    lv_obj_align(contanier_result,LV_ALIGN_CENTER,0,0);
    lv_obj_add_flag(contanier_result,LV_OBJ_FLAG_HIDDEN); //设置该界面隐藏
    lv_obj_clear_flag(contanier_result,LV_OBJ_FLAG_SCROLLABLE);//设置禁止滑条

    /*放出透明界面*/
    lv_obj_t *img_result=lv_img_create(contanier_result);
    lv_obj_set_size(img_result,400,300);//设置大小
    lv_obj_align(img_result,LV_ALIGN_CENTER,0,0);
    lv_img_set_src(img_result,BLACKGRAND_PATH2);


    /*创建再来一局按钮*/
    lv_obj_t *btn_again=lv_btn_create(contanier_result);
    lv_obj_add_style(btn_again,&style_pr,LV_STATE_PRESSED);
    lv_obj_add_style(btn_again,&style_def,0);
    lv_obj_add_style(btn_again,&style_border,0);
    lv_obj_set_size(btn_again,150,50);
    lv_style_set_bg_color(&style_pr,lv_palette_darken(LV_PALETTE_DEEP_ORANGE,2));
    lv_style_set_bg_color(&style_def,lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_again,LV_ALIGN_CENTER,0,-30);

    /*设置再来一局标签*/
    lv_obj_t * again=lv_label_create(btn_again);
    lv_label_set_text(again,"again");
    lv_obj_center(again);
    lv_obj_add_style(again,&style_start_label,LV_STATE_DEFAULT);

    /*创建换桌按钮*/
    lv_obj_t *btn_change=lv_btn_create(contanier_result);
    lv_obj_add_style(btn_change,&style_pr,LV_STATE_PRESSED);
    lv_obj_add_style(btn_change,&style_def,0);
    lv_obj_add_style(btn_change,&style_border,0);
    lv_obj_set_size(btn_change,150,50);
    lv_style_set_bg_color(&style_pr,lv_palette_darken(LV_PALETTE_DEEP_ORANGE,2));
    lv_style_set_bg_color(&style_def,lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_change,LV_ALIGN_CENTER,0,40);

    /*设置再来一局标签*/
    lv_obj_t * change=lv_label_create(btn_change);
    lv_label_set_text(change,"Substitute");  //Substitution：替代, 取代, 替换, 代换, 置换, 代用
    lv_obj_center(change);
    lv_obj_add_style(change,&style_start_label,LV_STATE_DEFAULT);

    /*创建退出按钮*/
    lv_obj_t *btn_Exit=lv_btn_create(contanier_result);
    lv_obj_add_style(btn_Exit,&style_pr,LV_STATE_PRESSED);
    lv_obj_add_style(btn_Exit,&style_def,0);
    lv_obj_add_style(btn_Exit,&style_border,0);
    lv_obj_set_size(btn_Exit,150,50);
    lv_style_set_bg_color(&style_pr,lv_palette_darken(LV_PALETTE_DEEP_ORANGE,2));
    lv_style_set_bg_color(&style_def,lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_Exit,LV_ALIGN_CENTER,0,110);
    lv_obj_add_event_cb(btn_Exit,exit_game,LV_EVENT_ALL,(void *)"exit"); //设置登陆函数

    /*设置再来一局标签*/
    lv_obj_t * Exit_label=lv_label_create(btn_Exit);
    lv_label_set_text(Exit_label,"Exit" LV_SYMBOL_NEW_LINE);  //Substitution：替代, 取代, 替换, 代换, 置换, 代用
    lv_obj_center(Exit_label);
    lv_obj_add_style(Exit_label,&style_start_label,LV_STATE_DEFAULT);


    /*========================================================================================================*/

    /*回调处理按键事件*/
    while(1){
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void){
    static uint64_t start_ms=0;
    if(start_ms==0){
        struct timeval tv_start;
        gettimeofday(&tv_start,NULL);
        start_ms=(tv_start.tv_sec*1000000+tv_start.tv_usec)/1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now,NULL);
    uint64_t now_ms;
    now_ms=(tv_now.tv_sec*1000000+tv_now.tv_usec)/1000;

    uint32_t time_ms=now_ms-start_ms;
    return time_ms;
}

/*退出函数*/
void exit_game(lv_event_t * e){
    string cont_start="start";
    string cont_login="login";
    string cont_register="register";
    string cont_mode="mode";
    string cont_play="play";

    lv_event_code_t code=lv_event_get_code(e);
    string btn=(char *)lv_event_get_user_data(e);
    if(code==LV_EVENT_CLICKED){
        if(btn==cont_start||btn=="exit"){
            sleep(2);
            exit(0);
        } else if(btn==cont_login){
            cout<<"blcak start"<<endl;
            // 给窗口login的容器重新添加隐藏属性，清除窗口start的隐藏属性
            lv_obj_add_flag(contanier_login,LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(contanier_start,LV_OBJ_FLAG_HIDDEN);
        } else if(btn==cont_register){
            cout<<"blcak register"<<endl;
            // 给窗口register的容器重新添加隐藏属性，清除窗口login的隐藏属性
            lv_obj_add_flag(contanier_register,LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(contanier_login,LV_OBJ_FLAG_HIDDEN);
        } else if(btn==cont_mode){
            cout<<"blcak login"<<endl;
            // 给窗口mode的容器重新添加隐藏属性，清除窗口login的隐藏属性
            lv_obj_add_flag(contanier_mode,LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(contanier_login,LV_OBJ_FLAG_HIDDEN);
        } else if(btn==cont_play){
            /*重新刷新棋盘*/
            lv_obj_t *img_play=lv_img_create(contanier_play);
            lv_obj_set_size(img_play,480,480);//设置棋盘大小
            lv_obj_center(img_play);
            lv_img_set_src(img_play,KETPAN_PATH);

            if(touch!=0){
                int ret=pthread_kill(touch,SIGUSR1);
                if(ret!=0){
                    cout<<"Failed to kill pthread"<<endl;
                }
            }

            cout<<"blcak mode"<<endl;
            /* 给窗口play的容器重新添加隐藏属性，清除窗口mode的隐藏属性*/
            lv_obj_add_flag(contanier_play,LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(contanier_mode,LV_OBJ_FLAG_HIDDEN);
        }
    }

}

/*开始函数*/
void start_game(lv_event_t * e){
    lv_event_code_t code=lv_event_get_code(e);

    if(code==LV_EVENT_CLICKED){
        cout<<"start"<<endl;
        // 给窗口1的容器添加隐藏属性，清除窗口2的隐藏属性
        lv_obj_add_flag(contanier_start,LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(contanier_login,LV_OBJ_FLAG_HIDDEN);

    }

}

/*注册函数*/
void register_funtion(lv_event_t * e){

    lv_event_code_t code=lv_event_get_code(e);
    if(code==LV_EVENT_CLICKED){
        cout<<"register-ing"<<endl;
        cout<<"给窗口login的容器添加隐藏属性,清除窗口register的隐藏属性"<<endl;
        lv_obj_add_flag(contanier_login,LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(contanier_register,LV_OBJ_FLAG_HIDDEN);
    }

}

/*登陆函数*/
void login_funtion(lv_event_t * e){
    lv_event_code_t code=lv_event_get_code(e);

    if(code==LV_EVENT_CLICKED){
        string check; //接收返回的登陆检测信息
        user_name_check=lv_textarea_get_text(user_name);
        cout<<"用户："<<user_name_check<<endl;
        password_check=lv_textarea_get_text(password);
        cout<<"密码："<<password_check<<endl;
        cout<<"check "<<check<<endl;
        /*发送账号密码，
         *参数1为server_socket，
         *参数2：0为注册1为登录，
         *参数3为用户名，参数4为密码
         *返回值为字符串_"登录失败,账户或密码错误_"或"_账户已存在_"*/
        check.clear();
        check=info.Send_NameAndPassword(server_socket,LOGIN,user_name_check,password_check);
        cout<<"check: "<<check<<endl;

        if(check=="登录失败,账户或密码错误"){
            cout<<"登录失败,账户或密码错误"<<endl;
        } else if(check=="nothing"){
            cout<<"请登录"<<endl;
        } else{
            cout<<"login done"<<endl;
            cout<<"给窗口login的容器添加隐藏属性,清除窗口mode的隐藏属性"<<endl;
            lv_obj_add_flag(contanier_login,LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(contanier_mode,LV_OBJ_FLAG_HIDDEN);
        }
    }

}

/*注册界面确定函数*/
void register_funtion_deal(lv_event_t * e){
    lv_event_code_t code=lv_event_get_code(e);

    if(code==LV_EVENT_CLICKED){


        user_name_check=lv_textarea_get_text(user_name_rg);
        cout<<"用户："<<user_name_check<<endl;
        password_check=lv_textarea_get_text(password_rg);
        cout<<"密码："<<password_check<<endl;
        string tmp=lv_textarea_get_text(password_rg_tmp);
        cout<<"密码："<<tmp<<endl;
        if(tmp!=password_check){
            cout<<"different password"<<endl;
        } else{
            string check="nothing"; //接收返回的登陆检测信息
            /*发送账号密码，
            *参数1为server_socket，
            *参数2：0为注册1为登录，
            *参数3为用户名，参数4为密码
            */
            check.clear();
            cout<<"check: "<<check<<endl;
            check=info.Send_NameAndPassword(server_socket,REGISTER,user_name_check,password_check);
            cout<<"check: "<<check<<endl;
            if(check=="账户已存在"){
                cout<<"账户已存在"<<endl;
            } else{
                cout<<"register done"<<endl;
                cout<<"给窗口register的容器添加隐藏属性,清除窗口mode的隐藏属性"<<endl;
                lv_obj_add_flag(contanier_register,LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(contanier_mode,LV_OBJ_FLAG_HIDDEN);
            }
        }
    }
}

/*文本点击函数*/
void ta_event_cb(lv_event_t * e){

    lv_event_code_t code=lv_event_get_code(e);
    lv_obj_t * ta=lv_event_get_target(e);
    lv_obj_t * kb=(lv_obj_t *)lv_event_get_user_data(e);
    if(code==LV_EVENT_FOCUSED){
        if(lv_indev_get_type(lv_indev_get_act())!=LV_INDEV_TYPE_KEYPAD){
            lv_keyboard_set_textarea(kb,ta);
            lv_obj_set_style_max_height(kb,LV_HOR_RES*2/3,0);
            lv_obj_clear_flag(kb,LV_OBJ_FLAG_HIDDEN);
            /*键盘覆盖按钮*/
            lv_obj_add_flag(btn3,LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(btn4,LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(btn_register_yes,LV_OBJ_FLAG_HIDDEN);
            lv_obj_scroll_to_view_recursive(ta,LV_ANIM_OFF);

        }
    } else if(code==LV_EVENT_DEFOCUSED){
        lv_keyboard_set_textarea(kb,NULL);
        lv_obj_add_flag(kb,LV_OBJ_FLAG_HIDDEN);
        /*再次显示按钮*/
        lv_obj_clear_flag(btn3,LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(btn4,LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(btn_register_yes,LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL,ta);


    } else if(code==LV_EVENT_READY||code==LV_EVENT_CANCEL){
        lv_obj_add_flag(kb,LV_OBJ_FLAG_HIDDEN);
        /*再次显示按钮*/
        lv_obj_clear_flag(btn3,LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(btn4,LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(btn_register_yes,LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_state(ta,LV_STATE_FOCUSED);
        lv_indev_reset(NULL,ta);   /*To forget the last clicked object to make it focusable again*/
    }
}

char map1[N][N]={0};
void mode_one_retract(pair<pair<int,int>,pair<int,int>> lately_pos){
    map1[lately_pos.first.first][lately_pos.first.second]=0;
    board.gomoku[lately_pos.first.first][lately_pos.first.second]='_';

    map1[lately_pos.second.first][lately_pos.second.second]=0;
    board.gomoku[lately_pos.second.first][lately_pos.second.second]='_';

}
int isvalpos(int x,int y){//缩小触点范围
    int x0=(x-166)/34;
    int y0=(y-30)/34;
    if(x>=166+x0*34+17-12&&x<=166+x0*34+17+12){
        return 1;
    }
    return 0;
}
void mode_watch(lv_event_t *e){//!需要3个开发板，待测试
    lv_event_code_t code=lv_event_get_code(e);
    if(code==LV_EVENT_CLICKED){
        cout<<"watch"<<endl;
        lv_obj_add_flag(contanier_mode,LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(contanier_play,LV_OBJ_FLAG_HIDDEN);
        
        pthread_create(&touch,NULL,[](void *arg)->void*{

            write(server_socket,"watch:game",strlen("watch:game"));//申请观战

            char buf[N*N];//接收棋盘
            memset(board.gomoku,'_',sizeof(board.gomoku));
            queue<pair<int,int>> newPeices;
            int x,y;
            while(1){//接收观战数据
                memset(buf,0,sizeof(buf));
                read(server_socket,buf,N*N);
                if('_'==buf[0]||'B'==buf[0]||'W'==buf[0]){
                    for(int i=0;i<N*N;i++){
                        if(board.gomoku[i/N][i%N]!=buf[i]){
                            newPeices.push(make_pair(i/N,i%N));//收集新增棋子
                            board.gomoku[i/N][i%N]=buf[i];
                            // printf("push_back: %c\n",buf[i]);
                        }
                    }
                    board.gomoku_show();
                    while(!newPeices.empty()){//处理新增棋子
                        // printf("已处理（在lcd画出）新棋子：%d,%d\n",newPeices.front().first,newPeices.front().second);
                        x=newPeices.front().first;
                        y=newPeices.front().second;
                        if('B'==board.gomoku[x][y]){//1黑2白
                            dis_key(x*34+166,y*34+30,"black");
                            cout<<"黑子落子："<<x<<","<<y<<" black"<<endl;
                            if(board.checkWin(x,y,'B')){
                                printf("黑子赢了\n");
                            }
                        } else if('W'==board.gomoku[x][y]){
                            dis_key(x*34+166,y*34+30,"white");
                            cout<<"白子落子："<<x<<","<<y<<" white"<<endl;
                            if(board.checkWin(x,y,'W')){
                                printf("白子赢了\n");
                            }
                        }
                        newPeices.pop();
                    }
                }
            }

            signal(SIGUSR1,sig_funtion);
            },NULL);
    }
}
void mode_one(lv_event_t *e){
    lv_event_code_t code=lv_event_get_code(e);
    if(code==LV_EVENT_CLICKED){
        cout<<"singel"<<endl;
        cout<<"给窗口mode的容器添加隐藏属性,清除窗口play的隐藏属性"<<endl;
        /*给窗口login的容器添加隐藏属性，清除窗口play的隐藏属性*/
        lv_obj_add_flag(contanier_mode,LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(contanier_play,LV_OBJ_FLAG_HIDDEN);
        pthread_create(&touch,NULL,[](void *arg)->void*{
            char player='B';
            AI ai;
            COORDINATE cor;
            map1[13][13]={0};
            TsDevice t2=TsDevice::TsInstance();
            struct input_event ts_event;
            int ts_fd=t2.ts_fd;
            int ts_x,ts_y,x,y;
            void mode_one_retract(pair<pair<int,int>,pair<int,int>> lately_pos);
            pair<pair<int,int>,pair<int,int>> lately_pos;
            while(1){
                if(player=='B'){
                    while(1){
                        read(ts_fd,&ts_event,sizeof(ts_event));
                        //分析获取到的输入设备的参数信息：类型、编码、数值
                        if(ts_event.type==EV_ABS&&ts_event.code==ABS_X){
                            ts_x=ts_event.value*800/1024; //存储X轴坐标，该句需要选择性修改，新版触摸屏必须转换	
                            // ts_x=ts_event.value; //存储X轴坐标，该句需要选择性修改，新版触摸屏必须转换	
                        }
                        if(ts_event.type==EV_ABS&&ts_event.code==ABS_Y){
                            ts_y=ts_event.value*480/600; //存储Y轴坐标，该句需要选择性修改，新版触摸屏必须转换	
                            // ts_y=ts_event.value; //存储Y轴坐标，该句需要选择性修改，新版触摸屏必须转换	
                        }
                        //当用户的手指离开触摸屏，则把坐标返回
                        if(ts_event.type==EV_KEY&&ts_event.code==BTN_TOUCH&&ts_event.value==0){
                            printf("ts_x=%d,ts_y=%d\n",ts_x,ts_y);
                            x=ts_x;
                            y=ts_y;//先把触摸屏返回的大坐标转成小坐标
                            x=(x-166)/34;
                            y=(y-30)/34;
                            if(board.isDropLegal(y,x)){
                                break;
                            }
                        }
                    }
                    int isvalpos(int x,int y);
                    if(isvalpos(ts_x,ts_y)){
                        /*转换x，y坐标*/
                        printf("wanjia:%d,%d\n",x,y);     //玩家下的小坐标在板子是x,y，  在棋盘的二位数组中是y,x
                        board.gomoku[y][x]='B';//图形棋盘  //二维数组棋总盘中的y,x   
                        map1[y][x]='B';//ai棋盘            //把玩家的棋子下到AI计算的棋盘上
                        lately_pos.first=make_pair(y,x);
                        dis_key(ts_x,ts_y,"black");//图形棋盘画子
                        // board.gomoku_show();
                        if(board.checkWin(y,x,'B')){
                            printf("黑子赢了\n");
                            break;
                        }
                        player='W';
                    }
                }
                // sleep(1);
                else if(player=='W'){
                    cor=ai.deepSearch(map1,'B','W',0,y,x);//ai思考  //玩家在板子下的是x,y  在二维数组下的是y,x
                    board.gomoku[cor.row][cor.col]='W';
                    map1[cor.row][cor.col]='W';
                    lately_pos.second=make_pair(cor.row,cor.col);
                    ts_x=cor.col*34+166;
                    ts_y=cor.row*34+30;
                    printf("ai:%d,%d\n",cor.row,cor.col);
                    dis_key(ts_x,ts_y,"white");
                    // board.gomoku_show();
                    if(board.checkWin(cor.row,cor.col,'W')){
                        printf("白子赢了\n");
                        break;
                    }
                    player='B';
                }
            }
            signal(SIGUSR1,sig_funtion);
            },
            NULL);

        pthread_detach(touch);
    }
}

/*接收信号结束线程*/
void sig_funtion(int){
    pthread_exit(0);
}

void mode_more(lv_event_t *e){
    lv_event_code_t code=lv_event_get_code(e);
    if(code==LV_EVENT_CLICKED){

        cout<<"play"<<endl;
        cout<<"切换成paly界面"<<endl;
        /*给窗口login的容器添加隐藏属性，清除窗口play的隐藏属性*/
        lv_obj_add_flag(contanier_mode,LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(contanier_play,LV_OBJ_FLAG_HIDDEN);


        sleep(1);
        /*send start 匹配开房*/
        info.Send_start_and_watch_Info(server_socket,1);
        cout<<"Send_start_and_watch_Info:send(server_socket,player_message,sizeof(player_message),0);"<<endl;
        color=info.Recv_ChessType(server_socket);
        cout<<"Recv_ChessType:  "<<color<<endl;

        /*创建一个线程实时获取坐标*/
        pthread_create(&touch,NULL,[](void *arg)->void*{
            signal(SIGUSR1,sig_funtion);
            int x=-1,y=-1;
            char player='W';
            int end=0;
            if(color=='B'){
                player='B';//确认棋手身份
                cout<<"先手"<<endl;
                //先手
                while(1){
                    get_xy(x,y);
                    printf("tsxy:%d,%d\n",x,y);
                    x=(x-166)/34;
                    y=(y-30)/34;
                    if(board.isDropLegal(y,x,'B')){
                        info.Send_position(server_socket,y,x,1);
                        printf("我（黑）方落子%d,%d\n",x,y);
                        x=x*34+166;
                        y=y*34+30;
                        dis_key(x,y,"black");
                        break;
                    }
                }
            }
            cout<<"循环下棋"<<endl;
            /*循环下棋*/
            while(1){
                /*接收对手下棋信息并绘制*/
                // int *tmp=info.Recv_position(server_socket);//!看不懂，自己来↓
                //way:down,local:(11,5),color:1
                char recvbuf[128];
                memset(recvbuf,0,128);
                read(server_socket,recvbuf,128);
                int colorflag;
                sscanf(recvbuf,"way:down,local:(%d,%d),color:%d",&y,&x,&colorflag);
                printf("解析接收到的数据：%d,%d  %d\n",y,x,colorflag);
                if(colorflag==1){//1黑2白
                    if(board.isDropLegal(y,x,'B'));
                    cout<<"对手落子："<<x<<","<<y<<" black"<<endl;
                    if(board.checkWin(y,x,'B')){
                        printf("对方赢了\n");
                        end=1;
                    }
                    x=x*34+166;
                    y=y*34+30;
                    dis_key(x,y,"black");
                    // board.gomoku[y][x]='B';
                } else if(colorflag==2){
                    if(board.isDropLegal(y,x,'W'));
                    cout<<"对手落子："<<x<<","<<y<<" white"<<endl;
                    if(board.checkWin(y,x,'W')){
                        printf("对方赢了\n");
                        end=1;
                    }
                    x=x*34+166;
                    y=y*34+30;
                    dis_key(x,y,"white");
                    // board.gomoku[y][x]='W';
                }


                /*我方落子*/
                while(1){//发送落子
                    get_xy(x,y);
                    x=(x-166)/34;
                    y=(y-30)/34;
                    if(board.isDropLegal(y,x,color)){
                        if(color=='B'){
                            info.Send_position(server_socket,y,x,1);
                            printf("我方落子：%d,%d white\n",x,y);
                            if(board.checkWin(y,x,color)){
                                printf("我赢了\n");
                                end=1;
                            }
                            x=x*34+166;
                            y=y*34+30;
                            dis_key(x,y,"black");
                            // board.gomoku[y][x]='B';
                        } else if(color=='W'){
                            info.Send_position(server_socket,y,x,2);
                            printf("我方落子：%d,%d white\n",x,y);
                            if(board.checkWin(y,x,color)){
                                printf("我赢了\n");
                                end=1;
                            }
                            x=x*34+166;
                            y=y*34+30;
                            dis_key(x,y,"white");
                            // board.gomoku[y][x]='W';
                        }
                        break;
                    }
                }

                /*判断输赢，退出循环*/
                if(end){
                    break;
                }
            }
            },
            NULL
        );
        pthread_detach(touch);
    }
}

void dis_key(int x,int y,const char* b_or_w,lv_obj_t*obj){
    lv_obj_t *dis_image=lv_img_create(obj);
    /*转换x，y坐标*/
    x=(x-166)/34;
    y=(y-30)/34;
    cout<<"("<<x<<","<<y<<")"<<endl;
    lv_obj_set_pos(dis_image,166+x*36-17,6+y*36-17);   //棋盘（0，0），真实坐标166，6
    board.gomoku_show();

    /*判断下棋黑白*/
    if(b_or_w=="black"){
        lv_img_set_src(dis_image,BLACK_KEY_PATH);
    } else if(b_or_w=="white"){
        lv_img_set_src(dis_image,WHITE_KEY_PATH);
    }
}

void get_xy(int &x,int &y){
    tsDevice=TsDevice::TsInstance();
    int ts_fd=tsDevice.ts_fd;
    struct input_event ts_event;
    memset(&ts_event,0,sizeof(ts_event));
    while(1){
        read(ts_fd,&ts_event,sizeof(ts_event));
        //分析获取到的输入设备的参数信息：类型、编码、数值
        if(ts_event.type==EV_ABS&&ts_event.code==ABS_X){

            x=ts_event.value*800/1024; //存储X轴坐标，该句需要选择性修改，新版触摸屏必须转换	
        }
        if(ts_event.type==EV_ABS&&ts_event.code==ABS_Y){

            y=ts_event.value*480/600; //存储Y轴坐标，该句需要选择性修改，新版触摸屏必须转换	
        }
        //当用户的手指离开触摸屏，则把坐标返回
        if(ts_event.type==EV_KEY&&ts_event.code==BTN_TOUCH&&ts_event.value==0){
            printf("x=%d,y=%d\n",y,x);
            break;
        }
    }
}