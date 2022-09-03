#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cassert>
#include <iostream>

namespace dm {
Texture::Texture(std::string const& filename) {
    int32_t width;
    int32_t height;
    int32_t numComponents;
    uint8_t* imageData = stbi_load(filename.c_str(), &width, &height,
                                   &numComponents, /* req_comp */ 4);

    if (imageData == nullptr) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return;
    }

    glGenTextures(/* numTextures */ 1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, imageData);

    stbi_image_free(imageData);
}

Texture::~Texture() { glDeleteTextures(/* numTextures */ 1, &m_texture); }

void Texture::bind(uint32_t unit) {
    assert(unit <= 31);

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}
} // namespace dm
