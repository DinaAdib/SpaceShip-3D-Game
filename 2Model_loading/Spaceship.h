#pragma once
#include "ObjectModel.h"

 class Spaceship : public ObjectModel
{
    float Fuel;
    float Velocity;
    float Acceleration;
    float const MaxVelocity = 25;
    float const MinVelocity = 0;


    // 2nd shader

    GLuint programID2;
    GLuint MatrixID2;
    GLuint vertexPosition_modelspaceID2;
    GLuint vertexUVID2;
    GLuint TextureID2;
    GLuint TextureIndex2;
    GLuint timeID;

    float mytime = 0.1f;




 public:
    Spaceship();
    ~Spaceship();
    void DrawSpaceshipafterCollision(GLuint MatrixID, GLuint vertexPosition_modelspaceID, GLuint vertexUVID);
};
