#ifndef YAREZO_GLOBAL_SETTINGS_H
#define YAREZO_GLOBAL_SETTINGS_H

#include "Utilities/T_Singleton.h"

namespace Yarezo {
    class GlobalSettings : public Utilities::T_Singleton<GlobalSettings> {

    public:
        GlobalSettings() {}
        bool displayModels = true;
        bool displayBackground = true;
        double fps = 0;
    };
}

#endif // YAREZO_GLOBAL_SETTINGS_H
