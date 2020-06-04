#ifndef YAREZO_APPLICATION_H
#define YAREZO_APPLICATION_H

#include <memory>

#include "Core/Core.h"
#include "Graphics/Window/YzWindow.h"

namespace Yarezo {

    class Application {
    public:
        Application();
        virtual ~Application();
        void run();

        std::shared_ptr<Graphics::YzWindow> getWindow() const { return m_Window; }

        inline static Application* getAppInstance() { return s_AppInstance; }
        static bool logFPS;
    private:
        std::shared_ptr<Graphics::YzWindow> m_Window;
        static Application* s_AppInstance;
    };

    Application* createApplication();
}

#endif //YAREZO_APPLICATION_H
