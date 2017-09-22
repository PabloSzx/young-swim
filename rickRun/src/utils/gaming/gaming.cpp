#include "gaming.h"
#include <iostream>
#include <time.h>

using namespace std;
int Gaming::getN(int lastPos) {
    int second;
    int third;
    if (lastPos == 0) {
        second = 3;
        third = 6;
    } else if (lastPos == 3) {
        second = 6;
        third = 0;
    } else {
        second = 0;
        third = 3;
    }
    int nrandom = rand() % 10;
    if (nrandom < 2)
    {
        return lastPos;
    } else if (nrandom < 6) {
        return second;
    } 
    return third;
}

btVector3 Gaming::getPlatformPos(int lastZPlatform, int lastYPlatform, int x) {
    btVector3 ret = btVector3(x, getN(lastYPlatform), getN(lastZPlatform));
    return ret;
}

