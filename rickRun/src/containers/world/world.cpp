#include "world.hpp"

std::vector<std::string> World::getRandomProp(int i) {
    std::vector<std::string> arr(2);
    // int n = rand() % 4;
    switch (i) {
        case 0:
        arr[0] = "mesh/arbol.obj";
        arr[1] = "mesh/arbol.png";
        break;
        // return {"mesh/arbol.obj", "mesh/arbol.png"};
        case 1 :
        arr[0] = "mesh/pasto.obj";
        arr[1] = "mesh/pasto.png";
        break;
        // return {"mesh/pasto.obj", "mesh/black.png"};
        case 2 :
        arr[0] = "mesh/planta1.obj";
        arr[1] = "mesh/planta1.png";
        break;
        // return {"mesh/planta1.obj", "mesh/black.png"};
        case 3:
        arr[0] = "mesh/planta2.obj";
        arr[1] = "mesh/planta2.png";
        break;
        // return {"mesh/planta2.obj", "mesh/black.png"};
    }

    return arr;
    // return {"",""};
}

World::World(int nPlataformas, int nHouses, int nProps, int nBackgroundMusic, double platformInitialVelocity) {
    this->nPlataformas = nPlataformas;
    this->nHouses = nHouses;
    this->nProps = nProps;
    this->nBackgroundMusic = nBackgroundMusic;
    this->propTypes.reserve(nProps);
    this->platformVelocity = platformInitialVelocity;
}
void World::reset(Parameters* rules) {
    this->platformVelocity = 0.0;

    // this->genPhysics();
    platformWorld = new Bullet(this->nPlataformas + 2, btVector3(0, 0, 0), PLATFORMS_START_INDEX);
    platformWorld->newPlane(btVector3(0, 1, 0), -3.7, 0); //0

    platformWorld->newFallBody(btVector3(rick->LX / 2, rick->LY / 2, rick->LZ / 2), btVector3(0.0, 5.0, 0.0), 1.0, btVector3(0, 0, 0), -1); //1

    distanceScore = new Bullet(1, btVector3(0, 0, 0), 0);
    distanceScore->newFallBody(btVector3(0, 0, 0), btVector3(0, 0, 0), 1.0, btVector3(0, 0, 0), 1);

    this->platPos = btVector3(1.0, 0.0, 0.0);
    platformWorld->newFallBody(btVector3(this->plataformas[0]->LX / 2, this->plataformas[0]->LY * 2, this->plataformas[0]->LZ / 2), this->platPos, 10000, btVector3(0, 0, 0), PLATFORMS_START_INDEX);
    for (int i = 1; i < this->nPlataformas; i += 1) {
        this->platPos = rules->getNextPlatformPos(this->platPos.getZ(), this->platPos.getY(), i * this->plataformas[0]->LX);

        platformWorld->newFallBody(btVector3(this->plataformas[0]->LX / 2, this->plataformas[0]->LY * 4, this->plataformas[0]->LZ / 2), this->platPos, 10000, btVector3(0, 0, 0), i + PLATFORMS_START_INDEX);
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

    this->propPos = btVector3(0, 0, 7);
    parallaxProps->newFallBody(btVector3(this->props[0]->LX / 2, this->props[0]->LY / 2 + 0.1, this->props[0]->LZ / 2), propPos, 1, btVector3(0.0, 0, 0), PARALLAX_START_INDEX);

    for (int i = 1; i < this->nProps; i += 1)
    {
        this->propPos = rules->getNextPropPos(this->propPos.getX(), this->propPos.getY(), this->propPos.getZ());
        parallaxProps->newFallBody(btVector3(this->props[0]->LX / 2 + 0.1, this->props[0]->LY / 2 + 0.1, this->props[0]->LZ / 2), this->propPos, 1, btVector3(0.0, 0, 0), i + PARALLAX_START_INDEX);
    }
}
void World::genRick() {
    this->rick = new Model(const_cast<char *>("mesh/rick.obj"), const_cast<char *>("mesh/littlerick.png"));
    this->rick->scale(glm::vec3(0.3f));
    this->rick->setColor(1.0f, 0.894f, 0.882f);
    this->rick->model2shader(shader_programme);
    platformWorld->newFallBody(btVector3(rick->LX / 2, rick->LY / 2, rick->LZ / 2), btVector3(0.0, 10.0, 0.0), 1.0, btVector3(0, 0, 0), -1); //1

    distanceScore = new Bullet(1, btVector3(0.0, 0.0, 0.0), 1);
    distanceScore->newFallBody(btVector3(0, 0, 0), btVector3(0, 0, 0), 1.0, btVector3(0, 0, 0), 0);
};
void World::genPlatforms(Parameters* rules) {
    this->plataformas = static_cast<Model **>(malloc(sizeof(Model *) * this->nPlataformas));
    this->plataformas[0] = new Model(const_cast<char *>("mesh/platform.obj"), const_cast<char *>("mesh/steel.jpg"));
    this->plataformas[0]->model2shader(shader_programme);

    this->platPos = btVector3(0.0, 0.0, 0.0);

    platformWorld->newFallBody(btVector3(this->plataformas[0]->LX / 2, this->plataformas[0]->LY * 2, this->plataformas[0]->LZ / 2), this->platPos, 10000, btVector3(0, 0, 0), PLATFORMS_START_INDEX);
    for (int i = 1; i < this->nPlataformas; i+=1) {
        this->platPos = rules->getNextPlatformPos(this->platPos.getZ(), this->platPos.getY(), i * this->plataformas[0]->LX);
        platformWorld->newFallBody(btVector3(this->plataformas[0]->LX / 2, this->plataformas[0]->LY * 4, this->plataformas[0]->LZ / 2), this->platPos, 10000, btVector3(0, 0, 0), i + PLATFORMS_START_INDEX);
    }
};
void World::genPhysics() {
    platformWorld = new Bullet(this->nPlataformas + 2, btVector3(0, 0, 0), PLATFORMS_START_INDEX);
    platformWorld->newPlane(btVector3(0, 1, 0), -3.7, 0); //0
    this->plano = new Model(const_cast<char *>("mesh/prueba.obj"), const_cast<char *>("mesh/piso.png"));
    this->plano->setColor(0.8f, 0.0f, 0.0f);
    this->plano->model2shader(shader_programme);
};
void World::genParallaxHouses(Parameters* rules) {
    parallaxHouses = new Bullet(nHouses, btVector3(0, 0, 0), 0);

    this->casas = static_cast<Model **>(malloc(sizeof(Model *) * nHouses));

    this->casas[0] = new Model(const_cast<char *>("mesh/casa.obj"), const_cast<char *>("mesh/casa.png"));
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
    // const char** ran = this->getRandomProp();
    std::vector<std::string> ran;
    for (int i = 0; i < 4; i += 1)
    {
        ran = this->getRandomProp(i);
        this->props[i] = new Model(const_cast<char *>(ran[0].c_str()), const_cast<char *>(ran[1].c_str()));
        this->props[i]->model2shader(shader_programme);
    }
    this->propPos = btVector3(0, 0, 7);
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
void World::morePlatformVelocity() {
    this->platformVelocity -= 5.0;
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
    // double force = 0.016*pow(platformVelocity, 2) + 0.36*abs(platformVelocity) + 9.8;
    platformWorld->applyForce(1, btVector3(0, -9.8, 0));
};
void World::dynamicPlatforms(Parameters* rules) {
    int previousPlatform;
    // cout << "lastPlatform " << platformWorld->getLastPlatform();
    if (abs(platformWorld->getTransformOrigin(platformWorld->getLastPlatform()).getX() - platformWorld->getTransformOrigin(1).getX()) > (20 * this->plataformas[0]->LX))
    {
        // cout << "dynamic" << endl;
        if (platformWorld->getLastPlatform() == PLATFORMS_START_INDEX) {
            previousPlatform = platformWorld->getNMax() - 1;
        } else {
            previousPlatform = platformWorld->getLastPlatform() - 1;
        }
        // cout << "previousPlatform " << platformWorld->getLastPlatform();

        double k = this->plataformas[0]->LX;
        double distancia = 0.04 * k * abs(platformVelocity) + 1 * k;
        this->platPos = rules->getNextPlatformPos(this->platPos.getZ(), this->platPos.getY(), platformWorld->getTransformOrigin(previousPlatform).getX() + distancia);
        platformWorld->editLastPlatform(this->platPos, 10000, btVector3(this->platformVelocity, 0, 0), platformWorld->getLastPlatform());
    }

};
void World::dynamicHouses(Parameters* rules) {
    int previousParallaxObj;
    btVector3 previousObj;

    if (abs(parallaxHouses->getTransformOrigin(parallaxHouses->getLastPlatform()).getX() - platformWorld->getTransformOrigin(1).getX()) > (10 * this->casas[0]->LX))
    {
        // cout << "last house modified " << parallaxHouses->getLastPlatform() << endl;
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
        // cout << "last props modified " << parallaxProps->getLastPlatform() << endl;

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
void World::drawRick() {
    this->rick->setpos(glm::vec3(this->rickPos.getX(), this->rickPos.getY(), this->rickPos.getZ()));
    this->rick->draw();
}
void World::drawPlatforms() {
    btVector3 plataformaPos;

    for (int i = 0; i < nPlataformas; i += 1)
    {
        plataformaPos = platformWorld->getTransformOrigin(i + PLATFORMS_START_INDEX);
        this->plataformas[0]->setpos(glm::vec3(plataformaPos.getX(), plataformaPos.getY(), plataformaPos.getZ()));
        this->plataformas[0]->draw();
    }

};
void World::drawPlane() {
    this->plano->setpos(glm::vec3(25.0, -5.0, 2.6));
    this->plano->draw();
    //for (float i = -20; i <= 100; i += 23.7) {
    //    {
    //        this->plano->setpos(glm::vec3(this->rickPos.getX() + i, -5, this->rickPos.getZ() ));
    //        this->plano->draw();
    //    }
    //}
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
    // std::vector<int> propTypes(this->nProps);
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

    int nrand = rand() % this->nBackgroundMusic;
    this->backgroundMusicNow = nrand;
    background[nrand]->play();
}

void World::backgroundMusic() {
    if (background[this->backgroundMusicNow]->timeToEnd() <= 5) {
        cout << "NUEVA MUSICA DE FONDO" << endl;
        while (true) {
            int nrand = rand() % this->nBackgroundMusic;
            if (nrand != this->backgroundMusicNow) {
                background[nrand]->play();
                this->backgroundMusicNow = nrand;
                break;
            }
        }

        // if (this->backgroundMusicNow == 0) {
        //     background[1]->play();
        //     this->backgroundMusicNow = 1;
        // } else {
        //     background[0]->play();
        //     this->backgroundMusicNow = 0;
        // }
    }
}
