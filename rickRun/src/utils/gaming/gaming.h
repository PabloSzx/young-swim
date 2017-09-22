#ifndef GAMING_H
#define GAMING_H

#ifdef APPLE
#include <BulletDynamics/btBulletDynamicsCommon.h>
#else
#include <btBulletDynamicsCommon.h>
#endif

#include "../../components/physics/physics.h"

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
  
public:
  Gaming(
    int minXVelocity, int maxXVelocity, int maxYVelocity, int maxZVelocity,
    int minX, int maxX, int minZ, int maxZ,
    double forceHorizontalJump, double forceVerticalUpJump, double forceVerticalDownJump,
    double forceForwardJump, double forceBackwardJump
  );
  void checkRickPos(Bullet *world);
  void checkRickVel(Bullet *world);
  void setForceHorizontalJump(double force);
  void setForceVerticalUpJump(double force);
  void setForceVerticalDownJump(double force);
  void setForceForwardJump(double force);
  void setForceBackwardJump(double force);
  
  static int getN(int lastPos);
  static btVector3 getPlatformPos(int lastZPlatform, int lastYPlatform, int x);
};

#endif
