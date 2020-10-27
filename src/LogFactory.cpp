#include "LogFactory.h"

/**
 * Main class for controll logging classes channels, formatting etc.
 * 
 */
LogFactory::LogFactory(bool useConsole) {
    formatSrt = "%date% [%level%] %clname%: %msg%";

    if (useConsole ) {
        LogChannel* ch = new LogCHSerial();
        chList.PushBack(ch);
        ch->setFormat(formatSrt);
    }
}

LogFactory::~LogFactory() { }

/**
 * Add new channel to stack
 * @param Channel class 
 */
LogChannel* LogFactory::addChannel(LogChannel* channel) {
    chList.PushBack(channel);
    return channel;
}


/**
 * 
 * Remove channel from stack
 * @param chName Channel name
 * @return
 *      0 - Channel removed
 *      1 - Channel not found
 */
int LogFactory::delChannel(const char* chName) {
    LinkedList::iterator it = chList.begin();
    while (it.Valid()) {
        if (strcmp(it.Item()->getName(),chName) == 0) {
            chList.Remove(it);
            return 0;
        }
        ++it;
	}
    return 1;
}

/**
 *  Set channel output format string
 *  @param chNname channel name
 *  @param formatString format string. See @link LogFormat
 */
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

/**
 * Add class to all channels with default show values
 * @param clName  class name
 * 
 */
void LogFactory::addClass(const char* clName) {
    LinkedList::iterator it = chList.begin();
    while (it.Valid()) {
        it.Item()->addClass(clName);
        ++it;
	}
}

/**
 * Print out message in each channel (depend of filter settings)
 * @param clName class name 
 * @param LogLevel log level
 * @param message message to print
 * 
 */
int LogFactory::print(const char* clName, LogLevel level, const char* message) {
    int ret = 0;
    LinkedList::iterator it = chList.begin();
    while (it.Valid()) {
        if ( it.Item()->print(clName,level,message) >= 0) ret++;
        ++it;
	}
    return ret;
}

/**
 * Set filter on specifyed channel
 * @param chName the channel name
 * @param clName class name 
 * @param LogLevel log level
 * @param isShow fiter value. True - print mrssage in channel; False - do not print out
 * @return 
 *      1 - channel no found
 *      0 - filter was set
 */
int LogFactory::setChFilter(const char* chName, const char* clName, LogLevel level, bool isShow ) {
    LinkedList::iterator it = chList.begin();
    while (it.Valid()) {
        if (strcmp(it.Item()->getName(),chName) == 0) {
            it.Item()->setFilter(clName,level,isShow);
            return 0;
        }
        ++it;
	}
    return 1;
}

/**
 *   Set filter in each channel
 * @param clName class name 
 * @param LogLevel log level
 * @param isShow fiter value. True - print mrssage in channel; False - do not print out
 * 
 */
void LogFactory::setFilter(const char* clName, LogLevel level, bool isShow ) {
    LinkedList::iterator it = chList.begin();
    while (it.Valid()) {
        it.Item()->setFilter(clName,level,isShow);
        ++it;
    }
    return;
}


void LogFactory::setDefaultFilter(LogLevel level, bool value) {
    LinkedList::iterator it = chList.begin();
    while (it.Valid()) {
        it.Item()->getFilterObject()->setDefault(level,value);
        ++it;
    }
    return;
}




LogFactory logFactory;
