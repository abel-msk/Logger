#ifndef _LOG_CHANNEL_
#define _LOG_CHANNEL_
#include "BinaryTree.h"
#include "LogFilter.h"
#include "LogLevels.h"
#define TIME_BUFF_LEN 100


#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_COMPILE_) 

#ifndef SERIAL_BAUD_SPEED 
#define SERIAL_BAUD_SPEED 115200
#endif

#include <WString.h>
// #include <Arduino.h>
#include <cstring>
#include <HardwareSerial.h>
#else
// #include <iostream>
// #include <string.h>
// #include <string>
#include <stdio.h>
// typedef std::string String;
#endif


class LogChannel {
    protected:
    const char* chName;
    int parserError;
    LogFilter* filter;
    LogFormat* formatter;
    
    public:
    LogChannel(const char* name = "unknown");
    virtual ~LogChannel();
    virtual const char* getName();
    virtual void setFormat(const char* fmt);
    virtual void addClass(char* clName);
    virtual void setFilter(char* clName, LogLevel level, bool show);
    virtual bool doFilter(char* clName, LogLevel level);
    virtual char* formatTS();
    virtual int print(char* clName, LogLevel level, const char* msg);
    virtual int out(char* buff);
    virtual LogFilter* getFilterObject();

};

class LogCHSerial: public LogChannel {
    public:
    LogCHSerial();
    virtual int out(char* msg);
};


#endif