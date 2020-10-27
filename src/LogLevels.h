
#ifndef __LOG_LEVELS_H_
#define __LOG_LEVELS_H_

#include "SLinkedList.h"
#include <string.h>

#define LOG_LEVEL_DEBUG "DEBUG"
#define LOG_LEVEL_ERROR "ERROR"
#define LOG_LEVEL_INFO "INFO"
#define LOG_LEVEL_WARNING "WARNING"

#define DEF_LOG_PRINT_DEBUG false
#define DEF_LOG_PRINT_INFO false
#define DEF_LOG_PRINT_WARN true
#define DEF_LOG_PRINT_ERROR true

enum LogLevel {
    L_INFO,
    L_WARN,
    L_DEBUG,
    L_ERR
};

struct LevelDefaults {
    LogLevel level;
    bool value;
    const char* name;
};

const char* level2str(LogLevel level);

// typedef SLinkedList<LevelDefaults*>::iterator LevelIterator;

class LogLevels : public SLinkedList<LevelDefaults*>  {
    public:
    typedef SLinkedList<LevelDefaults*>  list_t;
    typedef SListIterator<LevelDefaults*> iterator_t;
    LogLevels();
    void setDefault(LogLevel level, bool value);
    bool getDefault(LogLevel level);
    LogLevel getByName(const char* name);
    const char* getLevelName(LogLevel level);
};

#endif