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
  int nplataformas = 40;
  
  world = new Bullet(nplataformas + 2, btVector3(0, 0, 0), PLATFORMS_START_INDEX);
  world->newPlane(btVector3(0, 1, 0), -3.7, 0); //0
  
  Model *rick = new Model(const_cast<char *>("mesh/box.obj"));
  rick->setpos(glm::vec3(0.0f, 10.0f, 0.0f));
  rick->scale(glm::vec3(0.6f));
  rick->setColor(1.0f, 0.894f, 0.882f);
  rick->model2shader(shader_programme);
  
  world->newFallBody(btVector3(rick->LX, rick->LY, rick->LZ), btVector3(0, 10, 0), 1, btVector3(0, 0, 0), -1); //1
  
  Model **plataformas = static_cast<Model **>(malloc(sizeof(Model *) * nplataformas));
  double platformVelocity = 0.0;
  plataformas[0] = new Model(const_cast<char *>("mesh/platform.obj"));
  plataformas[0]->setpos(glm::vec3(0, 0, 0));
  plataformas[0]->scale(glm::vec3(1.0f));
  plataformas[0]->setColor(0.753f, 0.753f, 0.753f);
  plataformas[0]->model2shader(shader_programme);
  btVector3 platPos = btVector3(0, 0, 0);
  world->newFallBody(btVector3(plataformas[0]->LX / 2, plataformas[0]->LY / 2 + 0.1, plataformas[0]->LZ / 2), platPos, 10000, btVector3(platformVelocity, 0, 0), PLATFORMS_START_INDEX);
  
  for (int i = 1; i < nplataformas; i+=1) {
    plataformas[i] = new Model(const_cast<char *>("mesh/platform.obj"));
    plataformas[i]->setColor(0.753f, 0.753f, 0.753f);
    platPos = Gaming::getPlatformPos(platPos.getZ(), platPos.getY(), i * plataformas[i]->LX);
    plataformas[i]->setpos(glm::vec3(platPos.getX(), platPos.getY(), platPos.getZ()));
    plataformas[i]->model2shader(shader_programme);
    world->newFallBody(btVector3(plataformas[i]->LX / 2 + 0.1, plataformas[i]->LY / 2 + 0.1, plataformas[i]->LZ / 2), platPos, 10000, btVector3(platformVelocity, 0, 0), i + PLATFORMS_START_INDEX);
  }
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
  
  parallaxHouses = new Bullet(2, btVector3(0, 0, 0), PARALLAX_START_INDEX);
  parallaxProps = new Bullet(4, btVector3(0, 0, 0), PARALLAX_START_INDEX);

  // parallax->newPlane(btVector3(0, 1, 0), -3.0, 0); //0
  
  int distanciaEntreProps = 10;
  int distanciaEntreHouses = 25;
  double velocityProps = platformVelocity * 0.5;
  double velocityHouses = platformVelocity * 0.2;
  int distanciaEntreCapas = 5;
  
  Model *casa = new Model(const_cast<char *>("mesh/casa.obj"));
  casa->setpos(glm::vec3(0.0f, casa->LY / 2, 8.0f));
  casa->scale(glm::vec3(1.0f));
  casa->setColor(0.0f, 1.0f, 0.0f);
  casa->model2shader(shader_programme);
  btVector3 casapos = btVector3(distanciaEntreHouses * 0, 0, distanciaEntreCapas * 1);
  parallaxHouses->newFallBody(btVector3(casa->LX / 2, casa->LY / 2, casa->LZ / 2), casapos, 1, btVector3(0, 0, 0), 1);
  parallaxHouses->setVelocity(1, btVector3(-velocityHouses, 0, 0));
  
  Model *casa2 = new Model(const_cast<char *>("mesh/casa.obj"));
  casa2->setpos(glm::vec3(0.0f, casa2->LY / 2, 8.0f));
  casa2->scale(glm::vec3(1.0f));
  casa2->setColor(0.0f, 1.0f, 0.0f);
  casa2->model2shader(shader_programme);
  casapos = btVector3(distanciaEntreHouses * 1, 0, distanciaEntreCapas * 1);
  parallaxHouses->newFallBody(btVector3(casa2->LX / 2, casa2->LY / 2, casa2->LZ / 2), casapos, 1, btVector3(0, 0, 0), 1);
  parallaxHouses->setVelocity(2, btVector3(-velocityHouses, 0, 0));
  
  Model *arbol = new Model(const_cast<char *>("mesh/arbol.obj"));
  arbol->setpos(glm::vec3(0.0f, arbol->LY / 2, 8.0f));
  arbol->scale(glm::vec3(0.5f));
  arbol->setColor(0.0f, 1.0f, 0.0f);
  arbol->model2shader(shader_programme);
  btVector3 proppos = btVector3(distanciaEntreProps * 0, 0, distanciaEntreCapas * 0);
  parallaxProps->newFallBody(btVector3(arbol->LX / 2, arbol->LY / 2, arbol->LZ / 2), proppos, 1, btVector3(0, 0, 0), 2);
  parallaxProps->setVelocity(3, btVector3(-velocityProps, 0, 0));
  
  Model *pasto = new Model(const_cast<char *>("mesh/pasto.obj"));
  pasto->setpos(glm::vec3(0.0f, pasto->LY / 2, 8.0f));
  pasto->scale(glm::vec3(0.5f));
  pasto->setColor(0.0f, 1.0f, 0.0f);
  pasto->model2shader(shader_programme);
  proppos = btVector3(distanciaEntreProps * 1, 0, distanciaEntreCapas * 0);
  parallaxProps->newFallBody(btVector3(pasto->LX / 2, pasto->LY / 2, pasto->LZ / 2), proppos, 1, btVector3(0, 0, 0), 2);
  parallaxProps->setVelocity(4, btVector3(-velocityProps, 0, 0));
  
  Model *plantaone = new Model(const_cast<char *>("mesh/planta1.obj"));
  plantaone->setpos(glm::vec3(0.0f, plantaone->LY / 2, 8.0f));
  plantaone->scale(glm::vec3(0.5f));
  plantaone->setColor(0.0f, 1.0f, 0.0f);
  plantaone->model2shader(shader_programme);
  proppos = btVector3(distanciaEntreProps * 2, 0, distanciaEntreCapas * 0);
  parallaxProps->newFallBody(btVector3(plantaone->LX / 2, plantaone->LY / 2, plantaone->LZ / 2), proppos, 1, btVector3(0, 0, 0), 2);
  parallaxProps->setVelocity(5, btVector3(-velocityProps, 0, 0));
  
  Model *plantatwo = new Model(const_cast<char *>("mesh/planta2.obj"));
  plantatwo->setpos(glm::vec3(0.0f, plantatwo->LY / 2, 8.0f));
  plantatwo->scale(glm::vec3(0.5f));
  plantatwo->setColor(0.0f, 1.0f, 0.0f);
  plantatwo->model2shader(shader_programme);
  proppos = btVector3(distanciaEntreProps * 3, 0, distanciaEntreCapas * 0);
  parallaxProps->newFallBody(btVector3(plantatwo->LX / 2, plantatwo->LY / 2, plantatwo->LZ / 2), proppos, 1, btVector3(0, 0, 0), 2);
  parallaxProps->setVelocity(6, btVector3(-velocityProps, 0, 0));
  
  double nowTime;
  double lastTime = 0.0;
  btVector3 plataformaPos;
  btVector3 rickPos = btVector3(0, 0, 0);
  btVector3 previousObj;
  
  int previousPlatform;
  double previousParallaxObj;
  int minXVel = -2;
  int maxXVel = 2;
  int maxYVel = 12;
  int maxZVel = 3;
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
  
  while (!glfwWindowShouldClose(g_window))
  {  
    window_update_fps_counter (g_window);
    
    nowTime = glfwGetTime();
    
    if (abs(world->getTransformOrigin(world->getLastPlatform()).getX() - world->getTransformOrigin(1).getX()) > (20 * plataformas[0]->LX))
    {
      cout << "last platform modified" << endl;
      cout << world->getLastPlatform();
      if (world->getLastPlatform() == PLATFORMS_START_INDEX) {
        previousPlatform = world->getNMax() - 1;
      } else {
        previousPlatform = world->getLastPlatform() - 1;
      }
      platPos = Gaming::getPlatformPos(platPos.getZ(), platPos.getY(), world->getTransformOrigin(previousPlatform).getX() + plataformas[0]->LX);
      world->editLastPlatform(platPos, 10000, btVector3(platformVelocity, 0, 0), world->getLastPlatform());
    }

    if (abs(parallaxHouses->getTransformOrigin(parallaxHouses->getLastPlatform()).getX() - world->getTransformOrigin(1).getX()) > 20) {
      cout << "last parallax modified" << endl;
      cout << parallaxHouses->getLastPlatform();
      
      if (parallaxHouses->getLastPlatform() == PARALLAX_START_INDEX) {
        previousParallaxObj = parallaxHouses->getNMax() - 1;
      } else {
        previousParallaxObj = parallaxHouses->getLastPlatform() - 1;
      }
      previousObj = parallaxHouses->getTransformOrigin(previousParallaxObj);

      housepos = Gaming::getHousePos(previousObj.getX(), previousObj.getY(), previousObj.getZ());
      parallaxHouses->editLastPlatform(housepos, 10000, btVector3(platformVelocity * 0.5, 0, 0), parallax->getUserIndex(parallax->getLastPlatform()));
    }

    if (abs(parallaxProps->getTransformOrigin(parallaxProps->getLastPlatform()).getX() - world->getTransformOrigin(1).getX()) > 20)
    {
      cout << "last parallax modified" << endl;
      cout << parallaxProps->getLastPlatform();

      if (parallaxProps->getLastPlatform() == PARALLAX_START_INDEX)
      {
        previousParallaxObj = parallaxProps->getNMax() - 1;
      }
      else
      {
        previousParallaxObj = parallaxProps->getLastPlatform() - 1;
      }
      previousObj = parallaxProps->getTransformOrigin(previousParallaxObj);

      proppos = Gaming::getHousePos(previousObj.getX(), previousObj.getY(), previousObj.getZ());
      parallaxProps->editLastPlatform(proppos, 10000, btVector3(platformVelocity * 0.5, 0, 0), parallax->getUserIndex(parallax->getLastPlatform()));
    }

    if (nowTime >= 10.0f && firstTime)
    {
      platformVelocity = -5.0;
      for (int i = PLATFORMS_START_INDEX; i < nplataformas + PLATFORMS_START_INDEX; i += 1)
      {
        world->setVelocity(i, btVector3(platformVelocity, 0, 0));
      }
      for (int i = 0; i < 2; i += 1) {
        parallaxHouses->setVelocity(i, btVector3(platformVelocity * 0.5, 0, 0));
      }

      for (int i = 0; i < 4; i += 1) {
        parallaxProps->setVelocity(i, btVector3(platformVelocity, 0, 0));
      }

      firstTime = false;
    }
    
    if ((nowTime - lastTime) > 15) {
      cout << "dificultad mas" << endl;
      platformVelocity -= 5.0;
      for (int i = 0; i < nplataformas; i += 1) {
        world->setVelocity(i + PLATFORMS_START_INDEX, btVector3(platformVelocity, 0, 0));
      }

      for (int i = 0; i < 2; i += 1)
      {
        parallaxHouses->setVelocity(i, btVector3(platformVelocity * 0.5, 0, 0));
      }

      for (int i = 0; i < 4; i += 1)
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
    world->stepSimulation(fps);
    
    parallaxHouses->stepSimulation();
    parallaxProps->stepSimulation();
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
    
    pared->setpos(glm::vec3(50, 50, -2));
    pared->scale(glm::vec3(10, 1, 1));
    pared->draw();
    
    pared->setpos(glm::vec3(50, 50, 7.5));
    pared->scale(glm::vec3(10, 1, 1));
    pared->draw();
    
    casa1pos = parallaxHouses->getTransformOrigin(0);
    
    casa2pos = parallaxHouses->getTransformOrigin(1);
    
    arbol1pos = parallaxProps->getTransformOrigin(0);
    
    pasto1pos = parallaxProps->getTransformOrigin(1);
    
    plantaone1pos = parallaxProps->getTransformOrigin(2);
    
    plantatwo1pos = parallaxProps->getTransformOrigin(3);
    
    arbol->setpos(glm::vec3(arbol1pos.getX(), arbol1pos.getY(), arbol1pos.getZ()));
    arbol->draw();
    
    pasto->setpos(glm::vec3(pasto1pos.getX(), pasto1pos.getY(), pasto1pos.getZ()));
    pasto->draw();
    
    casa->setpos(glm::vec3(casa1pos.getX(), casa1pos.getY(), casa1pos.getZ()));
    casa->draw();
    
    casa2->setpos(glm::vec3(casa2pos.getX(), casa2pos.getY(), casa2pos.getZ()));
    casa2->draw();
    
    plantaone->setpos(glm::vec3(plantaone1pos.getX(), plantaone1pos.getY(), plantaone1pos.getZ()));
    plantaone->draw();
    
    plantatwo->setpos(glm::vec3(plantatwo1pos.getX(), plantatwo1pos.getY(), plantatwo1pos.getZ()));
    plantatwo->draw();
    
    /* SWAP BUFFER */
    
    window_swap();
  }
  
  glfwTerminate();
  return 0;
}
