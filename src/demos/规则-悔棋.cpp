//悔棋(未加请求对方同意)
#include<stdio.h>
#include<iostream>
#include<vector>
#define N 15 //棋盘规模
using namespace std;
char gomoku[N][N];//棋盘
static vector<pair<int,pair<int,int>>> avector;//棋子顺序位置表
static int m=0;//棋盘上的棋子数
int saveTract(int x,int y);//记录棋子位置
int retract();//悔棋
int main(){
    saveTract(1,1);
    gomoku[1][1]='*';

    saveTract(1,2);
    gomoku[1][2]='0';

    printf("%c \n",gomoku[1][1]);
    printf("%c \n",gomoku[1][2]);
    retract();//悔棋
    printf("%c \n",gomoku[1][1]);
    printf("%c \n",gomoku[1][2]);
    retract();//悔棋
    printf("%c \n",gomoku[1][1]);
    printf("%c \n",gomoku[1][2]);
    return 0;
}
int saveTract(int x,int y){
    ++m;
    pair<int,pair<int,int>> apair=make_pair(m,make_pair(x,y));
    avector.push_back(apair);
    cout<<apair.first<<" "<<apair.second.first<<","<<apair.second.second<<endl;
}
int retract(){
    pair<int,pair<int,int>> apair=avector.back();
    avector.pop_back();
    cout<<apair.first<<" "<<apair.second.first<<","<<apair.second.second<<endl;
    gomoku[apair.second.first][apair.second.second]='_';//置空最近落子
    --m;
}