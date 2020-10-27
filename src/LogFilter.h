#ifndef LOG_FILTER_H_
#define LOG_FILTER_H_

#include <string.h>
#include "LogFormat.h"
#include "BinaryTree.h"
#include "LogLevels.h"


#ifdef USE_REGEXP_SEARCH
#include <Regexp.h>
#endif



/**
 *   Class for represent compaund key as sum of class name and log lavel
 */
class compaundTreeKey {
    public:
    LogLevel level;
    const char* name;
    compaundTreeKey(const char* n = "", LogLevel l = LogLevel::L_INFO) 
        {
            name = strdup(n);
            level = l; 
        };
    ~compaundTreeKey() { delete name; };
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
    FilterEL(bool value) : show(value) {}
};


/**
 * 
 *  Contains list of elements describing show or hide 
 *  property for class name and log level facility.
 *  Perform operation for retrieving or set show property
 * 
 */
class  LogFilter {
    private:
    BinaryTree<compaundTreeKey*,FilterEL*,compaundKeyCmp> filterStore;
    LogLevels levelDefaults;

    // compaundTreeKey* tmpKey;  // allocated class for faster key creaton on run.
    public:
    // LogFilter();
    // ~LogFilter();
    void addClass(const char* clName);
    bool isExist(const char* clName, LogLevel level);
    void setFilter(const char* className, LogLevel level, bool value = true);
    void setFilterEX(const char* className, LogLevel leve, bool value = true);
    bool isShow(const char* className, LogLevel level);
    void setDefault(LogLevel level, bool value);
    // bool getDefView(LogLevel level);
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


#endif