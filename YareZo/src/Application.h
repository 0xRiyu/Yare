//
// Created by Drew on 2019-08-15.
//

#ifndef YAREZO_APPLICATION
#define YAREZO_APPLICATION

#include "src/Core.h"

namespace Yarezo {

    class YAREZO_API Application {
    public:
        Application();
        virtual ~Application();
        void run();
    };

    Application* createApplication();
}


#endif //YAREZO_APPLICATION
