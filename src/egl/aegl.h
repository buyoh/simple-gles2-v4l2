#ifndef EGL_SRC_EGL_EGL_H_
#define EGL_SRC_EGL_EGL_H_

#include <EGL/egl.h>

class AEgl {
public:
  AEgl();
  ~AEgl();
  bool initialize(void *eglNativeDisplay, void *eglNativeWindow,
                  bool enable_depth);

  EGLDisplay getDisplay() const;
  EGLSurface getSurface() const;

private:
  EGLDisplay display_;
  EGLContext context_;
  EGLSurface surface_;
};

#endif // EGL_SRC_EGL_EGL_H_