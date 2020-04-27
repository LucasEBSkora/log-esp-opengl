#ifndef __SPIRAL_MODEL_GENERATOR_HPP__
#define __SPIRAL_MODEL_GENERATOR_HPP__

#include <vector>

class SpiralModelGenerator {
  private:
    struct Vertex {
      float x;
      float y;
    };

    struct Line {
      struct Vertex start;
      struct Vertex end;
    };

    std::vector<float> vertexBufferData;
    std::vector<unsigned int> indexBufferData;

    float maxWidth;
    float maxHeight;

    float xInicial;
    unsigned short nLinhas;

    unsigned int currentIndex;

    float angularIncrement; 

  public:
    SpiralModelGenerator(float maxwidth, float maxheight, float xinicial, unsigned short nlinhas);
    ~SpiralModelGenerator();

    const float* getVertexBufferData() const;
    unsigned int getVertexBufferDataCount() const;
    
    const unsigned int* getIndexBufferData() const;
    unsigned getIndexBufferDataCount() const;

    void fillBuffers();
  
  private:

    void addVertexToBuffer(struct Vertex vertex);
    
    void createLines();
    struct Line nextLine(float lineAngle) const; 

    void createSegments();
    struct Vertex nextSpiralVertex(float lineAngle, Vertex lastVertex) const;

};

#endif