#pragma once
#ifndef MESH_HPP
#define MESH_HPP

#include "third_party/glad/glad.h"
#include "third_party/glm/glm.hpp"

#include "vertex.hpp"

#include <vector>

class Mesh
{
private:
    GLuint _vao, _vbo, _ebo;
    GLuint _indices_n = 0;
public:
    glm::mat4 _trans = glm::mat4(1.f);

    Mesh() = delete;

    inline Mesh(std::vector<Vertex> vertices);
    inline Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
    
    Mesh(const Mesh& other) = delete;
    const Mesh& operator=(const Mesh& other) = delete;

    Mesh(Mesh&& other)
    {
        _vao = other._vao;
        _vbo = other._vbo;
        _ebo = other._ebo;
        _indices_n = other._indices_n;

        other._vao = other._vbo = other._ebo = 0;
    }
    const Mesh& operator=(Mesh&& other)
    {
        _vao = other._vao;
        _vbo = other._vbo;
        _ebo = other._ebo;
        _indices_n = other._indices_n;

        other._vao = other._vbo = other._ebo = 0;

        return *this;
    }

    ~Mesh() 
    {
        if(_vao)
            glDeleteVertexArrays(1, &_vao);
        if(_vbo)
            glDeleteBuffers(1, &_vbo);
        if(_ebo)
            glDeleteBuffers(1, &_ebo);
    }

    void draw() const
    {
        glBindVertexArray(_vao);
        if (_indices_n)
            glDrawElements(GL_TRIANGLES, _indices_n, GL_UNSIGNED_INT, 0);
        else
            glDrawArrays(GL_TRIANGLES, 0, _indices_n);
    }
};

inline Mesh::Mesh(std::vector<Vertex> vertices)
{
    _indices_n = vertices.size() / 3;

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _nor));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _tex));
    glEnableVertexAttribArray(2);
}

inline Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
{
    _indices_n = indices.size();
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    if (_indices_n)
    {
        glGenBuffers(1, &_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _nor));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _tex));
    glEnableVertexAttribArray(2);
}

#endif