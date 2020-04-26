#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <string>
#include <unordered_map>


class Shader {
  private:
    struct ShaderProgramSource {
      std::string vertexSource;
      std::string fragmentSource;
    };


    std::string m_filePath;
    unsigned int m_Renderer_ID;
    std::unordered_map<std::string, int> m_uniformLocationCache;
  public:
    Shader(const std::string& filename);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

  private:
    struct ShaderProgramSource ParseShader();
    unsigned int compileShader(unsigned int type, const std::string& source);
    unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
    int getUniformLocation(const std::string& name);
    
};

#endif