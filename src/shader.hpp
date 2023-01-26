#pragma once
#ifndef SHADER_HPP
#define SHADER_HPP

#include "third_party/glad/glad.h"
#include "third_party/glm/glm.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string_view>

class Shader
{
private:
    GLuint _id = 0;

    void check_errors(GLuint id, std::string type) const;

    void del() 
    { 
        if(_id)
            glDeleteProgram(_id);
    }
public:
    static std::filesystem::path directory;
    
    //loads shaders(vertex and fragment) from: directory + path + (.vs | .fs)
    inline Shader(const std::filesystem::path& path, bool standart_dir = true);

    Shader() = delete;
    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;
    Shader(Shader&& other) = delete;
    Shader& operator=(Shader&& other) = delete;
    
    ~Shader() 
    {
        del();
    }

    void use() const
    {
        glUseProgram(_id);
    }


    //uniform setters
    void setBool(std::string_view name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(_id, name.begin()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(std::string_view name, int value) const
    { 
        glUniform1i(glGetUniformLocation(_id, name.begin()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(std::string_view name, float value) const
    { 
        glUniform1f(glGetUniformLocation(_id, name.begin()), value); 
    }
    // ------------------------------------------------------------------------
    void setVec2(std::string_view name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(_id, name.begin()), 1, &value[0]); 
    }
    void setVec2(std::string_view name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(_id, name.begin()), x, y); 
    }
    // ------------------------------------------------------------------------
    void setVec3(std::string_view name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(_id, name.begin()), 1, &value[0]); 
    }
    void setVec3(std::string_view name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(_id, name.begin()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void setVec4(std::string_view name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(_id, name.begin()), 1, &value[0]); 
    }
    void setVec4(std::string_view name, float x, float y, float z, float w) const
    { 
        glUniform4f(glGetUniformLocation(_id, name.begin()), x, y, z, w); 
    }
    // ------------------------------------------------------------------------
    void setMat2(std::string_view name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(_id, name.begin()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(std::string_view name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(_id, name.begin()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(std::string_view name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(_id, name.begin()), 1, GL_FALSE, &mat[0][0]);
    }
};

std::filesystem::path Shader::directory = "../resources/shaders/";

inline Shader::Shader(const std::filesystem::path& path, bool standart_dir)
{
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    auto vertexPath = standart_dir ? directory / path : path;
    auto fragmentPath = vertexPath;

    vShaderFile.open(vertexPath.concat(".vs").string());
    fShaderFile.open(fragmentPath.concat(".fs").string());

    std::stringstream vShaderStream, fShaderStream;
    // read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();		
    // close file handlers
    vShaderFile.close();
    fShaderFile.close();
    // convert stream into string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();	

    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    check_errors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    check_errors(fragment, "FRAGMENT");

    _id = glCreateProgram();

    glAttachShader(_id, vertex);
    glAttachShader(_id, fragment);
    glLinkProgram(_id);
    check_errors(_id, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::check_errors(GLuint id, std::string type) const
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(id, 1024, NULL, infoLog);
            throw std::runtime_error("ERROR::SHADER_COMPILATION_ERROR of type: " + type + "\n" + infoLog);
        }
    }
    else
    {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(id, 1024, NULL, infoLog);
            throw std::runtime_error("ERROR::PROGRAM_LINKING_ERROR of type: " + type + "\n" + infoLog);
        }
    }
}

#endif
