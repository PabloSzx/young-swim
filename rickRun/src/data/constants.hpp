#ifndef CONSTANTS_H
#define CONSTANTS_H

#ifdef APPLE
#define VERTEX_SHADER_FILE "shaders/mac/test_vs.glsl"
#define FRAGMENT_SHADER_FILE "shaders/mac/test_fs.glsl"
#else
#define VERTEX_SHADER_FILE "shaders/linux/test_vs.glsl"
#define FRAGMENT_SHADER_FILE "shaders/linux/test_fs.glsl"
#endif

#define GL_LOG_FILE "log/gl.log"

#define PI 3.14159265359

#define MAX_SHADER_LENGTH 262144

#define PLATFORMS_START_INDEX 2

#define PARALLAX_START_INDEX 0

#endif
