#ifndef _ROXY_APPLICATION_H_
#define _ROXY_APPLICATION_H_

#include <tchar.h>
#include <ctime>
#include <windows.h>

#include "Algorithms.h"

namespace Roxy {
    class Application {

    public:
        Application();
        ~Application();

        int loop();
    };
}

#endif
