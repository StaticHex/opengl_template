#ifndef PROCEDURE_GEOMETRY_H
#define PROCEDURE_GEOMETRY_H

#include <vector>
#include <glm/glm.hpp>

class LineMesh;
static int faceCount = 0;
static float cubeSize = 10.0;
void create_floor(std::vector<glm::vec4>& floor_vertices, std::vector<glm::uvec3>& floor_faces);
void create_cube(glm::vec4 position, std::vector<glm::vec4> &obj_vertices, std::vector<glm::uvec3> &obj_faces, 
				std::vector<glm::vec4> &vtx_normals);

#endif
