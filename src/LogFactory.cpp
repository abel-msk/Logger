#include "LogFactory.h"


LogFactory::LogFactory() {
    formatSrt = "%date% [%level%] %clname%: %msg%";
    logFormat = new LogFormat(formatSrt);
    logFilter = new LogFilter();
}

LogFactory::~LogFactory() {
    delete logFormat;
    delete logFilter;
}
void LogFactory::setFormatString(String fmtstr) {
    setFormatString(fmtstr.c_str());
}

void LogFactory::setFormatString(const char* fmtstr) {
    formatSrt = fmtstr;
    delete logFormat;
    logFormat = new LogFormat(formatSrt);
}

LogFormat* LogFactory::getFormater() {return logFormat;}
LogFilter* LogFactory::getFilter() {return logFilter;};

LogFactory logFactory;
