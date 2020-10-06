#include "LogChannel.h"
#include "LogFormat.h"
#include <stdlib.h>


/*
*  Construct teh channel instance with given name
*/
LogChannel::LogChannel(const char* name) {
    chName = strdup(name);
    filter = new LogFilter;
    formatter = NULL;
}

/*
*  Destructor
*/
LogChannel::~LogChannel() {
    delete chName;
    delete filter;
    if (formatter != NULL)  delete formatter;
}

/**
 * Retun channel name
 */
const char* LogChannel::getName() {
    return chName;
}

/**
 * 
 * Get format dtring and prepare tokens for log print out formatting
 * @param fmt format string @link LogFormat.h
 * 
 */
void LogChannel::setFormat(const char* fmt) {
    if( formatter != NULL) delete formatter;
    formatter = new LogFormat();
    parserError = formatter->parseFmt(fmt);
}

void LogChannel::addClass(const char* clName) {
    setFilter(clName, DEBUG, filter->getDefView(DEBUG));
    setFilter(clName, INFO, filter->getDefView(INFO));
    setFilter(clName, ERROR, filter->getDefView(ERROR));
    setFilter(clName, WARNING, filter->getDefView(WARNING));
}

void LogChannel::setFilter(const char* clName, LogLevel level, bool show) {
    filter->setFilter(clName, level, show);
}

bool LogChannel::doFilter(const char* clName, LogLevel level) {
    return filter->isShow(clName,level);
}

char* LogChannel::formatTS() {
        time_t rawtime;
        struct tm * timeinfo;
        //char humanTime[80];
        char* buff = (char*)malloc(TIME_BUFF_LEN);
        time (&rawtime);
        timeinfo = localtime(&rawtime);
        size_t size = strftime(buff,TIME_BUFF_LEN,"%d-%m-%Y %H:%M:%S",timeinfo);
        buff[size] = 0;
        return buff;
}

int LogChannel::print(const char* clName, LogLevel level, const char* msg) {
    int outputLen= 0;
    if( doFilter(clName,level) ) {
        char* tsBuff = formatTS();
        const char* buff;

        if ((parserError == 0) && (formatter != NULL)) {
            buff = formatter->compile(tsBuff,clName,level,msg);
        }
        else {
            buff = strdup(msg);
        }
        delete tsBuff;
        outputLen = out(buff);

        if( buff != NULL) {
            delete buff;
        }
    }
    return outputLen;
}


int LogChannel::out(const char* buff) {
    return 1;
}



/**
 *  Default example class for printout to serial console
 */
LogCHSerial::LogCHSerial():LogChannel("serial") {
#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_COMPILE_)  
    Serial.begin();
#endif
    
}

int LogCHSerial::out(const char* buff) {
#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_COMPILE_)     
    Serial.println(msg);
    return strlen(msg);
#else 
    return fprintf(stdout, "%s\n", buff); 
#endif
}



