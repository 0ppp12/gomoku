/*
 * @Author: victor victor@example.com
 * @Date: 2023-09-19 18:53:33
 * @LastEditors: victor victor@example.com
 * @LastEditTime: 2023-09-22 11:09:26
 * @FilePath: \work\stage5\game-project\the-gobang-game-of-cc-md-fk\src\client.cpp
 * @Description: ???????????,??????`customMade`, ??koroFileHeader?????? ????????: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include "client.h"


int Info_SendAndRev::INIT_SOCKET(const char* SERVER_IP,int PORT)
{
    //创建套接字
    int client_socket;
    struct sockaddr_in server_address;

    //创建客户端套接字
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) 
    {
        perror("socket");
        return 1;
    }

    // 设置服务器地址
    server_address.sin_family = AF_INET;
    // server_address.sin_addr.s_addr = inet_addr("192.168.13.64");
    if (inet_pton(AF_INET, SERVER_IP, &(server_address.sin_addr)) <= 0)
    {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return 1;
    }
    server_address.sin_port = htons(PORT);

    // 连接到服务器
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) 
    {
        perror("connect");
        return 1;
    }
    return client_socket;
}



//登录信息或者注册信息发送
bool Info_SendAndRev::Send_NameAndPassword(int client_socket,int flag,string name,string password)
{
    std::string client_info;
    client_info.clear();
    //注册
    if(flag==0)
    {
        client_info = "way:register,account:"+ name + "|password:" + password;
    }    
    //登录
    else if(flag==1)
    {
        client_info = "way:login,account:"+ name + "|password:" + password;
    }
    //打印
    std::cout<<client_info<<" "<<client_info.length()<< std::endl;
    //发送
    if(send(client_socket, client_info.c_str(), client_info.length(), 0)<0)
    {
        //错误处理
        std::cout<<"发送失败"<<std::endl;
        return false;
    }
    return true;
}
//发送棋盘信息
bool Info_SendAndRev::Send_Checkerboard_Info(int client_socket)
{
    //发送数组

    //整合信息，发送前置协议
    //下棋：way:down,local:(x,y)|color:black/white
    //悔棋：way:back,local:(x,y)|color:black/white
    //这是例子
    //应当发送整个棋盘信息a[x][y] = 0无 1黑 2白
    return true;

}
//接收信息
bool Info_SendAndRev::Rev_info(int client_socket,char * recvbuffer)
{
    //信息的解包
    char buffer[128]={0};
    strcpy(buffer,recvbuffer);
    //分离数组内容，验证是否合法加入
    char *way = strtok(buffer, ",");
    char *message = strtok(NULL,"\0");
    cout<<way<<endl;
    cout<<message<<endl;
    //根据解包之后的协议做选择
    /*void Get_NameAndPassword(char *recvbuffer)在服务器中的这个函数可以把字符串中的
    符号去掉，然后输出由字符串隔开的一个个字符串
    输或赢：object:xxx,result:win/lose
    加入房间:way:join,object:xxx
    或者更多
    */
   return true;
}

/*******************test*****************/
int main(){
    Info_SendAndRev info;
    int client_socket = info.INIT_SOCKET("172.19.255.126",9995);
    info.Send_NameAndPassword(client_socket,0,"victor","123456");
    return 0;
}