/*
g++ md52.cpp&&./a.out
 */
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>


 /***********************************
 * 非线性函数
 * (&是与,|是或,~是非,^是异或)
 *
 * 这些函数是这样设计的：
 *   如果X、Y和Z的对应位是独立和均匀的，
 *   那么结果的每一位也应是独立和均匀的。
 *
 * 函数F是按逐位方式操作：如果X，那么Y，否则Z。
 * 函数H是逐位奇偶操作符
 **********************************/
#define F(x,y,z) ((x & y) | (~x & z))  
#define G(x,y,z) ((x & z) | (y & ~z))  
#define H(x,y,z) (x^y^z)  
#define I(x,y,z) (y ^ (x | ~z))  


 /**************************************
 *向左换移(右环移)n个单位
 * ************************************/
#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))  

 /****************************************************
 * 每次操作对a，b，c和d中的其中三个作一次非线性函数运算
 *  F(b,c,d)   G(b,c,d)   H(b,c,d)   I(b,c,d)
 *
 * 然后将所得结果加上 第四个变量(a)，
 * F(b,c,d)+a
 *
 * 文本的一个子分组(x)
 * F(b,c,d)+a+x
 *
 * 和一个常数(ac)。
 * F(b,c,d)+a+x+ac
 *
 * 再将所得结果向右环移一个不定的数(s)，
 * ROTATE_LEFT( F(b,c,d)+a+x+ac , s )
 *
 * 并加上a，b，c或d中之一(b)。
 * ROTATE_LEFT( F(b,c,d)+a+x+ac , s )+b
 *
 * 最后用该结果取代a，b，c或d中之一(a)。
 * a=ROTATE_LEFT( F(b,c,d)+a+x+ac , s )+b
 *
 * ***************************************************/
#define FF(a,b,c,d,x,s,ac) { a += F(b,c,d) + x + ac;  a = ROTATE_LEFT(a,s); a += b; }
#define GG(a,b,c,d,x,s,ac) { a += G(b,c,d) + x + ac;  a = ROTATE_LEFT(a,s); a += b; }
#define HH(a,b,c,d,x,s,ac) { a += H(b,c,d) + x + ac;  a = ROTATE_LEFT(a,s); a += b; }
#define II(a,b,c,d,x,s,ac) { a += I(b,c,d) + x + ac;  a = ROTATE_LEFT(a,s); a += b; }


 //储存一个MD5 text信息 
typedef struct{
    unsigned int count[2];
    //记录当前状态，其数据位数   

    unsigned int state[4];
    //4个数，一共32位 记录用于保存对512bits信息加密的中间结果或者最终结果  

    unsigned char buffer[64];
    //一共64字节，512位      
}MD5_CTX;


//第一位1 其后若干个0,用于MD5Final函数时的补足
unsigned char PADDING[]={0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


//函数声明
void MD5Init(MD5_CTX *context);
void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen);
void MD5Final(MD5_CTX *context,unsigned char digest[16]);
void MD5Transform(unsigned int state[4],unsigned char block[64]);
void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len);
void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len);