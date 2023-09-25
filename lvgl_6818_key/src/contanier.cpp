#include "../head/btn_event.h"

#include <iostream>

lv_obj_t *  contanier_start = NULL; //作为开始界面容器
lv_obj_t *  contanier_login = NULL; //作为登陆界面容器
lv_obj_t *  contanier_register = NULL; //作为注册界面
lv_obj_t *  contanier_mode = NULL; // 模式选择界面
lv_obj_t *  contanier_result = NULL; //进入房间界面
lv_obj_t *  contanier_play = NULL; //下棋界面

lv_obj_t * password;               //创建密码对象
lv_obj_t * user_name;              //创建用户名对象
lv_obj_t * password_rg;            //创建注册密码对象
lv_obj_t * password_rg_tmp;        //创建注册密码对象
lv_obj_t * user_name_rg;           //创建注册用户名对象
lv_obj_t * btn3,*btn4;             //btn3注册按钮，btn登陆按钮，为了使用键盘时隐藏该按钮，设为全局
lv_obj_t * btn_register_yes;       //注册确定按钮


string user_name_check;       //获取textarea的用户名，待发送给服务端
string password_check;        //获取textarea的密码，待发送给服务端

void contanier( lv_disp_drv_t disp_drv)
{
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

    /*设置按钮边界*/
    static lv_style_t style_border;
    lv_style_init(&style_border);    
    lv_style_set_border_color(&style_border, lv_color_black()); // 设置边框颜色为黑色
    lv_style_set_border_width(&style_border, 2); // 设置边框宽度为2

    /*设置按钮圆形样式*/
    static lv_style_t style_circel;
    lv_style_init(&style_circel); 
    lv_style_set_radius(&style_circel, 50); // 设置按钮样式圆角弧度


    /*创建开始按钮*/
    lv_obj_t * btn1 = lv_btn_create(contanier_start);
    lv_obj_add_style(btn1, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn1, &style_def, 0);
    lv_obj_add_style(btn1, &style_border, 0);
    lv_obj_add_style(btn1, &style_circel, 0);
    lv_obj_set_size(btn1, 200, 100);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));     
    lv_obj_center(btn1);
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
    lv_obj_add_style(btn2, &style_border, 0);
    lv_obj_add_style(btn2, &style_circel, 0);
    lv_obj_set_size(btn2, 200, 100);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 140);
    lv_obj_add_event_cb(btn2,exit_game,LV_EVENT_ALL,(void *)"start"); //设置退出函数

    /*创建退出标签*/
    lv_obj_t * back_label = lv_label_create(btn2);
    lv_label_set_text(back_label, "EXIT " LV_SYMBOL_CLOSE);
    lv_obj_add_style(back_label,&style_start_label,LV_STATE_DEFAULT);
    lv_obj_center(back_label);
    /*========================================================================================================*/
    /*=========================================================================================================*/
    /*创建登陆界面容器*/
    contanier_login = lv_obj_create(lv_scr_act());       //基于屏幕创建一个和屏幕大小一样的容器contanier_6818
    lv_obj_set_size(contanier_login, disp_drv.hor_res , disp_drv.ver_res);       //设置到屏幕大小
    lv_obj_center(contanier_login);

    lv_obj_add_flag  (contanier_login, LV_OBJ_FLAG_HIDDEN); //设置该界面隐藏
    lv_obj_clear_flag(contanier_login, LV_OBJ_FLAG_SCROLLABLE);//设置禁止滑条

    /*放出开始界面*/
    lv_obj_t *img2 = lv_img_create(contanier_login);
    lv_obj_set_size(img2,disp_drv.hor_res,disp_drv.ver_res);//设置大小
    lv_obj_center(img2);
    lv_img_set_src(img2,BLACKGRAND_PATH);
 
      /*放出透明登陆界面*/
    lv_obj_t *img3 = lv_img_create(img2);    
    lv_obj_set_size(img3,350,250);//设置大小
    lv_obj_align(img3, LV_ALIGN_CENTER, 0, -20);
     lv_img_set_src(img3,BLACKGRAND_PATH2);

     /*创建键盘*/
    lv_obj_t * kb = lv_keyboard_create(contanier_login);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    std::cout<<"key"<<std::endl;

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

     /*创建注册按钮*/
    btn3 = lv_btn_create(contanier_login);
    lv_obj_add_style(btn3, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn3, &style_def, 0);
    lv_obj_add_style(btn3, &style_border, 0);
    lv_obj_set_size(btn3, 100, 50);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn3, LV_ALIGN_CENTER, -80, 80);
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
    lv_obj_add_style(btn4, &style_border, 0);
    lv_obj_set_size(btn4, 100, 50);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn4, LV_ALIGN_CENTER, 80, 80);
    lv_obj_add_event_cb(btn4,login_funtion,LV_EVENT_ALL,NULL); //设置登陆函数

    /*创建登陆标签*/
    static lv_style_t style_login_label;
    lv_style_init(&style_login_label);
    lv_obj_t * login_label = lv_label_create(btn4);
    lv_label_set_text(login_label, "Login" LV_SYMBOL_OK);
    lv_style_set_text_font(&style_login_label,&lv_font_montserrat_20);
    lv_obj_add_style(login_label,&style_login_label,LV_STATE_DEFAULT);
    lv_obj_center(login_label);

     /*创建返回按钮*/
    lv_obj_t *btn_black_login = lv_btn_create(contanier_login);
    lv_obj_add_style(btn_black_login, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn_black_login, &style_def, 0);
    lv_obj_add_style(btn_black_login, &style_border, 0);
    lv_obj_set_size(btn_black_login, 100, 50);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_black_login, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_event_cb(btn_black_login,exit_game,LV_EVENT_ALL,(void *)"login"); //设置登陆函数

    /*创建返回标签*/
    lv_obj_t * login_black = lv_label_create(btn_black_login);
    lv_label_set_text(login_black, "Black" LV_SYMBOL_NEW_LINE);
    lv_obj_add_style(login_black,&style_login_label,LV_STATE_DEFAULT);
    lv_obj_center(login_black);

    /*========================================================================================================*/
    /*创建注册界面容器*/
    contanier_register = lv_obj_create(lv_scr_act());
    lv_obj_set_size(contanier_register, disp_drv.hor_res,disp_drv.ver_res);       //设置到屏幕大小
    lv_obj_center(contanier_register);

    /*放出注册背景界面*/
    lv_obj_t *img_rg = lv_img_create(contanier_register);
    lv_obj_set_size(img_rg,disp_drv.hor_res,disp_drv.ver_res);//设置大小
    lv_obj_center(img_rg);
    lv_img_set_src(img_rg,BLACKGRAND_PATH);
 
      /*放出注册界面*/
    lv_obj_t *img_rg_m = lv_img_create(img_rg);    
    lv_obj_set_size(img_rg_m,350,250);//设置大小
    lv_obj_align(img_rg_m, LV_ALIGN_CENTER, 0, -60);
    lv_img_set_src(img_rg_m,BLACKGRAND_PATH2);

    lv_obj_add_flag  (contanier_register, LV_OBJ_FLAG_HIDDEN); //设置该界面隐藏
    lv_obj_clear_flag(contanier_register, LV_OBJ_FLAG_SCROLLABLE);//设置禁止滑条

     /*创建键盘*/
    lv_obj_t * kb_rg = lv_keyboard_create(contanier_register);
    lv_obj_add_flag(kb_rg, LV_OBJ_FLAG_HIDDEN);

    /*创建用户名文本*/
     user_name_rg = lv_textarea_create(contanier_register);
    lv_textarea_set_one_line(user_name_rg, true);
    lv_obj_align(user_name_rg, LV_ALIGN_CENTER, 0, -160);
    lv_textarea_set_placeholder_text(user_name_rg, "Your name");
    lv_obj_add_event_cb(user_name_rg, ta_event_cb, LV_EVENT_ALL, kb_rg);


    /*创建密码文本*/
    password_rg = lv_textarea_create(contanier_register);
    lv_textarea_set_one_line(password_rg, true);
    lv_textarea_set_password_mode(password_rg, true);
    lv_obj_align(password_rg, LV_ALIGN_CENTER, 0, -90);
    lv_textarea_set_placeholder_text(password_rg, "password:Min. 8 chars.");
    lv_obj_add_event_cb(password_rg, ta_event_cb, LV_EVENT_ALL, kb_rg);

    /*创建确定密码文本*/
    password_rg_tmp = lv_textarea_create(contanier_register);
    lv_textarea_set_one_line(password_rg_tmp, true);
    lv_textarea_set_password_mode(password_rg_tmp, true);
    lv_obj_align(password_rg_tmp, LV_ALIGN_CENTER, 0, -20);
    lv_textarea_set_placeholder_text(password_rg_tmp, "input again.");
    lv_obj_add_event_cb(password_rg_tmp, ta_event_cb, LV_EVENT_ALL, kb_rg);

     /*创建注册确定按钮*/
    btn_register_yes = lv_btn_create(contanier_register);
    lv_obj_add_style(btn_register_yes, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn_register_yes, &style_def, 0);
    lv_obj_add_style(btn_register_yes, &style_border, 0);
    lv_obj_set_size(btn_register_yes, 100, 50);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_register_yes, LV_ALIGN_CENTER, 0, 30);
    lv_obj_add_event_cb(btn_register_yes,register_funtion_deal,LV_EVENT_ALL,NULL); //设置注册函数

    /*设置注册标签*/
    lv_obj_t * register_label_rg = lv_label_create(btn_register_yes);
    lv_label_set_text(register_label_rg, LV_SYMBOL_OK);
    lv_obj_center(register_label_rg);

    /*创建返回按钮*/
    lv_obj_t *btn_black_register = lv_btn_create(contanier_register);
    lv_obj_add_style(btn_black_register, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn_black_register, &style_def, 0);
    lv_obj_add_style(btn_black_register, &style_border, 0);
    lv_obj_set_size(btn_black_register, 100, 50);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_black_register, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_event_cb(btn_black_register,exit_game,LV_EVENT_ALL,(void *)"register"); //设置登陆函数

    /*创建返回标签*/
    lv_obj_t * register_black = lv_label_create(btn_black_register);
    lv_label_set_text(register_black, "Black" LV_SYMBOL_NEW_LINE);
    lv_obj_add_style(register_black,&style_login_label,LV_STATE_DEFAULT);
    lv_obj_center(register_black);



    /*========================================================================================================*/
    /*游戏模式选择界面*/
    contanier_mode = lv_obj_create(lv_scr_act());       //基于屏幕创建一个和屏幕大小一样的容器contanier_6818
    lv_obj_set_size(contanier_mode, disp_drv.hor_res,disp_drv.ver_res);       //设置到屏幕大小
    lv_obj_center(contanier_mode);
    lv_obj_add_flag  (contanier_mode, LV_OBJ_FLAG_HIDDEN); //设置该界面隐藏
    lv_obj_clear_flag(contanier_mode, LV_OBJ_FLAG_SCROLLABLE);//设置禁止滑条

    /*放出模式界面*/
    lv_obj_t *img_mode = lv_img_create(contanier_mode);
    lv_obj_set_size(img_mode,disp_drv.hor_res,disp_drv.ver_res);//设置棋盘大小
    lv_obj_center(img_mode);
    lv_img_set_src(img_mode,BLACKGRAND_PATH);

     /*创建单机按钮*/
    lv_obj_t *btn_one = lv_btn_create(contanier_mode);
    lv_obj_add_style(btn_one, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn_one, &style_def, 0);
    lv_obj_add_style(btn_one, &style_border, 0);
    lv_obj_add_style(btn_one, &style_circel, 0);
    lv_obj_set_size(btn_one, 200, 100);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_center(btn_one);
    lv_obj_add_event_cb(btn_one,mode_one,LV_EVENT_ALL,NULL); //设置注册函数

    /*设置单机标签*/
    lv_obj_t * one = lv_label_create(btn_one);
    lv_label_set_text(one, "singel");
    lv_obj_center(one);
    lv_obj_add_style(one,&style_start_label,LV_STATE_DEFAULT);

 
    /*创建多人按钮*/
    lv_obj_t *btn_more = lv_btn_create(contanier_mode);
    lv_obj_add_style(btn_more, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn_more, &style_def, 0);
    lv_obj_add_style(btn_more, &style_border, 0);
    lv_obj_add_style(btn_more, &style_circel, 0);
    lv_obj_set_size(btn_more, 200, 100);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_more, LV_ALIGN_CENTER, 0, 140);
    lv_obj_add_event_cb(btn_more,mode_more,LV_EVENT_ALL,NULL); //设置注册函数

    /*设置多人标签*/
    lv_obj_t * more = lv_label_create(btn_more);
    lv_label_set_text(more, "multiplayer");
    lv_obj_center(more);
    lv_obj_add_style(more,&style_start_label,LV_STATE_DEFAULT);

    /*创建返回按钮*/
    lv_obj_t *btn_black_mode = lv_btn_create(contanier_mode);
    lv_obj_add_style(btn_black_mode, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn_black_mode, &style_def, 0);
    lv_obj_add_style(btn_black_mode, &style_border, 0);
    lv_obj_set_size(btn_black_mode, 100, 50);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_black_mode, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_event_cb(btn_black_mode,exit_game,LV_EVENT_ALL,(void *)"mode"); //设置登陆函数

    /*创建返回标签*/
    lv_obj_t * mode_black = lv_label_create(btn_black_mode);
    lv_label_set_text(mode_black, "Black" LV_SYMBOL_NEW_LINE);
    lv_obj_add_style(mode_black,&style_login_label,LV_STATE_DEFAULT);
    lv_obj_center(mode_black);
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

    lv_obj_t *img_player1 = lv_img_create(contanier_play);
    lv_obj_align(img_player1, LV_ALIGN_BOTTOM_LEFT, 0, 0);
     lv_img_set_src(img_player1,PLAYER1_PATH);

    lv_obj_t *img_player2 = lv_img_create(contanier_play);
    lv_obj_align(img_player2, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_img_set_src(img_player2,PLAYER2_PATH);

    /*创建认输按钮*/
    lv_obj_t *btn_defeat = lv_btn_create(contanier_play);
    lv_obj_add_style(btn_defeat, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn_defeat, &style_def, 0);
    lv_obj_add_style(btn_defeat, &style_border, 0);
    lv_obj_set_size(btn_defeat, 120, 50);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_defeat, LV_ALIGN_RIGHT_MID, 0, 50);
    // void mode_more(lv_event_t *e);
    // lv_obj_add_event_cb(btn_defeat,mode_more,LV_EVENT_ALL,NULL); //设置注册函数

    /*设置认输标签*/
    lv_obj_t * defeat = lv_label_create(btn_defeat);
    lv_label_set_text(defeat, "concede");
    lv_obj_center(defeat);
    lv_obj_add_style(defeat,&style_start_label,LV_STATE_DEFAULT);

    /*创建悔棋按钮*/
    lv_obj_t *btn_regret = lv_btn_create(contanier_play);
    lv_obj_add_style(btn_regret, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn_regret, &style_def, 0);
    lv_obj_add_style(btn_regret, &style_border, 0);
    lv_obj_set_size(btn_regret, 120, 50);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_regret, LV_ALIGN_RIGHT_MID, 0, 150);
    // void mode_more(lv_event_t *e);
    // lv_obj_add_event_cb(btn_defeat,mode_more,LV_EVENT_ALL,NULL); //设置注册函数

    /*设置认输标签*/
    lv_obj_t * regret = lv_label_create(btn_regret);
    lv_label_set_text(regret, "regret");
    lv_obj_center(regret);
    lv_obj_add_style(regret,&style_start_label,LV_STATE_DEFAULT);

       /*创建退出按钮*/
    lv_obj_t *btn_exit_play = lv_btn_create(contanier_play);
    lv_obj_add_style(btn_exit_play, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn_exit_play, &style_def, 0);
    lv_obj_add_style(btn_exit_play, &style_border, 0);
    lv_obj_set_size(btn_exit_play, 100, 50);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_exit_play, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_event_cb(btn_exit_play,exit_game,LV_EVENT_ALL,(void *)"exit"); //设置登陆函数

    /*创建退出标签*/
    lv_obj_t * play_exit = lv_label_create(btn_exit_play);
    lv_label_set_text(play_exit, "Exit" LV_SYMBOL_CLOSE);
    lv_obj_add_style(play_exit,&style_login_label,LV_STATE_DEFAULT);
    lv_obj_center(play_exit);   

    /*创建返回按钮*/
    lv_obj_t *btn_black_play = lv_btn_create(contanier_play);
    lv_obj_add_style(btn_black_play, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn_black_play, &style_def, 0);
    lv_obj_add_style(btn_black_play, &style_border, 0);
    lv_obj_set_size(btn_black_play, 100, 50);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_black_play, LV_ALIGN_TOP_LEFT, 0, 80);
    lv_obj_add_event_cb(btn_black_play,exit_game,LV_EVENT_ALL,(void *)"play"); //设置登陆函数

    /*创建返回标签*/
    lv_obj_t * play_black = lv_label_create(btn_black_play);
    lv_label_set_text(play_black, "Black" LV_SYMBOL_NEW_LINE);
    lv_obj_add_style(play_black,&style_login_label,LV_STATE_DEFAULT);
    lv_obj_center(play_black);     

    /*========================================================================================================*/
    /*创建结果界面容器*/
    contanier_result = lv_obj_create(contanier_play);       //创建一个容器
    lv_obj_set_size(contanier_result, 400,300);       //设置到大小
    lv_obj_center(contanier_result);
    lv_obj_align(contanier_result, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag  (contanier_result, LV_OBJ_FLAG_HIDDEN); //设置该界面隐藏
    lv_obj_clear_flag(contanier_result, LV_OBJ_FLAG_SCROLLABLE);//设置禁止滑条

    /*放出透明界面*/
    lv_obj_t *img_result = lv_img_create(contanier_result);    
    lv_obj_set_size(img_result,400,300);//设置大小
    lv_obj_align(img_result, LV_ALIGN_CENTER, 0, 0);
    lv_img_set_src(img_result,BLACKGRAND_PATH2);

    
    /*创建再来一局按钮*/
    lv_obj_t *btn_again = lv_btn_create(contanier_result);
    lv_obj_add_style(btn_again, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn_again, &style_def, 0);
    lv_obj_add_style(btn_again, &style_border, 0);
    lv_obj_set_size(btn_again, 150, 50);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_again, LV_ALIGN_CENTER, 0, -30);

    /*设置再来一局标签*/
    lv_obj_t * again = lv_label_create(btn_again);
    lv_label_set_text(again, "again");
    lv_obj_center(again);
    lv_obj_add_style(again,&style_start_label,LV_STATE_DEFAULT);

    /*创建换桌按钮*/
    lv_obj_t *btn_change = lv_btn_create(contanier_result);
    lv_obj_add_style(btn_change, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn_change, &style_def, 0);
    lv_obj_add_style(btn_change, &style_border, 0);
    lv_obj_set_size(btn_change, 150, 50);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_change, LV_ALIGN_CENTER, 0, 40);

    /*设置再来一局标签*/
    lv_obj_t * change = lv_label_create(btn_change);
    lv_label_set_text(change, "Substitute");  //Substitution：替代, 取代, 替换, 代换, 置换, 代用
    lv_obj_center(change);
    lv_obj_add_style(change,&style_start_label,LV_STATE_DEFAULT);

    /*创建退出按钮*/
    lv_obj_t *btn_Exit = lv_btn_create(contanier_result);
    lv_obj_add_style(btn_Exit, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn_Exit, &style_def, 0);
    lv_obj_add_style(btn_Exit, &style_border, 0);
    lv_obj_set_size(btn_Exit, 150, 50);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn_Exit, LV_ALIGN_CENTER, 0, 110);
    lv_obj_add_event_cb(btn_Exit,exit_game,LV_EVENT_ALL,(void *)"exit"); //设置登陆函数

    /*设置再来一局标签*/
    lv_obj_t * Exit_label = lv_label_create(btn_Exit);
    lv_label_set_text(Exit_label, "Exit" LV_SYMBOL_NEW_LINE);  //Substitution：替代, 取代, 替换, 代换, 置换, 代用
    lv_obj_center(Exit_label);
    lv_obj_add_style(Exit_label,&style_start_label,LV_STATE_DEFAULT);


    /*========================================================================================================*/

}