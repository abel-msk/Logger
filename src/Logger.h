#ifndef LOGGER_H_
#define LOGGER_H_
#include <stdlib.h>

#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_AVR)
#define ARDUINO_ARCH
#include <WString.h>
#else
#include <string>
#include <stdio.h>
typedef std::string String;
#endif

#include <time.h>
#include "LogFilter.h"
#include "LogFactory.h"


class Logger {
    private:
    const char* className;
    LogFilter logFilter;
    LogFormat *formatter;

    public:
    Logger();
    Logger(String clName="Unknown");

    bool debug(String msg);
    bool error(String msg);
    bool info(String msg);

    bool printOut(LogLevel level, String msg);
};

#endif