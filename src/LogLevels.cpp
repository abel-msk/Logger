
#include "LogLevels.h"

const char* level2str(LogLevel level) {
    switch (level) {
    case LogLevel::L_INFO: 
        return LOG_LEVEL_INFO;
    case LogLevel::L_DEBUG:
        return LOG_LEVEL_DEBUG;
    case LogLevel::L_ERR:
        return LOG_LEVEL_ERROR;
    case LogLevel::L_WARN:
        return LOG_LEVEL_WARNING;    
    default:
        return "";
    }
}

/**
 *  Init available levels list
 *  Set all lefels in default mode show=true
 */
LogLevels::LogLevels() {
    LevelDefaults* def =  new LevelDefaults();
    def->level = LogLevel::L_INFO;
    def->value = true;
    def->name = "INFO";
    PushBack(def);

    def =  new LevelDefaults();
    def->level = LogLevel::L_ERR;
    def->value = true;
    def->name = "ERROR";
    PushBack(def);

    def =  new LevelDefaults();
    def->level = LogLevel::L_DEBUG;
    def->value = true;
    def->name = "DEBUG";
    PushBack(def);

    def =  new LevelDefaults();
    def->level = LogLevel::L_WARN;
    def->value = true;
    def->name = "WARN";
    PushBack(def);
}

void LogLevels::setDefault(LogLevel level, bool value) {
    iterator_t it = begin();
    while (it.Valid()) {
        if (it.Item()->level == level) {
            it.Item()->value = value;
            return;
        }
        ++it;
	}
}

bool LogLevels::getDefault(LogLevel level) {
    iterator_t it = begin();
    while (it.Valid()) {
        if (it.Item()->level == level) {
            return it.Item()->value;
        }
        ++it;
	}
    return false;
}

LogLevel LogLevels::getByName(const char* name) {
    iterator_t it = begin();
    while (it.Valid()) {
        if (strcmp(it.Item()->name,name) == 0) {
            return it.Item()->level;
        }
        ++it;
	}
    return LogLevel::L_INFO;
}

const char* LogLevels::getLevelName(LogLevel level) {
    iterator_t it = begin();
    while (it.Valid()) {
        if (it.Item()->level == level ) {
            return it.Item()->name;
        }
        ++it;
	}
    return "unknown";
}
