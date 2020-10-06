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
    delete clName;
}

bool Logger::debug(String msg) { return print(LogLevel::DEBUG,msg);}
bool Logger::info(String msg) { return print(LogLevel::INFO,msg);}
bool Logger::error(String msg) { return print(LogLevel::ERROR,msg);}
bool Logger::warn(String msg) { return print(LogLevel::ERROR,msg);}



bool Logger::print(LogLevel level, String msg) { 
    int count = logFactory.print(clName,level,msg.c_str());
    return count>0;
 }




