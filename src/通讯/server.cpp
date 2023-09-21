/*
 * @Author: victor victor@example.com
 * @Date: 2023-09-19 18:53:21
 * @LastEditors: victor victor@example.com
 * @LastEditTime: 2023-09-21 16:18:19
 * @FilePath: \work\stage5\game-project\the-gobang-game-of-cc-md-fk\src\通讯\server.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 * @Author: victor victor@example.com
 * @Date: 2023-09-19 18:53:21
 * @LastEditors: victor victor@example.com
 * @LastEditTime: 2023-09-21 14:04:58
 * @FilePath: \work\stage5\game-project\the-gobang-game-of-cc-md-fk\项目代码\server.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AEn
 */
#include <iostream>
#include "server.h"
using namespace std;
std::mutex m;
int game_flag=0;

int main(void)
{
    string info_buffer[100];                                                                                                                               
    if(!File_read("./info.txt",info_buffer))
    {
        cout<<"读取文件失败"<<endl;
    }
    //1.创建套接字
    int sockfd =  socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        perror("socket");
        return -1;
    }
    //2.绑定
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(11451);
    addr.sin_addr.s_addr = INADDR_ANY;
    int ret =  bind(sockfd, (struct sockaddr*)(&addr),sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        return -1;
    }

    //3.监听
    ret = listen(sockfd, 5);
    if(ret < 0)
    {
        perror("listen");
        return -1;
    }

    //创建epoll实例
    int epfd = epoll_create(10);
    if(epfd <0)
    {
        perror("create");
        return -1;
    }
    
    //把监听套接字描述符添加到epoll监听队列
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = sockfd;
    int eret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event);
    if(eret<0)
    {
        perror("add error");
        return -1;
    }

    while(1)
    {
        //等待事件发生
        struct epoll_event evt[10];
        int size = epoll_wait(epfd, evt, 10, -1);
        if(size < 0)
        {
            perror("wait error");
            continue;
        }

        for(int i=0; i<size; i++)
        {
            //判断是否是监听套接字sockfd
            if(evt[i].data.fd == sockfd)
            {
                //4.接受连接（默认是阻塞）
                struct sockaddr_in clientaddr;
                socklen_t len = sizeof(clientaddr);
                int clientfd = accept(sockfd, (struct sockaddr *)(&clientaddr), &len);
                if(clientfd < 0)
                {
                    perror("accept");
                    return -1;
                }
                printf("客户端IP:%s\n", inet_ntoa(clientaddr.sin_addr));
                //把与客户端通信的套接字描述符也添加到监听队列中
                struct epoll_event event;
                event.events = EPOLLIN;
                event.data.fd = clientfd;
                int eret = epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &event);
                if(eret<0)
                {
                    perror("add error");
                    return -1;
                }
            }
            else//客户端来数据
            {
                //读取客户端数据
                char recvbuffer[128]={0};
                int rsize = recv(evt[i].data.fd, recvbuffer, 128, 0);
                if(rsize <= 0)
                {
                    //客户端掉线
                    int eret = epoll_ctl(epfd, EPOLL_CTL_DEL, evt[i].data.fd, &evt[i]);
                    if(eret<0)
                    {
                        perror("del error");
                        return -1;
                    }
                    close(evt[i].data.fd);
                    continue;
                }
                //数据处理
                if(way_choose(recvbuffer,info_buffer)==2)
                {
                    //再次等待登录信息
                    continue;
                }
                else if(way_choose(recvbuffer,info_buffer)==1)
                {
                    //登录成功
                    game_flag=1;
                }
                if(game_flag==1)
                {
                    //添加套接字
                    vector<Room> rooms;
                    
                    game_flag = 0;
                }
            }
        }
    }
}

//登录与注册选择
int way_choose(char *recvbuffer,std::string *buff)
{
    //分离数组内容，验证是否合法加入
    char *way = strtok(recvbuffer, ",");
    char *message = strtok(NULL,"\0");
    cout<<way<<endl;
    cout<<message<<endl;
    int i=0;
    //登录
    if (strcmp(way, "way:login") == 0)
    {
        while(buff[i].length()!=0)
        {
            if (strcmp(buff[i].c_str(), message) == 0)
            {
                cout << "登录成功" << endl;
                return 1;
            }
            i++;
        }
        cout << "登录失败" << endl;
    }
    //注册
    else if(strcmp(way,"way:register")==0)
    {
        if(File_write("./info.txt",message))
        {
            if(!File_read("./info.txt",buff))
            {
                cout<<"读取文件失败"<<endl;
                return false;
            }
            return 2;
        }
    }
    return false;
}

bool File_read(string filename,string *buff)
{
    m.lock();
     //1.创建文件输入流--读文件
    ifstream in(filename);
    if(!in.is_open())
    {
        cout<<"打开失败"<<endl;
        return false;
    }

    //2.读文件
    char buffer[128];
    int a=0;
    while(in>>buffer){  
        buff[a]=buffer;
        a++;  
    }
    
    in.close();
    m.unlock();
    return true;
}

bool File_write(string filename,string info)
{
    m.lock();
    //写文件
    ofstream out(filename,std::ios::app);
    if(!out.is_open())
    {
        cout<<"写打开失败"<<endl;
        return false;
    }
    out<<info;
    out<<'\n';
    out.close();
    m.unlock();
    return true;
}
