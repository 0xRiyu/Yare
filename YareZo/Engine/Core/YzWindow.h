//
// Created by Drew on 2019-08-17.
//

#ifndef YAREZO_YZ_WINDOW_H
#define YAREZO_YZ_WINDOW_H

#include "Core/Core.h"
#include "Core/YzInputHandler.h"
#include "Core/YzCamera.h"

#include <memory>
#include <string>

namespace Yarezo {

    struct WindowProperties {
        unsigned short width;
        unsigned short height;
    };

    class YzWindow {
    public:
        virtual ~YzWindow() = default;
        virtual void onUpdate() = 0;
        virtual void closeWindow() = 0;
        virtual void setKeyInputCallback() = 0;
        virtual void setMouseInputCallback() = 0;
        virtual void setScrollInputCallback() = 0;
        virtual void setFrameBufferResizeCallback() = 0;
        virtual WindowProperties getWindowProperties()            const { return m_Properties; }
        virtual std::shared_ptr<YzInputHandler> getKeyHandler()   const { return m_KeyHandler; }
        virtual std::shared_ptr<YzInputHandler> getMouseHandler() const { return m_MouseHandler; }
        virtual std::shared_ptr<YzCamera> getCamera()             const { return m_Camera; }
        virtual void setIcon(const std::string& filePath) = 0;

        static std::shared_ptr<YzWindow> createNewWindow(WindowProperties& properties);
        virtual void* getNativeWindow() const = 0;

        bool windowResized = false;
    protected:
        WindowProperties m_Properties;
        std::shared_ptr<YzInputHandler> m_KeyHandler;
        std::shared_ptr<YzInputHandler> m_MouseHandler;
        std::shared_ptr<YzCamera> m_Camera;
    };
}



#endif //YAREZO_YZ_WINDOW_H
