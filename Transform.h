#ifndef TRANSFORM_H_IN
#define TRANSFORM_H_IN

#include <glm/glm.hpp>
// #include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

namespace dm {
struct Transform {
    Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
            : m_pos(std::move(pos)),
              m_rot(std::move(rot)),
              m_scale(std::move(scale)) {}

    Transform() = default;

    // Model in OpenGL jargon refers to structure
    // containing position, rotation and scale.
    glm::mat4 getModel() const {
        glm::mat4 posMatrix = glm::translate(m_pos);
        glm::mat4 scaleMatrix = glm::scale(m_scale);
        glm::mat4 rotXMatrix = glm::rotate(m_rot.x, glm::vec3(1, 0, 0));
        glm::mat4 rotYMatrix = glm::rotate(m_rot.y, glm::vec3(0, 1, 0));
        glm::mat4 rotZMatrix = glm::rotate(m_rot.z, glm::vec3(0, 0, 1));

        glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

        return posMatrix * rotMatrix * scaleMatrix;
    }

    glm::vec3 m_pos;
    glm::vec3 m_rot; // Usually one would represent rotation via quaternions
    glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
};
} // namespace dm

#endif // _H_IN
