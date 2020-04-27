
#include "GLErrorUtils.hpp"
#include "renderer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <GLFW/glfw3.h>
#include <GL/glew.h>


#include <iostream>

void error_callback(int error, const char* description) {
    std::cout << description << std::endl;
}

int main(void)
{
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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

    if (glewInit() != GLEW_OK) {
      std::cout << "GLEW is not ok" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    {

      float positions[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f
      };

      unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
      };

      //docs.gl

      Texture texture("res/textures/Kuo-Toa+Druid-finished.png");
      texture.bind();

      
      VertexArray va;
      VertexBuffer vb(positions, 4 * (2 + 2)* sizeof (float));
      
      VertexBufferLayout layout;
      layout.Push<float>(2);
      layout.Push<float>(2);
      va.addBuffer(vb, layout);

      IndexBuffer ib(indices, 6);

      Shader shader("res/shaders/basic.shader");
      
      shader.bind();

      shader.setUniform1i("u_Texture", 0);

      Renderer renderer;

      float festa = 0.0f;
      float inc = 0.01f;
      /* Loop until the user closes the window */
      while (!glfwWindowShouldClose(window)) {
          /* Render here */

          renderer.clear();

          shader.setUniform4f("u_Color", 1.0f - festa, festa, 1.0f - festa, 1.0f);

          renderer.draw(va, ib, shader);

          /* Swap front and back buffers */
          glfwSwapBuffers(window);

          /* Poll for and process events */
          glfwPollEvents();

          festa += inc;
          if (festa > 1.0f || festa < 0.0f) inc *= -1;
      }

    }

    glfwTerminate();
    return 0;
}