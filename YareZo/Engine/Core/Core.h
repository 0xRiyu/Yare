//
// Created by Drew on 2019-08-15.
//

#ifndef YAREZO_CORE
#define YAREZO_CORE

#define NONCOPYABLE(type_identifier)								\
    type_identifier(const type_identifier&) = delete;				\
    type_identifier& operator=(const type_identifier&) = delete;


#if (_WIN64)
    #if YZ_DYNAMIC_LINK
        #ifdef YZ_BUILD_DLL
            #define YAREZO_API __declspec(dllexport)
        #else
            #define YAREZO_API __declspec(dllimport)
        #endif
    #else
        #define YAREZO_API
    #endif
#elif (__linux__)
    #if YZ_DYNAMIC_LINK
        #ifdef YZ_BUILD_DLL
            #define YAREZO_API __attribute__((visibility("default")))
        #else
            #define YAREZO_API
        #endif
    #endif
#else
    #error YareZo only supports Linux and Windows.
#endif

#endif //YAREZO_CORE
