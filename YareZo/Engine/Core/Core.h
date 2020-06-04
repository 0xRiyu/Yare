#ifndef YAREZO_CORE_H
#define YAREZO_CORE_H

#define NONCOPYABLE(type_identifier)                                \
    type_identifier(const type_identifier&) = delete;               \
    type_identifier& operator=(const type_identifier&) = delete;

#endif //YAREZO_CORE_H
