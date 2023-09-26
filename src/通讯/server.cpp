/*
 * @Author: victor victor@example.com
 * @Date: 2023-09-19 18:53:21
 * @LastEditors: victor victor@example.com
 * @LastEditTime: 2023-09-26 20:35:41
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
int number_of_rooms=1;//该变量是房间号
vector<Room> rooms;
int sockfd;
Room member_1;//全局数据转运数组
int epfd;
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
    sockfd =  socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        perror("socket");
        return -1;
    }
    //2.绑定
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9996);
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
    epfd = epoll_create(10);
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

    //登录状态记录数组
    int login_status[10];

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
                cout<<"socket"<<evt[i].data.fd<<endl;
                if(rsize <= 0)
                {
                    //客户端掉线
                    int eret = epoll_ctl(epfd, EPOLL_CTL_DEL, evt[i].data.fd, &evt[i]);
                    if(eret<0)
                    {
                        perror("del error");
                        return -1;
                    }
                    login_status[i]=0;
                    close(evt[i].data.fd);
                    cout<<"33"<<endl;
                    continue;
                }
                //数据处理
                int pw_sgin=0;
                Player f=way_choose(recvbuffer,player_buffer,evt[i].data.fd);
                if(f.name[0]!='\0')
                {
                    //登录成功
                    login_status[i]=1;
                }
                else if(login_status[i]==1)
                {
                    //选择观战者还是对战者
                    cout<<"1"<<endl;
                    pw_sgin=Decide_WatcherOrPlayer(recvbuffer);
                    cout<<pw_sgin<<endl;
                }
                //对战
                if(pw_sgin==1)
                {
                    if (rooms.empty()) {
                        std::cout << "动态数组为空" << std::endl;
                        //添加套接字
                        Player m;
                        m = f;
                        m.sockfd = evt[i].data.fd;
                        
                        //cout<<"socket"<<m.sockfd<<endl;
                        Room r;
                        r.sign=1;
                        r.people[0] = m;
                        rooms.push_back(r);
                        char color_member[]="black";
                        int eret = epoll_ctl(epfd, EPOLL_CTL_DEL, evt[i].data.fd, &evt[i]);
                        if(eret<0)
                        {
                            perror("del error");
                            return -1;
                        }
                        send(evt[i].data.fd,color_member,sizeof(color_member),0);
                        //cout<<"sockett"<<rooms[0].people->sockfd<<endl;
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
                                //这时就要判断到底是哪一个人走了
                                //是黑棋对手还是白棋对手
                                if (!member.people[0].is_on && member.num!=0)
                                {
                                    
                                    Player n;
                                    n = f;
                                    n.sockfd = evt[i].data.fd;
                                    n.is_on=1;
                                    member.people[0] = n;
                                    member.sign = 2;
                                    member_1  = member;
                                    cout<<"加入了黑棋玩家"<<endl;
                                    cout<<"该房间是遗老房间"<<endl;
                                    char color_member[]="black";
                                    send(evt[i].data.fd,color_member,sizeof(color_member),0);
                                    //创建线程？线程函数？
                                    //添加任务
                                    thread.add_task(pool1,Play_And_Communicate,(void*)&member.num);
                                    int eret = epoll_ctl(epfd, EPOLL_CTL_DEL, evt[i].data.fd, &evt[i]);
                                    if(eret<0)
                                    {
                                        perror("del error");
                                        return -1;
                                    }
                                    cout<<"该房间不可加入"<<endl;
                                    break;
                                }
                                else if (!member.people[1].is_on && member.num!=0)
                                {
                                    Player k;
                                    k = f;
                                    k.sockfd = evt[i].data.fd;
                                    k.is_on=1;
                                    member.people[1] = k;
                                    member.sign = 2;
                                    member_1  = member;
                                    cout<<"该房间是遗老房间"<<endl;
                                    cout<<"加入了白棋玩家"<<endl;
                                    char color_member[]="wlite";
                                    send(evt[i].data.fd,color_member,sizeof(color_member),0);
                                    //创建线程？线程函数？
                                    //添加任务
                                    thread.add_task(pool1,Play_And_Communicate,(void*)&member.num);
                                    int eret = epoll_ctl(epfd, EPOLL_CTL_DEL, evt[i].data.fd, &evt[i]);
                                    if(eret<0)
                                    {
                                        perror("del error");
                                        return -1;
                                    }
                                    cout<<"该房间不可加入"<<endl;
                                    break;
                                }
                                else
                                {
                                    Player m;
                                    m = f;
                                    m.sockfd = evt[i].data.fd;
                                    member.people[1] = m;
                                    member.sign = 2;
                                    member.num = number_of_rooms;
                                    cout<<"这不是遗老房间"<<endl;
                                    cout<<"该房间人数已满"<<endl;
                                    //创建线程？线程函数？
                                    //添加任务
                                    thread.add_task(pool1,Play_And_Communicate,(void*)&member.num);
                                    cout<<"该房间不可加入"<<endl;
                                    char color_member[]="wlite";
                                    send(evt[i].data.fd,color_member,sizeof(color_member),0);
                                    int eret = epoll_ctl(epfd, EPOLL_CTL_DEL, evt[i].data.fd, &evt[i]);
                                    if(eret<0)
                                    {
                                        perror("del error");
                                        return -1;
                                    }
                                    //已经完成插入，可以退出
                                    number_of_rooms++;
                                    break;
                                }
                                
                            }
                            
                        }
                            Player p;
                            p = f;
                            p.sockfd = evt[i].data.fd;
                            Room o;
                            o.people[0] = p;
                            o.sign = 1;
                            rooms.push_back(o);
                        
                    }
                }
                //观战
                else if(pw_sgin==2)
                {
                    for (Room& member :rooms)
                    {
                        cout<<"进入观战"<<endl;
                        if(member.num!=0 && member.wtach_sign<5 && member.sign==2)
                        {
                            char buff[]="12345";
                            int j=0;
                            string room_nums_str = to_string(member.num);
                            cout<<"房间号"<<room_nums_str<<endl;
                            for(Player& play :member.watch_people)
                            {
                                if(play.sockfd==0)
                                {
                                    cout<<"22"<<endl;
                                    play.sockfd = evt[i].data.fd;
                                    send(play.sockfd, &buff[j], 1, 0);
                                    send(play.sockfd,room_nums_str.c_str(),room_nums_str.length(),0);
                                    cout<<"观战成功"<<endl;
                                    int eret = epoll_ctl(epfd, EPOLL_CTL_DEL, evt[i].data.fd, &evt[i]);
                                    if(eret<0)
                                    {
                                        perror("del error");
                                        return -1;
                                    }
                                    break;
                                }
                                j++;
                            }    
                        }
                        break;
                    }
                    send(evt[i].data.fd,"没有观战房间可以进入", sizeof("没有观战房间可以进入"), 0);
                }
            }
        }
    }
}

//选择登录或者注册的模式，并返回玩家的信息结构体
Player  way_choose(char *recvbuffer,Player *buff,int scokfd)
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
    message[strlen(message)]='\0';

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
                    cout << "登录成功" << endl;
                    string message;
                    message="登录成功";
                    send(scokfd, message.c_str(), message.length(), 0);
                    return buff[i];
                }
            }
            i++;
        }
        string message;
        message="登录失败,账户或密码错误";
        send(scokfd, message.c_str(), message.length(), 0);
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
                string message;
                message="账户已存在";
                send(scokfd, message.c_str(), message.length(), 0);
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
        string message;
        message="注册成功";
        send(scokfd, message.c_str(), message.length(), 0);
        cout<<"注册成功"<<endl;
        
    }
    return emptyPlayer;
}
//对战
void* Play_And_Communicate(void *arg)
{
    int fd[10]={0};
    int mum=*(int *)arg;
    Player player_1 ;
    Player player_2 ;
    int room_num;
    int destory_flag =0;
    for(Room& member:rooms)
    {
        if(member.num==mum)
        {
            //拿到指定房间的特定信息
            member.people[0].is_on =1;
            member.people[2].is_on =1;
            player_1 = member.people[0];
            player_2 = member.people[1];
            room_num = member.num;
            break;
        }
    }
    cout<<player_1.sockfd<<endl;
    cout<<player_2.sockfd<<endl;
    fd[0]=player_1.sockfd;
    fd[1]=player_2.sockfd;
    cout<<fd[0]<<endl;
    cout<<fd[1]<<endl;
    //创建epoll实例
    int epfd_01 = epoll_create(10);
    if(epfd_01 <0)
    {
        perror("create");
    }

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = sockfd;
    int eret = epoll_ctl(epfd_01, EPOLL_CTL_ADD, sockfd, &event);
    if(eret<0)
    {
        perror("add error");
    }

    //分别将两个结构体的中的套接字加入监听队列
    //把监听套接字描述符添加到epoll监听队列
    event.events = EPOLLIN;
    event.data.fd = player_1.sockfd;
    eret = epoll_ctl(epfd_01, EPOLL_CTL_ADD, player_1.sockfd, &event);
    if(eret<0)
    {
        perror("add error01");
    }

    //添加第二个用户的套接字
    //把监听套接字描述符添加到epoll监听队列、
    event.events = EPOLLIN;
    event.data.fd = player_2.sockfd;
    int ret = epoll_ctl(epfd_01, EPOLL_CTL_ADD, player_2.sockfd, &event);
    if(ret<0)
    {
        perror("add error02");
    }
    

    while (!destory_flag)
    {
        //等待事件发生
        struct epoll_event evt[10];
        int size = epoll_wait(epfd_01, evt, 10, -1);
        if(size < 0)
        {
            perror("wait error");
            continue;
        }

        for(Room& member:rooms)
        {
            if(member.num==mum)
            {
                //拿到指定房间的特定信息
                for(Player& play :member.watch_people)
                {
                    cout<<"watch:"<<play.sockfd<<endl;
                    if(play.sockfd!=0 && play.is_on==0)
                    {
                        struct epoll_event event;
                        event.events = EPOLLIN;
                        event.data.fd = play.sockfd;
                        for(int i=0;i<10;i++)
                        {
                            if(fd[i]==0)
                            {
                                fd[i]=play.sockfd;
                                play.is_on=1;
                                break;
                            }
                        }
                        for(int i=0;i<10;i++)
                        {
                            cout<<fd[i]<<endl;
                        }
                        cout<<"join22"<<endl;
                        int eret = epoll_ctl(epfd_01, EPOLL_CTL_ADD, play.sockfd, &event);
                        if(eret<0)
                        {
                            perror("add error");
                        }
                    }
                }
            break;    
            }
        }

        //进行主游戏传输过程
        for (int i = 0; i < size; i++)
        {
            char recvbuffer[128]={0};
            //黑旗来消息
            if (evt[i].data.fd == player_1.sockfd)
            {
                int rsize = recv(evt[i].data.fd, recvbuffer, 128, 0);
                if(rsize <= 0)
                {
                    //客户端掉线
                    int eret = epoll_ctl(epfd_01, EPOLL_CTL_DEL, evt[i].data.fd, &evt[i]);
                    if(eret<0)
                    {
                        perror("del error");
                        break;
                    }
                    close(evt[i].data.fd);
                    for (auto it = rooms.begin(); it != rooms.end(); ++it) {
                        std::cout << "Room num = " << it->num << std::endl;
                        if (it->num==room_num)
                        {
                           //这里表示找到了房间
                            it->sign--;
                            it->people[0].is_on=0;
                            break;
                        }             
                    }
                }
                //数据处理
                char buffer_change[128]="Changeroom";
                //下面让所有人知道有玩家退出房间
                if (!strcmp(buffer_change,recvbuffer))
                {
                   //这里下面就进行换房间的操作,这里是黑棋想要换房间
                    destory_flag=handle_room_player(mum,destory_flag,0);
                    int eret = epoll_ctl(epfd_01, EPOLL_CTL_DEL, evt[i].data.fd, &evt[i]);
                    if(eret<0)
                    {
                        perror("del error");
                        break;
                    }
                    mutex1.lock();
                    struct epoll_event event;
                    event.events = EPOLLIN;
                    event.data.fd = player_1.sockfd;
                    eret = epoll_ctl(epfd, EPOLL_CTL_ADD, player_1.sockfd, &event);
                    if(eret<0)
                    {
                        perror("add error");
                    }
                    mutex1.unlock();
                    char buffer02[] = "black leave the room";
                    //向所有人进行广播，某个玩家离开房间
                    send_message_to_all_clients(fd,10,buffer02);
                    break;

                }
                printf("recv:%s\n", recvbuffer);
                send_message_to_all_clients(fd,10,recvbuffer);
                // send_message_to_all_clients(fd,10,recvbuffer);
                //这里可以考虑在服务器进行逻辑判断输赢
                //处理完成数据后，对另外一个客户端发送数据
            }
            else if(evt[i].data.fd == player_2.sockfd)//白棋来消息
            {

                int rsize = recv(evt[i].data.fd, recvbuffer, 128, 0);
                if(rsize <= 0)
                {
                    //客户端掉线
                    int eret = epoll_ctl(epfd_01, EPOLL_CTL_DEL, evt[i].data.fd, &evt[i]);
                    if(eret<0)
                    {
                        perror("del error");
                        break;
                    }
                    close(evt[i].data.fd);
                    for (auto it = rooms.begin(); it != rooms.end(); ++it) {
                        std::cout << "Room num = " << it->num << std::endl;
                        if (it->num==room_num)
                        {
                            //这里表示找到了房间
                            it->sign--;
                            it->people[1].is_on=0;
                            break;
                        }
                        
                    }
                }
                char buffer_change[128]="Changeroom";
                //下一局游戏游戏照常进行就行，因此不需要做处理，只需要对player中的分数给进行修改即可
                if (!strcmp(buffer_change,recvbuffer))
                {
                    //这里下面就进行换房间的操作,这里是黑棋想要换房间
                    destory_flag=handle_room_player(mum,destory_flag,1);
                    int eret = epoll_ctl(epfd_01, EPOLL_CTL_DEL, evt[i].data.fd, &evt[i]);
                    if(eret<0)
                    {
                        perror("del error");
                        break;
                    }
                    mutex1.lock();
                    struct epoll_event event;
                    event.events = EPOLLIN;
                    event.data.fd = player_2.sockfd;
                    eret = epoll_ctl(epfd, EPOLL_CTL_ADD, player_2.sockfd, &event);
                    if(eret<0)
                    {
                        perror("add error");
                    }
                    mutex1.unlock();
                    char buffer02[] = "white leave the room";
                    //向所有人进行广播，某个玩家离开房间
                    send_message_to_all_clients(fd,10,buffer02);
                    break;

                }
                //数据处理
                printf("recv2:%s\n", recvbuffer);
                send_message_to_all_clients(fd,10,recvbuffer);
                // send_message_to_all_clients(fd,10,recvbuffer);
            }
            //退出
            else 
            {
                int rsize = recv(evt[i].data.fd, recvbuffer, 128, 0);
                if(rsize <= 0)
                {
                    //客户端掉线
                    int eret = epoll_ctl(epfd_01, EPOLL_CTL_DEL, evt[i].data.fd, &evt[i]);
                    if(eret<0)
                    {
                        perror("del error");
                        break;
                    }
                    close(evt[i].data.fd);
                }
                //比对字符串
                
                char exit_message[]="watcher:out";
                if(strcmp(exit_message,recvbuffer)==0)
                {
                    for(Room& member:rooms)
                    {
                        if(member.num==mum)
                        {
                            for(Player& play :member.watch_people) 
                            if(play.sockfd==evt[i].data.fd)
                            {
                                cout<<evt[i].data.fd<<endl;
                                for(int a=0;a<10;a++)
                                {
                                    if(fd[a]==evt[i].data.fd)
                                    {
                                        cout<<fd[a]<<endl;
                                        cout<<evt[i].data.fd<<endl;
                                        fd[a]=0;
                                        cout<<"1231"<<endl;
                                        break;
                                    }
                                }
                                play.sockfd=0;
                                play.is_on=0;
                                int eret = epoll_ctl(epfd_01, EPOLL_CTL_DEL, evt[i].data.fd, &evt[i]);
                                if(eret<0)
                                {
                                    perror("del error");
                                    break;
                                }
                                mutex1.lock();
                                struct epoll_event event;
                                event.events = EPOLLIN;
                                event.data.fd = evt[i].data.fd;
                                eret = epoll_ctl(epfd, EPOLL_CTL_ADD, evt[i].data.fd, &event);
                                if(eret<0)
                                {
                                    perror("add error");
                                }
                                mutex1.unlock();
                            }
                            break;
                        }
                    }
                }

            }
        }
    }
}

//辨认观战还是对战
int Decide_WatcherOrPlayer(char *recvbuffer)
{
    cout<<recvbuffer<<endl;
    char watch_message[]="watch:game";
    char player_message[]="start:game";
    if(strcmp(watch_message,recvbuffer)==0)
    {
        return 2;
    }
    else if(strcmp(player_message,recvbuffer)==0)
    {
        return 1;
    }
    return  0;
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

// 清空 Room 结构体数组内所有成员的函数
void clearRoom(Room *room) {
    for (int i = 0; i < 2; i++) {
        // 清空 Player 结构体成员
        memset(room->people[i].name, 0, sizeof(room->people[i].name));
        memset(room->people[i].password, 0, sizeof(room->people[i].password));
        room->people[i].sockfd = 0;
        memset(room->people[i].play_score, 0, sizeof(room->people[i].play_score));
        memset(room->people[i].message, 0, sizeof(room->people[i].message));
    }
    room->sign = 0;
    room->num = 0;
}
//way:login,account:龙俊豪|password:111111
//way:register,account:龙俊豪|password:123456

//广播函数
void send_message_to_all_clients(int *sockfd, int num_clients, char* message) 
{
    for (int i = 0; i < num_clients; i++) 
    { 
        if(sockfd[i]!=0)
        {
            cout<<sockfd[i]<<endl;
            send(sockfd[i], message, strlen(message), 0);
            cout<<"send"<<endl;
        }
        
    }
}

//房间内的玩家的退出处理函数
int handle_room_player(int mum,int destory_flag,int player)
{
    for(Room& member:rooms)
    {
        if(member.num==mum)
        {
            if (member.sign!=0)
            {
                //拿到指定房间的特定信息
                member.people[player].is_on = 0;
                destory_flag = 1;
                member.sign--;
                break;
            }
        }
    }
    return destory_flag;
}
