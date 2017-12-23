#include "Spaceship.h"

Spaceship::Spaceship():ObjectModel("spaceship_clean.obj","Spaceship.bmp")
{
    printf( "\n Spaceship constructor");
    ObjectModel::getBuffers()->Create();
    ObjectBoundingBox = ObjectModel::getBuffers()->getBufferBoundingBox();

    // automatically calculatd in scaleBB
    /*xLength = ObjectModel::ObjectBoundingBox.Xmax - ObjectModel::ObjectBoundingBox.Xmin;
    yLength = ObjectModel::ObjectBoundingBox.Ymax - ObjectModel::ObjectBoundingBox.Ymin;
    zLength = ObjectModel::ObjectBoundingBox.Zmax - ObjectModel::ObjectBoundingBox.Zmin;*/

    ObjectModel::scaleBoundingBox(0.25f, 0.25f, 0.25f);

    Center.x = 0;
    Center.y = 0;
    Center.z = 0;

// Fuel Calculation
    Fuel = 10000;
    Velocity = 0;

    /// 2nd Shader computation
    programID2 = LoadShaders("TransformVertexShader.vertexshader", "CollisionShading.fragmentshader");
    // Get a handle for our "MVP" uniform
    MatrixID2 = glGetUniformLocation(programID2, "MVP");

    // Get a handle for our buffers
    vertexPosition_modelspaceID2 = glGetAttribLocation(programID2, "vertexPosition_modelspace");
    TextureID2 = glGetUniformLocation(programID2, "myTextureSampler");
    TextureIndex2 = loadBMP_custom("hit.bmp");
    timeID = glGetUniformLocation(programID2, "time");

}
Spaceship::~Spaceship()
{}

void Spaceship::DrawSpaceshipafterCollision(GLuint MatrixID, GLuint vertexPosition_modelspaceID, GLuint vertexUVID)
{
    mytime += 0.1;
    glUseProgram(programID2);

    glUniform1f(timeID, mytime);
    glEnableVertexAttribArray(vertexPosition_modelspaceID);
    glBindBuffer(GL_ARRAY_BUFFER, ObjectModel::ObjectBuffers->getVertexBuffer());
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
    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_2D, TextureIndex2);
    glUniform1i(TextureID2, 11);
    glDrawArrays(GL_TRIANGLES, 0, ObjectBuffers->getVerticesVector().size());

}


