
#ifndef SRC_LIB_GL_RENDER_H_
#define SRC_LIB_GL_RENDER_H_

// A GlRenderer renders into opengl surfaces.
//
class GlRenderer {
 public:
  virtual bool initialize() = 0;
  virtual bool render() = 0;
  virtual void stop() = 0;
};

#endif  // SRC_LIB_GL_RENDER_H_