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


public:
	ObjectModel( char* texName, Buffers* buffers); // construct object 
	~ObjectModel();
	
	//bool Create(); 

	/*GLuint getVertexBuffer(); 
	GLuint getUVBuffer(); 
	GLuint getTexture(); 
	std::vector<glm::vec3> getVerticesVector();
	std::vector<glm::vec2> getUVsVector();
	std::vector<glm::vec3> getNormalsVector();
*/
	bool Draw(GLuint programID, GLuint MatrixID, GLuint vertexPosition_modelspaceID, GLuint vertexUVID , GLuint TextureID);
	bool setModelMatrix(glm::mat4 model); 
	bool setTexture(char texName[]);
	//bool SetUVBuffers( GLuint UV); 
	//bool SetVertexBuffers(GLuint vertex);


};

