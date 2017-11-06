#ifndef WORLD_H
#define WORLD_H

#include "../../components/model/model.hpp"
#include "../../components/physics/physics.hpp"
#include "../../components/parameters/parameters.hpp"
#include "../../components/sound/sound.hpp"
#include <string>
#include <vector>

#include "../../data/constants.hpp"

extern Bullet *platformWorld;
extern Bullet *parallaxHouses;
extern Bullet *parallaxProps;
extern Bullet *distanceScore;

extern sound **background;
extern GLuint shader_programme_cube;

extern bool debug;
class World {
private:
  int nPlataformas;
  int nHouses;
  int nProps;
  int nBackgroundMusic;
  double platformVelocity;
  
  Model* rick;
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
  
  World(int nPlataformas, int nHouses, int nProps, int nBackgroundMusic, double platformInitialVelocity);//listo
  void reset(Parameters* rules);
  void genRick();//listo
  void genPlatforms(Parameters* rules);//listo
  void genPhysics();//listo
  void genParallaxHouses(Parameters* rules); //listo
  void genParallaxProps(Parameters* rules);//listo
  void startPlatformVelocity();//listo
  void morePlatformVelocity();//listo
  void startHousesVelocity();//listo
  void moreHousesVelocity();//listo
  void startPropsVelocity();//listo
  void morePropsVelocity();//listo
  void getPhysicsPos();//listo
  void gravityRick();//listo
  void dynamicPlatforms(Parameters* rules);//listo
  void dynamicHouses(Parameters* rules);//listo
  void dynamicProps(Parameters* rules);//listo
  void genCube();

  void drawRick();//listo
  void drawPlatforms();//listo
  void drawPlane();//listo
  void drawHouses(Parameters* rules);
  void drawProps();
  void drawCube();
  

  void initBackgroundMusic();
  void backgroundMusic();
  
  
  btVector3 getRickPos();
};

#endif
