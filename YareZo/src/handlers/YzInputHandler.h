#ifndef YAREZO_INPUTHANDLER_H
#define YAREZO_INPUTHANDLER_H

namespace Yarezo {
	class InputHandler {
	public:
		virtual ~InputHandler() = default;
		virtual void handle() = 0;
	};
}

#endif // YAREZO_INPUTHANDLER_H

