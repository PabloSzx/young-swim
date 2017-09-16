#ifndef PHYSICS_H
#define PHYSICS_H

#ifdef APPLE
#include <BulletDynamics/btBulletDynamicsCommon.h>
#else
#include <bullet/btBulletDynamicsCommon.h>
#endif

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
  void setGravity(btVector3 vect);
  void newPlane(btVector3 plane, btScalar constant);
  void newFallBody(btScalar radius, btVector3 pos);
  void setVelocity(int i, btVector3 vel);
  void stepSimulation();
  btVector3 getTransformOrigin(int i);
};

#endif
