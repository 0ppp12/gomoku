#include "../head/btn_event.h"
extern "C"
{
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
}

/*引用contanier.cpp的全局变量*/
extern lv_obj_t *  contanier_start; //作为开始界面容器
extern lv_obj_t *  contanier_login; //作为登陆界面容器
extern lv_obj_t *  contanier_register; //作为注册界面
extern lv_obj_t *  contanier_mode; // 模式选择界面
extern lv_obj_t *  contanier_result ; //进入房间界面
extern lv_obj_t *  contanier_play ; //下棋界面
extern lv_obj_t * password;               //创建密码对象
extern lv_obj_t * user_name;              //创建用户名对象
extern lv_obj_t * password_rg;            //创建注册密码对象
extern lv_obj_t * password_rg_tmp;        //创建注册密码对象
extern lv_obj_t * user_name_rg;           //创建注册用户名对象
extern lv_obj_t * btn3,*btn4;             //btn3注册按钮，btn登陆按钮，为了使用键盘时隐藏该按钮，设为全局
extern lv_obj_t * btn_register_yes;       //注册确定按钮
extern string user_name_check;       //获取textarea的用户名，待发送给服务端
extern string password_check;        //获取textarea的密码，待发送给服务端

/*引用main.cpp的全局变量*/
extern Info_SendAndRev info;              //创建连接类对象
extern int client_socket;  



 /*退出函数*/
 void exit_game(lv_event_t * e)
{   string cont_start = "start";
    string cont_login = "login";
    string cont_register = "register";
    string cont_mode = "mode";
    string cont_play = "play";

    lv_event_code_t code = lv_event_get_code(e);
    string btn = (char *)lv_event_get_user_data(e);
    if (code == LV_EVENT_CLICKED)
    {
        if(btn == cont_start || btn == "exit")
        {
        sleep(2);
        exit(0);
        }
        else if(btn == cont_login)
        {
            cout<<"blcak start"<<endl;
        // 给窗口login的容器重新添加隐藏属性，清除窗口start的隐藏属性
            lv_obj_add_flag  (contanier_login, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(contanier_start, LV_OBJ_FLAG_HIDDEN);
        }
        else if (btn == cont_register)
        {
            cout<<"blcak register"<<endl;
        // 给窗口register的容器重新添加隐藏属性，清除窗口login的隐藏属性
            lv_obj_add_flag  (contanier_register, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(contanier_login, LV_OBJ_FLAG_HIDDEN);
        }
        else if(btn == cont_mode)
        {
            cout<<"blcak login"<<endl;
        // 给窗口mode的容器重新添加隐藏属性，清除窗口login的隐藏属性
            lv_obj_add_flag  (contanier_mode ,LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(contanier_login, LV_OBJ_FLAG_HIDDEN);
        }
         else if (btn == cont_play)
        {
            cout<<"blcak mode"<<endl;
        // 给窗口play的容器重新添加隐藏属性，清除窗口mode的隐藏属性
            lv_obj_add_flag  (contanier_play ,LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(contanier_mode, LV_OBJ_FLAG_HIDDEN);
        }
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
    
}

/*注册函数*/
 void register_funtion(lv_event_t * e)
{
    
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        cout<<"register-ing"<<endl;
        cout<<"给窗口login的容器添加隐藏属性,清除窗口register的隐藏属性"<<endl;
        lv_obj_add_flag  (contanier_login, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(contanier_register, LV_OBJ_FLAG_HIDDEN);
        
    }
    
}

/*登陆函数*/
 void login_funtion(lv_event_t * e)
{
     lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED)
     {
        string check = "nothing"; //接收返回的登陆检测信息
        user_name_check = lv_textarea_get_text(user_name);
        cout<<"用户："<<user_name_check<<endl;
        password_check = lv_textarea_get_text(password);
        cout<<"密码："<<password_check<<endl;
        cout<<"check "<<check<<endl;
        /*发送账号密码，
         *参数1为client_socket，
         *参数2：0为注册1为登录，
         *参数3为用户名，参数4为密码
         *返回值为字符串_"登录失败,账户或密码错误_"或"_账户已存在_"*/
        check = info.Send_NameAndPassword(client_socket,LOGIN,user_name_check,password_check);
        cout<<"check: "<<check<<endl;

        if(check == "登录失败,账户或密码错误")
        {
            cout<<"登录失败,账户或密码错误"<<endl;
        }
        else if (check == "nothing")
        {
            cout<<"请登录"<<endl;
        }
        else
        {
        cout<<"login done"<<endl;
        cout<<"给窗口login的容器添加隐藏属性,清除窗口mode的隐藏属性"<<endl;
        lv_obj_add_flag  (contanier_login, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(contanier_mode, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

/*注册界面确定函数*/
void register_funtion_deal(lv_event_t * e)
{
   lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED)
     {

        string check = "nothing"; //接收返回的登陆检测信息
        user_name_check = lv_textarea_get_text(user_name_rg);
        cout<<"用户："<<user_name_check<<endl;
        password_check = lv_textarea_get_text(password_rg);
        cout<<"密码："<<password_check<<endl;
        string tmp = lv_textarea_get_text(password_rg_tmp);
        cout<<"密码："<<tmp<<endl;
        if(tmp != password_check)
        {
            cout <<"different password"<<endl;
        }
        else
        {
            /*发送账号密码，
            *参数1为client_socket，
            *参数2：0为注册1为登录，
            *参数3为用户名，参数4为密码
            */
            check = info.Send_NameAndPassword(client_socket,REGISTER,user_name_check,password_check);
             cout<<"check: "<<check<<endl;
            if(check == "账户已存在")
            {
                cout<<"账户已存在"<<endl;
            }
            else
            {
            cout<<"register done"<<endl;
            cout<<"给窗口register的容器添加隐藏属性,清除窗口mode的隐藏属性"<<endl;
            lv_obj_add_flag  (contanier_register, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(contanier_mode, LV_OBJ_FLAG_HIDDEN);
            }

        
        }
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
            /*键盘覆盖按钮*/
            lv_obj_add_flag(btn3, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(btn4, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(btn_register_yes, LV_OBJ_FLAG_HIDDEN);
            lv_obj_scroll_to_view_recursive(ta, LV_ANIM_OFF);
            
        }
    }
    else if(code == LV_EVENT_DEFOCUSED) {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        /*再次显示按钮*/
        lv_obj_clear_flag(btn3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(btn4, LV_OBJ_FLAG_HIDDEN);  
        lv_obj_clear_flag(btn_register_yes, LV_OBJ_FLAG_HIDDEN);     
        lv_indev_reset(NULL, ta);
    

    }
    else if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        /*再次显示按钮*/
        lv_obj_clear_flag(btn3, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(btn4, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(btn_register_yes, LV_OBJ_FLAG_HIDDEN);   
        lv_obj_clear_state(ta, LV_STATE_FOCUSED);
        lv_indev_reset(NULL, ta);   /*To forget the last clicked object to make it focusable again*/
    }
}

 void mode_one(lv_event_t *e)
 {
    lv_event_code_t code = lv_event_get_code(e);
     if(code == LV_EVENT_CLICKED)
     {
        
        cout<<"singel"<<endl;
        cout<<"给窗口mode的容器添加隐藏属性,清除窗口play的隐藏属性"<<endl;
            /*给窗口login的容器添加隐藏属性，清除窗口play的隐藏属性*/
        lv_obj_add_flag  (contanier_mode, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(contanier_play, LV_OBJ_FLAG_HIDDEN);
 

    }
 } 

 void mode_more(lv_event_t *e)
 {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED)
     {
        // user_name_check = lv_textarea_get_text(user_name_rg);
        // cout<<"用户："<<user_name_check<<endl;
        // password_check = lv_textarea_get_text(password_rg);
        // cout<<"密码："<<password_check<<endl;
        // string tmp = lv_textarea_get_text(password_rg_tmp);
        // cout<<"密码："<<tmp<<endl;
        // if(tmp != password_check)
        // {
        //     cout <<"different password"<<endl;
        // }
    }
 }
 