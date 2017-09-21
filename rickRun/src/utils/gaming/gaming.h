#ifndef GAMING_H
#define GAMING_H

#ifdef APPLE
#include <BulletDynamics/btBulletDynamicsCommon.h>
#else
#include <btBulletDynamicsCommon.h>
#endif

class Gaming{
private:
public:
    Gaming();
    static int getN(int lastPos);
    static btVector3 getPlatformPos(int lastZPlatform, int lastYPlatform, int x);
};

#endif
