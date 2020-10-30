#include "Logger.h"

Logger::Logger(const char* cn) {
    clName = strdup(cn);
    Logger::init();
}

Logger::Logger(String cn) {
    clName = strdup(cn.c_str());
    Logger::init();    
}


Logger::~Logger() {
    if (clName != NULL) free(clName);
}

void Logger::init() {
    if ( clName != NULL ) {
        logFactory.addClass(clName);
        isAdded = true;
    }
}

void Logger::setName(const char* cn) {
    if ( clName != NULL) {
        free(clName);
    }
    clName = strdup(cn);
}

const char* Logger::getName() {
    return clName;
}

bool Logger::debug(String msg) { return print(LogLevel::L_DEBUG,msg);}
bool Logger::info(String msg) { return print(LogLevel::L_INFO,msg);}
bool Logger::error(String msg) { return print(LogLevel::L_ERR,msg);}
bool Logger::warn(String msg) { return print(LogLevel::L_ERR,msg);}



bool Logger::print(LogLevel level, String msg) { 
    if (! isAdded ) {
        init();
    }
    int count = logFactory.print(clName,level,msg.c_str());
    return count>0;
 }




