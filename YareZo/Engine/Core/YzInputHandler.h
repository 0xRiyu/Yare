#ifndef YAREZO_INPUTHANDLER_H
#define YAREZO_INPUTHANDLER_H

namespace Yarezo {
	class YzInputHandler {
	public:
		virtual ~YzInputHandler() = default;
		virtual void handle() = 0;
	};
}

#endif // YAREZO_INPUTHANDLER_H

