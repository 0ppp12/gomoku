#include "Checkerboard.h"
#include "player.h"

int main(){
    //创建棋手类对象
    Player local_Player;
    //连接服务器
    int serverSockfd = local_Player.getServerSockfd();
    //发送请求先后手身份
    local_Player.sendGetColorRequest(serverSockfd);
    //开始下棋
    local_Player.recvSendDropRequest(serverSockfd);
}