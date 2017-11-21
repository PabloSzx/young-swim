#ifndef GLERROR_H
#define GLERROR_H

#include <GLFW/glfw3.h>
#include <iostream>

void _check_gl_error(const char *file, int line);

///
/// Usage
/// [... some opengl calls]
/// glCheckError();
///
#define check_gl_error() _check_gl_error(__FILE__, __LINE__)

#endif // GLERROR_H