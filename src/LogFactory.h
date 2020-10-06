#ifndef LOG_FACTRY_H_
#define LOG_FACTRY_H_
#include "LogFormat.h"
#include "LogFilter.h"
#include "LogChannel.h"
#include "SLinkedList.h"

class LogFactory {
    typedef SLinkedList<LogChannel*>  LinkedList;
    private:
    LinkedList chList;
    const char* formatSrt;

    public:
    LogFactory(bool useConsole = true);
    ~LogFactory();


    LogChannel* addChannel(LogChannel* channel);
    int delChannel(const char* chName);

    // int setChannelOFF(const char* chName);
    // int setChannelON(const char* chName);

    void setChannelFormat(const char* chName, const char* formatString = NULL);

    void addClass(const char* clName);
    int setChFilter(const char* chName, const char* clName, LogLevel level, bool isShow );
    void setFilter(const char* clName, LogLevel level, bool isShow );

    int print(const char* clName, LogLevel level, const char* mesage);


};

extern LogFactory logFactory;

#endif