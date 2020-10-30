#include "LogChannel.h"
#include "LogFormat.h"
#include "LogLevels.h"
#include <stdlib.h>
#ifdef __DEBUG_ESP_MEM_LEAK__
#include <esp.h>
#endif


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

void LogChannel::addClass(char* clName) {
    filter->addClass(clName);
}

void LogChannel::setFilter(const char* clName, LogLevel level, bool show) {
    filter->setFilter(clName, level, show);
}

bool LogChannel::doFilter(char* clName, LogLevel level) {
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

        // Serial.println();
        // Serial.print(F("Time info = "));
        // Serial.print(buff);
        // Serial.println();
        // Serial.print(F("Time len = "));
        // Serial.print(strlen(buff));
        // Serial.println();


        return buff;
}

int LogChannel::print(char* clName, LogLevel level, const char* msg) {

#ifdef __DEBUG_ESP_MEM_LEAK__
    Serial.print(F("Free mem before  print = "));
    Serial.println(ESP.getFreeHeap());   
#endif  

    int outputLen= 0;
    if( doFilter(clName,level) ) {

        char* tsBuff = formatTS();
        char* buff = NULL;

        if ((parserError == 0) && (formatter != NULL)) {
            buff = formatter->compile(tsBuff,clName,level,msg);
        }
        else {
            buff = strdup(msg);
        }
        free((void*)tsBuff);

        outputLen = out(buff);

        if( buff != NULL) {
            free((void*)buff);
        }
    }

#ifdef __DEBUG_ESP_MEM_LEAK__
    Serial.print(F("Free mem after  print = "));
    Serial.println(ESP.getFreeHeap()); 
#endif  

    return outputLen;
}


int LogChannel::out(char* buff) {
    return 1;
}

 LogFilter* LogChannel::getFilterObject() {
     return filter;
 }



/**
 *  Default example class for printout to serial console
 */
LogCHSerial::LogCHSerial():LogChannel("serial") {
#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_COMPILE_)  
    Serial.begin(SERIAL_BAUD_SPEED);
#endif
    
}

int LogCHSerial::out(char* buff) {
#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_COMPILE_)     
    Serial.println(buff);
    return strlen(buff);
#else 
    return fprintf(stdout, "%s\n", buff); 
#endif
}





