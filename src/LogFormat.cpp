#include "LogFormat.h"
// #include "FElement.h"


const char* level2str(LogLevel level) {
    switch (level) {
    case INFO: 
        return LOG_LEVEL_INFO;
    case DEBUG:
        return LOG_LEVEL_DEBUG;
    case ERROR:
        return LOG_LEVEL_ERROR;
    default:
        return "";
    }
}

char* appendStr(char* buff, const char* token) {
    unsigned int bLen = strlen(buff);
    unsigned int tLen  = strlen(token);

    if ((buff != NULL) && (tLen > 0 )) {
        if ((bLen + tLen + 1) >  LOGSTR_BUFF_LEN) {
            buff = (char*)realloc(buff,bLen+tLen+1);
        }
        if (buff != NULL) {
            strncpy(buff+bLen,token,tLen);
            buff[bLen+tLen]=0;
        }
    }
    return buff;
}

int LogFormat::hasError() {
    return parseErr;
}

/**
 *   When initilized, analyze template string.
 *   Extract constant part and special tokens (variables) witch will be replaced with variable.
 * 
*    @param fmt message template
 */
LogFormat::LogFormat(String fmt) {
    Tokenizer tok(fmt.c_str());
    const char* text;
    TemplateEl* el;
    parseErr = 0;

    while ((text = tok.getToken(STR_TOK_DELIM)) != 0 ) {
        
        if (strlen(text) > 0 ) {
            if ( strncmp(text,STR_TOK_DATE,strlen(STR_TOK_DATE)) == 0) {
                if ((el = getAttr(text,STR_TOK_DATE)) != 0)
                    el->setType(DATE);
            } else  if (strncmp(text,STR_TOK_CLNAME,strlen(STR_TOK_CLNAME))==0) {
                if ((el = getAttr(text,STR_TOK_CLNAME)) != 0) 
                    el->setType(CLNAME);
            } else if (strncmp(text,STR_TOK_LEVEL,strlen(STR_TOK_LEVEL))==0) {
                if ((el = getAttr(text,STR_TOK_LEVEL)) != 0)
                    el->setType(LEVEL);
            } else if (strncmp(text,STR_TOK_MSG,strlen(STR_TOK_MSG))==0) {
                if ((el = getAttr(text,STR_TOK_MSG)) != 0)
                    el->setType(MSG);
            }
            else {
                el = new TemplateEl(PLAIN,text);
            }
            tokensList.push_back(el);
        }
        free((void*)text);
    }
}

/**
* Check attributes as trailing symbols int the token string 
* Token has format 'token_name[L|R|C][0-9]*'
* where: 
*       [L|R|C]  is a allign attributes (optional) default L - left
*       [0-9]*  - max length of field in output (optional) 
* @param token The full text of token with attributes char
* @param key ley part of token
* @return 
*       new TemplateEl object with defined allign and max length
*       0 - Incorrect format ans set  parseErr to error num.
*   
*/
TemplateEl* LogFormat::getAttr(const char* token, const char* key) {
    TemplateEl* el = new TemplateEl();
    char* attr = (char *)token+strlen(key);

    //  Got end of string return default values
    if ( *attr == '0') {
        el->setAlign(VAL_ALIGN_LEFT);
        el->setLength(0);
        return el;
    }
    //  The char point to alligning
    if (*attr == ALLIGN_LEFT_DEF ) {
        el->setAlign(VAL_ALIGN_LEFT);
        attr++;
    }
    else if (*attr == ALLIGN_RIGH_DEF ) {
        el->setAlign(VAL_ALIGN_RIGHT);
        attr++;
    }
    else if (*attr == ALLIGN_CENTER_DEF ) {
        el->setAlign(VAL_ALIGN_CENTER);
        attr++;
    }
    else if ((*attr < '0') ||  (*attr > '9')) {
        parseErr = PARSE_ERR_INCORRECT_SIZE_ATTR;
        return 0; //incorrect format
    }
    
    if ( *attr == '0') {
        el->setLength(0);
        return el;
    }
    //  The digits set max field length
    for (int offset = 0; attr[offset-1] != '0'; offset++) {
        if ((attr[offset-1] < '0') || (attr[offset-1] > '9')) {
            parseErr = PARSE_ERR_INCORRECT_ALLIGN_ATTR;
            return 0;  //incorrect format
        }
    }
    
    #ifdef ARDUINO_ARCH
        String lenStr(attr);
        el->setLength(lenStr.toInt());
    #else 
        el->setLength(std::stoi(attr));
    #endif 

    return el;
}

const char* LogFormat::parse(const char* date, const char*  className, LogLevel level, const char*  msg) 
{
    return parse(date, className, level2str(level), msg);
}

const char*  LogFormat::parse(const char* date, const char*  className, const char* level, const char*  msg) 
{
    char* buff = (char*)malloc(LOGSTR_BUFF_LEN);
    buff[0] = 0;
    bool isMSGInserted = false;
    TemplateEl* el;
    tokensList.reset();
    
    while ((el=tokensList.getNext()) != 0) { 
        
            switch (el->getType()) {
                case PLAIN: 
                    buff = appendStr(buff,el->getText());
                    break;
                case DATE: 
                    buff = appendStr(buff,el->getReplaced(date));
                    break;
                case CLNAME: 
                    buff = appendStr(buff,el->getReplaced(className));
                    break;
                case LEVEL:
                    buff = appendStr(buff,el->getReplaced(level));
                    break;
                case MSG:
                    isMSGInserted = true;
                    buff = appendStr(buff,msg);   
            }
            if ( buff == NULL)  return NULL;
        }
        if (!isMSGInserted) {
            buff = appendStr(buff,msg); 
        }
    return buff;
}







