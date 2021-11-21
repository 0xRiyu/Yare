#ifndef YARE_INPUTHANDLER_H
#define YARE_INPUTHANDLER_H

namespace Yare {
    class InputHandler {
       public:
        virtual ~InputHandler() = default;
        virtual void handle() = 0;
    };
}  // namespace Yare

#endif  // YARE_INPUTHANDLER_H
