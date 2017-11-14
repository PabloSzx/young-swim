#ifndef MENU_H
#define MENU_H

#include <GLFW/glfw3.h>
#include <vector>
#include <string>
// #include "../../util/gltext/gltext.hpp"
#include "../sound/sound.hpp"
#include "../time/time.hpp"
//#include "../physics/physics.hpp"

extern bool restart;
extern int jumpButton;
extern int resetButton;
extern GLFWwindow *g_window;
extern sound** background;
extern float deltaTime;
//extern Bullet *platformWorld;
class Menu {
    private:
        Time* inputTimer;
        int status;
        int difficulty;
        int step;
        int maxStepStatus0;
        int maxStepStatus1;
        std::vector< std::vector<std::string> > text;
        GLTtext *label;
    public:
        Menu();
        void checkInput();
        int getMaxStep();
        void stepPlus();
        void stepMinus();
        void confirm();
        void drawText();
        void changeDifficulty(int n);
        void setColor(float r, float g, float b, float o);
        void setColor(float rgb, float o);
};



#endif
