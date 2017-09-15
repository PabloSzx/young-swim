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
#include <glut/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "./utils/gl_utils/gl_utils.h"
#include "./utils/window/window.h"
#include "./components/model/model.h"
#include <BulletDynamics/btBulletDynamicsCommon.h>

#include "./data/constants.h"
#include "./data/global.h"
// int g_gl_width = 1280; //constants
// int g_gl_height = 720; //constants
// GLFWwindow* g_window = NULL; //main

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


// // camera
// glm::vec3 cameraPos   = glm::vec3(0.0f, 2.0f, 7.0f);
// glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
// glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

// bool firstMouse = true;
// float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
// float pitch =  0.0f;
// float lastX =  g_gl_width / 2.0;
// float lastY =  g_gl_height / 2.0;
// float fov   =  45.0f;

// // timing
// float deltaTime = 0.0f;	// time between current frame and last frame
// float lastFrame = 0.0f;

// int maxrand = 10;

using namespace std;

int fiftyfifty() {
  if (rand()%2 == 1) {
    return 1;
  }
  return -1;
}

int main(int argc, char const *argv[]){
  
  // if (argc < 2) {
  //   cout << "error, ingrese cantidad de modelos" << endl;
  //   exit(EXIT_FAILURE);
    
  // }
  
  // int N = atoi(argv[1]);
  
  srand (time(NULL));

  float x,y,z;
  
  restart_gl_log ();
  start_gl ();

  glEnable (GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
  // glEnable (GL_CULL_FACE); // cull face
  glCullFace (GL_BACK); // cull back face
  glFrontFace (GL_CCW); // set counter-clock-wise vertex order to mean the front
  glClearColor (0.8f, 0.5f, 0.5f, 1.0f); // grey background to help spot mistakes
  // glViewport (0, 0, g_gl_width, g_gl_height);
  
  glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);
  glfwSetCursorPosCallback(g_window, mouse_callback);
  glfwSetScrollCallback(g_window, scroll_callback);
  // tell GLFW to capture our mouse
  glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  /*-------------------------------CREATE SHADERS-------------------------------*/
  GLuint shader_programme = create_programme_from_files (
    VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE
  );
  
  glm::mat4 projection = glm::perspective(glm::radians(fov), (float)g_gl_width / (float)g_gl_height, 0.1f, 100.0f);
  glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
  
  
  // int matrix_location = glGetUniformLocation (shader_programme, "matrix");
  
  int view_mat_location = glGetUniformLocation (shader_programme, "view");
  glUseProgram (shader_programme);
  glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &view[0][0]);
  int proj_mat_location = glGetUniformLocation (shader_programme, "proj");
  glUseProgram (shader_programme);
  glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, &projection[0][0]);
  
  // malla **monos = static_cast<malla**>(malloc(sizeof(malla*) * N));

  malla *rick = new malla(const_cast<char *>("mallas/rick.obj"));
  rick->setpos(glm::vec3(0.0f, 0.0f, 0.0f));
  rick->scale(glm::vec3(1.0f));
  rick->setmatloc(shader_programme, "matrix");
  rick->model2shader(shader_programme);

  cout << rick->LX << " " << rick->LY << " " << rick->LZ << endl;

  malla *eje = new malla(const_cast<char*>("mallas/axis.obj"));
  eje->setpos(glm::vec3(0.0f,0.0f,0.0f));
  eje->scale(glm::vec3(1.0f));
  eje->setmatloc(shader_programme, "matrix");
  eje->model2shader(shader_programme);
  
  
  malla *axisX = new malla((char*)"mallas/x.obj");
  axisX->setpos(glm::vec3(8.0f,0.0f,0.0f));
  axisX->scale(glm::vec3(0.5f));
  axisX->setmatloc(shader_programme, "matrix");
  axisX->model2shader(shader_programme);
  
  
  malla *axisY = new malla((char*)"mallas/y.obj");
  axisY->setpos(glm::vec3(0.0f,8.0f,0.0f));
  axisY->scale(glm::vec3(0.5f));
  axisY->setmatloc(shader_programme, "matrix");
  axisY->model2shader(shader_programme);
  
  
  malla *axisZ = new malla((char*)"mallas/z.obj");
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
  btCollisionShape *fallShape = new btSphereShape(rick->LY);

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

  // for (int i = 0; i < N; i++) {
  //   monos[i] = new malla((char*)"mallas/suzanne.obj");
  //   //sepos(glm::vec3 p);
  //   x = fiftyfifty() * (float)(rand() % maxrand);
  //   y = fiftyfifty() * (float)(rand() % maxrand);
  //   z = fiftyfifty() * (float)(rand() % maxrand);
    
  //   monos[i]->setpos(glm::vec3(x, y, z));
  //   monos[i]->setmatloc(shader_programme, "matrix");
  //   monos[i]->model2shader(shader_programme);
  // }
  
  // for (int i = 0; i < N; i++) {
  //   for (int j = 0; j < N; j++) {
  //     if (i != j) {
  //       if (monos[i]->colisiona(monos[j])) {
  //         cout << "COLISION!!  " << i << "-" << j << endl;
  //       }
  //     }
  //   }
  // }
  
  
  // render loop
  // -----------
  while (!glfwWindowShouldClose(g_window)){
    // per-frame time logic
    // --------------------
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    dynamicsWorld->stepSimulation(1 / 60.f, 10);

    btTransform trans;
    fallRigidBody->getMotionState()->getWorldTransform(trans);

    std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;
    // input
    // -----
    processInput(g_window);
    
    // render
    // ------
    glClearColor(0.81640625f, 0.81640625f, 0.81640625f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // activate shader
    glUseProgram (shader_programme);
    
    // pass projection matrix to shader (note that in this case it could change every frame)
    projection = glm::perspective(glm::radians(fov), (float)g_gl_width / (float)g_gl_height, 0.1f, 100.0f);
    glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, &projection[0][0]);
    
    // camera/view transformation
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &view[0][0]);
    
    // glm::mat4 mat = glm::mat4();
    
    // for (int i = 0; i < N; i++) {
    //   monos[i]->model2shader(shader_programme);
    //   // glUniformMatrix4fv(matrix_location, 1, GL_FALSE, &(monos[i]->getmodel())[0][0]);
    //   glBindVertexArray(monos[i]->getvao());
    //   glDrawArrays(GL_TRIANGLES,0,monos[i]->getnumvertices());
    // }

    // glUniformMatrix4fv(matrix_location, 1, GL_FALSE, &mat[0][0]);
    rick->setpos(glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
    // rick->setpos(glm::vec3(0, 0, 0));

    rick->model2shader(shader_programme);
    glBindVertexArray(rick->getvao());
    glDrawArrays(GL_TRIANGLES, 0, rick->getnumvertices());

    eje->model2shader(shader_programme);
    glBindVertexArray(eje->getvao());
    glDrawArrays(GL_TRIANGLES,0,eje->getnumvertices());
    
    
    axisX->model2shader(shader_programme);
    glBindVertexArray(axisX->getvao());
    glDrawArrays(GL_TRIANGLES,0,axisX->getnumvertices());
    
    
    axisY->model2shader(shader_programme);
    glBindVertexArray(axisY->getvao());
    glDrawArrays(GL_TRIANGLES,0,axisY->getnumvertices());
    
    
    axisZ->model2shader(shader_programme);
    glBindVertexArray(axisZ->getvao());
    glDrawArrays(GL_TRIANGLES,0,axisZ->getnumvertices());
    /*muros*/
    
    // for (GLfloat i = 0; i < 50.0f; i+=1.0f) {
      //   for (GLfloat j = 0; j < 50.0f; j+=1.0f) {
        //     mat = glm::mat4();
        //     mat = glm::translate(mat, cameraPos + glm::vec3(10.0f, j, i));
        //     mat = glm::scale(mat, glm::vec3(5.0f, 5.0f, 5.0f));
        //     mat = glm::rotate(mat, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //     glUniformMatrix4fv(matrix_location, 1, GL_FALSE, &mat[0][0]);
        //     glBindVertexArray(plano->getvao());
        //     glDrawArrays(GL_TRIANGLES,0,plano->getnumvertices());
        //   }
        // }
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(g_window);
        glfwPollEvents();
        
      }
      // glfw: terminate, clearing all previously allocated GLFW resources.
      // ------------------------------------------------------------------
      glfwTerminate();
      return 0;
    }
    
    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void processInput(GLFWwindow *window){
      if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
      
      float cameraSpeed = 2.5 * deltaTime;
      if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      cameraPos += cameraSpeed * cameraFront;
      if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      cameraPos -= cameraSpeed * cameraFront;
      if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
      if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    
    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow* window, int width, int height){
      // make sure the viewport matches the new window dimensions; note that width and
      // height will be significantly larger than specified on retina displays.
      glViewport(0, 0, width, height);
    }
    
    // glfw: whenever the mouse moves, this callback is called
    // -------------------------------------------------------
    void mouse_callback(GLFWwindow* window, double xpos, double ypos){
      if (firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
      }
      
      float xoffset = xpos - lastX;
      float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
      lastX = xpos;
      lastY = ypos;
      
      float sensitivity = 0.1f; // change this value to your liking
      xoffset *= sensitivity;
      yoffset *= sensitivity;
      
      yaw += xoffset;
      pitch += yoffset;
      
      // make sure that when pitch is out of bounds, screen doesn't get flipped
      if (pitch > 89.0f)
      pitch = 89.0f;
      if (pitch < -89.0f)
      pitch = -89.0f;
      
      glm::vec3 front;
      front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
      front.y = sin(glm::radians(pitch));
      front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
      cameraFront = glm::normalize(front);
    }
    
    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    // ----------------------------------------------------------------------
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
      if (fov >= 1.0f && fov <= 45.0f)
      fov -= yoffset;
      if (fov <= 1.0f)
      fov = 1.0f;
      if (fov >= 45.0f)
      fov = 45.0f;
    }
    
