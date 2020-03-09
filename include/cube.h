#ifndef CUBE_H_
#define CUBE_H_

#include "mesh.h"
#include "shader.h"

class Cube : public Mesh {
public:
  Cube(GLuint vertex_index);
  ~Cube();
};

#endif  // CUBE_H_
