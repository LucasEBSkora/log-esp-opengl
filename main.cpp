
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource {
  std::string vertexSource;
  std::string fragmentSource;
};

static struct ShaderProgramSource ParseShader(const std::string& filePath) {
  std::ifstream stream(filePath);

  std::string line;
  std::stringstream ss[2];

  enum class ShaderType {
    NONE = -1, 
    VERTEX = 0,
    FRAGMENT = 1
  };

  ShaderType type = ShaderType::NONE;
  
  while (getline(stream, line)) {
    if(line.find("#shader") != std::string::npos) {
        std::cout << line << std::endl;
      if (line.find("vertex") != std::string::npos) {
        type = ShaderType::VERTEX;

      } else if ((line.find("fragment") != std::string::npos)) {
        type = ShaderType::FRAGMENT;
      } 

    } else {

        ss[(int)type] << line << '\n';

      }
  }

  return {ss[0].str(), ss[1].str()};

} 

void error_callback(int error, const char* description) {
    std::cout << description << std::endl;
}

static unsigned int compileShader(unsigned int type, const std::string& source) {
  
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);
  
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result); //integer, vector (pointer)

  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = new char[length];
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "failed to compile shader" << std::endl;
    std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;

  } else {
    std::cout << "compiled shader succesfully!" << std::endl;
  }

  return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
  
  unsigned int program = glCreateProgram();
  unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
  
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



    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
      std::cout << "GLEW is not ok" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

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
    unsigned int buffer;
    glGenBuffers(1, &buffer); //cria um buffer novo, com a id armazena na variável buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer); //seleciona o buffer usado
    glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof (float), positions, GL_STATIC_DRAW);  //adiciona os dados no buffer criado

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof (float), (const void*) 0);

    unsigned int ibo;
    glGenBuffers(1, &ibo); //cria um buffer novo, com a id armazena na variável buffer

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //seleciona o buffer usado
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof (unsigned int), indices, GL_STATIC_DRAW);  //adiciona os dados no buffer criado

    ShaderProgramSource source = ParseShader("res/shaders/basic.shader");

    unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}