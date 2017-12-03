#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h> // GLFW helper library

#include <omp.h>
#include <thread>
#include "../../util/gltext/gltext.hpp"
#include "../sound/sound.hpp"
#include "../time/time.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include "../parameters/parameters.hpp"
#include "../model/model.hpp"
//#include "../physics/physics.hpp"

extern bool highscore;
extern int highscorePuntaje;
extern int g_gl_width;
extern int g_gl_height;
extern bool restart;
extern bool played;
extern int jumpButton;
extern int resetButton;
extern GLFWwindow *g_window;
extern sound** background;
extern float deltaTime;
extern int globalStatus;
extern Parameters* rules;
extern omp_lock_t loading;
extern Model *young_swim;
extern Model *logo;
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
        void setGlobalStatus(int n);
        void setGlobalStatus(int n, const char* m);
        std::string getDifficultyName();
        void loadRickMeshes(); 
        void drawArbitrary(int x, int y, double size, int n);
        void drawArbitrary(int x, int y, double size, char* str);
};



#endif
