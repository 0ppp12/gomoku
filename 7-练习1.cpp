/*** 
 * @Author: PENG
 * @Date: 2023-09-16 15:47:28
 * @LastEditTime: 2023-09-16 15:55:06
 * @LastEditors: PENG
 * @Description: 
 * @FilePath: \c++\day7\7-练习1.cpp
 * @Copyright(c)
 */
#include <iostream>
using namespace std;


template <typename T1, typename T2>
T1 myCmp(T1 a, T2 b) {
    return a>b?a:b;
}

int main(){
    double a = myCmp<double,double>(1,2.21);
    cout << a << endl;
}