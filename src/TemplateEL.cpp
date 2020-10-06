#include "TemplateEl.h"
#include <stdlib.h>
#include <string.h>

#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_AVR)
#include <WString.h>
#else
#include <string>
typedef std::string String;
#endif

TemplateEl::TemplateEl() {
    text = strdup("");    

    type = PLAIN;
    align = VAL_ALIGN_LEFT;
    maxLen=0;
}

TemplateEl::TemplateEl(tokenType t, const char* txt) {
    type = t;
    text = strdup(txt);
    align = VAL_ALIGN_LEFT;

}

TemplateEl::~TemplateEl() {
    delete text;
}

void TemplateEl::setType(tokenType t) {
    type = t;
}

tokenType TemplateEl::getType() {
    return type;
}

void TemplateEl::setText(const char* txt) {
    text = strdup(txt);
}

const char* TemplateEl::getText() {
    return strdup(text);
}

void TemplateEl::setAlign(int al) {
    align = al;
}

void TemplateEl::setLength(int l) {
    maxLen = l;
}

/*
    Format input string according to placeholder size and allign
    if input string less than declared for field, cut teh input string
    otherway add required spaces at the end for left aligning, at begining for right aligning 
    or around for center aligning

    @param subst - input string
    @return new allocated char buff

*/
const char* TemplateEl::getReplaced(const char* subst) {
    unsigned int substLen = strlen(subst);
    char *resBuff;
    const char* source = subst;
    unsigned int i;
    
    if ((maxLen == 0 ) || (maxLen == substLen)) {
        if ( (resBuff=(char*)malloc(substLen+1)) == 0) return nullptr;
        strncpy(resBuff,source,substLen);
        maxLen = substLen;
    }
    else {
        if ( (resBuff=(char*)malloc(maxLen)) == 0) return nullptr;

        if (( substLen > maxLen) && (align == VAL_ALIGN_LEFT)) {  // cut right part of string
            strncpy(resBuff,source,maxLen);
        }
        else if (( substLen > maxLen) && (align == VAL_ALIGN_RIGHT)) { // cut left part of string
            strncpy(resBuff,source+substLen-maxLen,maxLen);
        }
        else if (( substLen < maxLen) && (align == VAL_ALIGN_LEFT)) { 
            strncpy(resBuff,source,substLen);
            for (i = substLen ; i < maxLen; i++) resBuff[i] = ' ';
        }
        else if (( substLen < maxLen) && (align == VAL_ALIGN_RIGHT)) { 
            for (i = 0; i < (maxLen-substLen); i++) resBuff[i] = ' ';
            strncpy(resBuff+i,source,substLen);
        }
        else if (( substLen < maxLen) && (align == VAL_ALIGN_CENTER)) { 
            for (i = 0; i < (maxLen-substLen)/2; i++) resBuff[i] = '.';
            strncpy(resBuff+i,source,substLen);
            i+=substLen;
            for (; i < maxLen; i++) resBuff[i] = '.';
        } 
        else {
            strncpy(resBuff,source,substLen);
        }  
    }
    resBuff[maxLen] = 0;
    return resBuff;
}
