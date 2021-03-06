#pragma once

#include "../Library.hpp"
#include "Enums.hpp"
#include "PixelType.hpp"
#include <glad.h>

namespace Scissio {
struct NoCreate {};

constexpr inline NoCreate NO_CREATE{};

class SCISSIO_API Texture {
public:
    explicit Texture(const NoCreate&);
    explicit Texture(GLuint target);
    Texture(Texture&& other) noexcept;
    Texture(const Texture& other) = delete;
    virtual ~Texture();
    Texture& operator=(const Texture& other) = delete;
    Texture& operator=(Texture&& other) noexcept;
    void swap(Texture& other) noexcept;

    void bind() const;
    void bind(GLuint location) const;
    void generateMipmaps();
    void texParameteri(GLenum parameter, GLint value);

    GLuint getHandle() const {
        return handle;
    }

    GLuint getTarget() const {
        return target;
    }

    operator bool() const {
        return handle > 0;
    }

protected:
    GLuint handle;
    GLuint target;
};
} // namespace Scissio