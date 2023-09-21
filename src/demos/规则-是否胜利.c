
#include<stdio.h>
#define N 15 //棋盘规模
//'*'表示黑子，'0'表示白子；
// x,y：落子坐标x：1-15，y:1-15 gomoku棋局地址 flagWin胜方('0'/'*')
int checkWin(int x,int y,char gomoku[N][N],char flagWin);//检查棋子x，y落下后是否胜利
int main(){
    char gomoku[N][N];
	//测试：五个黑子，5,1为最后落子位置
    for(int i=1;i<5;i++){//1,1 2,1 3,1 4,1   5,1
        gomoku[i][1]='*';
    }

    if(checkWin(5,1,gomoku,'*'))
        printf("5,1 win1");
    printf("\n");

    gomoku[3][1]='0';
    if(checkWin(5,1,gomoku,'*'))
        printf("5,1 win2");
	printf("\n");

    for(int i=1;i<5;i++){
        gomoku[i][6-i]='*';//1,5 2,4 3,3 4,2   5,1
    }
    if(checkWin(5,1,gomoku,'*'))
        printf("5,1 win3");
    printf("\n");
    gomoku[3][3]='0';
    if(checkWin(5,1,gomoku,'*'))
        printf("5,1 win4");

    for(int i=1;i<5;i++){
        gomoku[i][i]='*';//1,1 2,2 3,3 4,4   5,5
    }
    if(checkWin(5,5,gomoku,'*'))
        printf("5,5 win5");
    printf("\n");
    gomoku[3][3]='0';
    if(checkWin(5,5,gomoku,'*'))
        printf("5,5 win6");
    return 0;
}
int checkWin(int x,int y,char gomoku[N][N],char flagWin){
	int i,j,cou1=0;
	for(i=y+1;i<N;i++)//'*'落子位置正下方
		if(gomoku[x][i]==flagWin)
			cou1++;
		else break;
	for(i=y-1;i>0;i--)//正上方
		if(gomoku[x][i]==flagWin)
			cou1++;
		else break;
	if(cou1>=4) return 1;//竖
	cou1=0;

	for(i=x+1;i<N;i++)//正右方
		if(gomoku[i][y]==flagWin)
			cou1++;
		else break;
	for(i=x-1;i>0;i--)//正左方
		if(gomoku[i][y]==flagWin)
			cou1++;
		else break;
	if(cou1>=4) return 1;//横5
	cou1=0;

	for(i=x+1,j=y+1;i<N,y<N;i++,j++)//右下
		if(gomoku[i][j]==flagWin)
			cou1++;
		else break;
	for(i=x-1,j=y-1;i>0,j>0;i--,j--)//左上
		if(gomoku[i][j]==flagWin)
			cou1++;
		else break;
	if(cou1>=4) return 1;//  \5
	cou1=0;

	for(i=x-1,j=y+1;i>0,j<N;i--,j++)//左下
		if(gomoku[i][j]==flagWin)
			cou1++;
		else break;
	for(i=x+1,j=y-1;i<N,j>0;i++,j--)//右上
		if(gomoku[i][j]==flagWin)
			cou1++;
		else break;
	if(cou1>=4) return 1;// /5
	return 0;//胜负未定
}