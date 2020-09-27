#ifndef LOG_FILTER_H_
#define LOG_FILTER_H_

#include <string>
#include "LogFormat.h"
#include "BinaryTree.h"

#ifdef USE_REGEXP_SEARCH
#include <Regexp.h>
#endif

#define DEF_LOG_PRINT_DEBUG false
#define DEF_LOG_PRINT_INFO false
#define DEF_LOG_PRINT_ERROR true

/**
 *   Class for represent compaund key as sum of class name and log lavel
 */
class compaundTreeKey {
    public:
    LogLevel level;
    const char* name;
    compaundTreeKey(const char* n =  NULL, LogLevel l = LogLevel::INFO) {name = n; level =l; };
};

/**
 *   Class for perform comparison BinaryTree nodes.
 */
struct compaundKeyCmp {
    int compare(compaundTreeKey* thisKey, compaundTreeKey* otherKey) {
        int strcmpres = 0;
        if ( (strcmpres = strcmp(thisKey->name,otherKey->name)) == 0 )  {
            return thisKey->level - otherKey->level;
        }
        return strcmpres;
    }
};

/**
 *  Node data for element in filter tree.  
 */
struct FilterEL {
    bool show;
};

extern  BinaryTree<compaundTreeKey*,FilterEL*,compaundKeyCmp> filterStore;

/**
 * 
 *  Contains list of elements describing show or hide 
 *  property for class name and log level facility.
 *  Perform operation for retrieving or set show property
 * 
 */
class  LogFilter {
    private:

    compaundTreeKey* tmpKey;  // allocated class for faset key creaton on run.
    public:
    LogFilter();
    void setShow(const char* className, LogLevel level);
    void setShowEX(const char* className, LogLevel leve);
    void setHide(const char* className, LogLevel level);
    void setHideEX(const char* className, LogLevel level);
    bool isShow(const char* className, LogLevel level);
    bool getDefView(LogLevel level);
    // classList getClassList();
};

/**
 *  Handle action for trees elements whrn we search multiple elements
 */
struct filterListAction: public BinaryTreeAction<compaundTreeKey*,FilterEL*> {
    private:
    bool setShow;
    LogLevel level;
    const char* reg_expr;

    public:
    filterListAction(const char* expr, LogLevel l, bool isShow = true);
    void run(compaundTreeKey* key, FilterEL* cl);
};


extern LogFilter logFilter;

#endif