/*
 * @Author: victor victor@example.com
 * @Date: 2023-09-19 18:53:33
 * @LastEditors: victor victor@example.com
 * @LastEditTime: 2023-09-21 14:05:37
 * @FilePath: \work\stage5\game-project\the-gobang-game-of-cc-md-fk\src\client.cpp
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include "server.h"

int main()
{
    //�����׽���
    int client_socket;
    struct sockaddr_in server_address;

    // �����ͻ����׽���
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) 
    {
        perror("socket");
        return 1;
    }

    // ���÷�������ַ
    server_address.sin_family = AF_INET;
    // server_address.sin_addr.s_addr = inet_addr("192.168.13.64");
    if (inet_pton(AF_INET, "192.168.13.64", &(server_address.sin_addr)) <= 0)
    {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return 1;
    }
    server_address.sin_port = htons(11451);

    // ���ӵ�������
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) 
    {
        perror("connect");
        return 1;
    }

    //��־��0Ϊע�ᣬ1Ϊ��¼
    int login_flag;
    std::cin>>login_flag;
     //���Ϳͻ�������
    Send_info(client_socket,login_flag);

    close(client_socket);
    return 0;
}
//��¼��ע�ᷢ��
bool Send_info(int client_socket,int a)
{
    std::string client_info;
    client_info.clear();
    std::string client_name;
    std::cin >> client_name;
    std::string client_password;
    std::cin >> client_password;
    if(a==1)
    {
        client_info = "way:login,account:"+ client_name + "|password:" + client_password;
    }
    else if(a==0)
    {
        client_info = "way:register,account:"+ client_name + "|password:" + client_password;
    }
    std::cout<<client_info<<" "<<client_info.length()<< std::endl;
    if(send(client_socket, client_info.c_str(), client_info.length(), 0)<0)
    {
        std::cout<<"��¼��Ϣ����ʧ��"<<std::endl;
        return false;
    }
    return true;
}
