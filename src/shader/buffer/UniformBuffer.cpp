#define _CRT_SECURE_NO_WARNINGS
#include "shader/buffer/UniformBuffer.h"

#include <memory>
#include <cassert>

#include "util/GLcheck.h"

std::set<GLint> UniformBuffer::allocatedBindPoints;
UniformBuffer::UniformBuffer(size_t size, void* data)
    : BufferCore(GL_UNIFORM_BUFFER, allocateBindPoint(), size, data) { }
UniformBuffer::~UniformBuffer() {
    allocatedBindPoints.erase(bufferBindPoint);
}

GLint UniformBuffer::allocateBindPoint() {
    if (allocatedBindPoints.size() == MaxBuffers()) {
        char buff[1024];
        snprintf(buff, sizeof(buff), "Uniform Buffer Bindings exceeded!\nLimit = %d\n\nsdl_exp UniformBuffer objs are not designed for sharing buffer bindings.", MaxBuffers());
        throw std::exception(buff);
    }
    for (unsigned int i = MaxBuffers() - 1; i >= 0; --i) {
        if (allocatedBindPoints.find(i) == allocatedBindPoints.end()) {
            allocatedBindPoints.insert(i);
            return i;
        }
    }
    assert(false);  // Should never reach here
    return -1;
}

GLint UniformBuffer::MaxSize() {
    return BufferCore::maxSize(GL_UNIFORM_BUFFER);
}
GLint UniformBuffer::MaxBuffers() {
    return BufferCore::maxBuffers(GL_UNIFORM_BUFFER);
}

// Comment out this include if not making use of Shaders/ShaderCore
#include "shader/Shaders.h"
#ifdef SRC_SHADER_SHADERS_H_
bool Shaders::setMaterialBuffer(const std::shared_ptr<UniformBuffer> &buffer) {
    return addBuffer(MATERIAL_UNIFORM_BLOCK_NAME, buffer);
}
#endif  // SRC_SHADER_SHADERS_H_