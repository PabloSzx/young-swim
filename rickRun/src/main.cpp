#include <assimp/cimport.h>     // C importer
#include <assimp/scene.h>       // collects data
#include <assimp/postprocess.h> // various extra operations
#include <GL/glew.h>            // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h>         // GLFW helper library
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "./components/sound/sound.hpp"

#include "./util/shader/shader.hpp"
#include "./util/window/window.hpp"
#include "./components/model/model.hpp"
#include "./util/log/log.hpp"
#include "./components/parameters/parameters.hpp"
#include "./components/input/input.hpp"
#include "./components/camera/camera.hpp"
#include "./components/physics/physics.hpp"
#include "./containers/world/world.hpp"
#include "./components/time/time.hpp"
// #include "./util/gltext/gltext.hpp"
// #include "./components/menu/menu.hpp"

#include <btBulletDynamicsCommon.h>
#include <sstream>

#include "./data/constants.hpp"
#include "./data/global.hpp"

    using namespace std;

    const char* ConvertDoubleToString(double value){ //Hay que modularizar esto :c
        std::stringstream ss ;
        ss << value;
        const char* str = ss.str().c_str();
        return str;
    }

int main()
{
  restart = false;
  fullscreen = false;
  srand(time(NULL));

  log_restart_gl_log();

  window_start_gl();

  window_flags();

  input_setCallbacks();

  // gltInit();

  shader_programme = shader_create_programme_from_files();
  shader_programme_cube = shader_create_programme_from_files(VERTEX_SHADER_FILE_CUBE, FRAGMENT_SHADER_FILE_CUBE);

  int distanciaEntreProps = 20;
  int distanciaEntreHouses = 50;
  int distanciaEntreCapas = 5;

  int minXVel = -2;
  int maxXVel = 2;
  int maxYVel = 12;
  int maxZVel = 6;
  int minX = -3;
  int maxX = 3;
  int minZ = -3;
  int maxZ = 7;
  double forceHorizontalJump = 14.2;
  double forceVerticalUpJump = 8.0;
  double forceVerticalDownJump = -0.2;
  double forceBackwardJump = -12.2;
  double forceForwardJump = 12.2;

  double frame_start = 0.0;

  Parameters *rules = new Parameters(
      minXVel, maxXVel, maxYVel, maxZVel,
      minX, maxX, minZ, maxZ,
      forceHorizontalJump, forceVerticalUpJump, forceVerticalDownJump,
      forceForwardJump, forceBackwardJump,
      distanciaEntreHouses, distanciaEntreProps,
      distanciaEntreCapas);

  // Menu *menu = new Menu();

  glfwSwapInterval(1);
  glfwSetInputMode(g_window, GLFW_STICKY_KEYS, 1);

  core = new World(40, 20, 20, 6, 0.0);

  core->genPhysics();

  core->genRick();

  core->genPlatforms(rules);

  core->genParallaxHouses(rules);

  core->genParallaxProps(rules);

  core->initBackgroundMusic();

  float g = 0.5f;
  background[0]->set_gain(g);
  background[1]->set_gain(g);
  background[2]->set_gain(g);
  background[3]->set_gain(g);
  background[4]->set_gain(g);
  background[5]->set_gain(g);


  // GLTtext *textT = gltCreateText();
  // gltSetText(textT, "Puntaje:");
  // GLTtext *textPuntaje = gltCreateText();
  //   GLTtext *textPerdiste = gltCreateText();

  int size = 1;
  int posT1x = 5;
  int posT12y = 5;
  int posPuntajex = 80;


  // background[0]->set_gain(0.9f);
  core->genCube();
  timer = new Time();
  Time *fpsTimer = new Time();

  while (!glfwWindowShouldClose(g_window))
  {

    fpsTimer->updateNow();
    if (fpsTimer->every(1.0))
    {
      cout << "distance score: " << rules->getDistance(distanceScore) << endl;
      // gltSetText(textPuntaje,ConvertDoubleToString(rules->getDistance(distanceScore)) );
      if(rules->getDistance(distanceScore) > 10){
          // gltSetText(textPerdiste,"");
      }
      window_update_fps_counter(g_window);
    }

    if (restart)
    {
      cout << "restart" << endl;
      // gltSetText(textPerdiste,"Perdiste!! Sigue intentando");

      core->reset(rules);

      restart = false;
      timer->restart();
      fpsTimer->restart();
    } else {
      // gltSetText(textPerdiste, "");
    }

    float startTicks = glfwGetTime();

    core->dynamicPlatforms(rules);

    core->dynamicHouses(rules);

    core->dynamicProps(rules);

    if (timer->getUpdateNow() < 5.0)
    {
      platformWorld->setVelocity(2, btVector3(0, 0, 0));

    }
    else if (timer->checkFirstTime(5.0))
    {
      core->startPlatformVelocity();
    }
    else if (timer->every(8.0))
    {
      cout << "Mas velocidad" << endl;
      core->morePlatformVelocity();

    }

    rules->checkRickPos(platformWorld);
    rules->checkRickVel(platformWorld);

    core->getPhysicsPos();

    core->gravityRick();

    window_frameCounter();

    platformWorld->stepSimulation(1/_fps);

    parallaxHouses->stepSimulation(1/_fps);
    parallaxProps->stepSimulation(1/_fps);

    distanceScore->stepSimulation(1/_fps);

    input_processInput(g_window);

    // menu->checkInput();

    window_clear();

    // menu->drawText();

    // gltColor(1.0f, 1.0f, 1.0f, 0.0f);
    // gltDrawText2D(textT, posT1x, posT12y, size);
    // gltDrawText2D(textPuntaje, posPuntajex, posT12y, size);

    // gltColor(1.0f, 1.0f, 1.0f, 0.0f);
    // gltDrawText2D(textPerdiste, 100, 100, 4);

    camera_viewProjUpdate();


    if (!debug) {
      core->drawCube();
      core->drawRick();

      core->drawPlatforms();

      core->drawPlane();

      core->drawHouses(rules);
      core->drawProps();
    } else {
    platformWorld->debugDrawWorld();
    parallaxHouses->debugDrawWorld();
    parallaxProps->debugDrawWorld();
    }

    window_calculateFps();

    core->backgroundMusic();


    window_swap();
    platformWorld->checkCollision(&allowJump);
  }

  // gltDeleteText(textT);
  //   gltDeleteText(textPuntaje);
  //   gltDeleteText(textPerdiste);

  glfwTerminate();
  return 0;
}
