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
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "./utils/shader/shader.hpp"
#include "./utils/window/window.hpp"
#include "./components/model/model.hpp"
#include "./utils/log/log.hpp"
#include "./components/gaming/gaming.hpp"
#include "./components/input/input.hpp"
#include "./components/camera/camera.hpp"
#include "./components/physics/physics.hpp"
#include "./containers/world/world.hpp"

#ifdef APPLE
#include <BulletDynamics/btBulletDynamicsCommon.h>
#else
#include <btBulletDynamicsCommon.h>
#endif

#include "./data/constants.hpp"
#include "./data/global.hpp"

using namespace std;

int main(){
  bool firstTime = true;
  srand (time(NULL));
  
  log_restart_gl_log ();
  
  window_start_gl();
  
  window_flags();
  
  input_setCallbacks();
  
  shader_programme = shader_create_programme_from_files ();
  color = glGetUniformLocation(shader_programme, "color");
  
  camera_viewMatrixLocation();
  camera_projMatrixLocation();
  
  int distanciaEntreProps = 20;
  int distanciaEntreHouses = 20;
  int distanciaEntreCapas = 5;
  
  int minXVel = -2;
  int maxXVel = 2;
  int maxYVel = 12;
  int maxZVel = 6;
  int minX = -3;
  int maxX = 3;
  int minZ = -3;
  int maxZ = 7;
  double forceHorizontalJump = 0.2;
  double forceVerticalUpJump = 8.0;
  double forceVerticalDownJump = -0.2;
  double forceBackwardJump = -0.2;
  double forceForwardJump = 0.2;
  
  Gaming *rules = new Gaming(
    minXVel, maxXVel, maxYVel, maxZVel,
    minX, maxX, minZ, maxZ,
    forceHorizontalJump, forceVerticalUpJump, forceVerticalDownJump,
    forceForwardJump, forceBackwardJump,
    distanciaEntreHouses, distanciaEntreProps,
    distanciaEntreCapas
  );
  
  World *base = new World(40, 20, 20, 0.0);
  
  base->genPhysics();
  
  base->genRick();
  
  base->genPlatforms();
  
  base->genParallaxHouses(rules);
  
  base->genParallaxProps(rules);
  
  double nowTime;
  double lastTime = 0.0;
  
  while (!glfwWindowShouldClose(g_window))
  {  
    window_update_fps_counter (g_window);
    
    nowTime = glfwGetTime();
    
    base->dynamicPlatforms();
    
    base->dynamicHouses(rules);
    
    base->dynamicProps(rules);
    
    if (nowTime >= 5.0f && firstTime) {
      base->startPlatformVelocity();
      
      firstTime = false;
    }
    
    if ((nowTime - lastTime) > 15) {
      cout << "Mas velocidad" << endl;
      base->morePlatformVelocity();
      
      lastTime = nowTime;
    }
    
    rules->checkRickPos(world);
    rules->checkRickVel(world);
    
    base->getPhysicsPos();
    
    base->gravityRick();
    
    window_frameCounter();
    
    /* PHYSICS */
    world->checkCollision(&allowJump);
    world->stepSimulation(fps);
    
    parallaxHouses->stepSimulation(fps);
    parallaxProps->stepSimulation(fps);
    /* INPUT */
    input_processInput(g_window);
    
    /* CLEAR */
    window_clear();
    
    /* CAMERA */
    
    camera_projectionMatrixPerspective();
    camera_viewMatrixPerspective(glm::vec3(base->getRickPos().getX(), base->getRickPos().getY() + 2.0, base->getRickPos().getZ() + 4.5));
    
    /* MODEL DRAW */
    base->drawRick();
    
    base->drawPlatforms();
    
    base->drawPlane();
    
    base->drawHouses(rules);
    base->drawProps();
    
    /* SWAP BUFFER */
    
    window_swap();
  }
  
  glfwTerminate();
  return 0;
}
