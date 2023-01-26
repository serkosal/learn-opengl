#pragma once
#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "third_party/glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb_image/stb_image.h"

#include <filesystem>
#include <stdexcept>

class Texture
{
public:
    static std::filesystem::path directory;

    enum class Type {
        unknown = -1,

        diffuse,
        specular,
    };
private:
    GLuint _id = 0;
    Type _type = Type::diffuse;

    void del() { glDeleteTextures(1, &_id); }
public:
    Texture() = delete;
    
    Texture(const Texture& other) = delete;
    Texture& operator=(const Texture& other) = delete;

    Texture(Texture&& other) : _id(other._id), _type(other._type)
    {
        other._id = 0;
    }
    Texture& operator=(Texture&& other)
    {
        this->_id = other._id;
        this->_type = other._type;
        other._id = 0;

        return *this;
    }

    ~Texture() 
    {
        if (_id)
            this->del();
    }

    inline Texture(
        const std::filesystem::path& path, 
        Type type = Type::diffuse,
        bool flip=true,
        bool standart_dir=true
    );

    void bind() const { glBindTexture(GL_TEXTURE_2D, _id); }
    void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
};

std::filesystem::path Texture::directory = "../resources/textures/";

inline Texture::Texture(const std::filesystem::path& path, Type type, bool flip, bool standart_dir) 
    : _type(type)
{
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    auto result_path = standart_dir ? directory / path : path;

    unsigned char *data = stbi_load(result_path.c_str(), &width, &height, &nrChannels, 0);
    if(data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else
    {
        stbi_image_free(data);
        throw std::runtime_error("Error to load texture located: " + path.string());
    }
}



#endif