#pragma once

#include <iostream>
#define TS_PATH	    "/dev/input/event0"  //指的是触摸屏的文件路径

class TsDevice{
    TsDevice(std::string devnam = TS_PATH);
public:
    int TS_GetVal();
    static TsDevice& TsInstance();
public:
    int ts_x;
    int ts_y;
private:
    int ts_fd;              //触摸屏文件描述符
    std::string devname;    //触摸屏设备文件
};