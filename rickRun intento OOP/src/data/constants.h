#ifndef CONSTANTS_H
#define CONSTANTS_H

#define GL_LOG_FILE "logs/gl.log"

#define MAX_SHADER_LENGTH 262144

// #define _USE_MATH_DEFINES
#define GL_LOG_FILE "logs/gl.log"

#ifdef APPLE
#define VERTEX_SHADER_FILE "shaders/mac/test_vs.glsl"
#define FRAGMENT_SHADER_FILE "shaders/mac/test_fs.glsl"
#else
#define VERTEX_SHADER_FILE "shaders/linux/test_vs.glsl"
#define FRAGMENT_SHADER_FILE "shaders/linux/test_fs.glsl"
#endif

// #define width = 1280;
// #define height = 720;
#endif