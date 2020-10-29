#include "LogFormat.h"
#include "LogLevels.h"
#ifdef __DEBUG_ESP_MEM_LEAK__
#include <esp.h>
#endif
// #include "FElement.h"




char* appendStr(char* buff, const char* token) {

    unsigned int bLen = strlen(buff);
    unsigned int tLen  = strlen(token);

    if ((buff != NULL) && (tLen > 0 )) {
        if ((bLen + tLen + 1) >  LOGSTR_BUFF_LEN) {
            buff = (char*)realloc((void*)buff,bLen+tLen+1);
        }
        if (buff != NULL) {
            //strcat(buff,token);
            strncpy(buff+bLen,token,tLen);
            buff[bLen+tLen]=0;
        }
    }

    return buff;
}


LogFormat::LogFormat() {
    parseErr = 0;

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
int LogFormat::parseFmt(String fmt) {
    Tokenizer tok(fmt.c_str());
    const char* text;
    TemplateEl* el;
    parseErr = 0;

    while ((text = tok.getToken(STR_TOK_DELIM)) != 0 ) {
        
        if (strlen(text) > 0 ) {
            if ( strncmp(text,STR_TOK_DATE,strlen(STR_TOK_DATE)) == 0) {
                el = getAttr(DATE,text,STR_TOK_DATE);

            } else  if (strncmp(text,STR_TOK_CLNAME,strlen(STR_TOK_CLNAME))==0) {
                el = getAttr(CLNAME,text,STR_TOK_CLNAME);

            } else if (strncmp(text,STR_TOK_LEVEL,strlen(STR_TOK_LEVEL))==0) {
                el = getAttr(LEVEL,text,STR_TOK_LEVEL);

            } else if (strncmp(text,STR_TOK_MSG,strlen(STR_TOK_MSG))==0) {
                el = getAttr(MSG,text,STR_TOK_MSG);
            }
            else {
                el = new TemplateEl(PLAIN,text);
            }

            // assert((el==0)&&"Format string pasing error.");
            if ( el == 0 ) return parseErr==0?-1:parseErr;
            tokensList.PushBack(el);
        }
        free((void*)text);
    }

    return parseErr;
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
TemplateEl* LogFormat::getAttr(tokenType tType, const char* token, const char* key) {
    TemplateEl* el = new TemplateEl();
    el->setType(tType);
    char* attr = (char *)token+strlen(key);

    //  Got end of string return default values
    if ( *attr == 0) {
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

char* LogFormat::compile( char* date,  char*  className, LogLevel level, const char*  msg) 
{
    return compile(date, className, level2str(level), msg);
}

char*  LogFormat::compile( char* date,  char*  className, const char* level, const char*  msg) 
{
#ifdef __DEBUG_ESP_MEM_LEAK__    
    Serial.print(F("Free mem before compile = "));
    Serial.println(ESP.getFreeHeap());  
#endif

    char* buff = (char*)malloc(LOGSTR_BUFF_LEN);
    buff[0] = 0;

#ifdef __DEBUG_ESP_MEM_LEAK__ 
    Serial.print(F("Free mem after alloc = "));
    Serial.println(ESP.getFreeHeap());  
#endif

    //char buff [LOGSTR_BUFF_LEN];
    char* txt;

    bool isMSGInserted = false;
    TemplateEl* el;
    // tokensList.reset();
    SListIterator<TemplateEl*> it = tokensList.begin();

    while (it.Valid()) { 
            el = it.Item();

#ifdef __DEBUG_ESP_MEM_LEAK__ 
            Serial.print(F("Process template part type"));
            Serial.print(el->getType());
            Serial.print(F("text, mem before = "));
            Serial.println(ESP.getFreeHeap());  
#endif
            switch (el->getType()) {
                case PLAIN:  
                    txt =  el->getText();
                    buff = appendStr(buff,txt);
                    free((void*)txt); 
                    break;
                case DATE: 
                    txt = el->getReplaced(date);
                    buff = appendStr(buff,txt);                   
                    free((void*)txt);                    
                    break;
                case CLNAME: 
                    txt = el->getReplaced(className);
                    buff = appendStr(buff,txt);
                    free((void*)txt);
                    break;
                case LEVEL:
                    txt = el->getReplaced(level);
                    buff = appendStr(buff,txt);
                    free((void*)txt);                    
                    break;
                case MSG:
                    isMSGInserted = true;
                    buff = appendStr(buff,msg);   

            }
            if ( buff == NULL)  return NULL;
            
            ++it;

#ifdef __DEBUG_ESP_MEM_LEAK__ 
            Serial.print(F("Process template part type"));
            Serial.print(el->getType());
            Serial.print(F(", mem after = "));
            Serial.println(ESP.getFreeHeap());  

            Serial.print(F("Replace message2, mem before = "));
            Serial.println(ESP.getFreeHeap()); 
#endif
        }

        if (!isMSGInserted) {
            buff = appendStr(buff,msg); 
        }

#ifdef __DEBUG_ESP_MEM_LEAK__
    Serial.print(F("Replace message2, mem after = "));
    Serial.println(ESP.getFreeHeap());  

    Serial.print(F("Nonal log message len = "));
    Serial.println(strlen(buff));
    Serial.print(F("Free mem after compile = "));
    Serial.println(ESP.getFreeHeap());  
#endif     

    return buff;
}







