#include <iostream>
#include "touchscreen.h"


int main(void){
    TsDevice& TS = TsDevice::TsInstance();
    while (1)
    {
        TS.TS_GetVal();
    }
}