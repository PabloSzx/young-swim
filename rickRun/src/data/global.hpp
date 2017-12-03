#ifndef GLOBALS_H
#define GLOBALS_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <omp.h>
#include <thread>

#include "../components/physics/physics.hpp"
#include "../containers/world/world.hpp"
#include "../components/time/time.hpp"
#include "../components/menu/menu.hpp"
#include "../components/parameters/parameters.hpp"
#include "../components/model/model.hpp"

// #define fullhd //comentar esta linea si se desea 768p

#ifdef fullhd
int g_gl_width = 1920;
int g_gl_height = 1080;
#else
int g_gl_width = 1366;
int g_gl_height = 768;
#endif

GLFWwindow *g_window = NULL; //main

omp_lock_t loading;

glm::vec3 sun = glm::vec3(0.7f, -12.2f, 2.3f);
// Sounds
// sound *snd_01 = new sound((const char*)"audio/RICK_RUN2.wav");
// sound *snd_02 = new sound((const char*)"audio/RICK_RUN1.wav");

sound **background = static_cast<sound **>(malloc(sizeof(sound *) * 9));;;

World *core;

Parameters *rules;
// camera
// glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
// glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
// glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraPos = glm::vec3(-14.4f, 2.52f, -4.3f);
glm::vec3 cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 cameraFrontInit = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool joystickPresent = false;

bool mouseIn = false;

bool allowJump = true;
bool allowDownJump = true;
double jumpHorizontalForce;
double jumpVerticalUpForce;
double jumpVerticalDownForce;
double jumpForwardForce;
double jumpBackwardForce;

Model *ys;
Model *ys1;
Model *ys2;
Model *ys3;

Model *young_swim;
Model *logo;

Menu *menu;
bool debug = false;
bool restart;
int globalStatus = 0; //0: menu previo, 1: menu play again, 2: playing
bool fullscreen = true;

Time *timer;
int jumpButton = -1;
int resetButton = -1;
bool firstMouse = true;
// float yaw = -90.0f; // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
// float pitch = 0.0f;
float yaw = 0; // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = -20.0f;

float lastX = g_gl_width / 2.0;
float lastY = g_gl_height / 2.0;
// float lastX = 1373.54;
// float lastY = 607.734;

float fov = 45.0f;

int estadoRick = 0;
double vx = 0.0;
double vy = 0.0;
double vz = 0.0;

double fps = 1500.0;
float _fps;
float _maxFps = 15.0f;
float _frameTime;

bool plus50 = false;

bool cameraFree = false;

bool played = false;
// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

int maxrand = 10;

GLuint shader_programme;
GLuint shader_programme_cube;

GLuint color;
GLuint sunLocation;
int proj_mat_location;
int view_mat_location;


glm::mat4 view;
glm::mat4 projection;

Bullet *platformWorld; //MUNDO DE RICK Y SUS PLATAFORMAS... AQUI RICK RECIBE GRAVEDAD Y LAS PLATAFORMAS SE MUEVEN XD
Bullet *parallaxHouses;
Bullet *parallaxProps;
Bullet *distanceScore;

int cube_V_location;
int cube_P_location;

GLuint cube_sp;
GLuint cube_vao;
GLuint cube_map_texture;

void progressLoading(int a) {
    window_clear(0.05f);
    // int n = a;
    // char barra[] = "[----------]";
    // char barra[12] = [ "|", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "|" ];
    // string barra("|          |");

    // for (int i = 1; i < n + 1; i++)
    // {
    //     barra[i] = '#';
    // }
    switch (a) {
        case 0:
        {
            ys->setpos(glm::vec3(-3.0f, 1.0f, -4.3f));
            ys->draw();

            break;
        }
        case 1:
        {
            ys1->setpos(glm::vec3(-3.0f, 1.0f, -4.3f));
            ys1->draw();

            break;
        }
        case 2:
        {
            ys2->setpos(glm::vec3(-3.0f, 1.0f, -4.3f));
            ys2->draw();

            break;
        }
        case 3:
        {
            ys3->setpos(glm::vec3(-3.0f, 1.0f, -4.3f));
            ys3->draw();

            break;
        }
        case 4:
        {
            young_swim->setpos(glm::vec3(-3.0f, 1.0f, -4.3f));
            young_swim->draw();

            break;
        }
        // case 5:
        // {
        //     ys->setpos(glm::vec3(-3.0f, 1.0f, -4.3f));
        //     ys->draw();

        //     break;
        // }
    }

    // menu->drawArbitrary(g_gl_width * 0.6, g_gl_height * 0.8, 5, const_cast<char *>("Cargando..."));
    // menu->drawArbitrary(g_gl_width * 0.3, g_gl_height * 0.8, 5, const_cast<char *>(barra));

    logo->setpos(glm::vec3(-3.0f, 5.0f, -4.3f));

    logo->draw();


    window_swap();
}

void drawArbitrary(int x, int y, double size, char* str) {
    menu->drawArbitrary(x, y, size, str);
}

#endif
