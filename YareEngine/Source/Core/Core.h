#ifndef YARE_CORE_H
#define YARE_CORE_H

#define NONCOPYABLE(type_identifier)                  \
    type_identifier(const type_identifier&) = delete; \
    type_identifier& operator=(const type_identifier&) = delete;

#endif  // YARE_CORE_H
