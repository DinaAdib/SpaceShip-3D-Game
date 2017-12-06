#pragma once
#include "GameHeader.h"

class Buffers
{
	char ObjectName[100];

	std::vector<glm::vec3> Vertices;
	std::vector<glm::vec2> Uvs;
	std::vector<glm::vec3> Normals;

	GLuint VertexBuffer;
	GLuint UVBuffer;
	GLuint NormalBuffer; 
	

public:
	Buffers(char name[]);
	~Buffers(); 

	GLuint getVertexBuffer();
	GLuint getUVBuffer();
	GLuint getNormalBuffer();


	std::vector<glm::vec3> getVerticesVector();
	std::vector<glm::vec2> getUVsVector();
	std::vector<glm::vec3> getNormalsVector();

	bool Create();

};