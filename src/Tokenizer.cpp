#include "Tokenizer.h"
#include <stdlib.h> 
#include <string.h> 

/**
 **/
char* copyTokPart(char *token,int len){
    if (token == 0) {
        token = (char *)malloc(len+2);
    }
    else {
        token = (char *)realloc(token, len);
    }
    return token;
}

Tokenizer::Tokenizer (const char* src) {
    counter=0;
    source = src;
    len = strlen(src);
}

void Tokenizer::reset() {
    counter = 0;
}

char Tokenizer::getChar() {
    if ( counter == len) return 0;
    char c = source[counter];
    counter++;
    return c;
}

const char *Tokenizer::getToken(char delim) {
    char c;
    int tokLastBegin = 0;
    char* token = 0;
    int offset = 0;
    char* start = (char*)source+counter;
    char* srcLastBegin = (char*)source+counter;

    while((c=getChar()) != 0) {
              
        if(c==delim) {
            char sc=getChar();
            if (sc != delim ) {  // sequental %% mean plain char % not a delimiter
                if (sc!=0)putBack();
                //putBack();
                if ((token = copyTokPart(token,tokLastBegin+offset+1)) == 0) { return 0;}
                strncpy(token+tokLastBegin,srcLastBegin,offset);
                token[tokLastBegin+offset] = 0;
                return token;
            }
            else // we got %% replace with one % and treat as regular symbol
            {
                if ((token = copyTokPart(token,tokLastBegin+offset+2)) == 0) { return 0;}
                strncpy(token+tokLastBegin,srcLastBegin,offset);
                offset++;
                token[tokLastBegin+offset-1]=delim;
                token[tokLastBegin+offset]=0;
                srcLastBegin = start + counter; // shift pointer to next part of token after double delim chars
                tokLastBegin += offset;
                offset = 0;
            }
        }
        else {
             offset++;
        }
    }
    //  We got finish od source& append last part of token.

    if ( offset > 0) {
        if ((token = copyTokPart(token,tokLastBegin+offset+1)) == 0) { return 0;}
        strncpy(token+tokLastBegin,srcLastBegin,offset);
        token[tokLastBegin+offset]=0;
    }

    return token;
}



bool Tokenizer::isParseComplete() {
    return counter == len;
}

void Tokenizer::putBack() {
    if ( counter > 0 ) counter--;
}
void Tokenizer::skipNext() {
    counter++;
}
