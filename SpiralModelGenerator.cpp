#include "SpiralModelGenerator.hpp"

#include <iostream>
#include <math.h>

SpiralModelGenerator::SpiralModelGenerator(float maxwidth, float maxheight, float xinicial, unsigned short nlinhas) :
  maxWidth{maxwidth},
  maxHeight{maxheight},
  xInicial{xinicial},
  nLinhas{nlinhas},
  currentIndex{0},
  angularIncrement{M_PI/nlinhas}
 {
  if (nLinhas < 3) std::cout << "Cuidado! vai dar merda isso aí" << std::endl;
}

SpiralModelGenerator::~SpiralModelGenerator() {
  
}

const float* SpiralModelGenerator::getVertexBufferData() const {
  return vertexBufferData.data();
}

unsigned int SpiralModelGenerator::getVertexBufferDataCount() const {
  return 2*vertexBufferData.size();
}

const unsigned int* SpiralModelGenerator::getIndexBufferData() const {
  return indexBufferData.data();
}

unsigned SpiralModelGenerator::getIndexBufferDataCount() const {
  return indexBufferData.size();
}

void SpiralModelGenerator::fillBuffers() {
  currentIndex = 0;
  createLines();
  createSegments();
}

void SpiralModelGenerator::addVertexToBuffer(struct SpiralModelGenerator::Vertex vertex) {
  vertexBufferData.push_back(vertex.x);
  vertexBufferData.push_back(vertex.y);
  
}

void SpiralModelGenerator::createLines() {
  
  
  for (float theta = 0.0f; theta < M_PI; theta += angularIncrement) {
    indexBufferData.push_back(currentIndex++);
    struct Line next = nextLine(theta);
    
    addVertexToBuffer(next.start);
    addVertexToBuffer(next.end);

    indexBufferData.push_back(currentIndex++);
  }
  
}

struct SpiralModelGenerator::Line SpiralModelGenerator::nextLine(float lineAngle) const {
  float length = fmax(maxWidth, maxHeight);

  return {
    {-(length)*cos(lineAngle), -(length)*sin(lineAngle)}, 
    {(length)*cos(lineAngle), (length)*sin(lineAngle)}
  };
}

void SpiralModelGenerator::createSegments() {
  struct Vertex lastVertex = {0.0f, 0.0f};
  struct Vertex currentVertex = {xInicial, 0.0f};


  addVertexToBuffer(currentVertex);

  double theta = 0;

  while (fabs(lastVertex.x) < maxWidth && fabs(lastVertex.y) < maxHeight) {
    
    

    lastVertex = currentVertex;

    indexBufferData.push_back(currentIndex++);


    // currentVertex = nextSpiralVertex(theta, lastVertex);

    // const float *cu = _nextSpiralVertex(theta, angularIncrement, lastVertex.x, lastVertex.y);
    currentVertex = nextSpiralVertex(theta, lastVertex);
    addVertexToBuffer(currentVertex);
   
    indexBufferData.push_back(currentIndex);

    theta += angularIncrement;

    std::cout << '(' << lastVertex.x << ", " << lastVertex.y << ")\n";
  }

}

struct SpiralModelGenerator::Vertex SpiralModelGenerator::nextSpiralVertex(double lineAngle, SpiralModelGenerator::Vertex lastVertex) const {
  
  if(nLinhas < 3) {
    return {lastVertex.x, maxHeight};
  }


  double MA = lineAngle + M_PI_2;
  double NLA = lineAngle + angularIncrement;
  
  lastVertex.x =  (lastVertex.y - tan(MA)*lastVertex.x)/(tan(NLA) - tan(MA));
  lastVertex.y = tan(NLA)*lastVertex.x;

  return lastVertex;

}
