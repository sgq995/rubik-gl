#ifndef APP_H_
#define APP_H_

#include "shader.h"
#include "window.h"

class App {
public:
  App();
  ~App();

  const char *GetError() const;

  void Start();
  void Run();

  int system_status() const;

protected:
  bool InitShader(Shader *shader);

private:
  int system_status_ = -1;

  bool running_ = false;

  Window window_;
};

#endif  // APP_H_
