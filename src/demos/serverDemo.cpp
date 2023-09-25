/* 
g++ serverDemo.cpp server.cpp -pthread&&./a.out
g++ playerDemo.cpp player.cpp&&./a.out
g++ playerDemo.cpp player.cpp&&./a.out
 */
#include "server.h"
#include<iostream>
using namespace std;
void test1();
int main(){
    test1();
}
void test1(){//转发-双方落子数据
    Server s;
    pair<int,int> heibaifd= s.sendSetColor();
    int heifd=heibaifd.first;
    int baifd=heibaifd.second;
    s.forwardHeiBaiRequest(heifd,baifd);
}
void test2(){//悔棋测试
    //单独一个线程等待对方请求悔棋
    pthread_t t1;
    // pthread_create(&t1,NULL,isAgreeRetract,NULL);
    printf("做其它正事(下棋)\n");
    pthread_join(t1,NULL);
    printf("正事做完了\n");
}

