#ifndef TEXTURE_H_IN
#define TEXTURE_H_IN

#include <string>
#include <GL/glew.h>

namespace dm {
class Texture {
public:
    Texture(std::string const &);
    ~Texture();

    void bind(uint32_t unit);

    Texture(Texture const &) = delete;
    Texture(Texture &&) = delete;
    Texture &operator=(Texture const &) = delete;
    Texture operator=(Texture &&) = delete;

private:
    GLuint m_texture;
};
} // namespace dm

#endif // TEXTURE_H_IN
