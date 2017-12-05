#include "world.hpp"

std::vector<std::string> World::getRandomProp(int i) {
    std::vector<std::string> arr(3);
    switch (i) {
        case 0:
        arr[0] = "mesh/arbol.obj";
        arr[1] = "assets/arbol.png";
        arr[2] = "assets/arbol_normal.png";
        break;
        case 1 :
        arr[0] = "mesh/pasto.obj";
        arr[1] = "assets/pasto.png";
        arr[2] = "assets/pasto_normal.png";
        break;
        case 2 :
        arr[0] = "mesh/planta1.obj";
        arr[1] = "assets/planta1.png";
        arr[2] = "assets/planta1_normal.png";
        break;
        case 3:
        arr[0] = "mesh/planta2.obj";
        arr[1] = "assets/planta2.png";
        arr[2] = "assets/planta2_normal.png";
        break;
    }

    return arr;
}

World::World(int nPlataformas, int nHouses, int nProps, int nBackgroundMusic, double platformInitialVelocity) {
    this->nPlataformas = nPlataformas;
    this->nHouses = nHouses;
    this->nProps = nProps;
    this->nBackgroundMusic = nBackgroundMusic;
    this->propTypes.reserve(nProps);
    this->platformVelocity = platformInitialVelocity;
    this->runAnimationPos = 0;
    this->deathAnimationPos = 0;
}
void World::reset(Parameters* rules) {
    this->platformVelocity = 0.0;

    platformWorld = new Bullet(this->nPlataformas + 2, btVector3(0, 0, 0), PLATFORMS_START_INDEX);
    platformWorld->newPlane(btVector3(0, 1, 0), -3.7, 0); //0

    platformWorld->newFallBody(btVector3(this->rick["animation_run"][runAnimationPos]->LX / 2, this->rick["animation_run"][runAnimationPos]->LY / 2, this->rick["animation_run"][runAnimationPos]->LZ / 2), btVector3(0.0, 5.0, 0.0), 1.0, btVector3(0, 0, 0), 1); //1

    distanceScore = new Bullet(1, btVector3(0, 0, 0), 0);
    distanceScore->newFallBody(btVector3(0, 0, 0), btVector3(0, 0, 0), 1.0, btVector3(0, 0, 0), 1);

    this->platPos = btVector3(1.0, 0.0, 0.0);
    platformWorld->newFallBody(this->plataformas[0]->convexShape, this->platPos, 10000, btVector3(0, 0, 0), PLATFORMS_START_INDEX);

    for (int i = 1; i < this->nPlataformas; i += 1) {
        this->platPos = rules->getNextPlatformPos(this->platPos.getZ(), this->platPos.getY(), i * this->plataformas[0]->LX);

        if ((int)rand()%5 == 1) {
            platformWorld->newFallBody(this->plataformas[0]->convexShape, this->platPos, 10000, btVector3(0, 0, 0), -(i + PLATFORMS_START_INDEX));
        } else
        {
            platformWorld->newFallBody(this->plataformas[0]->convexShape, this->platPos, 10000, btVector3(0, 0, 0), i + PLATFORMS_START_INDEX);
        }
    }


    parallaxHouses = new Bullet(this->nHouses, btVector3(0, 0, 0), 0);

    this->casaPos = btVector3(0, 0, 12);
    parallaxHouses->newFallBody(btVector3(this->casas[0]->LX / 2, this->casas[0]->LY / 2 + 0.1, this->casas[0]->LZ / 2), this->casaPos, 1, btVector3(0.0, 0, 0), PARALLAX_START_INDEX);
    for (int i = 1; i < this->nHouses; i += 1)
    {
        this->casaPos = rules->getNextHousePos(this->casaPos.getX(), this->casaPos.getY(), this->casaPos.getZ());
        parallaxHouses->newFallBody(btVector3(this->casas[0]->LX / 2, this->casas[0]->LY / 2 + 0.1, this->casas[0]->LZ / 2), this->casaPos, 1, btVector3(0.0, 0, 0), i);
    }

    parallaxProps = new Bullet(this->nProps, btVector3(0, 0, 0), 0);

    this->propPos = btVector3(0, -5, 7);
    parallaxProps->newFallBody(btVector3(this->props[0]->LX / 2, this->props[0]->LY / 2 + 0.1, this->props[0]->LZ / 2), propPos, 1, btVector3(0.0, 0, 0), PARALLAX_START_INDEX);

    for (int i = 1; i < this->nProps; i += 1)
    {
        this->propPos = rules->getNextPropPos(this->propPos.getX(), this->propPos.getY(), this->propPos.getZ());
        parallaxProps->newFallBody(btVector3(this->props[0]->LX / 2 + 0.1, this->props[0]->LY / 2 + 0.1, this->props[0]->LZ / 2), this->propPos, 1, btVector3(0.0, 0, 0), i + PARALLAX_START_INDEX);
    }

    if (debug) {
        platformWorld->m_pDebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawWireframe);
    }
}

class Objeto {
    public:
    int x;
    Objeto(int n)
    {
        this->x = n;
    }
};

void World::loadRickMeshes() {
    cout << "90" << endl;
    vector<Model *> running;
    cout << "92" << endl;
    cout << "92.5" << endl;
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run01.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run02.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run03.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run04.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run05.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run06.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run07.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run08.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run09.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run10.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run11.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run12.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run13.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run14.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run15.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    cout << "108" << endl;

    vector<Model *> death;
    death.push_back(new Model(const_cast<char *>("mesh/death/rick_falling01.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    death.push_back(new Model(const_cast<char *>("mesh/death/rick_falling02.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    death.push_back(new Model(const_cast<char *>("mesh/death/rick_falling03.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    death.push_back(new Model(const_cast<char *>("mesh/death/rick_falling04.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    death.push_back(new Model(const_cast<char *>("mesh/death/rick_falling05.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    death.push_back(new Model(const_cast<char *>("mesh/death/rick_falling06.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));

    cout << "120" << endl;
    this->rick.insert(make_pair("animation_run", running));
    this->rick.insert(make_pair("animation_death", death));

    omp_unset_lock(&loading);
}

void World::genRick() {
    distanceScore = new Bullet(1, btVector3(0.0, 0.0, 0.0), 1);
    distanceScore->newFallBody(btVector3(0, 0, 0), btVector3(0, 0, 0), 1.0, btVector3(0, 0, 0), 0);

    vector<Model*> running;

    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run01.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run02.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    progressLoading(1);
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run03.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));

    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run04.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));

    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run05.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));

    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run06.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));

    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run07.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run08.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));

    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run09.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    progressLoading(2);
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run10.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));

    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run11.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));

    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run12.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run13.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run14.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    running.push_back(new Model(const_cast<char *>("mesh/run/rick_run15.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    progressLoading(3);
    vector<Model *> death;
    death.push_back(new Model(const_cast<char *>("mesh/death/rick_falling01.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));

    death.push_back(new Model(const_cast<char *>("mesh/death/rick_falling02.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    death.push_back(new Model(const_cast<char *>("mesh/death/rick_falling03.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));

    death.push_back(new Model(const_cast<char *>("mesh/death/rick_falling04.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));

    death.push_back(new Model(const_cast<char *>("mesh/death/rick_falling05.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));
    progressLoading(4);

    death.push_back(new Model(const_cast<char *>("mesh/death/rick_falling06.obj"), const_cast<char *>("assets/littlerick.png"), const_cast<char *>("assets/littlerick_normal.png"), 0.3f));

    this->rick.insert(make_pair("animation_run", running));
    this->rick.insert(make_pair("animation_death", death));

    platformWorld->newFallBody(btVector3(0.932329, 1.40309, 0.42939), btVector3(0.0, 10.0, 0.0), 1.0, btVector3(0, 0, 0), 1); //1
    omp_unset_lock(&loading);
};
void World::genPlatforms(Parameters* rules) {
    this->plataformas = static_cast<Model **>(malloc(sizeof(Model *) * this->nPlataformas));
    this->plataformas[0] = new Model(const_cast<char *>("mesh/platform.obj"), const_cast<char *>("assets/steel.jpg"), const_cast<char *>("assets/steel_normal.png"));
    this->plataformas[0]->model2shader(shader_programme);
    this->plataformas[1] = new Model(const_cast<char *>("mesh/platform.obj"), const_cast<char *>("assets/gold.jpeg"), const_cast<char *>("assets/steel_normal.png"));
    this->plataformas[1]->model2shader(shader_programme);

    this->platPos = btVector3(0.0, 0.0, 0.0);

    platformWorld->newFallBody(this->plataformas[0]->convexShape, this->platPos, 10000, btVector3(0, 0, 0), PLATFORMS_START_INDEX);

    for (int i = 1; i < this->nPlataformas; i+=1) {
        this->platPos = rules->getNextPlatformPos(this->platPos.getZ(), this->platPos.getY(), i * this->plataformas[0]->LX);
        if ((int)rand()%5 == 1) {
            platformWorld->newFallBody(this->plataformas[1]->convexShape, this->platPos, 10000, btVector3(0, 0, 0), -(i + PLATFORMS_START_INDEX));
        } else {
            platformWorld->newFallBody(this->plataformas[0]->convexShape, this->platPos, 10000, btVector3(0, 0, 0), i + PLATFORMS_START_INDEX);
        }
    }
};
void World::genPhysics() {
    platformWorld = new Bullet(this->nPlataformas + 2, btVector3(0, 0, 0), PLATFORMS_START_INDEX);
    platformWorld->newPlane(btVector3(0, 1, 0), -3.7, 0); //0
    this->plano = new Model(const_cast<char *>("mesh/prueba.obj"), const_cast<char *>("assets/piso.png"), const_cast<char *>("assets/piso_normal.png"));
    this->plano->setColor(0.8f, 0.0f, 0.0f);
    this->plano->model2shader(shader_programme);
};
void World::genParallaxHouses(Parameters* rules) {
    parallaxHouses = new Bullet(nHouses, btVector3(0, 0, 0), 0);

    this->casas = static_cast<Model **>(malloc(sizeof(Model *) * nHouses));

    this->casas[0] = new Model(const_cast<char *>("mesh/casa.obj"), const_cast<char *>("assets/casa.png"), const_cast<char *>("assets/casa_normal.png"));
    this->casas[0]->setColor(0.545f, 0.271f, 0.075f);
    this->casas[0]->scale(glm::vec3(5.0f,5.0f,1.0f));
    this->casas[0]->model2shader(shader_programme);

    this->casaPos = btVector3(0, 0, 12);
    parallaxHouses->newFallBody(btVector3(this->casas[0]->LX / 2, this->casas[0]->LY / 2 + 0.1, this->casas[0]->LZ / 2), this->casaPos, 1, btVector3(this->platformVelocity * 0.5, 0, 0), PARALLAX_START_INDEX);

    for (int i = 1; i < this->nHouses; i += 1)
    {
        this->casaPos = rules->getNextHousePos(this->casaPos.getX(), this->casaPos.getY(), this->casaPos.getZ());
        parallaxHouses->newFallBody(btVector3(this->casas[0]->LX / 2, this->casas[0]->LY / 2 + 0.1, this->casas[0]->LZ / 2), this->casaPos, 1, btVector3(this->platformVelocity * 0.5, 0, 0), i);
    }

};
void World::genParallaxProps(Parameters* rules) {
    parallaxProps = new Bullet(nProps, btVector3(0, 0, 0), 0);
    this->props = static_cast<Model **>(malloc(sizeof(Model *) * nProps));
    std::vector<std::string> ran;
    for (int i = 0; i < 4; i += 1)
    {
        ran = this->getRandomProp(i);
        this->props[i] = new Model(const_cast<char *>(ran[0].c_str()), const_cast<char *>(ran[1].c_str()), const_cast<char *>(ran[2].c_str()));
        this->props[i]->model2shader(shader_programme);
    }
    this->propPos = btVector3(0, -5, 7);
    parallaxProps->newFallBody(btVector3(this->props[0]->LX / 2, this->props[0]->LY / 2 + 0.1, this->props[0]->LZ / 2), propPos, 1, btVector3(this->platformVelocity, 0, 0), PARALLAX_START_INDEX);

    for (int i = 0; i < this->nProps; i += 1) {
        this->propTypes[i] = rand() % 4;
    }
    for (int i = 1; i < this->nProps; i += 1)
    {
        this->propPos = rules->getNextPropPos(this->propPos.getX(), this->propPos.getY(), this->propPos.getZ());
        parallaxProps->newFallBody(btVector3(this->props[0]->LX / 2 + 0.1, this->props[0]->LY / 2 + 0.1, this->props[0]->LZ / 2), this->propPos, 1, btVector3(this->platformVelocity, 0, 0), i + PARALLAX_START_INDEX);
    }

};
void World::startPlatformVelocity() {
    this->platformVelocity = -5.0;
    for (int i = PLATFORMS_START_INDEX; i < nPlataformas + PLATFORMS_START_INDEX; i += 1)
    {
        platformWorld->setVelocity(i, btVector3(this->platformVelocity, 0, 0));
    }

    distanceScore->setVelocity(0, btVector3(this->platformVelocity, 0, 0));

    this->startHousesVelocity();
    this->startPropsVelocity();
};
void World::startHousesVelocity() {
    for (int i = 0; i < this->nHouses; i += 1) {
        parallaxHouses->setVelocity(i, btVector3(this->platformVelocity * 0.5, 0, 0));
    }
};
void World::startPropsVelocity() {
    for (int i = 0; i < this->nProps; i += 1) {
        parallaxProps->setVelocity(i, btVector3(this->platformVelocity, 0, 0));
    }
};
void World::morePlatformVelocity(double moreVelocity) {
    this->platformVelocity -= moreVelocity;
    for (int i = 0; i < nPlataformas; i += 1) {
        platformWorld->setVelocity(i + PLATFORMS_START_INDEX, btVector3(platformVelocity, 0, 0));
    }

    distanceScore->setVelocity(0, btVector3(this->platformVelocity, 0, 0));

    this->moreHousesVelocity();
    this->morePropsVelocity();
};
void World::moreHousesVelocity() {
    for (int i = 0; i < nHouses; i += 1)
    {
        parallaxHouses->setVelocity(i, btVector3(platformVelocity * 0.5, 0, 0));
    }

};
void World::morePropsVelocity() {
    for (int i = 0; i < nProps; i += 1)
    {
        parallaxProps->setVelocity(i, btVector3(platformVelocity, 0, 0));
    }

};
void World::gravityRick() {
    platformWorld->applyForce(1, btVector3(0, -9.8, 0));
};
void World::dynamicPlatforms(Parameters* rules) {
    int previousPlatform;
    if (abs(platformWorld->getTransformOrigin(platformWorld->getLastPlatform()).getX() - platformWorld->getTransformOrigin(1).getX()) > (20 * this->plataformas[0]->LX))
    {
        if (platformWorld->getLastPlatform() == PLATFORMS_START_INDEX) {
            previousPlatform = platformWorld->getNMax() - 1;
        } else {
            previousPlatform = platformWorld->getLastPlatform() - 1;
        }

        double k = this->plataformas[0]->LX;
        double distancia = 0.04 * k * abs(platformVelocity) + 1 * k;
        this->platPos = rules->getNextPlatformPos(this->platPos.getZ(), this->platPos.getY(), platformWorld->getTransformOrigin(previousPlatform).getX() + distancia);

        if ((int)rand()%5 == 1) {
            platformWorld->editLastPlatform(this->platPos, 10000, btVector3(this->platformVelocity, 0, 0), -platformWorld->getLastPlatform());
        } else {
            platformWorld->editLastPlatform(this->platPos, 10000, btVector3(this->platformVelocity, 0, 0), platformWorld->getLastPlatform());
        }
    }

};
void World::dynamicHouses(Parameters* rules) {
    int previousParallaxObj;
    btVector3 previousObj;

    if (abs(parallaxHouses->getTransformOrigin(parallaxHouses->getLastPlatform()).getX() - platformWorld->getTransformOrigin(1).getX()) > (10 * this->casas[0]->LX))
    {
        if (parallaxHouses->getLastPlatform() == PARALLAX_START_INDEX) {
            previousParallaxObj = parallaxHouses->getNMax() - 1;
        } else {
            previousParallaxObj = parallaxHouses->getLastPlatform() - 1;
        }
        previousObj = parallaxHouses->getTransformOrigin(previousParallaxObj);

        this->casaPos = rules->getNextHousePos(previousObj.getX(), previousObj.getY(), previousObj.getZ());
        parallaxHouses->editLastPlatform(this->casaPos, 1, btVector3(this->platformVelocity * 0.5, 0, 0), parallaxHouses->getUserIndex(parallaxHouses->getLastPlatform()));
    }

};
void World::dynamicProps(Parameters *rules) {
    int previousParallaxObj;
    btVector3 previousObj;

    if (abs(parallaxProps->getTransformOrigin(parallaxProps->getLastPlatform()).getX() - platformWorld->getTransformOrigin(1).getX()) > (10 * this->props[0]->LX))
    {
        if (parallaxProps->getLastPlatform() == PARALLAX_START_INDEX)
        {
            previousParallaxObj = parallaxProps->getNMax() - 1;
        }
        else
        {
            previousParallaxObj = parallaxProps->getLastPlatform() - 1;
        }
        previousObj = parallaxProps->getTransformOrigin(previousParallaxObj);

        this->propPos = rules->getNextHousePos(previousObj.getX(), previousObj.getY(), previousObj.getZ());
        parallaxProps->editLastPlatform(this->propPos, 1, btVector3(this->platformVelocity, 0, 0), parallaxProps->getUserIndex(parallaxProps->getLastPlatform()));
    }

};
void World::getPhysicsPos() {
    this->rickPos = platformWorld->getTransformOrigin(1);
}

void World::nextAnimationRun() {
    runAnimationPos += 1;
    if (runAnimationPos >= 14)
    {
        runAnimationPos = 0;
    }

    deathAnimationPos = 0;
}

bool World::nextAnimationDeath() {
    deathAnimationPos += 1;
    runAnimationPos = 0;
    if (deathAnimationPos >= 6)
    {
        return true;
    }
    return false;


}

void World::setRunAnimationPos(int n) {
    runAnimationPos = n;
}

void World::setDeathAnimationPos(int n) {
    deathAnimationPos = n;
}

void World::drawRick() {
    int deathPos = deathAnimationPos > 5 ? 5 : deathAnimationPos;
    switch (estadoRick) {
        case 0:
        case 1:
        case 2:
        {
            this->rick["animation_run"][runAnimationPos]->setpos(glm::vec3(this->rickPos.getX(), this->rickPos.getY(), this->rickPos.getZ()));
            this->rick["animation_run"][runAnimationPos]->draw();
            break;
        }
        case 3:
        {
            this->rick["animation_death"][deathPos]->setpos(glm::vec3(this->rickPos.getX(), this->rickPos.getY(), this->rickPos.getZ()));
            this->rick["animation_death"][deathPos]->draw();
            break;
        }
    }
    omp_unset_lock(&loading);
}
void World::drawPlatforms() {
    btVector3 plataformaPos;

    for (int i = PLATFORMS_START_INDEX; i < nPlataformas; i += 1)
    {
        plataformaPos = platformWorld->getTransformOrigin(i);

        if (platformWorld->getUserIndex(i) <= -2)
        {
            this->plataformas[1]->setpos(glm::vec3(plataformaPos.getX(), plataformaPos.getY(), plataformaPos.getZ()));
            this->plataformas[1]->draw();
        }
        else
        {
            this->plataformas[0]->setpos(glm::vec3(plataformaPos.getX(), plataformaPos.getY(), plataformaPos.getZ()));
            this->plataformas[0]->draw();
        }
    }

};
void World::drawPlane() {
    this->plano->setpos(glm::vec3(this->rickPos.getX() + 25.0, -5.0, 2.6));
    this->plano->draw();
};
void World::drawHouses(Parameters* rules) {
    for (int i = 0; i < this->nHouses; i += 1)
    {
        this->casaPos = parallaxHouses->getTransformOrigin(i);
        this->casas[0]->setpos(glm::vec3(this->casaPos.getX(), this->casaPos.getY(), 8 + rules->getDistanciaEntreCapas()));
        this->casas[0]->draw();
    }

    for (int i = 0; i < this->nHouses; i += 1)
    {
        this->casaPos = parallaxHouses->getTransformOrigin(i);
        this->casas[0]->setpos(glm::vec3(this->casaPos.getX(), this->casaPos.getY(), -4 - rules->getDistanciaEntreCapas()));
        this->casas[0]->draw();
    }


};
void World::drawProps() {
    for (int i = 0; i < this->nProps; i += 1)
    {
        this->propPos = parallaxProps->getTransformOrigin(i);
        this->props[propTypes[i]]->setpos(glm::vec3(this->propPos.getX(), this->propPos.getY(), 8));
        this->props[propTypes[i]]->draw();
    }

    for (int i = 0; i < this->nProps; i += 1)
    {
        this->propPos = parallaxProps->getTransformOrigin(i);
        this->props[propTypes[i]]->setpos(glm::vec3(this->propPos.getX(), this->propPos.getY(), -4));
        this->props[propTypes[i]]->draw();
    }

};

btVector3 World::getRickPos() {
    return this->rickPos;
}

void World::initBackgroundMusic() {
    background[0] = new sound(const_cast<char *>("audio/RICK_RUN1.wav"));
    background[1] = new sound(const_cast<char *>("audio/RICK_RUN2.wav"));
    background[2] = new sound(const_cast<char *>("audio/RICK_RUN3.wav"));
    background[3] = new sound(const_cast<char *>("audio/RICK_RUN4.wav"));
    background[4] = new sound(const_cast<char *>("audio/RICK_RUN5.wav"));
    background[5] = new sound(const_cast<char *>("audio/RICK_RUN6.wav"));
    background[6] = new sound(const_cast<char *>("audio/salto.wav"));
    background[7] = new sound(const_cast<char *>("audio/potazo.wav"));
    background[8] = new sound(const_cast<char *>("audio/muerte.wav"));
    background[9] = new sound(const_cast<char *>("audio/coin.wav"));

    this->backgroundMusicNow = 1;
    background[1]->play();
    background[0]->play();
    background[0]->stop();
    background[2]->play();
    background[2]->stop();
    background[3]->play();
    background[3]->stop();
    background[4]->play();
    background[4]->stop();
    background[5]->play();
    background[5]->stop();
    background[6]->play();
    background[6]->stop();
    background[7]->play();
    background[7]->stop();
    background[8]->play();
    background[8]->stop();
    background[9]->play();
    background[9]->stop();
}

void World::backgroundMusic() {
    if (background[this->backgroundMusicNow]->timeToEnd() <= 10.0) {
        while (true) {
            int nrand = rand() % this->nBackgroundMusic;
            if (nrand != this->backgroundMusicNow) {
                background[nrand]->play();
                this->backgroundMusicNow = nrand;
                break;
            }
        }
    }
}

void World::genCube() {
    cubo = new Model(const_cast<char *>("mesh/cubo.obj"), const_cast<char *>("assets/texture_cube2k.png"), const_cast<char *>("assets/texture_normal.png"));
    cubo->scale(glm::vec3(3.0f, 2.0f, 1.8f));
    cubo->setmatloc(shader_programme_cube, "matrix");
    cubo->model2shader(shader_programme_cube);
}

void World::drawCube() {
    cubo->setpos(glm::vec3(this->getRickPos().getX() + 15.0, 11.0, 4.5));
    cubo->draw(shader_programme_cube);
}
