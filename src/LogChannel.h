#ifndef _LOG_CHANNEL_
#define _LOG_CHANNEL_
#include "BinaryTree.h"
#include "LogFilter.h"
#define TIME_BUFF_LEN 100


#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_COMPILE_) 
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
    ~LogChannel();
    virtual const char* getName();
    virtual void setFormat(const char* fmt);
    virtual void addClass(const char* clName);
    virtual void setFilter(const char* clName, LogLevel level, bool show);
    virtual bool doFilter(const char* clName, LogLevel level);
    virtual char* formatTS();
    virtual int print(const char* clName, LogLevel level, const char* msg);
    virtual int out(const char* buff);

};

class LogCHSerial: public LogChannel {
    public:
    LogCHSerial();
    virtual int out(const char* msg);
};


#endif