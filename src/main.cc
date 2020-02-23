#include <iostream>

#include "app.h"
#include "config.h"

int main(int argc, char **argv) {
  App app;
  
  app.Start();

  if (app.system_status() < 0) {
    std::cout << app.GetError() << std::endl;
  }
  return app.system_status();
}
