#include <iostream>
#include <chrono>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "GLErrorUtils.hpp"
#include "renderer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

#include "SpiralModelGenerator.hpp"

static double getTimeInRelationTo(std::chrono::time_point<std::chrono::high_resolution_clock> t0) {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t0).count()/1000.0;
}


void error_callback(int error, const char* description) {
    std::cout << description << std::endl;
}

int main(int argc, char** argv)
{

  if(argc != 2) {
    std::cout << "usage: ./build/log-esp-opengl <N>\nWhere N = number of lines to go up to" << std::endl;
    exit(-1);
  } else if (atoi(argv[1]) < 1) {
    std::cout << "invalid value for N!" << std::endl;
    exit(-1);
  }

  unsigned int maxNLinhas = (unsigned int) atoi(argv[1]);
  GLFWwindow* window;
  

  glfwSetErrorCallback(error_callback);
  /* Initialize the library */
  if (!glfwInit()) {
    std::cout << "oh boy" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


  /* Create a windowed mode window and its OpenGL context */
  int w_width = 1080;
  int w_height = 720;
  window = glfwCreateWindow(w_width, w_height, "log-esp-opengl", NULL, NULL);
  float aspect_ratio = (float)w_width/w_height;
  if (!window)
  {
      glfwTerminate();
      return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glfwSwapInterval(1);

  GLcall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  GLcall(glEnable(GL_BLEND));
  // glBlendEquation(GL_FUNC_ADD); ??

  if (glewInit() != GLEW_OK) {
    std::cout << "GLEW is not ok" << std::endl;
  }

  std::cout << glGetString(GL_VERSION) << std::endl;
  {


    Shader shader("res/shaders/basic.shader");
    
    shader.bind();
    shader.setUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

    Camera camera;
    camera.setProjAspectRatio(aspect_ratio, 1.0f);
    
    shader.setUniforMat4f("u_MVP", camera.getResult());

    VertexBufferLayout layout;
    layout.Push<float>(2);
    
    SpiralModelGenerator spiral(aspect_ratio*1.0f, 1.0f, 0.1f, 1);
    spiral.fillBuffers();

    VertexArray va;
    VertexBuffer vb(spiral.getVertexBufferData(), spiral.getVertexBufferDataCount()* sizeof (float));
    
    va.addBuffer(vb, layout);

    Renderer renderer;

    IndexBuffer ib(spiral.getIndexBufferData(), 0);
    unsigned int j = 0;

    const auto initTime = std::chrono::high_resolution_clock::now();
    double t0 = getTimeInRelationTo(initTime);

    unsigned int i = 1;

    while (i <= maxNLinhas) {

      renderer.clear();

      if (j < spiral.getIndexBufferDataCount() && getTimeInRelationTo(initTime) - t0 >= M_E/exp(i)) {
        j += 2;
        ib.reInitialize(spiral.getIndexBufferData(), j);
        t0 = getTimeInRelationTo(initTime);
        
      }

      renderer.draw(GL_LINES, va, ib, shader);


      /* Swap front and back buffers */
      glfwSwapBuffers(window);
      
      if (j >= spiral.getIndexBufferDataCount()) {
        if (getTimeInRelationTo(initTime) - t0 >= 1.0f) {
          ++i;
          spiral = SpiralModelGenerator(aspect_ratio*1.0f, 1.0f, 0.1f, i);
          spiral.fillBuffers();

          
          vb.reInitialize(spiral.getVertexBufferData(), spiral.getVertexBufferDataCount()* sizeof(float));
          j = 0;
          va.addBuffer(vb, layout);

          ib.reInitialize(spiral.getIndexBufferData(), j);
          t0 = getTimeInRelationTo(initTime);
          
        }
      }

    }

  }

  glfwTerminate();
  return 0;
}