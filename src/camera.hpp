#pragma once
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "third_party/glm/glm.hpp"
#include "third_party/glm/gtc/matrix_transform.hpp"

struct Camera
{
    glm::vec3 _pos = glm::vec3(0.f);

    float _yaw = 90.f;
    float _pitch = 0.f;

    float _fov = 45.f;

    float _sensitivity = 1.f;
    float _speed = 1.f;

    glm::vec3 _world_up = glm::vec3(0.f, 1.f, 0.f);


    void ProcessMouseMovement(float xoffset, float yoffset)
    {
        _yaw   += xoffset * _sensitivity;
        _pitch += yoffset * _sensitivity;


        if (_pitch > 89.0f)
            _pitch = 89.0f;
        if (_pitch < -89.0f)
            _pitch = -89.0f;
    }

    void move(const glm::vec3& offset)
    {
        _pos += offset;
    }

    glm::vec3 get_dir() const
    {
        return glm::normalize(
            glm::vec3{
                cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)),
                sin(glm::radians(_pitch)),
                sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))
            }
        );
    }

    glm::vec3 get_right() const { return glm::normalize(glm::cross(get_dir(), _world_up));  }
    glm::vec3 get_up() const    { return glm::normalize(glm::cross(get_right(), get_dir()));}

    glm::mat4 get_view() const { return glm::lookAt(_pos, _pos + get_dir(), get_up()); }
    glm::mat4 get_proj(float aspect_ratio) const 
    { return glm::perspective(_fov, aspect_ratio, 0.1f, 100.f); }

};

#endif