//
// Created by Drew on 2019-08-15.
//

#ifndef YAREZO_APPLICATION
#define YAREZO_APPLICATION

#include <memory>

#include "Core/Core.h"
#include "Core/YzWindow.h"

namespace Yarezo {

    class Application {
    public:
        Application();
        virtual ~Application();
        void run();

        std::shared_ptr<YzWindow> getWindow() const { return m_Window; }

        inline static Application* getAppInstance() { return s_AppInstance; }
        static bool logFPS;
    private:
        std::shared_ptr<YzWindow> m_Window;
        static Application* s_AppInstance;
    };

    Application* createApplication();
}


#endif //YAREZO_APPLICATION
