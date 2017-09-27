#ifndef GAMING_H
#define GAMING_H

#ifdef APPLE
#include <BulletDynamics/btBulletDynamicsCommon.h>
#else
#include <btBulletDynamicsCommon.h>
#endif

#include "../../components/physics/physics.hpp"

extern double jumpHorizontalForce;
extern double jumpVerticalUpForce;
extern double jumpVerticalDownForce;
extern double jumpForwardForce;
extern double jumpBackwardForce;

class Gaming{
private:
  int maxYVelocity;
  int maxXVelocity;
  int maxZVelocity;
  int minXVelocity;
  int minX;
  int maxX;
  int minZ;
  int maxZ;
  double forceHorizontalJump;
  double forceVerticalUpJump;
  double forceVerticalDownJump;
  double forceForwardJump;
  double forceBackwardJump;
  double distanciaEntreHouse;
  double distanciaEntreProp;
  double distanciaEntreCapas;
  
public:
  Gaming(
    int minXVelocity, int maxXVelocity, int maxYVelocity, int maxZVelocity,
    int minX, int maxX, int minZ, int maxZ,
    double forceHorizontalJump, double forceVerticalUpJump, double forceVerticalDownJump,
    double forceForwardJump, double forceBackwardJump,
    double distanciaEntreHouse, double distanciaEntreProp,
    double distanciaEntreCapas
  );
  void checkRickPos(Bullet *world);
  void checkRickVel(Bullet *world);
  void setForceHorizontalJump(double force);
  void setForceVerticalUpJump(double force);
  void setForceVerticalDownJump(double force);
  void setForceForwardJump(double force);
  void setForceBackwardJump(double force);
  btVector3 getHousePos(int previousXHouse, int x, int z);
  btVector3 getPropPos(int previousXProp, int x, int z);
  
  static int getN(int previousPos);
  static btVector3 getPlatformPos(int previousZPlatform, int previousYPlatform, int x);
  btVector3 getHousePos(double previousXHouse, double y, double z);
  btVector3 getPropPos(double previousXProp, double y, double z);
  double getDistanciaEntreCapas();
};

#endif
