#include"Buffers.h"


Buffers::Buffers(char *name)
{
    int i = 0;
    while (name[i] != '\0')
    {
        ObjectName[i] = name[i];
        i++;
    }
    ObjectName[i] = '\0';
    BufferBoundingBox.Xmin = 1000;
    BufferBoundingBox.Ymin = 1000;
    BufferBoundingBox.Zmin = 1000;
    BufferBoundingBox.Xmax = -1000;
    BufferBoundingBox.Ymax = -1000;
    BufferBoundingBox.Zmax = -1000;
}

Buffers::~Buffers()
{}

GLuint Buffers::getVertexBuffer()
{
    return VertexBuffer;
}
GLuint Buffers::getUVBuffer()
{
    return UVBuffer;
}

GLuint Buffers::getNormalBuffer()
{
    return NormalBuffer;
}

std::vector<glm::vec3>   Buffers::getVerticesVector()
{
    return Vertices;
}

std::vector<glm::vec2>  Buffers::getUVsVector()
{
    return Uvs;
}

std::vector<glm::vec3>  Buffers::getNormalsVector(){
    return Normals;
}

bool Buffers::Create()
{
    bool Loading = loadOBJ(ObjectName, Vertices, Uvs, Normals, BufferBoundingBox);
    // Generate Vertex Buffer
    glGenBuffers(1, &VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec3), &Vertices[0], GL_STATIC_DRAW);
    // Generate UV Buffer
    glGenBuffers(1, &UVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
    glBufferData(GL_ARRAY_BUFFER, Uvs.size() * sizeof(glm::vec2), &Uvs[0], GL_STATIC_DRAW);
    // Generate Normals Buffer
    glGenBuffers(1, &NormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(glm::vec3), &Normals[0], GL_STATIC_DRAW);
    return Loading;
}

Box& Buffers::getBufferBoundingBox()
{
    return BufferBoundingBox;
}
