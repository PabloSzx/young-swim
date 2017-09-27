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

const char* getRandomProp() {
  int n = rand() % 4;
  switch (n) {
    case 0:
    return "mesh/arbol.obj";
    case 1:
    return "mesh/pasto.obj";
    case 2 : 
    return "mesh/planta1.obj";
    case 3:
    return "mesh/planta2.obj";
  }
  return "";
}

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
  int nplataformas = 40;
  
  world = new Bullet(nplataformas + 2, btVector3(0, 0, 0), PLATFORMS_START_INDEX);
  world->newPlane(btVector3(0, 1, 0), -3.7, 0); //0
  
  Model *rick = new Model(const_cast<char *>("mesh/rick.obj"), glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.6f));
  rick->setColor(1.0f, 0.894f, 0.882f);
  rick->model2shader(shader_programme);
  
  world->newFallBody(btVector3(rick->LX, rick->LY * 2, rick->LZ), btVector3(0, 10, 0), 1, btVector3(0, 0, 0), -1); //1
  
  Model **plataformas = static_cast<Model **>(malloc(sizeof(Model *) * nplataformas));
  double platformVelocity = 0.0;
  plataformas[0] = new Model(const_cast<char *>("mesh/platform.obj"));
  plataformas[0]->setColor(0.753f, 0.753f, 0.753f);
  plataformas[0]->model2shader(shader_programme);
  btVector3 platPos = btVector3(0, 0, 0);
  world->newFallBody(btVector3(plataformas[0]->LX / 2, plataformas[0]->LY * 2, plataformas[0]->LZ / 2), platPos, 10000, btVector3(platformVelocity, 0, 0), PLATFORMS_START_INDEX);


  for (int i = 1; i < nplataformas; i+=1) {
    platPos = Gaming::getPlatformPos(platPos.getZ(), platPos.getY(), i * plataformas[0]->LX);
    plataformas[i] = new Model(const_cast<char *>("mesh/platform.obj"), glm::vec3(platPos.getX(), platPos.getY(), platPos.getZ()));
    plataformas[i]->setColor(0.753f, 0.753f, 0.753f);
    plataformas[i]->model2shader(shader_programme);
    world->newFallBody(btVector3(plataformas[0]->LX / 2 + 0.1, plataformas[0]->LY * 2, plataformas[0]->LZ / 2), platPos, 10000, btVector3(platformVelocity, 0, 0), i + PLATFORMS_START_INDEX);
  }

  // cout << rick->LX << " " << rick->LY << " " << rick->LZ << endl;

  Model *plano = new Model(const_cast<char *>("mesh/plano.obj"));
  plano->setColor(0.8f, 0.0f, 0.0f);
  plano->model2shader(shader_programme);
  
  int nHouses = 20;
  int nProps = 20;
  parallaxHouses = new Bullet(nHouses, btVector3(0, 0, 0), 0);
  parallaxProps = new Bullet(nProps, btVector3(0, 0, 0), 0);
  
  // parallax->newPlane(btVector3(0, 1, 0), -3.0, 0); //0
  
  int distanciaEntreProps = 20;
  int distanciaEntreHouses = 20;
  double velocityProps = platformVelocity * 0.5;
  double velocityHouses = platformVelocity * 0.2;
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
    distanciaEntreHouses, distanciaEntreProps
  );
  
  Model **casas = static_cast<Model **>(malloc(sizeof(Model *) * nHouses));
  
  casas[0] = new Model(const_cast<char *>("mesh/casa.obj"));
  casas[0]->setColor(0.545f, 0.271f, 0.075f);
  casas[0]->model2shader(shader_programme);
  btVector3 casaPos = btVector3(0, 0, 12);
  parallaxHouses->newFallBody(btVector3(casas[0]->LX / 2, casas[0]->LY / 2 + 0.1, casas[0]->LZ / 2), casaPos, 1, btVector3(platformVelocity * 0.5, 0, 0), PARALLAX_START_INDEX);
  
  for (int i = 1; i < nHouses; i += 1)
  {
    casaPos = rules->getHousePos(casaPos.getX(), casaPos.getY(), casaPos.getZ());
    casas[i] = new Model(const_cast<char *>("mesh/casa.obj"), glm::vec3(casaPos.getX(), casaPos.getY(), casaPos.getZ()));
    casas[i]->setColor(0.545f, 0.271f, 0.075f);
    casas[i]->model2shader(shader_programme);
    parallaxHouses->newFallBody(btVector3(casas[i]->LX / 2, casas[i]->LY / 2 + 0.1, casas[i]->LZ / 2), casaPos, 1, btVector3(platformVelocity * 0.5, 0, 0), i);
  }
  Model **props = static_cast<Model **>(malloc(sizeof(Model *) * nProps));

  props[0] = new Model(const_cast<char *>(getRandomProp()));
  props[0]->setColor(0.196f, 0.804f, 0.196f);
  props[0]->model2shader(shader_programme);
  btVector3 propPos = btVector3(0, 0, 7);
  parallaxProps->newFallBody(btVector3(props[0]->LX / 2, props[0]->LY / 2 + 0.1, props[0]->LZ / 2), propPos, 1, btVector3(platformVelocity, 0, 0), PARALLAX_START_INDEX);
  
  for (int i = 1; i < nProps; i += 1)
  {
    propPos = rules->getPropPos(propPos.getX(), propPos.getY(), propPos.getZ());
    props[i] = new Model(const_cast<char *>(getRandomProp()), glm::vec3(propPos.getX(), propPos.getY(), propPos.getZ()));
    props[i]->setColor(0.196f, 0.804f, 0.196f);
    props[i]->model2shader(shader_programme);
    parallaxProps->newFallBody(btVector3(props[i]->LX / 2 + 0.1, props[i]->LY / 2 + 0.1, props[i]->LZ / 2), propPos, 1, btVector3(platformVelocity, 0, 0), i + PARALLAX_START_INDEX);
  }
  
  double nowTime;
  double lastTime = 0.0;
  btVector3 plataformaPos;
  btVector3 rickPos = btVector3(0, 0, 0);
  btVector3 previousObj;
  
  int previousPlatform;
  double previousParallaxObj;
  
  bool firstfirst = true;
  
  while (!glfwWindowShouldClose(g_window))
  {  
    window_update_fps_counter (g_window);
    
    nowTime = glfwGetTime();
    
    if (abs(world->getTransformOrigin(world->getLastPlatform()).getX() - world->getTransformOrigin(1).getX()) > (20 * plataformas[0]->LX))
    {
      // cout << "last platform modified" << world->getLastPlatform() << endl;
      if (world->getLastPlatform() == PLATFORMS_START_INDEX) {
        previousPlatform = world->getNMax() - 1;
      } else {
        previousPlatform = world->getLastPlatform() - 1;
      }
      platPos = Gaming::getPlatformPos(platPos.getZ(), platPos.getY(), world->getTransformOrigin(previousPlatform).getX() + plataformas[0]->LX);
      world->editLastPlatform(platPos, 10000, btVector3(platformVelocity, 0, 0), world->getLastPlatform());
    }
    
    if (abs(parallaxHouses->getTransformOrigin(parallaxHouses->getLastPlatform()).getX() - world->getTransformOrigin(1).getX()) > (10 * casas[0]->LX))
    {
      // cout << "last house modified " << parallaxHouses->getLastPlatform() << endl;
      if (parallaxHouses->getLastPlatform() == PARALLAX_START_INDEX) {
        previousParallaxObj = parallaxHouses->getNMax() - 1;
      } else {
        previousParallaxObj = parallaxHouses->getLastPlatform() - 1;
      }
      previousObj = parallaxHouses->getTransformOrigin(previousParallaxObj);
      
      casaPos = rules->getHousePos(previousObj.getX(), previousObj.getY(), previousObj.getZ());
      parallaxHouses->editLastPlatform(casaPos, 1, btVector3(platformVelocity * 0.5, 0, 0), parallaxHouses->getUserIndex(parallaxHouses->getLastPlatform()));
    }
    
    if (abs(parallaxProps->getTransformOrigin(parallaxProps->getLastPlatform()).getX() - world->getTransformOrigin(1).getX()) > (10 * props[0]->LX))
    {
      // cout << "last props modified " << parallaxProps->getLastPlatform() << endl;
      
      if (parallaxProps->getLastPlatform() == PARALLAX_START_INDEX)
      {
        previousParallaxObj = parallaxProps->getNMax() - 1;
      }
      else
      {
        previousParallaxObj = parallaxProps->getLastPlatform() - 1;
      }
      previousObj = parallaxProps->getTransformOrigin(previousParallaxObj);
      
      propPos = rules->getHousePos(previousObj.getX(), previousObj.getY(), previousObj.getZ());
      parallaxProps->editLastPlatform(propPos, 1, btVector3(platformVelocity, 0, 0), parallaxProps->getUserIndex(parallaxProps->getLastPlatform()));
    }
    
    if (nowTime >= 5.0f && firstTime)
    {
      cout << "PARTIMOS" << endl;
      platformVelocity = -5.0;
      for (int i = PLATFORMS_START_INDEX; i < nplataformas + PLATFORMS_START_INDEX; i += 1)
      {
        world->setVelocity(i, btVector3(platformVelocity, 0, 0));
      }
      for (int i = 0; i < nHouses; i += 1) {
        parallaxHouses->setVelocity(i, btVector3(platformVelocity * 0.5, 0, 0));
      }
      
      for (int i = 0; i < nProps; i += 1) {
        parallaxProps->setVelocity(i, btVector3(platformVelocity, 0, 0));
      }
      
      firstTime = false;
    }
    
    if ((nowTime - lastTime) > 15) {
      cout << "Mas velocidad" << endl;
      platformVelocity -= 5.0;
      for (int i = 0; i < nplataformas; i += 1) {
        world->setVelocity(i + PLATFORMS_START_INDEX, btVector3(platformVelocity, 0, 0));
      }
      
      for (int i = 0; i < nHouses; i += 1)
      {
        parallaxHouses->setVelocity(i, btVector3(platformVelocity * 0.5, 0, 0));
      }
      
      for (int i = 0; i < nProps; i += 1)
      {
        parallaxProps->setVelocity(i, btVector3(platformVelocity, 0, 0));
      }
      lastTime = nowTime;
    }
    
    rules->checkRickPos(world);
    rules->checkRickVel(world);
    
    rickPos = world->getTransformOrigin(1);
    
    world->applyForce(1, btVector3(0, -9.8, 0));
    window_frameCounter();
    
    /* PHYSICS */
    world->checkCollision(&allowJump);
    world->stepSimulation(60);
    
    parallaxHouses->stepSimulation(60);
    parallaxProps->stepSimulation(60);
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
    
    for (int i = 0; i < nplataformas; i += 1)
    {
      plataformaPos = world->getTransformOrigin(i + PLATFORMS_START_INDEX);
      plataformas[i]->setpos(glm::vec3(plataformaPos.getX(), plataformaPos.getY(), plataformaPos.getZ()));
      plataformas[i]->draw();
    }
    
    for (float i = -20; i <= 20; i += 1.1) {
      for (float j = -20; j <= 20; j += 1.1)
      {
        plano->setpos(glm::vec3(rickPos.getX() + i, -5, rickPos.getZ() + j));
        plano->draw();
      }
    }
    
    /* PARALLAX DERECHO */
    
    for (int i = 0; i < nHouses; i += 1)
    {
      casaPos = parallaxHouses->getTransformOrigin(i);
      casas[i]->setpos(glm::vec3(casaPos.getX(), casaPos.getY(), 8 + distanciaEntreCapas));
      casas[i]->draw();
    }
    
    for (int i = 0; i < nProps; i += 1)
    {
      propPos = parallaxProps->getTransformOrigin(i);
      props[i]->setpos(glm::vec3(propPos.getX(), propPos.getY(), 8));
      props[i]->draw();
    }
    
    /* PARALLAX IZQUIERDO */
    
    for (int i = 0; i < nHouses; i += 1)
    {
      casaPos = parallaxHouses->getTransformOrigin(i);
      casas[i]->setpos(glm::vec3(casaPos.getX(), casaPos.getY(), -4 - distanciaEntreCapas));
      casas[i]->draw();
    }
    
    for (int i = 0; i < nProps; i += 1)
    {
      propPos = parallaxProps->getTransformOrigin(i);
      props[i]->setpos(glm::vec3(propPos.getX(), propPos.getY(), -4));
      props[i]->draw();
    }
    
    /* SWAP BUFFER */
    
    window_swap();
  }
  
  glfwTerminate();
  return 0;
}
