#include "Camera.hpp"

Camera::Camera() : proj{glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f)},
  cameraPos{0.0f, 0.0f, 0.0f},
  modelPos{0.0f, 0.0f, 0.0f} {
    calculateResult();
}

Camera::~Camera() {

}

void Camera::setProj(float x1, float x2, float y1, float y2, float z1, float z2) {
  proj = glm::ortho(x1,x2, y1, y2, z1, z2);
  calculateCamera();
}

void Camera::setProjAspectRatio(float aspect, float scale) {
  setProj(-scale*aspect, scale*aspect, -scale, scale, -1.0f, 1.0f);
}

void Camera::setCameraPos(float x, float y, float z) {
  cameraPos = glm::vec3(x, y, z);
  calculateCamera();
}

void Camera::setModelPos(float x, float y, float z) {
  modelPos = glm::vec3(x, y, z);
  calculateResult();
}

void Camera::rotateCamera(float angle, float vx, float vy, float vz) {
  
  if (angle != 0.0f && (vx != 0.0f || vy != 0.0f || vz != 0.0f)) {
  
    rotations.push_back({angle, {vx, vy, vz}});
    cameraRotated = glm::rotate(cameraRotated, angle, {vx, vy, vz});
  }

  calculateResult();
}

void Camera::resetRotation(float angle, float vx, float vy, float vz) {
  rotations.clear();
  
  rotateCamera(angle, vx, vy, vz);
}



const glm::mat4& Camera::getResult() {
  return result;
}

void Camera::calculateCamera() {
  camera = glm::translate(proj, cameraPos);
  applyRotations();
}

void Camera::applyRotations() {
  cameraRotated = camera;
  for (struct Camera::Rotation r : rotations) {
    cameraRotated = glm::rotate(cameraRotated, r.angle, r.normal);
  }
  calculateResult();
}

void Camera::calculateResult() {
  model = glm::translate(cameraRotated, modelPos);
  result = model;
}
