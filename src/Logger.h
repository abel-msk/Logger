#ifndef _LOG_HUB_H_
#define _LOG_HUB_H_
#include "LogFactory.h"


class Logger {
    private:
    bool isAdded = false;
    char* clName = NULL; 
    void init();

    public:
    Logger(const char* className);    
    Logger(String className);

    ~Logger();
    const char* getName();
    void setName(const char* cn);
    bool debug(String msg);
    bool error(String msg);
    bool info(String msg);
    bool warn(String msg);
    bool print(LogLevel level, String msg);


};

#endif