#pragma once
#include "framework.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // program ID
    unsigned int ID;

    // 생성자는 shader를 읽고 생성합니다.
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // shader를 활성화하고 사용합니다.
    void use();
    // Uniform 유틸리티 함수들
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setMat4(const std::string& name, glm::mat4 value) const;
};

