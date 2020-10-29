#ifndef _LOG_HUB_H_
#define _LOG_HUB_H_
#include "LogFactory.h"


class LogHUB {
    private:
    char* clName;

    public:
    LogHUB(const char* clName="Unknown");    
    LogHUB(String clName);
    ~LogHUB();

    bool debug(String msg);
    bool error(String msg);
    bool info(String msg);
    bool warn(String msg);
    bool print(LogLevel level, String msg);
};

#endif