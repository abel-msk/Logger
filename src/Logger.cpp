#include "Logger.h"



Logger::Logger(String cn) {
    clName = strdup(cn.c_str());
    logFactory.addClass(clName);
}

Logger::Logger(const char* cn) {
    clName = strdup(cn);
    logFactory.addClass(clName);
}

Logger::~Logger() {
    free(clName);
}

bool Logger::debug(String msg) { return print(LogLevel::L_DEBUG,msg);}
bool Logger::info(String msg) { return print(LogLevel::L_INFO,msg);}
bool Logger::error(String msg) { return print(LogLevel::L_ERR,msg);}
bool Logger::warn(String msg) { return print(LogLevel::L_ERR,msg);}



bool Logger::print(LogLevel level, String msg) { 
    int count = logFactory.print(clName,level,msg.c_str());
    return count>0;
 }




