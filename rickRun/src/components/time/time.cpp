#include "time.hpp"


Time::Time() {
    this->now = 0.0;
    this->last = 0.0;
    this->first = true;
}
void Time::lastIsNow() {
    this->last = this->now;
}
void Time::updateNow() {
    this->now = glfwGetTime();
}
double Time::getUpdateNow()
{
    this->now = glfwGetTime();
    return this->now;
}
double Time::getLast() {
    return this->last;
}
bool Time::checkFirstTime(double t) {
    if ((this->now >= t) && (this->first)) {
        this->first = false;
        return true;
    }
    return false;
}
bool Time::every(double t) {
    if ((this->now - this->last) >= t) {
        this->lastIsNow();
        return true;
    }
    return false;
}
void Time::setGlobalTime(double t) {
    glfwSetTime(t);
}
void Time::restart() {
    this->first = true;
    this->now = 0.0;
    this->last = 0.0;
    this->setGlobalTime(0.0);
}
