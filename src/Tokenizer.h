
#ifndef TOKENIZER_H_
#define TOKENIZER_H_

/**
 * Devide string by tokens.  Toekns devided by % sign
 * double percent symmols treat as single and does not split tokens
 */
class Tokenizer {
    private:
    const char* source;
    int counter;
    int len;
    
    public:
    Tokenizer(const char* src);
    char getChar(); 
    const char *getToken(char);
    void putBack();
    void reset();
    void skipNext();
    bool isParseComplete();
};

#endif
