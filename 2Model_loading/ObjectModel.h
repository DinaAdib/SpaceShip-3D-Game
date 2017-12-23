#pragma once
#include "GameHeader.h"
#include "Buffers.h"

struct Point
{
    float x, y, z;
};
class ObjectModel
{
protected:
    char TextureName[100];
    GLuint TextureIndex;
    int Type;
    glm::mat4 Translation;
    glm::mat4 Scaling;
    glm::mat4 Rotation;

    glm::mat4 ModelMatrix;
    Buffers* ObjectBuffers;
    Box ObjectBoundingBox;
    float xLength = 0, yLength = 0, zLength = 0;
    // Sphere Collision
    float Radius;
    Point Center;

public:
    ObjectModel( char* texName, Buffers* buffers , int type); // construct object
    ObjectModel(char* ObjName, char *texName); // special constructor for spaceship child class
    ObjectModel();
    ~ObjectModel();
     Box& getBoundingBox();
    virtual Buffers* getBuffers();
    int getType();


    void translateObject(float x, float y, float z);
    virtual void translateBoundingBox(float x, float y, float z);
    virtual void scaleBoundingBox(float x, float y, float z);

    glm::mat4 GetModelTranslation();
    glm::mat4 GetModelRotation();
    glm::mat4 GetModelScaling();


    virtual bool Draw(GLuint vertexPosition_modelspaceID, GLuint vertexUVID, GLuint vertexNormal_modelspaceID, bool LightShaderUsed);
    virtual void LightShader(GLuint ShaderLightID, GLuint MatrixID_Light, GLuint ModelMatrixID_Light, GLuint ViewMatrixID_Light, GLuint LightPosition_ID, GLuint TextureID_Light,Point center_ship,GLuint camera_position);
    virtual void TransformationShader(GLuint programID, GLuint MatrixID, GLuint vertexPosition_modelspaceID, GLuint vertexUVID , GLuint TextureID);
   virtual bool setModelMatrix(glm::mat4 model);
    virtual void constructModelMatrix(glm::vec3 Translation, glm::vec3 Scaling, glm::vec3 Rotation);
    virtual bool setTexture(char texName[]);
    virtual bool detectCollision(Box bb);
    bool detectCollision(float r, Point c);
    void rotateObject(glm::mat4 Rotation);
    Point getCenter();
    float getRadius();
};
