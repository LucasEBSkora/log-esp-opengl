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

  struct Vertex lastVertex = {xInicial, 0.0f};
  struct Vertex currentVertex = {0.f, 0.0f};
  
  float theta = 0;

  while (fabs(currentVertex.x) < maxWidth && abs(currentVertex.y) < maxHeight) {
    
    addVertexToBuffer(lastVertex);
    indexBufferData.push_back(currentIndex++);

    currentVertex = nextSpiralVertex(theta, lastVertex);
    addVertexToBuffer(currentVertex);
    indexBufferData.push_back(currentIndex);

    
  }
}


struct SpiralModelGenerator::Vertex SpiralModelGenerator::nextSpiralVertex(float lineAngle, SpiralModelGenerator::Vertex lastVertex) const {
  return {0.0f, 0.0f};
}
