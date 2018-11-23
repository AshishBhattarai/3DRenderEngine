#include "render_engine.hpp"

SkyboxRenderer::SkyboxRenderer(SkyboxShader& shader) :
	shader(&shader),
  skybox_mesh(nullptr)
{
  float skyboxVertices[]  = {
    1.0f, 1.0f, 1.0f,   1.0f,-1.0f, 1.0f,  -1.0f,-1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,   // (front)
    1.0f, 1.0f, 1.0f,   1.0f, 1.0f,-1.0f,   1.0f,-1.0f,-1.0f,   1.0f,-1.0f, 1.0f,   // (right)
    1.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,  -1.0f, 1.0f,-1.0f,   1.0f, 1.0f,-1.0f,   // (top)
   -1.0f, 1.0f, 1.0f,  -1.0f,-1.0f, 1.0f,  -1.0f,-1.0f,-1.0f,  -1.0f, 1.0f,-1.0f,   // (left)
   -1.0f,-1.0f,-1.0f,  -1.0f,-1.0f, 1.0f,   1.0f,-1.0f, 1.0f,   1.0f,-1.0f,-1.0f,   // (bottom)
    1.0f,-1.0f,-1.0f,   1.0f, 1.0f,-1.0f,  -1.0f, 1.0f,-1.0f,  -1.0f,-1.0f,-1.0f    // (back)
  };

  unsigned int indices[] = {
     0, 1, 2,   2, 3, 0,    //  (front)
     4, 5, 6,   6, 7, 4,    //  (right)
     8, 9,10,  10,11, 8,    //  (top)
    12,13,14,  14,15,12,    //  (left)
    16,17,18,  18,19,16,    //  (bottom)
    20,21,22,  22,23,20     //  (back)
  };

  skybox_mesh = std::make_unique<ShapeMesh>(skyboxVertices, 3, 24, false, false);
  skybox_mesh->setIndices(indices, 36);
}

SkyboxRenderer::~SkyboxRenderer() {}

void SkyboxRenderer::render(Skybox* skybox) {
  glDepthFunc(GL_LEQUAL);
  glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getCubemapID());
	shader->start();
  shader->loadEnableFog(skybox->getFogEnable());
	glBindVertexArray(skybox_mesh->getVAO());
  glDrawElements(GL_TRIANGLES, skybox_mesh->getCount(), GL_UNSIGNED_INT, 0);
  glDepthFunc(GL_LESS);
}