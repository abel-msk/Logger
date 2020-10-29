#ifndef LOG_FORMAT_H_
#define LOG_FORMAT_H_

#include <stdlib.h>
#include "TemplateEl.h"
#include "SLinkedList.h"
#include "Tokenizer.h"
#include "LogLevels.h"


#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_AVR)
#define ARDUINO_ARCH
#include <WString.h>
#else
#include <string>
typedef std::string String;
#endif

#define STR_TOK_DATE "date"
#define STR_TOK_CLNAME "clname"
#define STR_TOK_LEVEL "level"
#define STR_TOK_MSG "msg"
#define STR_TOK_DELIM '%'

#define ALLIGN_LEFT_DEF 'L'
#define ALLIGN_RIGH_DEF 'R'
#define ALLIGN_CENTER_DEF 'C'

#define PARSE_ERR_INCORRECT_SIZE_ATTR 1
#define PARSE_ERR_INCORRECT_ALLIGN_ATTR 2
#define PARSE_ERR_INCORRECT_TOK_KEY 3

#define LOGSTR_BUFF_LEN 256 


/**
 * When initilized, analyze template string.
 * Extract constant part and special tokens witch will be replaced with values 
 * like log level timestamp etc.
 *   
 * Variables are market with '%' (percent sigh) on both ends.
 * The format of variable definig is:
 * %name<L|R|C><d*>%
 * where 
 *       name - variable name
 *       <L|R|C> - Optional.  Aligning value in string
 *       <d*>    - Оptional decimal number. exact size of fields where value will be inserted
 * 
 * Known токены names  are:
 *     %date%    - will be replaced with time stamp
 *     %clname%  - class name
 *     %level%   - log level  ERROR DEBUG INFO
 *     %msg%     -  logging message
 *   
 * use %% (doubke percent) for masking percent symbol.
 * 
 * Method parse, compile final log  message  in according to format string
 */
class LogFormat {
    private:
    // const char* format;
    SLinkedList<TemplateEl*> tokensList;
    int parseErr;
    TemplateEl* getAttr(tokenType tType, const char* token, const char* key);

    public:
    LogFormat();
    int hasError();
    int parseFmt(String fmt);
    char* compile(char* date, char* name, LogLevel level, const char* msg);
    char* compile(char* date, char* name, const char* level, const char* msg);
};


#endif