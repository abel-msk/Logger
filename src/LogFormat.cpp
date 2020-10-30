#include "LogFormat.h"
#include "LogLevels.h"
#ifdef __DEBUG_ESP_MEM_LEAK__
#include <esp.h>
#endif
#ifdef __DEBUG_X64_GCC___
#include <iostream>
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

    #ifdef __DEBUG_X64_GCC___
    std::cout << "LogFormat:parseFmt.  Format=" <<  fmt << std::endl;
    #endif
    Tokenizer tok(fmt.c_str());
    const char* text;
    TemplateEl* el;
    parseErr = 0;

    while ((text = tok.getToken(STR_TOK_DELIM)) != 0 ) {
        #ifdef __DEBUG_X64_GCC___
        std::cout << "LogFormat:parseFmt.  Token=" <<  text << std::endl;
        #endif
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
    
    #ifdef __DEBUG_X64_GCC___
    std::cout << "LogFormat:getAttr. Token="<< token  <<", Key=" <<  key << std::endl;
    #endif
    parseErr = 0;
    TemplateEl* el = new TemplateEl();
    el->setType(tType);
    char* attr = (char *)token+strlen(key);
    el->setLength(0);
    el->setAllign(VAL_ALLIGN_LEFT);

    //  Got end of string return default values
    if ( *attr != '\x00') {
        //  Got char addition char, check allign 
        if (*attr == ALLIGN_LEFT_DEF ) {
            el->setAllign(VAL_ALLIGN_LEFT);
            attr++;
        }
        else if (*attr == ALLIGN_RIGH_DEF ) {
            el->setAllign(VAL_ALLIGN_RIGHT);
            attr++;
        }
        else if (*attr == ALLIGN_CENTER_DEF ) {
            el->setAllign(VAL_ALLIGN_CENTER);
            attr++;
        }
    }

    if (( *attr != '\x00') && ((*attr < '0') ||  (*attr > '9'))) {
        #ifdef __DEBUG_X64_GCC___
        std::cout << "LogFormat:getAttr. Check length="<< *attr  << std::endl;
        #endif
        parseErr = PARSE_ERR_INCORRECT_ALLIGN_ATTR;
    } else if (*attr != '\x00'){

        //  The digits set max field length
        for (int offset = 0; attr[offset] != '\x00'; offset++) {
            #ifdef __DEBUG_X64_GCC___
            std::cout << "LogFormat:getAttr. Got Char="<< attr[offset] << std::endl;
            #endif
            if ((attr[offset] < '0') || (attr[offset] > '9')) {
                parseErr = PARSE_ERR_INCORRECT_SIZE_ATTR;
                break;
            }
        }
        if (parseErr == 0 ) {
            #ifdef ARDUINO_ARCH
            String lenStr(attr);
            el->setLength(lenStr.toInt());
            #else 
            el->setLength(std::stoi(attr));
            #endif 
        }

    }
          
    #ifdef __DEBUG_X64_GCC___
    std::cout << "LogFormat:getAttr. Attr allign="<< el->getAllign()  << ", Length=" << el->getLength() << std::endl;
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
                    buff = appendStr(buff,el->getText());
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







