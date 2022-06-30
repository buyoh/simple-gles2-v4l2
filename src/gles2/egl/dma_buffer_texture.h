#ifndef EGL_SRC_GLES2_EGL_DMA_BUFFER_TEXTURE_H_
#define EGL_SRC_GLES2_EGL_DMA_BUFFER_TEXTURE_H_

#include <vector>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "v4l2/v4l2_device.h"

// TODO: rename DMABufferTextures
class DMABufferTexture {
 public:
  DMABufferTexture() {}
  ~DMABufferTexture() = default;  // TODO: release

  bool initialize(EGLDisplay egl_display,
                  V4L2Device& v4l2_device,
                  int width,
                  int height);
  // void release();  // TODO:

  const std::vector<GLuint>& textures() const { return textures_; }

  void bindTexture(int idx) const;

  // for testing // WIP
  void dequeue(V4L2Device& v4l2_device, int idx) const;
  void queue(V4L2Device& v4l2_device, int idx) const;

 private:
  std::vector<GLuint> textures_;
  std::vector<int> dmabuf_fds_;
};

#endif  // EGL_SRC_GLES2_EGL_DMA_BUFFER_TEXTURE_H_