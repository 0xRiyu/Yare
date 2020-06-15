#ifndef YARE_GLOBAL_SETTINGS_H
#define YARE_GLOBAL_SETTINGS_H

#include "Utilities/T_Singleton.h"

namespace Yare {
    class GlobalSettings : public Utilities::T_Singleton<GlobalSettings> {

    public:
        GlobalSettings() {}
        bool displayModels = true;
        bool displayBackground = true;
        bool logFps = false;
        double fps = 0;
    };
}

#endif // YARE_GLOBAL_SETTINGS_H
