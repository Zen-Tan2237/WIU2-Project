#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"
#include "LoadOBJ.h"
#include <string>


/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string& meshName, glm::vec3 color, float length = 1.f);
	static Mesh* GenerateRing(const std::string& meshName, glm::vec3 color, unsigned numSlice, float outerR, float innerR);
	static Mesh* GenerateCube(const std::string& meshName, glm::vec3 color, float length = 1.f);
	static Mesh* GenerateSphere(const std::string& meshName, glm::vec3 color, float radius, int numSlice, int numStack);
	static Mesh* GenerateHemisphere(const std::string& meshName, glm::vec3 color, unsigned numStack, unsigned numSlice, float radius);
	static Mesh* GenerateCylinder(const std::string& meshName, glm::vec3 color, unsigned numSlice, float radius, float height);
	static Mesh* GenerateTorus(const std::string& meshName,glm::vec3 color, float innerR, float outerR, int numSlice, int numStack);
	static Mesh* GenerateOBJ(const std::string& meshName, const std::string& file_path);
	static Mesh* GenerateOBJMTL(const std::string& meshName, const std::string& file_path, const std::string& mtl_path);
	static Mesh* GenerateText(const std::string& meshName, unsigned numRow, unsigned numCol);
};

#endif