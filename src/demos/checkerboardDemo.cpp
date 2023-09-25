/*
g++ checkerboardDemo.cpp checkerboard.cpp&&./a.out
 */
#include <iostream>
#include "checkerboard.h"
#include "player.h"
void gomoku_show(char gomoku[][N]);
using namespace std;
int main(){
	Checkerboard checkerboard;
	checkerboard.gomoku[5][7]='B';//落子
	checkerboard.gomoku_show();
	// for(int i=1;i<=5;i++){
	for(int i=0;i<=4;i++){
		int x=i,y=0;
		if(checkerboard.isDropLegal(x,y)){
			checkerboard.gomoku[x][y]='B';
			if(checkerboard.checkWin(x,y,'B')){//检查胜负
				printf("我赢了\n");
			}
		}
	}
	checkerboard.gomoku_show();
	int x,y;
	while(1){
		scanf("%d,%d",&x,&y);
		if(checkerboard.isDropLegal(x,y)){
			checkerboard.gomoku[x][y]='B';
			if(checkerboard.checkWin(x,y,'B')){//检查胜负
				printf("我赢了\n");
			}
		}else{
			continue;;
		}
		checkerboard.gomoku_show();
	}
}
