#include "Logger.h"

#include <iostream>
#include <stdio.h>
#include <string.h>

Logger::Logger(String clName) {
    className = clName.c_str();
    // Add class  to filter list
    formatter = logFactory.getFormater();
    }

bool Logger::debug(String msg) { return printOut(LogLevel::DEBUG,msg);}
bool Logger::info(String msg) { return printOut(LogLevel::INFO,msg);}
bool Logger::error(String msg) { return printOut(LogLevel::ERROR,msg);}


bool Logger::printOut(LogLevel level, String msg) { 
    //String(humanTime) + " [" +level + "] "+className+":"+msg




    if (logFilter.isShow(className,level)) {
        time_t rawtime;
        struct tm * timeinfo;
        char humanTime[80];
        time (&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(humanTime,sizeof(humanTime),"%d-%m-%Y %H:%M:%S",timeinfo);

        const char* buff = formatter->parse(humanTime,className,level2str(level),msg.c_str());

        #if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_AVR)
        Serial.println(buff);
        #else 
        printf("%s\n",buff);
        #endif

        // free((void*)buff);
        return true;
    }
    return false;
 }




