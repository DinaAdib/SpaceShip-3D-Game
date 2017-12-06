#include "ObjectModel.h"

//
////	// Shall they be added in GLobal Header file? or passed to the function?
//GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
//
//// Get a handle for our "MVP" uniform
//GLuint MatrixID = glGetUniformLocation(programID, "MVP");
//GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
//GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");
//GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

ObjectModel::ObjectModel(char* texName, Buffers* buffers)
{
	//Stupid Solution
	/*int i=0; 
	while (name[i] != '\0')
	{
		ObjectName[i] = name[i]; 
		i++;
	}
	ObjectName[i] = '\0';*/
	ObjectBuffers = buffers;
	int i = 0;
	while (texName[i] != '\0')
	{
		TextureName[i] = texName[i];
		i++;
	}
	TextureName[i] = '\0';

	TextureIndex = loadBMP_custom(TextureName);
}


ObjectModel::~ObjectModel()
{
//	glDeleteBuffers(1, ObjectBuffers->getVertexBuffer);
//	glDeleteBuffers(1, ObjectBuffers->getNormalBuffer);
//	glDeleteBuffers(1, ObjectBuffers->getUVBuffer);
}

//bool ObjectModel::Create()
//{
//	TextureIndex = loadBMP_custom(TextureName);
//	
//	
//	return true; 
//}
/*
GLuint ObjectModel::getVertexBuffer()
{
	return VertexBuffer;
}
GLuint ObjectModel::getUVBuffer()
{
	return UVBuffer; 
}
GLuint ObjectModel::getTexture()
{
	return TextureIndex;
}
std::vector<glm::vec3> ObjectModel::getVerticesVector()
{
	return Vertices; 
}
std::vector<glm::vec2> ObjectModel::getUVsVector()
{
	return Uvs; 
}
std::vector<glm::vec3> ObjectModel::getNormalsVector()
{
	return Normals; 
}
*/
bool ObjectModel::Draw(GLuint programID, GLuint MatrixID, GLuint vertexPosition_modelspaceID, GLuint vertexUVID, GLuint TextureID)
{

	glEnableVertexAttribArray(vertexPosition_modelspaceID);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectBuffers->getVertexBuffer());
	glVertexAttribPointer
		(
			vertexPosition_modelspaceID,  // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
			);
	glEnableVertexAttribArray(vertexUVID);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectBuffers->getUVBuffer());
	glVertexAttribPointer
		(
			vertexUVID,                   // The attribute we want to configure
			2,                            // size : U+V => 2
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
			);
	computeMatricesFromInputs();
	
	glm::mat4 SSMVP = getProjectionMatrix() * getViewMatrix() * ModelMatrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &SSMVP[0][0]);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, TextureIndex);
	glUniform1i(TextureID, 10);
	glDrawArrays(GL_TRIANGLES, 0, ObjectBuffers->getVerticesVector().size());

	return true; 
}

bool ObjectModel::setModelMatrix(glm::mat4 model)
{ 
	ModelMatrix = model; 
	return true; 
}

bool ObjectModel::setTexture(char texName[])
{

	int i = 0;
	while (texName[i] != '\0')
	{
		TextureName[i] = texName[i];
		i++;
	}
	TextureName[i] = '\0';
	TextureIndex = loadBMP_custom(TextureName);

	return true; 
}
