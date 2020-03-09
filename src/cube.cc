#include "cube.h"

#include <vector>

Cube::Cube(GLuint vertex_index)
    : Mesh::Mesh() {
  std::vector<GLfloat> verticies({
    -0.5f, -0.5f, 0.5f, 
     0.5f, -0.5f, 0.5f, 
     0.5f,  0.5f, 0.5f, 
    -0.5f,  0.5f, 0.5f, 
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f
  });
  CreateVertexBuffer(vertex_index, verticies);

  std::vector<GLuint> indicies({
    // top
    3, 2, 6,
    3, 6, 7,

    // bottom
    4, 5, 1,
    4, 1, 0,
    
    // front
    0, 1, 2, 
    0, 2, 3,

    // back
    5, 4, 7,
    5, 7, 6,

    // left
    4, 0, 3,
    4, 3, 7,

    // right
    1, 5, 6,
    1, 6, 2
  });
  CreateIndexBuffer(indicies);
}

Cube::~Cube() {

}
