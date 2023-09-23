/*
g++ 棋盘-落子是否合法.cpp&&./a.out
*/
#include <iostream>
using namespace std;
#define N 15 //棋盘规模
char gomoku[N][N];
int isDropLegal(int x,int y);
int emptyBoard();//清空棋盘
int main(){
    emptyBoard();
    int x,y;
    printf("client 请下棋(x,y)：\n");
    for(int i=0;i<10;i++){
        scanf("%d,%d",&x,&y);
        int flag=isDropLegal(x,y);
        if(flag==0){
            printf("落子合法，不提示，允许落子\n");
            gomoku[x][y]='0';
        }
        if(flag==1){
            printf("提示用户范围不对，让他重新输\n");
        }
        if(flag==2){
            printf("提示用户这个位置已经有棋子了，让他重新输\n");
        }
    }
}
int isDropLegal(int x,int y){
    if(x<1||x>N-1||y<1||y>N-1){
        return 1;
    }
    if(gomoku[x][y]!='_'){
        return 2;
    }
    return 0;
}
int emptyBoard(){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            gomoku[i][j]='_';
        }
    }
    return 0;
}