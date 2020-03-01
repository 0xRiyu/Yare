#ifndef YAREZO_HANDLER
#define YAREZO_HANDLER

#include <memory>

namespace Yarezo {
	class Handler {
	public:
		virtual ~Handler() = default;
		virtual void handle(std::shared_ptr<Camera> currentCamera) = 0;
	};
}
#endif // !YAREZO_HANDLER

