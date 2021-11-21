#ifndef YARE_APPLICATION_H
#define YARE_APPLICATION_H

#include <memory>

#include "Core/Core.h"
#include "Graphics/Window/Window.h"

namespace Yare {

    class Application {
       public:
        Application();
        virtual ~Application();
        void run();

        std::shared_ptr<Graphics::Window> getWindow() const { return m_Window; }

        inline static Application* getAppInstance() { return s_AppInstance; }

       private:
        std::shared_ptr<Graphics::Window> m_Window;
        static Application*               s_AppInstance;
    };

    Application* createApplication();
}  // namespace Yare

#endif  // YARE_APPLICATION_H
