#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
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

#include <btBulletDynamicsCommon.h>

#include "./data/constants.hpp"
#include "./data/global.hpp"



using namespace std;

int main() {
  restart = false;
  fullscreen = false;
  srand (time(NULL));

  log_restart_gl_log ();

  window_start_gl();

  window_flags();

  input_setCallbacks();

  shader_programme = shader_create_programme_from_files ();
  color = glGetUniformLocation(shader_programme, "color");
  sunLocation = glGetUniformLocation(shader_programme, "sun");

  camera_viewMatrixLocation();
  camera_projMatrixLocation();

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
  double forceHorizontalJump = 8.2;
  double forceVerticalUpJump = 8.0;
  double forceVerticalDownJump = -0.2;
  double forceBackwardJump = -8.2;
  double forceForwardJump = 8.2;


  double frame_start = 0.0;

  Parameters *rules = new Parameters(
    minXVel, maxXVel, maxYVel, maxZVel,
    minX, maxX, minZ, maxZ,
    forceHorizontalJump, forceVerticalUpJump, forceVerticalDownJump,
    forceForwardJump, forceBackwardJump,
    distanciaEntreHouses, distanciaEntreProps,
    distanciaEntreCapas);

    glfwSwapInterval(1);
    glfwSetInputMode(g_window, GLFW_STICKY_KEYS, 1);

    core = new World(40, 20, 20, 6, 0.0);

    core->genPhysics();

    core->genRick();

    core->genPlatforms(rules);

    core->genParallaxHouses(rules);

    core->genParallaxProps(rules);

    core->initBackgroundMusic();

    Model *crosshair = new Model(const_cast<char *>("mesh/crosshair.obj"));
    crosshair->setColor(0.0f, 0.0f, 0.0f);
    crosshair->scale(glm::vec3(0.05f));
    crosshair->model2shader(shader_programme);

    Time *timer = new Time();

    while (!glfwWindowShouldClose(g_window))
    {

      // int count;
      // const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

      // cout << "count: " << count << endl;
      // cout << "axes: " << axes << endl;
      //const char* name = glfwGetJoystickName(GLFW_JOYSTICK_1);
      //cout << "name: " << name << endl;
      // cout << "distance score: " << rules->getDistance(distanceScore) << endl;

      if (restart) {
        core->reset(rules);

        restart = false;
        timer->restart();
      }
      core->backgroundMusic();
      window_update_fps_counter (g_window);

      core->dynamicPlatforms(rules);

      core->dynamicHouses(rules);

      core->dynamicProps(rules);

      if (timer->getUpdateNow() < 5.0) {
        platformWorld->setVelocity(2, btVector3(0, 0, 0));
      } else if (timer->checkFirstTime(5.0)) {
        core->startPlatformVelocity();
      } else if (timer->every(6.0)) {
        cout << "Mas velocidad" << endl;
        core->morePlatformVelocity();
      }


      rules->checkRickPos(platformWorld);
      rules->checkRickVel(platformWorld);

      core->getPhysicsPos();

      core->gravityRick();

      window_frameCounter();

      /* PHYSICS */
      platformWorld->checkCollision(&allowJump);
      platformWorld->stepSimulation(fps);

      parallaxHouses->stepSimulation(fps);
      parallaxProps->stepSimulation(fps);

      distanceScore->stepSimulation(fps);

      /* INPUT */
      input_processInput(g_window);
      glUniform3f(sunLocation, sun.x, sun.y, sun.z);

      /* CLEAR */
      window_clear();

      /* CAMERA */
      double xpos, ypos;
      glfwGetCursorPos(g_window, &xpos, &ypos);

      glfwSetCursorPos(g_window, xpos + 0.01 * ((g_gl_width / 2.0) - xpos), ypos + 0.01 * ((g_gl_height / 2.0 + 1800.0) - ypos));

      glfwGetCursorPos(g_window, &xpos, &ypos);

      int xmid = g_gl_width / 2;
      int ymid = g_gl_height / 2;

      glm::vec3 front;

      front.z = (glm::radians(xpos - xmid));
      front.y = (glm::radians(-ypos + ymid));
      front.x = 100;
      cameraFront = glm::normalize(front);

      camera_projectionMatrixPerspective();
      camera_viewMatrixPerspective(glm::vec3(core->getRickPos().getX(), core->getRickPos().getY() + 2.0, core->getRickPos().getZ() + 4.5));

      /* MODEL DRAW */
      // glm::vec3 crosshairPos = cameraPos + glm::vec3(core->getRickPos().getX(), core->getRickPos().getY() + 2.0, core->getRickPos().getZ() + 4.5)  + cameraFront;
      // crosshair->setpos(crosshairPos);
      // crosshair->draw();
      core->drawRick();

      core->drawPlatforms();

      core->drawPlane();

      core->drawHouses(rules);
      core->drawProps();

      // cout << fps << endl;

      /* SWAP BUFFER */

      window_swap();

    }

    glfwTerminate();
    return 0;
  }
