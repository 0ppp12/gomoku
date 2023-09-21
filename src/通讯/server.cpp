/*
 * @Author: victor victor@example.com
 * @Date: 2023-09-19 18:53:21
 * @LastEditors: victor victor@example.com
 * @LastEditTime: 2023-09-21 16:18:19
 * @FilePath: \work\stage5\game-project\the-gobang-game-of-cc-md-fk\src\ͨѶ\server.cpp
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 * @Author: victor victor@example.com
 * @Date: 2023-09-19 18:53:21
 * @LastEditors: victor victor@example.com
 * @LastEditTime: 2023-09-21 14:04:58
 * @FilePath: \work\stage5\game-project\the-gobang-game-of-cc-md-fk\��Ŀ����\server.cpp
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AEn
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
        cout<<"��ȡ�ļ�ʧ��"<<endl;
    }
    //1.�����׽���
    int sockfd =  socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        perror("socket");
        return -1;
    }
    //2.��
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

    //3.����
    ret = listen(sockfd, 5);
    if(ret < 0)
    {
        perror("listen");
        return -1;
    }

    //����epollʵ��
    int epfd = epoll_create(10);
    if(epfd <0)
    {
        perror("create");
        return -1;
    }
    
    //�Ѽ����׽�����������ӵ�epoll��������
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
        //�ȴ��¼�����
        struct epoll_event evt[10];
        int size = epoll_wait(epfd, evt, 10, -1);
        if(size < 0)
        {
            perror("wait error");
            continue;
        }

        for(int i=0; i<size; i++)
        {
            //�ж��Ƿ��Ǽ����׽���sockfd
            if(evt[i].data.fd == sockfd)
            {
                //4.�������ӣ�Ĭ����������
                struct sockaddr_in clientaddr;
                socklen_t len = sizeof(clientaddr);
                int clientfd = accept(sockfd, (struct sockaddr *)(&clientaddr), &len);
                if(clientfd < 0)
                {
                    perror("accept");
                    return -1;
                }
                printf("�ͻ���IP:%s\n", inet_ntoa(clientaddr.sin_addr));
                //����ͻ���ͨ�ŵ��׽���������Ҳ��ӵ�����������
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
            else//�ͻ���������
            {
                //��ȡ�ͻ�������
                char recvbuffer[128]={0};
                int rsize = recv(evt[i].data.fd, recvbuffer, 128, 0);
                if(rsize <= 0)
                {
                    //�ͻ��˵���
                    int eret = epoll_ctl(epfd, EPOLL_CTL_DEL, evt[i].data.fd, &evt[i]);
                    if(eret<0)
                    {
                        perror("del error");
                        return -1;
                    }
                    close(evt[i].data.fd);
                    continue;
                }
                //���ݴ���
                if(way_choose(recvbuffer,info_buffer)==2)
                {
                    //�ٴεȴ���¼��Ϣ
                    continue;
                }
                else if(way_choose(recvbuffer,info_buffer)==1)
                {
                    //��¼�ɹ�
                    game_flag=1;
                }
                if(game_flag==1)
                {
                    //����׽���
                    vector<Room> rooms;
                    
                    game_flag = 0;
                }
            }
        }
    }
}

//��¼��ע��ѡ��
int way_choose(char *recvbuffer,std::string *buff)
{
    //�����������ݣ���֤�Ƿ�Ϸ�����
    char *way = strtok(recvbuffer, ",");
    char *message = strtok(NULL,"\0");
    cout<<way<<endl;
    cout<<message<<endl;
    int i=0;
    //��¼
    if (strcmp(way, "way:login") == 0)
    {
        while(buff[i].length()!=0)
        {
            if (strcmp(buff[i].c_str(), message) == 0)
            {
                cout << "��¼�ɹ�" << endl;
                return 1;
            }
            i++;
        }
        cout << "��¼ʧ��" << endl;
    }
    //ע��
    else if(strcmp(way,"way:register")==0)
    {
        if(File_write("./info.txt",message))
        {
            if(!File_read("./info.txt",buff))
            {
                cout<<"��ȡ�ļ�ʧ��"<<endl;
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
     //1.�����ļ�������--���ļ�
    ifstream in(filename);
    if(!in.is_open())
    {
        cout<<"��ʧ��"<<endl;
        return false;
    }

    //2.���ļ�
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
    //д�ļ�
    ofstream out(filename,std::ios::app);
    if(!out.is_open())
    {
        cout<<"д��ʧ��"<<endl;
        return false;
    }
    out<<info;
    out<<'\n';
    out.close();
    m.unlock();
    return true;
}
