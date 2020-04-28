#include <iostream>
#include <chrono>

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

static double getTimeSinceEpoch(std::chrono::high_resolution_clock::time_point* t = nullptr)
{
    using Clock = std::chrono::high_resolution_clock;
    return std::chrono::duration<double>((t != nullptr ? *t : Clock::now() ).time_since_epoch()).count();
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

    IndexBuffer ib(spiral.getIndexBufferData(), spiral.getIndexBufferDataCount());
    unsigned int i = 0;

    float t0 = getTimeSinceEpoch();

    while (!glfwWindowShouldClose(window)) {
      /* Render here */

      renderer.clear();

      shader.setUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
      shader.setUniforMat4f("u_MVP", camera.getResult());

      std::cout << t0 << "  " << getTimeSinceEpoch() << std::endl; 

      if (i < spiral.getIndexBufferDataCount() && getTimeSinceEpoch() - t0 >= 1.0f/atoi(argv[1])) {
        std::cout << "chupa meu cu" << std::endl;
        i += 2;
        t0 = getTimeSinceEpoch();
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