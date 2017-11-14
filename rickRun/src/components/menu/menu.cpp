#include "./menu.hpp"

using namespace std;

const char *ConvertDoubleToString(double value)
{
    std::stringstream ss;
    ss << value;
    const char *str = ss.str().c_str();
    return str;
}

Menu::Menu()
{
    this->inputTimer = new Time();
    this->setGlobalStatus(0);
    this->difficulty = 0;
    this->step = 1;

    this->maxStepStatus0 = 5;
    this->maxStepStatus1 = 3;

    gltInit();

    vector<string> textStatus0;
    textStatus0.push_back("Jugar");

    textStatus0.push_back("Dificultad novato  (ON)");
    textStatus0.push_back("Dificultad media   (OFF)");
    textStatus0.push_back("Dificultad insane  (OFF)");
    textStatus0.push_back("SALIR");

    this->text.push_back(textStatus0);

    vector<string> textStatus1;
    textStatus1.push_back("Jugar denuevo");
    textStatus1.push_back("Volver al menu");
    textStatus1.push_back("Salir");

    this->text.push_back(textStatus1);

    this->label = gltCreateText();
}

void Menu::setGlobalStatus(int n) {
    globalStatus = n;
}

void Menu::changeDifficulty(int n)
{
    this->difficulty = n;

    switch (n) {
        case 0:
        {
            this->text[0][1] = this->text[0][1].substr(0, 19) + "(ON)";
            this->text[0][2] = this->text[0][2].substr(0, 19) + "(OFF)";
            this->text[0][3] = this->text[0][3].substr(0, 19) + "(OFF)";
            break;
        }
        case 1:
        {
            this->text[0][1] = this->text[0][1].substr(0, 19) + "(OFF)";
            this->text[0][2] = this->text[0][2].substr(0, 19) + "(ON)";
            this->text[0][3] = this->text[0][3].substr(0, 19) + "(OFF)";
            break;
        }
        case 2:
        {
            this->text[0][1] = this->text[0][1].substr(0, 19) + "(OFF)";
            this->text[0][2] = this->text[0][2].substr(0, 19) + "(OFF)";
            this->text[0][3] = this->text[0][3].substr(0, 19) + "(ON)";
            break;
        }
    }
}

int Menu::getMaxStep()
{
    switch (globalStatus)
    {
    case 0:
        return this->maxStepStatus0;
    case 1:
        return this->maxStepStatus1;
    default:
        return -1;
    }
}

void Menu::stepPlus()
{
    if (this->step < getMaxStep())
    {
        this->step += 1;
    }
}

void Menu::stepMinus()
{
    if (this->step > 1)
    {
        this->step -= 1;
    }
}

void Menu::setColor(float r, float g, float b, float o)
{
    gltColor(r, g, b, o);
}

void Menu::setColor(float rgb, float o)
{
    gltColor(rgb, rgb, rgb, o);
}

void Menu::confirm()
{
    switch (globalStatus)
    {
    case 0: // Menu
    {
        switch (step)
        {
        case 1: // Jugar
        {
            this->setGlobalStatus(2);
            break;
        }
        case 2: // Dificultad novato
        {
            this->changeDifficulty(0);
            break;
        }
        case 3: // Dificultad media
        {
            this->changeDifficulty(1);
            break;
        }
        case 4: // Dificultad insane
        {
            this->changeDifficulty(2);
            break;
        }
        case 5: // Salir
        {
            glfwSetWindowShouldClose(g_window, true);
            break;
        }
        }
        break;
    }
    case 1: // Play again
    {
        switch (step)
        {
        case 1: // Jugar denuevo
        {
            this->setGlobalStatus(2);
            restart = true;
            break;
        }
        case 2: // Volver al menu
        {
            this->setGlobalStatus(0);
            restart = true;
            break;
        }
        case 3: // Salir
        {
            glfwSetWindowShouldClose(g_window, true);
            break;
        }
        }
        break;
    }
    }
}

void Menu::drawText(int distance)
{
    if (globalStatus == 2)
    { //Esta jugando
        gltSetText(this->label, ConvertDoubleToString(distance));
        gltDrawText2D(label, 5, 5, 2);
    }
    else
    {
        vector<string>::iterator it = this->text[globalStatus].begin();

        this->setColor(1.0f, 0.0f);

        int x = 80;
        int y = 80;

        int contador = 1;
        while (it != this->text[globalStatus].end())
        {
            if (contador == this->step) {
                this->setColor(0.5f, 0.0f);
            }
            gltSetText(this->label, (*it).c_str());
            gltDrawText2D(this->label, x, y, 4);
            y += 70;
            if (contador == this->step)
            {
                this->setColor(1.0f, 0.0f);
            }

            ++it;
            ++contador;
        }
    }
}

void Menu::checkInput()
{
    this->inputTimer->updateNow();
    if (this->inputTimer->every(0.4) && globalStatus != 2) {
        if (glfwJoystickPresent(GLFW_JOYSTICK_1))
        {
            int count;

            const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

            int count2;

            const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count2);

            // for (int i = 0; i < 8; i++) {
            //     cout << "axes " << i << "  " << axes[i] << endl;
            // }
            // cout << "COUNT ES " << count << endl;
            if (axes[7] < -0.5) {
                this->stepMinus();
            } else if (axes[7] > 0.5) {
                this->stepPlus();
            }

            if (buttons[jumpButton] == GLFW_PRESS) {
                this->confirm();
            }



        }
        else
        {
            bool modified = false;
            if (glfwGetKey(g_window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                this->stepMinus();
            }
            else if (glfwGetKey(g_window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                this->stepPlus();
            }
            else if (glfwGetKey(g_window, GLFW_KEY_ENTER) == GLFW_PRESS)
            {
                this->confirm();
            }
        }


        // cout << this->step << endl;
    }

}

void Menu::restartTime() {
    this->inputTimer->restart();
}

