#include "LogFactory.h"


// LogFactory::LogFactory() {
//     formatSrt = "%date% [%level%] %clname%: %msg%";

// }

LogFactory::LogFactory(bool useConsole) {
    formatSrt = "%date% [%level%] %clname%: %msg%";

    if (useConsole ) {
        LogChannel* ch = new LogCHSerial();
        chList.PushBack(ch);
        ch->setFormat(formatSrt);
    }
}


LogFactory::~LogFactory() { }

LogChannel* LogFactory::addChannel(LogChannel* channel) {
    chList.PushBack(channel);
    return channel;
}



int LogFactory::delChannel(const char* chName) {
    LinkedList::iterator it = chList.begin();
    while (it.Valid()) {
        if (strcmp(it.Item()->getName(),chName) == 0) {
            chList.Remove(it);
            return 1;
        }
        ++it;
	}
    return 0;
}


    // // int setChannelOFF(const char* chName);
    // // int setChannelON(const char* chName);


void LogFactory::setChannelFormat(const char* chName, const char* formatString) {
    LinkedList::iterator it = chList.begin();
    while (it.Valid()) {
        if (strcmp(it.Item()->getName(),chName) == 0) {
            it.Item()->setFormat(formatString);
            return;
        }
        ++it;
	}
}

void LogFactory::addClass(const char* clName) {
    LinkedList::iterator it = chList.begin();
    while (it.Valid()) {
        it.Item()->addClass(clName);
        ++it;
	}
}

int LogFactory::print(const char* clName, LogLevel level, const char* message) {
    int ret = 0;
    LinkedList::iterator it = chList.begin();
    while (it.Valid()) {
        if ( it.Item()->print(clName,level,message) >= 0) ret++;
        ++it;
	}
    return ret;
}



LogFactory logFactory;
