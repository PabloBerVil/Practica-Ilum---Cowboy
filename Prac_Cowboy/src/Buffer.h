#pragma once
#include "VertexVec.h"
#include <vector>
#include "Shader.h"



class Buffer
{

public:
    Buffer(std::vector<Vertex> _vertex, std::vector<IndexVex> _index, Shader* _shader = nullptr);

    ~Buffer();

    void Draw(const Shader* _pShader) const;

    void setPosition(glm::vec3 _pos);
    void setRotation(float _rot);

    glm::vec3 getPosition();
    float getRotation();

private:
    GLuint bufferVertex;
    GLuint bufferVertexId;

    int indexsize;
    float rot;
    glm::vec3 pos;
};