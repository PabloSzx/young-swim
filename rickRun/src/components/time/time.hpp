#ifndef TIME_H
#define TIME_H

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

class Time {
private:
    double now;
    double last;
    bool first;
public:
    Time();
    void updateNow();
    double getUpdateNow();
    double getLast();
    void lastIsNow();
    void setGlobalTime(double t);
    bool checkFirstTime(double t);
    bool every(double t);
    void restart();
};

#endif
