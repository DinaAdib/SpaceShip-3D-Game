#include "ObjectModel.h"
#include <iostream>
using namespace std;


ObjectModel::ObjectModel()
{
    printf("\n Default Constructor \n");
}

glm::mat4 ObjectModel::GetModelTranslation()
{
    return this->Translation;
}
glm::mat4 ObjectModel::GetModelScaling()
{
    return this->Scaling;
}
glm::mat4 ObjectModel::GetModelRotation()
{
    return this->Rotation;
}

ObjectModel::ObjectModel(char* texName, Buffers* buffers , int type)
{
    ObjectBuffers = buffers;
    int i = 0;
    while (texName[i] != '\0')
    {
        TextureName[i] = texName[i];
        i++;
    }
    TextureName[i] = '\0';

    TextureIndex = loadBMP_custom(TextureName);
    Type = type;
    ObjectBoundingBox = getBuffers()->getBufferBoundingBox();
    xLength = ObjectModel::ObjectBoundingBox.Xmax - ObjectModel::ObjectBoundingBox.Xmin;
    yLength = ObjectModel::ObjectBoundingBox.Ymax - ObjectModel::ObjectBoundingBox.Ymin;
    zLength = ObjectModel::ObjectBoundingBox.Zmax - ObjectModel::ObjectBoundingBox.Zmin;

    // collision with a sphere
    Radius = xLength > yLength ? xLength : yLength;
    Radius = Radius > zLength ? Radius : zLength;
    Radius = Radius / 2;
    Center.x = 0;
    Center.y = 0;
    Center.z = 0;
}

// Special Constructor for spaceship
ObjectModel::ObjectModel(char* ObjName, char *texName)
{
    ObjectBuffers = new Buffers(ObjName);
    int i = 0;
    while (texName[i] != '\0')
    {
        TextureName[i] = texName[i];
        i++;
    }
    TextureName[i] = '\0';
    TextureIndex = loadBMP_custom(TextureName);
    Type = 0;
}

ObjectModel::~ObjectModel()
{
    //glDeleteBuffers(1, ObjectBuffers->getVertexBuffer);
    //glDeleteBuffers(1, ObjectBuffers->getNormalBuffer);
    //glDeleteBuffers(1, ObjectBuffers->getUVBuffer);
}


bool ObjectModel::Draw(GLuint vertexPosition_modelspaceID, GLuint vertexUVID, GLuint vertexNormal_modelspaceID,bool LightShaderUsed)
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

    if (LightShaderUsed)
    {
        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(vertexNormal_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, ObjectBuffers->getNormalBuffer());
        glVertexAttribPointer(
                    vertexNormal_modelspaceID,    // The attribute we want to configure
                    3,                            // size
                    GL_FLOAT,                     // type
                    GL_FALSE,                     // normalized?
                    0,                            // stride
                    (void*)0                      // array buffer offset
                    );
    }

    glDrawArrays(GL_TRIANGLES, 0, ObjectBuffers->getVerticesVector().size());

    return true;
}


void ObjectModel::LightShader(GLuint ShaderLightID, GLuint MatrixID_Light, GLuint ModelMatrixID_Light, GLuint ViewMatrixID_Light, GLuint LightPosition_ID, GLuint TextureID_Light,Point center_ship, GLuint camera_position)
{
    computeMatricesFromInputs();
    glUseProgram(ShaderLightID);

    glm::mat4 MVP = getProjectionMatrix() * getViewMatrix() * ModelMatrix;

    glUniformMatrix4fv(MatrixID_Light, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID_Light, 1, GL_FALSE, &ModelMatrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID_Light, 1, GL_FALSE, &(getViewMatrix())[0][0]);

    glm::vec3 lightPos = glm::vec3(center_ship.x,center_ship.y,center_ship.z-1);
    glUniform3f(LightPosition_ID, lightPos.x, lightPos.y, lightPos.z);

    glm::vec3 campos = glm::vec3(getCameraPosition());
    glUniform3f(camera_position,campos.x,campos.y,campos.z);

    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_2D, TextureIndex);
    glUniform1i(TextureID_Light, 11);

}

void ObjectModel::TransformationShader(GLuint programID, GLuint MatrixID, GLuint vertexPosition_modelspaceID, GLuint vertexUVID, GLuint TextureID)
{

    glm::mat4 SSMVP = getProjectionMatrix() * getViewMatrix() * ModelMatrix;

    glUseProgram(programID);

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &SSMVP[0][0]);
    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_2D, TextureIndex);
    glUniform1i(TextureID, 11);
}


bool ObjectModel::setModelMatrix(glm::mat4 model)
{
    ModelMatrix = model;
    return true;
}

void ObjectModel::constructModelMatrix( glm::vec3 Translation, glm::vec3 Scaling, glm::vec3 Rotation)
{
    this->Translation = translate(mat4(), Translation);
    this->Scaling = scale(mat4(),Scaling);
    this->Rotation = eulerAngleYXZ(Rotation.y, Rotation.x, Rotation.z);
    this->ModelMatrix = this->Translation*this->Rotation*this->Scaling;
    //ORDER MATTERS : Scale BoundingBox then translate it ...NEVER THE OPPOSITE
    scaleBoundingBox(Scaling.x, Scaling.y, Scaling.z);
    translateBoundingBox(Translation.x, Translation.y, Translation.z);
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

Box& ObjectModel::getBoundingBox()
{
    return ObjectBoundingBox;
}

Buffers* ObjectModel::getBuffers()
{
    return ObjectBuffers;
}

bool ObjectModel::detectCollision(Box b)
{
    bool Xintersection = (ObjectBoundingBox.Xmin <= b.Xmax && ObjectBoundingBox.Xmax >= b.Xmin);// || (ObjectBoundingBox.Xmax <= b.Xmin && ObjectBoundingBox.Xmin >= b.Xmax);
    bool Yintersection = (ObjectBoundingBox.Ymin <= b.Ymax && ObjectBoundingBox.Ymax >= b.Ymin);// || (ObjectBoundingBox.Ymax <= b.Ymin && ObjectBoundingBox.Xmin >= b.Ymax);
    bool Zintersection = (ObjectBoundingBox.Zmin <= b.Zmax && ObjectBoundingBox.Zmax >= b.Zmin);// || (ObjectBoundingBox.Zmax <= b.Zmin && ObjectBoundingBox.Xmin >= b.Zmax);
    return (Xintersection && Yintersection && Zintersection);
}
bool ObjectModel::detectCollision(float r, Point c)
{
    float X = Center.x - c.x;
    float Y= Center.y - c.y;
    float Z= Center.z - c.z;
    float distance = sqrt(X*X + Y*Y + Z*Z);
    return (r + Radius > distance);
}

int ObjectModel::getType()
{
    return Type;
}

void ObjectModel::translateBoundingBox(float x, float y, float z)
{
    //Changes the position of xMin,yMin,Zmin based on the scaled Xlength and yLength
    Center.x = x;
    Center.y = y;
    Center.z = z;
}

void ObjectModel::translateObject(float x, float y, float z)
{
    int xNew = Center.x + x, yNew = Center.y + y, zNew = Center.z + z;
    this->Translation = translate(mat4(), glm::vec3(xNew,yNew,zNew));
    translateBoundingBox(xNew, yNew, zNew);

}

void ObjectModel::scaleBoundingBox(float x, float y, float z)
{
    //Update xLength , yLength , zLength
    struct Box myBox = ObjectBuffers->getBufferBoundingBox();
    xLength = myBox.Xmax - myBox.Xmin;
    yLength = myBox.Ymax - myBox.Ymin;
    zLength = myBox.Zmax - myBox.Zmin;
    xLength *= x;
    yLength *= y;
    zLength *= z;
    Radius = xLength > yLength ? xLength : yLength;
    Radius = Radius > zLength ? Radius : zLength;
    Radius = Radius / 2;
}

void ObjectModel::rotateObject(glm::mat4 newRotation)
{
    this->Rotation = newRotation;
    ModelMatrix = Translation*Rotation*Scaling;
}

Point ObjectModel::getCenter()
{
    return this->Center;
}

float ObjectModel::getRadius()
{
    return this->Radius;
}
