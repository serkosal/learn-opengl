#pragma once
#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "third_party/glm/glm.hpp"

struct Vertex
{
    glm::vec3 _pos;
    glm::vec3 _nor;
    glm::vec2 _tex;

    Vertex(
        const glm::vec3 position = glm::vec3(0.f), 
        const glm::vec3 normal   = glm::vec3(0.f),
        const glm::vec2 texture  = glm::vec2(0.f)
    ) : 
        _pos(position),
        _nor(normal),
        _tex(texture)
    {}
};


#endif