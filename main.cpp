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

// by Shital Shah in answer on https://stackoverflow.com/a/41580187

static double getTimeInRelationTo(std::chrono::time_point<std::chrono::high_resolution_clock> t0) {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t0).count()/1000.0;
}


void error_callback(int error, const char* description) {
    std::cout << description << std::endl;
}

int main(int argc, char** argv)
{

  if(argc != 2) {
    std::cout << "usage: ./build/log-esp-opengl <N>\nWhere N = number of lines to use" << std::endl;
    exit(-1);
  }

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
  window = glfwCreateWindow(w_width, w_height, "Hello World", NULL, NULL);
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

    SpiralModelGenerator spiral(aspect_ratio*1.0f, 1.0f, 0.1f, atoi(argv[1]));
    spiral.fillBuffers();

    //docs.gl

    Texture texture("res/textures/Kuo-Toa+Druid-finished.png");
    texture.bind();
    
    VertexArray va;
    VertexBuffer vb(spiral.getVertexBufferData(), spiral.getVertexBufferDataCount()* sizeof (float));
    
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.addBuffer(vb, layout);

    // IndexBuffer ib(spiral.getIndexBufferData(), spiral.getIndexBufferDataCount());

    Shader shader("res/shaders/basic.shader");
    
    shader.bind();

    Camera camera;
    camera.setProjAspectRatio(aspect_ratio, 1.0f);

    Renderer renderer;

    IndexBuffer ib(spiral.getIndexBufferData(), 0);
    unsigned int i = 0;

    const auto initTime = std::chrono::high_resolution_clock::now();
    double t0 = getTimeInRelationTo(initTime);

    while (!glfwWindowShouldClose(window)) {
      /* Render here */

      renderer.clear();

      shader.setUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
      shader.setUniforMat4f("u_MVP", camera.getResult());


      if (i < spiral.getIndexBufferDataCount() && getTimeInRelationTo(initTime) - t0 >= 1.0f/pow(atoi(argv[1]), 1.5)) {
        i += 2;
        t0 = getTimeInRelationTo(initTime);
        std::cout << t0 << std::endl;
        ib.reInitialize(spiral.getIndexBufferData(), i);
      }

      renderer.draw(GL_LINES, va, ib, shader);


      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();

    }

  }

  glfwTerminate();
  return 0;
}