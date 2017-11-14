#include "./menu.hpp"

using namespace std;
Menu::Menu() {
    this->inputTimer = new Time();
    this->status = 0;
    this->difficulty = 1;
    this->step = 1;


    this->maxStepStatus0 = 5;
    this->maxStepStatus1 = 3;

    vector<string> textStatus0;
    textStatus0.push_back("Jugar");
    textStatus0.push_back("Dificultad novato (ON)");
    textStatus0.push_back("Dificultad media (OFF)");
    textStatus0.push_back("Dificultad insane (OFF)");
    textStatus0.push_back("SALIR");

    this->text.push_back(textStatus0);

    vector<string> textStatus1;
    textStatus1.push_back("Jugar denuevo");
    textStatus1.push_back("Volver al menu");
    textStatus1.push_back("Salir");

    this->text.push_back(textStatus1);

    this->label = gltCreateText();
}

void Menu::changeDifficulty(int n) {

}

int Menu::getMaxStep() {
    switch (this->status)
    {
    case 0:
        return this->maxStepStatus0;
    case 1:
        return this->maxStepStatus1;
    default:
        return -1;
    }
}

void Menu::stepPlus() {
    if (this->step == getMaxStep()) {
        this->step = 1;
    } else {
        this->step += 1;
    }
}

void Menu::stepMinus() { 
    if (this->step == 1) {
        this->step = getMaxStep();
    }
    else {
        this->step -= 1;
    }
}

void Menu::setColor(float r, float g, float b, float o) {
    gltColor(r, g, b, o);
}

void Menu::setColor(float rgb, float o) {
    gltColor(rgb, rgb, rgb, o);
}

void Menu::confirm() {
    switch (status) {
        case 0:
        break;
        case 1:
        break;
    }
}

void Menu::drawText() {
    vector<string>::iterator it = this->text[this->status].begin();

    this->setColor(1.0f, 0.0f);

    int x = 80;
    int y = 80;
    while (it != this->text[this->status].end()) {
        gltSetText(this->label, (*it).c_str());
        gltDrawText2D(this->label, x, y, 100);
        y += 10;
        ++it;
    }
}

void Menu::checkInput() {
    if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {

    } else {
        bool modified = false;
        if (glfwGetKey(g_window, GLFW_KEY_UP) == GLFW_PRESS) {
            this->stepPlus();
            modified = true;
        } else if (glfwGetKey(g_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            this->stepMinus();
            modified = true;
        } else if (glfwGetKey(g_window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            this->confirm();
        }
    }
}
