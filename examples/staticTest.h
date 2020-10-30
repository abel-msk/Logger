#ifndef __STATIC_TEST__
#define __STATIC_TEST__
// #include <string.h>
#include <string>


class staticTest {
    public:
    const char* str;
    staticTest(const char* n) : str(strdup(n)){};
    const char* getText();
};


#endif