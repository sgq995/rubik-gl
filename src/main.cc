#include <iostream>

#include "config.h"

int main(int argc, char **argv) {
    using namespace std;

    cout << "Hello world" << endl;
    cout << "Version " << 
        RUBIKGL_VERSION_MAJOR << '.' <<
        RUBIKGL_VERSION_MINOR << endl;
    
    return 0;
}
