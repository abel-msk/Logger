#ifndef LOGGER_H_
#define LOGGER_H_

#include "LogFactory.h"


class Logger {
    private:
    char* clName;

    public:
    Logger(const char* clName="Unknown");    
    Logger(String clName);
    ~Logger();

    bool debug(String msg);
    bool error(String msg);
    bool info(String msg);
    bool warn(String msg);
    bool print(LogLevel level, String msg);
};

#endif