#pragma once

#include "libs.h"
#include <fstream>

void init_window(int *frameBufferWidth, int *frameBufferHeight, GLFWwindow **window);
void init_glew();
void init_opengl_options();
void init_shader(GLuint &program);