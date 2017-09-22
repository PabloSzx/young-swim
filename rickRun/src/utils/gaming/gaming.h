#ifndef GAMING_H
#define GAMING_H

#ifdef APPLE
#include <BulletDynamics/btBulletDynamicsCommon.h>
#else
#include <btBulletDynamicsCommon.h>
#endif

#include "../../components/physics/physics.h"

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

public:
    Gaming(int minXVelocity, int maxXVelocity, int maxYVelocity, int maxZVelocity, int minX, int maxX, int minZ, int maxZ);
    void checkRickPos(Bullet *world);
    void checkRickVel(Bullet *world);
    static int getN(int lastPos);
    static btVector3 getPlatformPos(int lastZPlatform, int lastYPlatform, int x);
};

#endif
