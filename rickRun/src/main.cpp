/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries for separate legal notices                          |
|******************************************************************************|
| This demo uses the Assimp library to load a mesh from a file, and supports   |
| many formats. The library is VERY big and complex. It's much easier to write |
| a simple Wavefront .obj loader. I have code for this in other demos. However,|
| Assimp will load animated meshes, which will we need to use later, so this   |
| demo is a starting point before doing skinning animation                     |
\******************************************************************************/
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

#include "./utils/gl_utils/gl_utils.h"
#include "./utils/window/window.h"
#include "./components/model/model.h"
#include "./utils/log/log.h"
#include "./components/input/input.h"
#include "./components/camera/camera.h"

#ifdef APPLE
#include <BulletDynamics/btBulletDynamicsCommon.h>
#else
#include <btBulletDynamicsCommon.h>
#endif


#include "./data/constants.h"
#include "./data/global.h"

using namespace std;

int main(){
  srand (time(NULL));
  
  float x,y,z;
  
  restart_gl_log ();
  window_start_gl();
  
  window_flags();
  
  setCallbacks();
  /*-------------------------------CREATE SHADERS-------------------------------*/
  shader_programme = create_programme_from_files (
    VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE
  );
  
  viewMatrixLocation();
  projMatrixLocation();
  
  Model *rick = new Model(const_cast<char *>("mesh/rick.obj"));
  rick->setpos(glm::vec3(0.0f, 0.0f, 0.0f));
  rick->scale(glm::vec3(1.0f));
  rick->setmatloc(shader_programme, "matrix");
  rick->model2shader(shader_programme);
  
  // cout << rick->LX << " " << rick->LY << " " << rick->LZ << endl;
  
  Model *eje = new Model(const_cast<char*>("mesh/axis.obj"));
  eje->setpos(glm::vec3(0.0f,0.0f,0.0f));
  eje->scale(glm::vec3(1.0f));
  eje->setmatloc(shader_programme, "matrix");
  eje->model2shader(shader_programme);
  
  Model *axisX = new Model(const_cast<char*>("mesh/x.obj"));
  axisX->setpos(glm::vec3(8.0f,0.0f,0.0f));
  axisX->scale(glm::vec3(0.5f));
  axisX->setmatloc(shader_programme, "matrix");
  axisX->model2shader(shader_programme);
  
  
  Model *axisY = new Model(const_cast<char*>("mesh/y.obj"));
  axisY->setpos(glm::vec3(0.0f,8.0f,0.0f));
  axisY->scale(glm::vec3(0.5f));
  axisY->setmatloc(shader_programme, "matrix");
  axisY->model2shader(shader_programme);
    
  Model *axisZ = new Model(const_cast<char*>("mesh/z.obj"));
  axisZ->setpos(glm::vec3(0.0f,0.0f,8.0f));
  axisZ->scale(glm::vec3(0.5f));
  axisZ->setmatloc(shader_programme, "matrix");
  axisZ->model2shader(shader_programme);
  
  btBroadphaseInterface *broadphase = new btDbvtBroadphase();
  
  btDefaultCollisionConfiguration *collisionConfiguration = new btDefaultCollisionConfiguration();
  btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collisionConfiguration);
  
  btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver;
  
  btDiscreteDynamicsWorld *dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
  
  dynamicsWorld->setGravity(btVector3(0, -9.8, 0));
  
  btCollisionShape *groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
  
  // btCollisionShape *fallShape = new btBoxShape(btVector3(rick->LX / 2, rick->LY / 2, rick->LZ / 2));
  btCollisionShape *fallShape = new btSphereShape(rick->LY / 2);
  
  btDefaultMotionState *groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
  
  btRigidBody::btRigidBodyConstructionInfo
  groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
  
  btRigidBody *groundRigidBody = new btRigidBody(groundRigidBodyCI);
  dynamicsWorld->addRigidBody(groundRigidBody);
  
  btDefaultMotionState *fallMotionState =
  new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
  btScalar mass = 1;
  
  btVector3 fallInertia(0, 0, 0);
  
  fallShape->calculateLocalInertia(mass, fallInertia);
  btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
  btRigidBody *fallRigidBody = new btRigidBody(fallRigidBodyCI);
  fallRigidBody->setLinearVelocity(btVector3(2.0, 0.0, 1.0));
  
  dynamicsWorld->addRigidBody(fallRigidBody);
  
  while (!glfwWindowShouldClose(g_window)){
    frameCounter();
    
    dynamicsWorld->stepSimulation(1 / 60.f, 10);
    
    btTransform trans;
    fallRigidBody->getMotionState()->getWorldTransform(trans);
    
    // std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;

    processInput(g_window);

    glClearColor(0.81f, 0.81f, 0.81f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram (shader_programme);
    
    projectionMatrixPerspective();
    
    viewMatrixPerspective();

    rick->setpos(glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
    // rick->setpos(glm::vec3(0, 0, 0));
    
    rick->model2shader(shader_programme);
    glBindVertexArray(rick->getvao());
    glDrawArrays(GL_TRIANGLES, 0, rick->getnumvertices());
    
    eje->model2shader(shader_programme);
    glBindVertexArray(eje->getvao());
    glDrawArrays(GL_TRIANGLES, 0, eje->getnumvertices());
    
    axisX->model2shader(shader_programme);
    glBindVertexArray(axisX->getvao());
    glDrawArrays(GL_TRIANGLES, 0, axisX->getnumvertices());
    
    axisY->model2shader(shader_programme);
    glBindVertexArray(axisY->getvao());
    glDrawArrays(GL_TRIANGLES, 0, axisY->getnumvertices());
    
    axisZ->model2shader(shader_programme);
    glBindVertexArray(axisZ->getvao());
    glDrawArrays(GL_TRIANGLES, 0, axisZ->getnumvertices());


    glfwSwapBuffers(g_window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
