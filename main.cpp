
#include "renderer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Shader.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>


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

    if (glewInit() != GLEW_OK) {
      std::cout << "GLEW is not ok" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    {

      float positions[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f,  0.5f,
        -0.5f,  0.5f
      };

      unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
      };

      //docs.gl

      VertexArray va;
      VertexBuffer vb(positions, 4 * 2 * sizeof (float));
      
      VertexBufferLayout layout;
      layout.Push<float>(2);
      va.addBuffer(vb, layout);

      IndexBuffer ib(indices, 6);

      Shader shader("res/shaders/basic.shader");
      shader.bind();

      shader.setUniform4f("u_Color", 0.5f, 0.5f, 0.7f, 0.5f);

      va.unbind();
      shader.unbind();
      vb.unbind();
      ib.unbind();

      float festa = 0.0f;
      float inc = 0.01f;
      /* Loop until the user closes the window */
      while (!glfwWindowShouldClose(window)) {
          /* Render here */
          GLcall(glClear(GL_COLOR_BUFFER_BIT));

          shader.bind();

          va.bind();
          shader.setUniform4f("u_Color", 1.0f - festa, festa, 1.0f - festa, 1.0f);

          GLcall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

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