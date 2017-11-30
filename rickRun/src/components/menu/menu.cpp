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
    this->setGlobalStatus(0, "CONSTRUCTOR DE MENU");
    this->difficulty = 0;
    this->difficultyName = "easy";
    this->step = 1;

    this->maxStepStatus0 = 5;
    this->maxStepStatus1 = 3;

    gltInit();

    vector<string> textStatus0;

    // textStatus0.push_back("<Use your arrows to move>");
    textStatus0.push_back("Jugar");
    textStatus0.push_back("Dificultad novato  (ON)");
    textStatus0.push_back("Dificultad media   (OFF)");
    textStatus0.push_back("Dificultad insane  (OFF)");
    textStatus0.push_back("SALIR");

    this->text.push_back(textStatus0);

    vector<string> textStatus1;
    // textStatus1.push_back("<Use your arrows to move>");
    textStatus1.push_back("Jugar de nuevo");
    textStatus1.push_back("Volver al menu");
    textStatus1.push_back("Salir");

    this->text.push_back(textStatus1);

    this->label = gltCreateText();
}

void Menu::setGlobalStatus(int n) {
    globalStatus = n;
    step = 0;
}

void Menu::setGlobalStatus(int n, const char* m) {
    // cout << "SET GLOBAL STATUS LLAMADO EN: " << m << "  " << m << "  " << m << endl;
    globalStatus = n;
    step = 0;
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
            difficultyName = "easy";
            break;
        }
        case 1:
        {
            this->text[0][1] = this->text[0][1].substr(0, 19) + "(OFF)";
            this->text[0][2] = this->text[0][2].substr(0, 19) + "(ON)";
            this->text[0][3] = this->text[0][3].substr(0, 19) + "(OFF)";
            difficultyName = "normal";
            break;
        }
        case 2:
        {
            this->text[0][1] = this->text[0][1].substr(0, 19) + "(OFF)";
            this->text[0][2] = this->text[0][2].substr(0, 19) + "(OFF)";
            this->text[0][3] = this->text[0][3].substr(0, 19) + "(ON)";
            difficultyName = "insane";
            break;
        }
    }
}

string Menu::getDifficultyName() {
    return this->difficultyName;
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
    if (this->inputTimer->every(0.3)) {
        if (this->step < getMaxStep())
        {
            this->step += 1;
        }
    }
}

void Menu::stepMinus()
{
    if (this->inputTimer->every(0.3))
    {

        if (this->step > 1)
        {
            this->step -= 1;
        }
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
    // omp_get
    omp_set_lock(&loading);
    switch (globalStatus)
    {
    case 0: // Menu
    {
        switch (step)
        {
        case 1: // Jugar
        {
            restart = true;
            this->setGlobalStatus(2, "CONFIRM-MENU-JUGAR");
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
            restart = true;
            this->setGlobalStatus(2, "PLAYAGAIN-JUGARDENUEVO");
            break;
        }
        case 2: // Volver al menu
        {
            this->setGlobalStatus(0, "PLAYAGAIN-VOLVERALMENU");
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
    omp_unset_lock(&loading);
}

void Menu::drawText(int distance)
{
    if (globalStatus >= 1)
    { //Esta jugando
        float c=0;
        string puntaje("Puntaje: ");
        string dist(ConvertDoubleToString(distance));
        gltSetText(this->label, (puntaje + dist).c_str());
        this->setColor(0.03f, 0.68f, 0.78f, 0.5f);
        gltDrawText2D(label, 5, 5, 2);
        //Si distancia es multiplo de 100
        if((distance % (100)) <=5 && distance>5){
          gltSetText(this->label, "Sigue Asi!");
          this->setColor(0.51f,0.94f,0.15f, 0.5f); //Ese tiene que ser random
          gltDrawText2D(label, 300, 600, 4);

        }
    }
    if (globalStatus <= 1)
    {
        vector<string>::iterator it = this->text[globalStatus].begin();

        this->setColor(1.0f, 0.0f);
        int x = 80;
        int y = 80;

        int contador = 1;
        while (it != this->text[globalStatus].end())
        {
            if (contador == this->step) {
                this->setColor(0.03f,0.68f,0.78f, 0.0f);
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
    if (jumpButton == -1) {
        int count;
        const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

        for (int i = 0; i < count; i++) {
            if (buttons[i] == GLFW_PRESS) {
                jumpButton = i;
            }
        }
    }
    if (globalStatus != 2) {
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
            // if (axes)
            if (axes[7] < -0.5 || axes[1] < -0.5) {
                this->stepMinus();
            } else if (axes[7] > 0.5 || axes[1] > 0.5) {
                this->stepPlus();
            }

            if (buttons[jumpButton] == GLFW_PRESS) {
                this->confirm();
            }



        }
        else
        {
            // bool modified = false;
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

void Menu::drawArbitrary(int x, int y, double size, int time) {
    gltSetText(this->label, ConvertDoubleToString(time));
    // gltSetText(time);
    this->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(label, x, y, size);
}

void Menu::drawArbitrary(int x, int y, double size, char *str)
{
    gltSetText(this->label, str);
    // gltSetText(time);
    this->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(label, x, y, size);
}
