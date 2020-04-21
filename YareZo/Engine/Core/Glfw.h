// We are are on windows we need to include windows.h to avoid the APIENTRY redefinition
// otherwise  just define it before including glfw
#ifdef _WIN32
#include <windows.h>
#else
#define APIENTRY
#endif
#include <GLFW/glfw3.h>
