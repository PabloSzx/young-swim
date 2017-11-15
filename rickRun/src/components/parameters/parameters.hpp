#ifndef ParametersS_H
#define ParametersS_H

#include <btBulletDynamicsCommon.h>


#include "../../components/physics/physics.hpp"

extern double jumpHorizontalForce;
extern double jumpVerticalUpForce;
extern double jumpVerticalDownForce;
extern double jumpForwardForce;
extern double jumpBackwardForce;
extern int globalStatus;
extern bool restart;
extern sound** background;

extern float deltaTime;
class Parameters{
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
  Parameters(
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
  double getDistance(Bullet* distanceScore);

  /* GENERACION DE PLATAFORMAS SEMI ALEATORIA Y PARALLAX OBJECTS */
  btVector3 getNextHousePos(int previousXHouse, int x, int z);
  btVector3 getNextPropPos(int previousXProp, int x, int z);
  int getN(int previousPos);
  btVector3 getNextPlatformPos(int previousZPlatform, int previousYPlatform, int x);
  btVector3 getNextHousePos(double previousXHouse, double y, double z);
  btVector3 getNextPropPos(double previousXProp, double y, double z);
  double getDistanciaEntreCapas();
};

#endif
