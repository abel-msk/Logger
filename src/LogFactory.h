#ifndef LOG_FACTRY_H_
#define LOG_FACTRY_H_
#include "LogFormat.h"
#include "LogFilter.h"

class LogFactory {
    private:
    LogFormat* logFormat;
    LogFilter* logFilter;
    const char* formatSrt;

    public:
    LogFactory();
    ~LogFactory();
    void setFormatString(const char* fmtstr);
    void setFormatString(String fmtstr);
    LogFormat* getFormater();
    LogFilter* getFilter();
};

extern LogFactory logFactory;

#endif