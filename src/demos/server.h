#include <iostream>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<cstring>
#include<unistd.h>
#include<pthread.h>
using namespace std;
#define PORT 1888
class Server{
    public:
    int sockfd;
    
    pair<int,int> sendSetColor();
    int forwardHeiBaiRequest(int heifd,int baifd);
    static int tcpAcceptCfd(int sockfd);
    int tcpInit();
    void* isAgreeRetract(void*arg);
};