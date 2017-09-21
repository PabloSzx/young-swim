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
#include "./utils/gaming/gaming.h"
#include "./components/input/input.h"
#include "./components/camera/camera.h"
#include "./components/physics/physics.h"

#ifdef APPLE
#include <BulletDynamics/btBulletDynamicsCommon.h>
#else
#include <btBulletDynamicsCommon.h>
#endif

#include "./data/constants.h"
#include "./data/global.h"

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
  
  world = new Bullet(44);
  // Gaming *rules = new Gaming();
  // world->newPlane(btVector3(0, 0, 1), -2.3);
  world->newPlane(btVector3(0, 1, 0), -3.7, 0); //0
  // world->newPlane(btVector3(0, 1, 0), -1.3);
  
  // world->newPlane(btVector3(0, 0, 1), 3.0);
  // world->newFallBody(btVector3(100, 100, 0.5), btVector3(0, 0, 4), 0);
  
  Model *rick = new Model(const_cast<char *>("mesh/box.obj"));
  rick->setpos(glm::vec3(0.0f, 10.0f, 0.0f));
  rick->scale(glm::vec3(0.6f));
  rick->setColor(1.0f, 0.894f, 0.882f);
  rick->model2shader(shader_programme);
  
  world->newFallBody(btVector3(rick->LX, rick->LY, rick->LZ), btVector3(0, 10, 0), 1, btVector3(0, 0, 0), -1); //1
  
  /* PAREDES */
  world->newFallBody(btVector3(100, 100, 0.5), btVector3(0, 0, -2), 0, btVector3(0, 0, 0), 1); //2
  world->newFallBody(btVector3(100, 100, 0.5), btVector3(0, 0, 7), 0, btVector3(0, 0, 0), 2);  //3
  
  int nplataformas = 20;
  Model **plataformas = static_cast<Model **>(malloc(sizeof(Model *) * nplataformas));
  double platformVelocity = 0.0;
  plataformas[0] = new Model(const_cast<char *>("mesh/platform.obj"));
  plataformas[0]->setpos(glm::vec3(0, 0, 0));
  plataformas[0]->scale(glm::vec3(1.0f));
  plataformas[0]->setColor(0.753f, 0.753f, 0.753f);
  plataformas[0]->model2shader(shader_programme);
  btVector3 platPos = btVector3(0, 0, 0);
  world->newFallBody(btVector3(plataformas[0]->LX / 2, plataformas[0]->LY / 2 + 0.1, plataformas[0]->LZ / 2), platPos, 10000, btVector3(platformVelocity, 0, 0), 4);
  
  for (int i = 1; i < nplataformas; i+=1) {
    plataformas[i] = new Model(const_cast<char *>("mesh/platform.obj"));
    plataformas[i]->setColor(0.753f, 0.753f, 0.753f);
    platPos = Gaming::getPlatformPos(platPos.getZ(), platPos.getY(), i * plataformas[i]->LX);
    plataformas[i]->setpos(glm::vec3(platPos.getX(), platPos.getY(), platPos.getZ()));
    plataformas[i]->model2shader(shader_programme);
    world->newFallBody(btVector3(plataformas[i]->LX / 2 + 0.1, plataformas[i]->LY / 2 + 0.1, plataformas[i]->LZ / 2), platPos, 10000, btVector3(platformVelocity, 0, 0), i + 4);
  }
  // for (int i = 0k; i < nplataformas; i+=1) {
    //   plataformas[i] = new Model(const_cast<char *>("mesh/platform.obj"));
    //   plataformas[i]->setpos(glm::vec3(i * plataformas[i]->LX, (i % 3) * 3, (i % 3) * plataformas[i]->LZ));
    //   plataformas[i]->scale(glm::vec3(1.0f));
    //   plataformas[i]->setColor(0.753f, 0.753f, 0.753f);
    //   plataformas[i]->model2shader(shader_programme);
    //   world->newFallBody(btVector3(plataformas[i]->LX / 2, plataformas[i]->LY / 2 + 0.1, plataformas[i]->LZ / 2), btVector3(i * plataformas[i]->LX, (i % 3) * 3, (i % 3) * plataformas[i]->LZ), 10000, btVector3(platformVelocity, 0, 0), i + 4);
    //   //4 - 39
    // }
    
    // cout << rick->LX << " " << rick->LY << " " << rick->LZ << endl;
    
    Model *plano = new Model(const_cast<char *>("mesh/plano.obj"));
    plano->setpos(glm::vec3(0.0f, 0.0f, 0.0f));
    plano->setColor(0.8f, 0.0f, 0.0f);
    plano->model2shader(shader_programme);
    
    Model *pared = new Model(const_cast<char *>("mesh/pared.obj"));
    pared->setpos(glm::vec3(0.0f, 0.0f, 0.0f));
    pared->setColor(0.5f, 0.5f, 0.5f);
    pared->model2shader(shader_programme);
    
    Model *eje = new Model(const_cast<char *>("mesh/axis.obj"));
    eje->setpos(glm::vec3(0.0f, 0.0f, 0.0f));
    eje->scale(glm::vec3(1.0f));
    eje->model2shader(shader_programme);
    
    Model *axisX = new Model(const_cast<char *>("mesh/x.obj"));
    axisX->setpos(glm::vec3(8.0f, 0.0f, 0.0f));
    axisX->scale(glm::vec3(0.5f));
    axisX->setColor(0.0f, 0.0f, 0.0f);
    axisX->model2shader(shader_programme);
    
    Model *axisY = new Model(const_cast<char *>("mesh/y.obj"));
    axisY->setpos(glm::vec3(0.0f, 8.0f, 0.0f));
    axisY->scale(glm::vec3(0.5f));
    axisY->setColor(0.0f, 0.0f, 0.0f);
    axisY->model2shader(shader_programme);
    
    Model *axisZ = new Model(const_cast<char *>("mesh/z.obj"));
    axisZ->setpos(glm::vec3(0.0f, 0.0f, 8.0f));
    axisZ->scale(glm::vec3(0.5f));
    axisZ->setColor(0.0f, 0.0f, 0.0f);
    axisZ->model2shader(shader_programme);
    
    Model *arbol = new Model(const_cast<char *>("mesh/arbol.obj"));
    arbol->setpos(glm::vec3(0.0f, arbol->LY / 2, 8.0f));
    arbol->scale(glm::vec3(0.5f));
    arbol->setColor(0.0f, 1.0f, 0.0f);
    arbol->model2shader(shader_programme);
    
    bool twoFirstTime = true;
    double nowTime;
    double lastTime;
    // btVector3 platPos;
    
    while (!glfwWindowShouldClose(g_window))
    {
      nowTime = glfwGetTime();
      
      if (nowTime >= 10.0f && firstTime)
      {
        // for (int i=, i < 18; )
        // world->setGravity(4, btVector3(1.5,-9.8,0));
        platformVelocity = -5.0;
        for (int i = 4; i < nplataformas + 4; i += 1)
        {
          world->setVelocity(i, btVector3(platformVelocity, 0, 0));
        }
        firstTime = false;
      } else {
        for (int i = 4; i < nplataformas + 4; i += 1)
        {
          world->setVelocity(i, btVector3(platformVelocity, 0, 0));
        }
      }
      
      if ((nowTime - lastTime) > 10)
      {
        // cout << "repos realizado" << endl;
        // /* PAREDES */
        // platformVelocity -= 5.0;
        // world->editBody(2, btVector3(100, 100, 0.5), btVector3(0, 0, -2), 0, btVector3(0, 0, 0), 1); //2
        // world->editBody(3, btVector3(100, 100, 0.5), btVector3(0, 0, 7), 0, btVector3(0, 0, 0), 2);  //3
        // for (int i = 0; i < nplataformas; i += 1)
        // {
          //   plataformas[i]->setpos(glm::vec3((i * plataformas[i]->LX), (i % 3) * 3, (i % 3) * plataformas[i]->LZ));
          //   plataformas[i]->scale(glm::vec3(1.0f));
          //   plataformas[i]->setColor(0.753f, 0.753f, 0.753f);
          //   plataformas[i]->model2shader(shader_programme);
            // world->editBody(i + 4, btVector3(plataformas[i]->LX / 2, plataformas[i]->LY / 2 + 0.1, plataformas[i]->LZ / 2), btVector3((i * plataformas[i]->LX), (i % 3) * 3, (i % 3) * plataformas[i]->LZ), 10000, btVector3(platformVelocity, 0, 0), i + 4);
          //   world->setVelocity(i + 4, btVector3(platformVelocity, 0, 0));
          // }
          // lastTime = nowTime;
        }
        
        world->applyForce(1, btVector3(0, -14.8, 0));
        window_frameCounter();
        
        /* PHYSICS */
        world->checkCollision(&allowJump);
        world->stepSimulation();
        btVector3 rickPos = world->getTransformOrigin(1);
        
        /* INPUT */
        input_processInput(g_window);
        
        /* CLEAR */
        window_clear();
        
        /* CAMERA */
        
        camera_projectionMatrixPerspective();
        camera_viewMatrixPerspective(glm::vec3(rickPos.getX(), rickPos.getY() + 2.0, rickPos.getZ() + 4.5));
        
        /* MODEL */
        glColor3f(1.0f, 0.0f, 0.0f);
        rick->setpos(glm::vec3(rickPos.getX(), rickPos.getY(), rickPos.getZ()));
        rick->draw();
        
        eje->draw();
        
        axisX->draw();
        
        axisY->draw();
        
        axisZ->draw();
        
        for (int i = 0; i < nplataformas; i += 1) {
          platPos = world->getTransformOrigin(i + 4);
          plataformas[i]->setpos(glm::vec3(platPos.getX(), platPos.getY(), platPos.getZ()));
          plataformas[i]->draw();
        }
        
        arbol->draw();
        
        for (float i = -20; i <= 20; i += 1.05) {
          for (float j = -20; j <= 20; j += 1.05)
          {
            plano->setpos(glm::vec3(rickPos.getX() + i, -5, rickPos.getZ() + j));
            plano->draw();
          }
        }
        pared->setpos(glm::vec3(50, 50, -2));
        pared->draw();
        
        pared->setpos(glm::vec3(50, 50, 7));
        pared->draw();
        
        /* SWAP BUFFER */
        
        window_swap();
      }
      
      glfwTerminate();
      return 0;
    }
    