#include "LogHUB.h"



LogHUB::LogHUB(String cn) {
    clName = strdup(cn.c_str());
    logFactory.addClass(clName);
}

LogHUB::LogHUB(const char* cn) {
    clName = strdup(cn);
    logFactory.addClass(clName);
}

LogHUB::~LogHUB() {
    free(clName);
}

bool LogHUB::debug(String msg) { return print(LogLevel::L_DEBUG,msg);}
bool LogHUB::info(String msg) { return print(LogLevel::L_INFO,msg);}
bool LogHUB::error(String msg) { return print(LogLevel::L_ERR,msg);}
bool LogHUB::warn(String msg) { return print(LogLevel::L_ERR,msg);}



bool LogHUB::print(LogLevel level, String msg) { 
    int count = logFactory.print(clName,level,msg.c_str());
    return count>0;
 }




