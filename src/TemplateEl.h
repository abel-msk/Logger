#ifndef TEMPLATE_EL_H_
#define TEMPLATE_EL_H_

enum tokenType {
    PLAIN=0,
    DATE,
    CLNAME,
    LEVEL,
    MSG
};

#define VAL_ALIGN_LEFT 1
#define VAL_ALIGN_RIGHT 2
#define VAL_ALIGN_CENTER 3


class TemplateEl {
    private:
    char* text;
    tokenType type;
    int align;
    unsigned int maxLen;

    public:
    TemplateEl();
    TemplateEl(tokenType t, const char* txt);
    ~TemplateEl();
    void setType(tokenType t);
    tokenType getType();
    void setText(const char* txt);
    char* getText();
    void setAlign(int);
    void setLength(int);
    char* getReplaced(const char* subst);
};

#endif