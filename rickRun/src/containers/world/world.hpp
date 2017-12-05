#ifndef WORLD_H
#define WORLD_H

#include "../../components/model/model.hpp"
#include "../../components/physics/physics.hpp"
#include "../../components/parameters/parameters.hpp"
#include "../../components/sound/sound.hpp"
#include "../../components/time/time.hpp"

#include <string>
#include <vector>
#include <map>
#include <omp.h>
#include <thread>
#include "../../data/constants.hpp"


extern Bullet *platformWorld;
extern Bullet *parallaxHouses;
extern Bullet *parallaxProps;
extern Bullet *distanceScore;

extern sound **background;
extern GLuint shader_programme_cube;

extern int estadoRick;

extern bool debug;

extern omp_lock_t loading;

extern void progressLoading(int a);

class World {
private:
  int nPlataformas;
  int nHouses;
  int nProps;
  int nBackgroundMusic;
  double platformVelocity;

  map< string, vector<Model*> > rick;
  int runAnimationPos;
  int deathAnimationPos;
  Model **plataformas;
  Model *plano;
  Model **casas;
  Model **props;
  Model *cubo;

  btVector3 rickPos;
  btVector3 platPos;
  btVector3 casaPos;
  btVector3 propPos;



  int backgroundMusicNow;

  std::vector<int> propTypes;

  std::vector<std::string> getRandomProp(int i);

public:
  thread cargando;

  World(int nPlataformas, int nHouses, int nProps, int nBackgroundMusic, double platformInitialVelocity);//listo
  void reset(Parameters* rules);
  void genRick();
  void genPlatforms(Parameters* rules);
  void genPhysics();
  void genParallaxHouses(Parameters* rules);
  void genParallaxProps(Parameters* rules);
  void startPlatformVelocity();
  void morePlatformVelocity(double moreVelocity);
  void startHousesVelocity();
  void moreHousesVelocity();
  void startPropsVelocity();
  void morePropsVelocity();
  void getPhysicsPos();
  void gravityRick();
  void dynamicPlatforms(Parameters* rules);
  void dynamicHouses(Parameters* rules);
  void dynamicProps(Parameters* rules);
  void genCube();

  void drawRick();
  void drawPlatforms();
  void drawPlane();
  void drawHouses(Parameters* rules);
  void drawProps();
  void drawCube();
  void nextAnimationRun();
  bool nextAnimationDeath();
  void setRunAnimationPos(int n);
  void setDeathAnimationPos(int n);
  void initBackgroundMusic();
  void backgroundMusic();
  void loadRickMeshes();
  btVector3 getRickPos();
};

#endif
