#include "procedure_geometry.h"
#include "config.h"

void create_floor(std::vector<glm::vec4>& floor_vertices, std::vector<glm::uvec3>& floor_faces)
{
	floor_vertices.push_back(glm::vec4(kFloorXMin, kFloorY, kFloorZMax, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMax, kFloorY, kFloorZMax, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMax, kFloorY, kFloorZMin, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMin, kFloorY, kFloorZMin, 1.0f));
	floor_faces.push_back(glm::uvec3(0, 1, 2));
	floor_faces.push_back(glm::uvec3(2, 3, 0));
}

void create_cube(glm::vec4 position, std::vector<glm::vec4> &obj_vertices, std::vector<glm::uvec3> &obj_faces,
				 std::vector<glm::vec4> &vtx_normals) {
	float offset = cubeSize / 2.0f;
	// Generate Z pointing faces
	for (int i = -offset; i < cubeSize; i += cubeSize) {
		for (int j = -offset; j < cubeSize; j += cubeSize) {
			for (int k = -offset; k < cubeSize; k += cubeSize) {
				obj_vertices.push_back(glm::vec4(position.x+k, position.y+j, position.z+i, 1.0f));
				if(i < 0)
					vtx_normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
				else
					vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
			}
		}
		if (i < 0) {
			obj_faces.push_back(glm::uvec3(faceCount, faceCount + 2, faceCount + 1));
			obj_faces.push_back(glm::uvec3(faceCount + 3, faceCount + 1, faceCount + 2));
		} else {
			obj_faces.push_back(glm::uvec3(faceCount, faceCount + 1, faceCount + 2));
			obj_faces.push_back(glm::uvec3(faceCount + 3, faceCount + 2, faceCount + 1));
		}
		faceCount += 4;
	}

	// Generate X pointing faces
	for (int i = -offset; i < cubeSize; i += cubeSize) {
		for (int j = -offset; j < cubeSize; j += cubeSize) {
			for (int k = -offset; k < cubeSize; k += cubeSize) {
				obj_vertices.push_back(glm::vec4(position.x + i, position.y + j, position.z + k, 1.0f));
				if(i < 0)
					vtx_normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f));
				else
					vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}
		if (i < 0) {
			obj_faces.push_back(glm::uvec3(faceCount, faceCount + 1, faceCount + 2));
			obj_faces.push_back(glm::uvec3(faceCount + 3, faceCount + 2, faceCount + 1));
		} else {
			obj_faces.push_back(glm::uvec3(faceCount+2, faceCount + 1, faceCount));
			obj_faces.push_back(glm::uvec3(faceCount + 1, faceCount + 2, faceCount + 3));
		}
		faceCount += 4;
	}

	// Generate Y pointing faces
	for (int i = -offset; i < cubeSize; i += cubeSize) {
		for (int j = -offset; j < cubeSize; j += cubeSize) {
			for (int k = -offset; k < cubeSize; k += cubeSize) {
				obj_vertices.push_back(glm::vec4(position.x + k, position.y + i, position.z + j, 1.0f));
				if(i < 0)
					vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
				else
					vtx_normals.push_back(glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));
			}
		}
		if (i < 0) {
			obj_faces.push_back(glm::uvec3(faceCount, faceCount + 1, faceCount + 2));
			obj_faces.push_back(glm::uvec3(faceCount + 3, faceCount + 2, faceCount + 1));
		}
		else {
			obj_faces.push_back(glm::uvec3(faceCount + 2, faceCount + 1, faceCount));
			obj_faces.push_back(glm::uvec3(faceCount + 1, faceCount + 2, faceCount + 3));
		}
		faceCount += 4;
	}
}