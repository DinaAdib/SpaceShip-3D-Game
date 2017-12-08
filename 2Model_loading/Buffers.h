#pragma once
#include "GameHeader.h"
struct Box {
	float Xmin=1000; 
	float Xmax = -1000;
	float Ymin = 1000;
	float Ymax = -1000;
	float Zmin = 1000;
	float Zmax = -1000;
};
class Buffers
{
	char ObjectName[100];

	std::vector<glm::vec3> Vertices;
	std::vector<glm::vec2> Uvs;
	std::vector<glm::vec3> Normals;

	GLuint VertexBuffer;
	GLuint UVBuffer;
	GLuint NormalBuffer; 
	
	struct Box BoundingBox; 

public:
	Buffers(char name[]);
	~Buffers(); 

	GLuint getVertexBuffer();
	GLuint getUVBuffer();
	GLuint getNormalBuffer();

	Box& getBoundingBox();
	std::vector<glm::vec3> getVerticesVector();
	std::vector<glm::vec2> getUVsVector();
	std::vector<glm::vec3> getNormalsVector();
	void translateBoundingBox(float x, float y, float z);
	void scaleBoundingBox(float x, float y, float z);
	bool Create();

};