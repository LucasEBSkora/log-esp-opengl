#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include <GL/glew.h>
#include <assert.h>

#define GLcall(x) GLClearError();\
  x;\
  assert(glLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool glLogCall(const char* function, const char* file, int line);


#endif