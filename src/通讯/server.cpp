/*
 * @Author: victor victor@example.com
 * @Date: 2023-09-19 18:53:21
 * @LastEditors: victor victor@example.com
 * @LastEditTime: 2023-09-22 17:14:57
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
#include "thread_pool.h"
using namespace std;
std::mutex mutex1;
int game_flag=0;
int number_of_rooms;//该变量是房间号
vector<Room> rooms;

int main(void)
{
    Player player_buffer[50]; 
    thread_pool *pool1 = new thread_pool;//初始化结构体 
    ThreadPool thread;//使用类对象                                                                                                                          
    if(!File_read("./info.txt",player_buffer))
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

    //初始化线程
    int ret1 = thread.init_pool(pool1,10);
    if (ret1<=0)
    {
        perror("init_pool failed!!!\n");
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
                Player f=way_choose(recvbuffer,player_buffer);
                cout<<"1"<<endl;
                if(f.name[0]!='\0')
                {
                    //登录成功
                    game_flag=1;
                    cout<<game_flag<<endl;
                }
                if(game_flag==1)
                {
                    if (rooms.empty()) {
                        std::cout << "动态数组为空" << std::endl;
                        //添加套接字
                        Player m;
                        m = f;
                        m.sockfd = evt[i].data.fd;
                        Room r;
                        r.people[0] = m;
                        rooms.push_back(r);
                    } else {
                        std::cout << "动态数组不为空" << std::endl;
                        for (Room& member :rooms) {
                            int signValue = member.sign;
                            std::cout << "sign值: " << signValue << std::endl;
                            if (signValue==2)
                            {
                                //房间已满
                                continue;
                            }
                            else if (signValue==1)
                            {
                                //房间未满，还差一个人就满
                                Player n;
                                n = f;
                                n.sockfd = evt[i].data.fd;
                                member.people[1] = n;
                                member.sign = 2;
                                member.num = number_of_rooms;
                                Room member_1  = member;
                                //创建线程？线程函数？
                                //添加任务
                                int ret2 = thread.add_task(pool1,Play_And_Communicate, static_cast<void*>(&member_1));
                                if (ret2<=0)
                                {
                                    perror("init_pool failed!!!\n");
                                    return -1;
                                }

                                //已经完成插入，可以退出
                                number_of_rooms++;
                                break;
                                
                            }
                            
                        }
                            Player p;
                            p = f;
                            p.sockfd = evt[i].data.fd;
                            Room o;
                            o.people[0] = p;
                            rooms.push_back(o);
                        
                    }
                    game_flag = 0;
                }
            }
        }
    }
}

//选择登录或者注册的模式，并返回玩家的信息结构体
Player  way_choose(char *recvbuffer,Player *buff)
{
    Player emptyPlayer;
    memset(emptyPlayer.name,0,sizeof(emptyPlayer));

    char buffer[128]={0};
    strcpy(buffer,recvbuffer);
    //分离数组内容，验证是否合法加入
    char *way = strtok(buffer, ",");
    if (strcmp(way,recvbuffer)==0) {
        return emptyPlayer;
    }
    char *message = strtok(NULL,"\0");
    message[strlen(message)-1]='\0';

    cout<<message<<endl;
    char name[10];
    char password[10]; 
    int i=0;
    
    char delimiters[] = " ,|:"; // 分隔符可以是空格、逗号、问号和感叹号
    char* token = std::strtok(message, delimiters);
    int b=0;
    while (token != nullptr) {
        if(b==1)
        {
            strcpy(name,token);
        }
        if(b==3)
        {
            strcpy(password,token);
            break;
        }
        token = std::strtok(nullptr, delimiters);
        b++;
    }
    //登录
    if (strcmp(way,"way:login") == 0)
    {
        while(strlen(buff[i].name)!=0)
        {    
            if (strcmp(buff[i].name, name) == 0)
            {
                if(strcmp(buff[i].password, password) == 0)
                {
                    cout <<"2"<<endl;
                    cout << "登录成功" << endl;
                    return buff[i];
                }
            }
            i++;
        }
        cout << "登录失败" << endl;
    }
    // //注册
    else if(strcmp(way,"way:register")==0)
    {
        strcat(message,":");
        strcat(message,name);
        strcat(message,"|password:");
        strcat(message,password);
        cout<<message<<endl;
        while(strlen(buff[i].name)!=0)
        {    
            if (strcmp(buff[i].name, name) == 0)
            {
                cout<<"账户已存在"<<endl;
                return emptyPlayer;
            }
            i++;
        }
        if(File_write("./info.txt",message))
        {
            if(!File_read("./info.txt",buff))
            {
                cout<<"读取文件失败"<<endl;
            }
        }
        cout<<"注册成功"<<endl;
    }
    return emptyPlayer;
}

void* Play_And_Communicate(void *arg)
{
    Room& play_room =(Room &)arg;

    Player& player_1 = play_room.people[0];
    Player& player_2 = play_room.people[1];
    
    //创建epoll实例
    int epfd_01 = epoll_create(10);
    if(epfd_01 <0)
    {
        perror("create");
    }

    //分别将两个结构体的中的套接字加入监听队列
    //把监听套接字描述符添加到epoll监听队列
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = player_1.sockfd;
    int eret = epoll_ctl(epfd_01, EPOLL_CTL_ADD, player_1.sockfd, &event);
    if(eret<0)
    {
        perror("add error");
    }


    //添加第二个用户的套接字
    //把监听套接字描述符添加到epoll监听队列
    struct epoll_event event1;
    event1.events = EPOLLIN;
    event1.data.fd = player_2.sockfd;
    int ret = epoll_ctl(epfd_01, EPOLL_CTL_ADD, player_2.sockfd, &event1);
    if(ret<0)
    {
        perror("add error");
    }

    while (1)
    {
        //等待事件发生
        struct epoll_event evt[10];
        int size = epoll_wait(epfd_01, evt, 10, -1);
        if(size < 0)
        {
            perror("wait error");
            continue;
        }

        //进行主游戏传输过程
        for (int i = 0; i < size; i++)
        {
            //白旗来消息
            if (evt[i].data.fd == player_1.sockfd)
            {
                cout<<"while"<<endl;
            }
            else//黑棋来消息
            {
                cout<<"black"<<endl;
            }
            
        }
        
    }
    


}

//得到名字与密码
void Get_NameAndPassword(char *recvbuffer)
{
    char delimiters[] = " ,|:"; // 分隔符可以是空格、逗号、问号和感叹号
    char* token = std::strtok(recvbuffer, delimiters);
    char name[10];
    char password[10];
    int a=0;
    while (token != nullptr) {
        if(a==3)
        {
            strcpy(name,token);
        }
        if(a==5)
        {
            strcpy(password,token);
            break;
        }
        token = std::strtok(nullptr, delimiters);
        a++;
    }
    cout<<name<<endl;
    cout<<password<<endl;
}

bool File_read(string filename,Player *buff)
{
    mutex1.lock();
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
        char delimiters[] = " ,|:"; // 分隔符可以是空格、逗号、问号和感叹号
        char* token = std::strtok(buffer, delimiters);
        int b=0;
        while (token != nullptr) {
            if(b==1)
            {
                strcpy(buff[a].name,token);
            }
            if(b==3)
            {
                strcpy(buff[a].password,token);
            }
            if(b==5)
            {
                strcpy(buff[a].play_score,token);
            }
            token = std::strtok(nullptr, delimiters);
            b++;
        }
        a++;  
    }
    in.close();
    mutex1.unlock();
    return true;
}

bool File_write(string filename,string info)
{
    mutex1.lock();
    //写文件
    ofstream out(filename,std::ios::app);
    if(!out.is_open())
    {
        cout<<"写打开失败"<<endl;
        return false;
    }
    out<<info+"|score:0";
    out<<'\n';
    out.close();
    mutex1.unlock();
    return true;
}
//way:login,account:龙俊豪|password:111111
//way:register,account:龙俊豪|password:123456