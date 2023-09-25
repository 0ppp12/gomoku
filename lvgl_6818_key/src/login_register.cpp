#include "../lvgl/lvgl.h"
#include <iostream>
using namespace std;
extern "C"
{
    #include <stdlib.h>
    #include <unistd.h>
}
extern lv_obj_t *  contanier1_6818;
extern lv_obj_t *  contanier2_6818;


/*退出函数*/
void register_funtion(lv_event_t * e)
{
    
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        sleep(2);
        exit(0);
    }
    
}

static void login_funtion(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED)
     {
        //cout<<"窗口1的容器添加隐藏属性,清除窗口2的隐藏属性"<<endl;
                // 给窗口1的容器添加隐藏属性，清除窗口2的隐藏属性
        //lv_obj_add_flag  (contanier1_6818, LV_OBJ_FLAG_HIDDEN);
        //lv_obj_clear_flag(contanier2_6818, LV_OBJ_FLAG_HIDDEN);

    }
    
}




void login_register()
{
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

    /*创建注册按钮*/
    lv_obj_t * btn1 = lv_btn_create(contanier2_6818);
    lv_obj_add_style(btn1, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn1, &style_def, 0);
    lv_obj_set_size(btn1, 200, 100);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_center(btn1);
    lv_obj_add_event_cb(btn1,register_funtion,LV_EVENT_ALL,NULL); //设置注册函数
    /*设置注册标签*/
    static lv_style_t style_start_label;
    lv_style_init(&style_start_label);
    lv_obj_t * start_label = lv_label_create(btn1);
    lv_label_set_text(start_label, "Register");
    lv_style_set_text_font(&style_start_label,&lv_font_montserrat_24);
    lv_obj_add_style(start_label,&style_start_label,LV_STATE_DEFAULT);
    lv_obj_center(start_label);

    /*创建登陆按钮*/
    lv_obj_t * btn2 = lv_btn_create(contanier2_6818);
    lv_obj_add_style(btn2, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn2, &style_def, 0);
    lv_obj_set_size(btn2, 200, 100);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 2));
    lv_style_set_bg_color(&style_def, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 140);
    lv_obj_add_event_cb(btn2,login_funtion,LV_EVENT_ALL,NULL); //设置登陆函数

    /*创建登陆标签*/
    static lv_style_t style_back_label;
    lv_style_init(&style_back_label);
    lv_obj_t * back_label = lv_label_create(btn2);
    lv_label_set_text(back_label, "Login" LV_SYMBOL_OK);
    lv_style_set_text_font(&style_back_label,&lv_font_montserrat_24);
    lv_obj_add_style(back_label,&style_back_label,LV_STATE_DEFAULT);
    lv_obj_center(back_label);
 
    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) {
        lv_timer_handler();
        usleep(5000);
    }
}