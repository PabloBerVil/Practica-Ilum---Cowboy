#pragma once
#include <cstdint>
#include <sstream>
#include <GL/glew.h>
#include <glm.hpp>

class Shader
{

public:
    Shader(GLenum shaderType1, const std::string& filetext1, GLenum shaderType2, const std::string& filetext2);
    ~Shader();

    uint32_t getId() const;
    const char* getError() const;
    void use() const;
    void setupAttribs() const;
    int getLocation(const char* name) const;

    void setInt(int loc, int val);
    void setFloat(int loc, float val);
    void setVec2(int loc, const glm::vec2& vec);
    void setVec3(int loc, const glm::vec3& vec);
    void setVec4(int loc, const glm::vec4& vec);
    void setMatrix(int loc, const glm::mat4& matrix);


private:
    char* m_error;
    GLuint program;


public:
    GLint m_vposLoc;
    GLint m_vcolorLoc;
    GLint m_matrixLoc;
    GLint m_vTextureLoc;
    GLint m_vNormalLoc;

};