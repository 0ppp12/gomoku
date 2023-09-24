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
void test1(){
    Server s;
    pair<int,int> heibaifd= s.sendSetColor();
    int heifd=heibaifd.first;
    int baifd=heibaifd.second;
    s.forwardHeiBaiRequest(heifd,baifd);
}