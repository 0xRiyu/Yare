//
// Created by Drew on 2019-08-15.
//

#ifndef YAREZO_APPLICATION
#define YAREZO_APPLICATION

#include <memory>

#include "src/Core.h"

namespace Yarezo {

    class Window;

    class YAREZO_API Application {
    public:
        Application();
        virtual ~Application();
        void run();

        std::shared_ptr<Window> getWindow() const { return m_Window; }

        inline static Application* getAppInstance() { return s_AppInstance; }
    private:
        std::shared_ptr<Window> m_Window;
        static Application* s_AppInstance;
    };

    Application* createApplication();
}


#endif //YAREZO_APPLICATION
