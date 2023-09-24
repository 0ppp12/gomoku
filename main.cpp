#include <iostream>
using namespace std;

#include "../lvgl/lvgl.h"
//#include "lvgl/demos/lv_demos.h"
#include "../lv_drivers/display/fbdev.h"
#include "../lv_drivers/indev/evdev.h"
#include "../head/client.h"   //连接头文件
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
// #define BLACKGRAND_PATH "S:/lzktemp/blackgrand.jpg"  //开始界面背景
// #define BLACKGRAND_PATH2 "S:/lzktemp/bg.jpg"        //登陆注册界面背景
// #define KETPAN_PATH "S:/lzktemp/Pan.jpg" //下棋界面棋盘
// #define KETPAN_BG_PATH "S:/lzktemp/Pan_bg.jpg" //下棋界面棋盘
#define BLACKGRAND_PATH "../../lzktemp/blackgrand.jpg"  //开始界面背景
#define BLACKGRAND_PATH2 "../../lzktemp/bg.jpg"        //登陆注册界面背景
#define KETPAN_PATH "../../lzktemp/Pan.jpg" //下棋界面棋盘
#define KETPAN_BG_PATH "../../lzktemp/Pan_bg.jpg" //下棋界面棋盘
#define LOGIN 1     //登陆
#define REGISTER 0  //注册

lv_obj_t *  contanier_start = NULL; //作为开始界面容器
lv_obj_t *  contanier_login = NULL; //作为登陆界面容器
lv_obj_t *  contanier_register = NULL; //作为注册界面
lv_obj_t *  contanier_mode = NULL; // 模式选择界面
lv_obj_t *  contanier_home = NULL; //进入房间界面
lv_obj_t *  contanier_play = NULL; //下棋界面
lv_obj_t * password;               //创建密码对象
lv_obj_t * user_name;              //创建用户名对象
lv_obj_t * btn3,*btn4;             //btn3注册按钮，btn登陆按钮，为了使用键盘时隐藏该按钮，设为全局
Info_SendAndRev info;              //创建连接类对象
int client_socket;                 //用户链接后返回的套接字描述符
string user_name_check;       //获取textarea的用户名，待发送给服务端
string password_check;        //获取textarea的密码，待发送给服务端


int main(int argc, char const *argv[])
{
    /*LittlevGL init*/
    lv_init();

    /*Linux frame buffer device init*/
    fbdev_init();

    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];

    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = fbdev_flush;
    disp_drv.hor_res    = 800;
    disp_drv.ver_res    = 480;
    lv_disp_drv_register(&disp_drv);

    evdev_init();
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;

    /*This function will be called periodically (by the library) to get the mouse position and state*/
    indev_drv_1.read_cb = evdev_read;
    lv_indev_drv_register(&indev_drv_1);

    /*========================================================================================================*/
    /*创建开始界面容器*/
    contanier_start = lv_obj_create(lv_scr_act());       //基于屏幕创建一个和屏幕大小一样的容器contanier_6818
    lv_obj_set_size(contanier_start, disp_drv.hor_res , disp_drv.ver_res);       //设置到屏幕大小
    lv_obj_center(contanier_start);

    /*放出开始界面*/
    lv_obj_t *img1 = lv_img_create(contanier_start);
    lv_obj_set_size(img1,disp_drv.hor_res,disp_drv.ver_res);//设置大小
    lv_obj_center(img1);
    lv_img_set_src(img1,BLACKGRAND_PATH);
    lv_obj_clear_flag(contanier_start, LV_OBJ_FLAG_SCROLLABLE);
   
    /*Properties to transition*/
    static lv_style_prop_t props[] = {
        LV_STYLE_TRANSFORM_WIDTH, LV_STYLE_TRANSFORM_HEIGHT, LV_STYLE_TEXT_LETTER_SPACE, LV_STYLE_PROP_INV
    };

    /*Transition descriptor when going back to the default state.
     *Add some delay to be sure the press transition is visible even if the press was very short*/
    static lv_style_transition_dsc_t transition_dsc_def;
    lv_style_transition_dsc_init(&transition_dsc_def, props, lv_anim_path_overshoot, 250, 100, NULL);

    /*Transition descriptor when going to pressed state.
     *No delay, go to presses state immediately*/
    static lv_style_transition_dsc_t transition_dsc_pr;
    lv_style_transition_dsc_init(&transition_dsc_pr, props, lv_anim_path_ease_in_out, 250, 0, NULL);

    /*Add only the new transition to he default state*/
    static lv_style_t style_def;
    lv_style_init(&style_def);
    lv_style_set_transition(&style_def, &transition_dsc_def);

    /*Add the transition and some transformation to the presses state.*/
    static lv_style_t style_pr;
    lv_style_init(&style_pr);
    lv_style_set_transform_width(&style_pr, 10);
    lv_style_set_transform_height(&style_pr, -10);
    lv_style_set_text_letter_space(&style_pr, 10);
    lv_style_set_transition(&style_pr, &transition_dsc_pr);

    /*创建开始按钮*/
    lv_obj_t * btn1 = lv_btn_create(contanier_start);
    lv_obj_add_style(btn1, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn1, &style_def, 0);
    lv_obj_set_size(btn1, 200, 100);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_center(btn1);
    void start_game(lv_event_t * e);
    lv_obj_add_event_cb(btn1,start_game,LV_EVENT_ALL,NULL); //设置开始函数

    /*设置开始标签*/
    static lv_style_t style_start_label;
    lv_style_init(&style_start_label);
    lv_obj_t * start_label = lv_label_create(btn1);
    lv_label_set_text(start_label, "S t a r t");
    lv_style_set_text_font(&style_start_label,&lv_font_montserrat_24);
    lv_obj_add_style(start_label,&style_start_label,LV_STATE_DEFAULT);
    lv_obj_center(start_label);

    /*创建退出按钮*/
    lv_obj_t * btn2 = lv_btn_create(contanier_start);
    lv_obj_add_style(btn2, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn2, &style_def, 0);
    lv_obj_set_size(btn2, 200, 100);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 140);
    void exit_game(lv_event_t * e);
    lv_obj_add_event_cb(btn2,exit_game,LV_EVENT_ALL,NULL); //设置退出函数

    /*创建退出标签*/
    static lv_style_t style_back_label;
    lv_style_init(&style_back_label);
    lv_obj_t * back_label = lv_label_create(btn2);
    lv_label_set_text(back_label, "EXIT" LV_SYMBOL_NEW_LINE);
    lv_style_set_text_font(&style_back_label,&lv_font_montserrat_24);
    lv_obj_add_style(back_label,&style_back_label,LV_STATE_DEFAULT);
    lv_obj_center(back_label);
    /*========================================================================================================*/
    /*与服务端进行连接*/
    const char *IP_4 = argv[1];
    int Port =stoi(argv[2]);
    do
    {
        client_socket = info.INIT_SOCKET(IP_4,Port);
    }
    while(client_socket == 1);
    
    cout<<"连接成功"<<endl;
       
    /*=========================================================================================================*/
    /*创建登陆界面容器*/
    contanier_login = lv_obj_create(lv_scr_act());       //基于屏幕创建一个和屏幕大小一样的容器contanier_6818
    lv_obj_set_size(contanier_login, disp_drv.hor_res , disp_drv.ver_res);       //设置到屏幕大小
    lv_obj_center(contanier_login);
    /*放出开始界面*/
    lv_obj_t *img2 = lv_img_create(contanier_login);
    lv_obj_set_size(img2,disp_drv.hor_res,disp_drv.ver_res);//设置大小
    lv_obj_center(img2);
    //往img1对象中添加图片资源   bmp  jpg
    lv_img_set_src(img2,BLACKGRAND_PATH);
 
      /*放出透明登陆界面*/
    // lv_style_t style_im;
    // lv_style_set_img_opa(&style_im,LV_OPA_80);
    lv_obj_t *img3 = lv_img_create(img2);    
    //lv_obj_add_style(img3,&style_im,LV_STATE_DEFAULT);cout<<"11111111"<<endl;
    lv_obj_set_size(img3,350,250);//设置大小
    lv_obj_align(img3, LV_ALIGN_CENTER, 0, -20);
     lv_img_set_src(img3,BLACKGRAND_PATH2);

    lv_obj_add_flag  (contanier_login, LV_OBJ_FLAG_HIDDEN); //设置该界面隐藏
    lv_obj_clear_flag(contanier_login, LV_OBJ_FLAG_SCROLLABLE);//设置禁止滑条

     /*创建键盘*/
    lv_obj_t * kb = lv_keyboard_create(contanier_login);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
     void ta_event_cb(lv_event_t * e); //声明文本点击事件
    cout<<"key"<<endl;
    /*设置标签样式*/
     static lv_style_t style_text_muted;
     lv_style_init(&style_text_muted);
     lv_color_t Text_color = lv_color_hex(0Xffffff);
     lv_style_set_text_font(&style_text_muted,&lv_font_montserrat_18);
     lv_style_set_text_color(&style_text_muted,Text_color); 

     /*创建用户名标签*/
     lv_obj_t * user_name_label = lv_label_create(contanier_login);
    lv_label_set_text(user_name_label, "Username");
    lv_obj_add_style(user_name_label, &style_text_muted, 0);
    lv_obj_align(user_name_label, LV_ALIGN_CENTER, -80, -130);

    /*创建用户名文本*/
     user_name = lv_textarea_create(contanier_login);
    lv_textarea_set_one_line(user_name, true);
    lv_obj_align(user_name, LV_ALIGN_CENTER, 0, -100);
    lv_textarea_set_placeholder_text(user_name, "Your name");
    lv_obj_add_event_cb(user_name, ta_event_cb, LV_EVENT_ALL, kb);
    //user_name_check = lv_textarea_get_text(user_name);
    //cout<<user_name_check<<endl;

    /*创建密码标签*/
    lv_obj_t * password_label = lv_label_create(contanier_login);
    lv_label_set_text(password_label, "Password");
     lv_obj_add_style(password_label, &style_text_muted, 0);
     lv_obj_align(password_label, LV_ALIGN_CENTER, -80, -50);

    /*创建密码文本*/
     password = lv_textarea_create(contanier_login);
    lv_textarea_set_one_line(password, true);
    lv_textarea_set_password_mode(password, true);
    lv_obj_align(password, LV_ALIGN_CENTER, 0, -20);
    lv_textarea_set_placeholder_text(password, "Min. 8 chars.");
    lv_obj_add_event_cb(password, ta_event_cb, LV_EVENT_ALL, kb);
    //password_check = lv_textarea_get_text(password);
    //cout<<password_check<<endl;

     /*创建注册按钮*/
    btn3 = lv_btn_create(contanier_login);
    lv_obj_add_style(btn3, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn3, &style_def, 0);
    lv_obj_set_size(btn3, 100, 50);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn3, LV_ALIGN_CENTER, -80, 80);
    void register_funtion(lv_event_t * e);
    lv_obj_add_event_cb(btn3,register_funtion,LV_EVENT_ALL,NULL); //设置注册函数

    /*设置注册标签*/
    static lv_style_t style_register_label;
    lv_style_init(&style_register_label);
    lv_obj_t * register_label = lv_label_create(btn3);
    lv_label_set_text(register_label, "Register");
    lv_style_set_text_font(&style_register_label,&lv_font_montserrat_20);
    lv_obj_add_style(register_label,&style_register_label,LV_STATE_DEFAULT);
    lv_obj_center(register_label);

    /*创建登陆按钮*/
    btn4 = lv_btn_create(contanier_login);
    lv_obj_add_style(btn4, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn4, &style_def, 0);
    lv_obj_set_size(btn4, 100, 50);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn4, LV_ALIGN_CENTER, 80, 80);
    void login_funtion(lv_event_t * e);

    lv_obj_add_event_cb(btn4,login_funtion,LV_EVENT_ALL,NULL); //设置登陆函数

    /*创建登陆标签*/
    static lv_style_t style_login_label;
    lv_style_init(&style_login_label);
    lv_obj_t * login_label = lv_label_create(btn4);
    lv_label_set_text(login_label, "Login" LV_SYMBOL_OK);
    lv_style_set_text_font(&style_login_label,&lv_font_montserrat_20);
    lv_obj_add_style(login_label,&style_login_label,LV_STATE_DEFAULT);
    lv_obj_center(login_label);
    /*========================================================================================================*/
    /*========================================================================================================*/
    /*创建下棋界面容器*/
    contanier_play = lv_obj_create(lv_scr_act());       //基于屏幕创建一个和屏幕大小一样的容器contanier_6818
    lv_obj_set_size(contanier_play, disp_drv.hor_res,disp_drv.ver_res);       //设置到屏幕大小
    lv_obj_center(contanier_play);
    lv_obj_add_flag  (contanier_play, LV_OBJ_FLAG_HIDDEN); //设置该界面隐藏
    lv_obj_clear_flag(contanier_play, LV_OBJ_FLAG_SCROLLABLE);//设置禁止滑条

    /*放出开始界面*/
    lv_obj_t *img_play_bg = lv_img_create(contanier_play);
    lv_obj_set_size(img_play_bg,disp_drv.hor_res,disp_drv.ver_res);//设置棋盘大小
    lv_obj_center(img_play_bg);
    lv_img_set_src(img_play_bg,KETPAN_BG_PATH);


    lv_obj_t *img_play = lv_img_create(contanier_play);
    lv_obj_set_size(img_play,disp_drv.hor_res-320,disp_drv.ver_res);//设置棋盘大小
    lv_obj_center(img_play);
     lv_img_set_src(img_play,KETPAN_PATH);

 

    /*========================================================================================================*/
    
    /*回调处理按键事件*/
    while(1) {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
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

 /*退出函数*/
 void exit_game(lv_event_t * e)
{
    
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        sleep(2);
        exit(0);
    }
    
}

/*开始函数*/
 void start_game(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED)
     {
        cout<<"start"<<endl;
                // 给窗口1的容器添加隐藏属性，清除窗口2的隐藏属性
        lv_obj_add_flag  (contanier_start, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(contanier_login, LV_OBJ_FLAG_HIDDEN);

    }
    
}/*退出函数*/

/*注册函数*/
 void register_funtion(lv_event_t * e)
{
    
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        sleep(2);
        exit(0);
    }
    
}

/*登陆函数*/
 void login_funtion(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED)
     {
        cout<<"login done"<<endl;
        cout<<"给窗口login的容器添加隐藏属性,清除窗口play的隐藏属性"<<endl;
            /*给窗口login的容器添加隐藏属性，清除窗口play的隐藏属性*/
        lv_obj_add_flag  (contanier_login, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(contanier_play, LV_OBJ_FLAG_HIDDEN);
        user_name_check = lv_textarea_get_text(user_name);
        cout<<"用户："<<user_name_check<<endl;
        password_check = lv_textarea_get_text(password);
        cout<<"密码："<<password_check<<endl;
        //info.Send_NameAndPassword(client_socket,LOGIN,user_name_check,password_check);
    }
    
}

/*文本点击函数*/
 void ta_event_cb(lv_event_t * e)
{

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * kb = (lv_obj_t *)lv_event_get_user_data(e);
    if(code == LV_EVENT_FOCUSED) {
        if(lv_indev_get_type(lv_indev_get_act()) != LV_INDEV_TYPE_KEYPAD) {
            lv_keyboard_set_textarea(kb, ta);
            lv_obj_set_style_max_height(kb, LV_HOR_RES * 2 / 3, 0);
            lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(btn3, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(btn4, LV_OBJ_FLAG_HIDDEN);
            lv_obj_scroll_to_view_recursive(ta, LV_ANIM_OFF);
            
        }
    }
    else if(code == LV_EVENT_DEFOCUSED) {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(btn3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(btn4, LV_OBJ_FLAG_HIDDEN);       
        lv_indev_reset(NULL, ta);

    }
    else if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(btn3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(btn4, LV_OBJ_FLAG_HIDDEN);   
        lv_obj_clear_state(ta, LV_STATE_FOCUSED);
        lv_indev_reset(NULL, ta);   /*To forget the last clicked object to make it focusable again*/
    }
}