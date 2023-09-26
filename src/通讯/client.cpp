/*
 * @Author: victor victor@example.com
 * @Date: 2023-09-19 18:53:33
 * @LastEditors: victor victor@example.com
 * @LastEditTime: 2023-09-26 17:19:55
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
char * Info_SendAndRev::Send_NameAndPassword(int client_socket,int flag,string name,string password)
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
    }
    //接收
    static char recvbuffer[128];
    if(recv(client_socket, recvbuffer, 128, 0)<0)
    {
        cout<< "接收失败"<<endl;
    }
    return recvbuffer;
}

//发送棋盘信息
bool Info_SendAndRev::Send_Checkerboard_Info(int client_socket,int a[13][13])
{
    string Checkerboard_Info;
    //发送数组
    string b;
    for(int i=0;i<13;i++){
        for(int j=0;j<13;j++){
            b+=to_string(a[i][j]);
        }
    }
    Checkerboard_Info="way:Checkerboard_Info,arry|"+b;
    //发送
    if(send(client_socket, Checkerboard_Info.c_str(), Checkerboard_Info.length(), 0)<0)
    {
        //错误处理
        std::cout<<"发送失败"<<std::endl;
        return false;
    }
    cout<<Checkerboard_Info<<endl;
    return true;
}

// 接收棋盘信息
int **Info_SendAndRev::Recv_Checkerboard_Info(int client_socket){
    //接收
    static char recvbuffer[128];
    if(recv(client_socket, recvbuffer, 128, 0)<0)
    {
        cout<< "接收失败"<<endl;
    }
    //解包
    char *way = strtok(recvbuffer, "|");
    char *arry = strtok(NULL, "|");
    int** arr = new int*[13];
    for(int i=0;i<13;i++){
        arr[i] = new int[13]; 
        for(int j=0;j<13;j++){
            arr[i][j] = atoi(std::to_string(arry[i*13+j]).c_str());
        }
    }
    return arr;
}

//发送登录或者观战信息
bool Info_SendAndRev::Send_start_and_watch_Info(int client_socket,int flag)
{
    char watch_message[]="watch:game";
    char player_message[]="start:game";
    if(flag==1)
    {
        send(client_socket,player_message,sizeof(player_message),0);
    }
    else if(flag==2)
    {
        send(client_socket,watch_message,sizeof(watch_message),0);
    }
    return true;
}


//发送落子位置
void Info_SendAndRev::Send_position(int client_socket,int x,int y,int color){
    string position_message="way:down,local:("+to_string(x)+","+to_string(y)+"),color:"+to_string(color);
    //发送
    if(send(client_socket, position_message.c_str(), position_message.length(), 0)<0)
    {
        //错误处理
        std::cout<<"发送失败"<<std::endl;
    }
}

//接收落子信息
int *Info_SendAndRev::Recv_position(int client_socket){
    //接收
    char recvbuffer[128];
    if(recv(client_socket, recvbuffer, 128, 0)<0)
    {
        cout<< "接收失败"<<endl;
    }

    char delimiters[] = " ,:()"; // 分隔符可以是空格、逗号、问号和感叹号
    char* token = std::strtok(recvbuffer, delimiters);
    int *arr=new int[3];
    int a=0;
    while (token != nullptr) {
        if(a==3)
        {
            arr[0] = stoi(std::to_string(*token).c_str())-'0';
        }
        if(a==4)
        {
            arr[1] = stoi(std::to_string(*token).c_str())-'0';
        }
        if(a==6)
        {
            arr[2] = stoi(std::to_string(*token).c_str())-'0';
            break;
        }
        token = std::strtok(nullptr, delimiters);
        a++;
    }
    return arr;
}
/*******************test*****************/
int client_socket;
void *thread_recv(void *arg)
{
    while(1)
    {
        char recvbuff[128];
        recv(client_socket, recvbuff, 128, 0);
        cout<<recvbuff<<endl;
        memset(recvbuff,0,sizeof(recvbuff));
    }
}

int main(){
    Info_SendAndRev info;
    client_socket = info.INIT_SOCKET("192.168.13.64",9996);
    string recvbuffer =info.Send_NameAndPassword(client_socket,1,"victor","123456");
    send(client_socket, "start:game", sizeof("start:game"), 0);
    char recvbuff[128];
    recv(client_socket, recvbuff, 128, 0);
    cout<<recvbuff<<endl;
    pthread_t tid;
    pthread_create(&tid,NULL,thread_recv,NULL);
    while(1){
        string buffer;
        cin>>buffer;
        send(client_socket, buffer.c_str(), buffer.length(), 0);
    }
    
    return 0;
}