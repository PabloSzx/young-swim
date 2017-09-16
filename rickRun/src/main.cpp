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
  
  Model *rick = new Model(const_cast<char *>("mesh/cube.obj"));
  rick->setpos(glm::vec3(0.0f, 0.0f, 0.0f));
  rick->scale(glm::vec3(0.3f));
  rick->model2shader(shader_programme);

  Model *plataforma = new Model(const_cast<char *>("mesh/platform.obj"));
  plataforma->setpos(glm::vec3(0.0f,0.0f,0.0f));
  plataforma->scale(glm::vec3(2.0f));
  plataforma->model2shader(shader_programme);
  // cout << rick->LX << " " << rick->LY << " " << rick->LZ << endl;

  Model *plano = new Model(const_cast<char *>("mesh/plano.obj"));
  plano->setpos(glm::vec3(0.0f,0.0f,0.0f));
  plano->model2shader(shader_programme);

  Model *eje = new Model(const_cast<char *>("mesh/axis.obj"));
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
  
  world = new Bullet(3);

  world->newPlane(btVector3(0, 8, 0), 1.3);

  world->newFallBody(btVector3(rick->LX, rick->LY, rick->LZ), btVector3(0, 20, 0), 1);

  world->newFallBody(btVector3(plataforma->LX / 2, plataforma->LY / 2, plataforma->LZ / 2), btVector3(3, 2, 2), 0);

  while (!glfwWindowShouldClose(g_window)){
    frameCounter();
    
    /* PHYSICS */
    world->stepSimulation();
    btVector3 rickPos = world->getTransformOrigin(1);
    btVector3 plataformaPos = world->getTransformOrigin(2);

    /* INPUT */
    processInput(g_window);

    /* CLEAR */
    window_clear();

    /* CAMERA */
    cameraPos = glm::vec3(rickPos.getX(), rickPos.getY() + 2.0, rickPos.getZ() + 4.5);

    projectionMatrixPerspective();
    viewMatrixPerspective();


    /* MODEL */

    rick->setpos(glm::vec3(rickPos.getX(), rickPos.getY(), rickPos.getZ()));
    rick->draw();

    eje->draw();
    
    axisX->draw();
    
    axisY->draw();
    
    axisZ->draw();

    plataforma->setpos(glm::vec3(plataformaPos.getX(), plataformaPos.getY(), plataformaPos.getZ()));
    plataforma->draw();

    for (float i = -20; i <= 20; i += 1.0) {
      for (float j = -20; j <= 20; j += 1.0)
      {
        plano->setpos(glm::vec3(i, 0, j));
        plano->draw();
      }
    }

    /* SWAP BUFFER */

    window_swap();
  }

  glfwTerminate();
  return 0;
}
