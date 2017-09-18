#ifndef PHYSICS_H
#define PHYSICS_H

#ifdef APPLE
#include <BulletDynamics/btBulletDynamicsCommon.h>
#else
#include <btBulletDynamicsCommon.h>
#endif

#include "iostream"

class Bullet {
private:
  btBroadphaseInterface *broadphase;
  btDefaultCollisionConfiguration *collisionConfiguration;
  btCollisionDispatcher *dispatcher;
  btSequentialImpulseConstraintSolver *solver;
  btDiscreteDynamicsWorld *dynamicsWorld;
  int nmax;
  int n;
  btCollisionShape **shapes;
  btDefaultMotionState **motionStates;
  btRigidBody **rigidBodys;
  
public:
  Bullet(int nmax);
  void setGravity(int i, btVector3 vect);
  void applyGravity(int i);
  void getGravity(int i);
  void newPlane(btVector3 plane, btScalar constant);
  void newFallBody(btVector3 extents, btVector3 pos, btScalar mass);
  void setVelocity(int i, btVector3 vel);
  void applyImpulse(int i, btVector3 impulse);
  void applyTranslate(int i, btVector3 vect);
  void stepSimulation();
  btVector3 getTransformOrigin(int i);
  void translate(int i, btVector3 pos);
  void wakeUp(int i);
  
};

#endif
