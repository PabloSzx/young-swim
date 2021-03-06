#ifndef PHYSICS_H
#define PHYSICS_H

#include "iostream"

#include <btBulletDynamicsCommon.h>
#include "../sound/sound.hpp"
#include "../debugDrawer/DebugDrawer.h"

extern bool restart;
extern int lastPlatformCollision;
extern sound **background;
extern int globalStatus;
extern int estadoRick;

extern float deltaTime;

extern bool plus50;

class Bullet {
private:
  btBroadphaseInterface *broadphase;
  btDefaultCollisionConfiguration *collisionConfiguration;
  btCollisionDispatcher *dispatcher;
  btSequentialImpulseConstraintSolver *solver;
  btDiscreteDynamicsWorld *dynamicsWorld;
  int nmax;
  int n;
  int lastPlatform;
  int resetEdit;
  btCollisionShape **shapes;
  btDefaultMotionState **motionStates;
  btRigidBody **rigidBodys;
  
public:
  DebugDrawer *m_pDebugDrawer;
  Bullet(int nmax, btVector3 gravity, int resetEdit);
  void setGravity(int i, btVector3 vect);
  void applyGravity(int i);
  void getGravity(int i);
  void newPlane(btVector3 plane, btScalar constant, int index);
  void newFallBody(btVector3 extents, btVector3 pos, btScalar mass, btVector3 velocity, int index);
  void newFallBody(btConvexHullShape *convexShape, btVector3 pos, btScalar mass, btVector3 velocity, int index);
  void editLastPlatform(btVector3 pos, btScalar mass, btVector3 velocity, int index);
  void editBody(int i, btVector3 extents, btVector3 pos, btScalar mass, btVector3 velocity, int index);
  void setVelocity(int i, btVector3 vel);
  btVector3 getVelocity(int i);
  void applyImpulse(int i, btVector3 impulse);
  void applyForce(int i, btVector3 force);
  void applyTranslate(int i, btVector3 vect);
  btVector3 getTotalForce(int i);
  void stepSimulation(double fps);
  btVector3 getTransformOrigin(int i);
  void translate(int i, btVector3 pos);
  void wakeUp(int i);
  void checkCollision(bool* allowJump);
  int getUserIndex(int i);
  void setUserIndex(int pos, int i);
  int getIndiceAsociado(int i);
  int getLastPlatform();
  int getNMax();
  void debugDrawWorld();
};

extern Bullet* platformWorld;
extern Bullet *distanceScore;
extern void drawArbitrary(int x, int y, double size, char *str);
#endif
