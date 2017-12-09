#pragma once
#include "GameHeader.h"
#include "Buffers.h"

class ObjectModel
{
 
	char TextureName[100];
	//	char ObjectName[100];
	//std::vector<glm::vec3> Vertices;
	//std::vector<glm::vec2> Uvs;
	//std::vector<glm::vec3> Normals;

	GLuint TextureIndex; 
	/*GLuint VertexBuffer;
	GLuint UVBuffer;*/

	Buffers* ObjectBuffers; 
	glm::mat4 ModelMatrix;
	int Type; 

public:
	ObjectModel( char* texName, Buffers* buffers , int type); // construct object 
	ObjectModel(char* ObjName, char *texName); // special constructor for spaceship child class 
	ObjectModel();
	~ObjectModel();
	 Box& getBoundingBox(); 
	virtual Buffers* getBuffers();
	int getType(); 
	virtual void translateObject(float x, float y, float z);
	virtual void scaleObject(float x, float y, float z);

	/*GLuint getVertexBuffer(); 
	GLuint getUVBuffer(); 
	GLuint getTexture(); 
	std::vector<glm::vec3> getVerticesVector();
	std::vector<glm::vec2> getUVsVector();
	std::vector<glm::vec3> getNormalsVector();
*/
	virtual bool Draw(GLuint programID, GLuint MatrixID, GLuint vertexPosition_modelspaceID, GLuint vertexUVID , GLuint TextureID);
	virtual bool setModelMatrix(glm::mat4 model);
	virtual bool setTexture(char texName[]);
	virtual bool detectCollision(Box bb);


};

