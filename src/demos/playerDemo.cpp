/* 
g++ serverDemo.cpp -pthread&&./a.out
g++ playerDemo.cpp checkerboard.cpp player.cpp&&./a.out
g++ playerDemo.cpp checkerboard.cpp player.cpp&&./a.out
 */
#include <iostream>
#include "player.h"
using namespace std;
void test1();//连接服务器，收发棋子坐标，测试
void test2();
int main(){
    test1();
}
void test2(){

}
void test1(){
    Player p;
    int serverSockfd=p.getServerSockfd();
    p.sendGetColorRequest(serverSockfd);
    p.recvSendDropRequest(serverSockfd);
}