#pragma once
#include "GameHeader.h"
struct Box
{
        float Xmin;
        float Xmax;
        float Ymin;
        float Ymax;
        float Zmin;
        float Zmax;
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

    //float radius;
    struct Box BufferBoundingBox;

public:
    Buffers(char * name);
    ~Buffers();

    GLuint getVertexBuffer();
    GLuint getUVBuffer();
    GLuint getNormalBuffer();

    Box& getBufferBoundingBox();
    std::vector<glm::vec3> getVerticesVector();
    std::vector<glm::vec2> getUVsVector();
    std::vector<glm::vec3> getNormalsVector();
    //void translateBoundingBox(float x, float y, float z);
    //void scaleBoundingBox(float x, float y, float z);
    bool Create();


};
