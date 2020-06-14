#ifndef YARE_INPUTHANDLER_H
#define YARE_INPUTHANDLER_H

namespace Yare {
    class YzInputHandler {
    public:
        virtual ~YzInputHandler() = default;
        virtual void handle() = 0;
    };
}

#endif // YARE_INPUTHANDLER_H
