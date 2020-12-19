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

    // �����ڴ� shader�� �а� �����մϴ�.
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // shader�� Ȱ��ȭ�ϰ� ����մϴ�.
    void use();
    // Uniform ��ƿ��Ƽ �Լ���
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setMat4(const std::string& name, glm::mat4 value) const;
};

