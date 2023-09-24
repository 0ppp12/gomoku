/* 
g++ serverDemo.cpp -pthread&&./a.out
g++ playerDemo.cpp player.cpp&&./a.out
g++ playerDemo.cpp player.cpp&&./a.out
 */
#include <iostream>
#include "player.h"
using namespace std;
void test1();
int main(){
    test1();
}
void test1(){
    Player p;
    int serverSockfd=p.getServerSockfd();
    char color=p.sendGetColorRequest(serverSockfd);
    p.recvSendDropRequest(serverSockfd,color);
}