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

#include "./utils/shader/shader.h"
#include "./utils/window/window.h"
#include "./components/model/model.h"
#include "./utils/log/log.h"
#include "./components/input/input.h"
#include "./components/camera/camera.h"
#include "./components/physics/physics.h"

#ifdef APPLE
#include <BulletDynamics/btBulletDynamicsCommon.h>
#else
#include <bullet/btBulletDynamicsCommon.h>
#endif

#include "./data/constants.h"
#include "./data/global.h"

using namespace std;

int main(){
  srand (time(NULL));
    
  restart_gl_log ();

  window_start_gl();
  
  window_flags();
  
  setCallbacks();


  shader_programme = create_programme_from_files ();
  
  viewMatrixLocation();
  projMatrixLocation();
  
  Model *rick = new Model(const_cast<char *>("mesh/rick.obj"));
  rick->setpos(glm::vec3(0.0f, 0.0f, 0.0f));
  rick->scale(glm::vec3(1.0f));
  rick->model2shader(shader_programme);
  
  // cout << rick->LX << " " << rick->LY << " " << rick->LZ << endl;
  
  Model *eje = new Model(const_cast<char*>("mesh/axis.obj"));
  eje->setpos(glm::vec3(0.0f,0.0f,0.0f));
  eje->scale(glm::vec3(1.0f));
  eje->model2shader(shader_programme);
  
  Model *axisX = new Model(const_cast<char*>("mesh/x.obj"));
  axisX->setpos(glm::vec3(8.0f,0.0f,0.0f));
  axisX->scale(glm::vec3(0.5f));
  axisX->model2shader(shader_programme);
  
  Model *axisY = new Model(const_cast<char*>("mesh/y.obj"));
  axisY->setpos(glm::vec3(0.0f,8.0f,0.0f));
  axisY->scale(glm::vec3(0.5f));
  axisY->model2shader(shader_programme);
    
  Model *axisZ = new Model(const_cast<char*>("mesh/z.obj"));
  axisZ->setpos(glm::vec3(0.0f,0.0f,8.0f));
  axisZ->scale(glm::vec3(0.5f));
  axisZ->model2shader(shader_programme);
  
  Bullet *world = new Bullet(2);

  world->newPlane(btVector3(0, 1, 0), 0);

  world->newFallBody(5, btVector3(0, 50, 0));

  while (!glfwWindowShouldClose(g_window)){
    frameCounter();
    
    /* PHYSICS */
    world->stepSimulation();
    btVector3 rickPos = world->getTransformOrigin(1);

    /* INPUT */
    processInput(g_window);

    /* CLEAR */
    window_clear();

    /* CAMERA */
    projectionMatrixPerspective();
    viewMatrixPerspective();

    /* MODEL */

    rick->setpos(glm::vec3(rickPos.getX(), rickPos.getY(), rickPos.getZ()));
    rick->draw();

    eje->draw();
    
    axisX->draw();
    
    axisY->draw();
    
    axisZ->draw();

    /* SWAP BUFFER */

    window_swap();
  }

  glfwTerminate();
  return 0;
}
