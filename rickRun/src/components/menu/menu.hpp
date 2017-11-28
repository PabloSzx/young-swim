#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h> // GLFW helper library
#include "../../util/gltext/gltext.hpp"
#include "../sound/sound.hpp"
#include "../time/time.hpp"
#include <sstream>
#include "../parameters/parameters.hpp"
//#include "../physics/physics.hpp"

extern bool restart;
extern int jumpButton;
extern int resetButton;
extern GLFWwindow *g_window;
extern sound** background;
extern float deltaTime;
extern int globalStatus;
extern Parameters* rules;
//extern Bullet *platformWorld;
class Menu {
    private:
        Time* inputTimer;
        int difficulty;
        std::string difficultyName;
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
        void drawText(int distance);
        void changeDifficulty(int n);
        void setColor(float r, float g, float b, float o);
        void setColor(float rgb, float o);
        void restartTime();
        void setGlobalStatus(int n, const char* m);
        std::string getDifficultyName();

        void drawArbitrary(int x, int y, double size, int n);
        void drawArbitrary(int x, int y, double size, char* str);
};



#endif
