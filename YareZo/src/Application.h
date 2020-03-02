//
// Created by Drew on 2019-08-15.
//

#ifndef YAREZO_APPLICATION
#define YAREZO_APPLICATION

#include <memory>

#include "src/Core.h"
#include "src/window.h"

namespace Yarezo {

    class Application {
    public:
        Application();
        virtual ~Application();
        void run();

        std::shared_ptr<Window> getWindow() const { return m_Window; }

        inline static Application* getAppInstance() { return s_AppInstance; }
        static bool logFPS;
    private:
        std::shared_ptr<Window> m_Window;
        static Application* s_AppInstance;
    };

    Application* createApplication();
}


#endif //YAREZO_APPLICATION
