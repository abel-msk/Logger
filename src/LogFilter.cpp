#include "LogFilter.h"


/**
 *  Initialize LogFilter class
 */
// LogFilter::LogFilter() {
// }

// LogFilter::~LogFilter() {
// }

/**
 *   Action class used for setup found nodes (found during passing the tree).
 *   Node is found when match class name regexp expression and log level
 *  @param expr regular expression for matchin class name part of key
 *  @param lvl log level
 *  @param isShow set found node to that value 
 */


filterListAction::filterListAction(const char* expr, LogLevel lvl, bool isShow)  {
    reg_expr = expr; level = lvl; setShow = isShow;
}

/**
 *   Called for each node in tree. 
 *   @param key node key
 *   @param cl the data element
 */
void filterListAction::run(compaundTreeKey* key, FilterEL* cl) {
    if (key->level == level) {

#ifdef USE_REGEXP_SEARCH        
        MatchState ms;
        ms.Target((char *)key->name);
        // Match  class name by regexp
        char result = ms.Match (reg_expr);
        if (result > 0) 
#endif        
        {  
            cl->show = setShow;

        }
    }
}

bool LogFilter::isExist(const char* clName, LogLevel level) {
    compaundTreeKey tmpKey(clName,level);
    // tmpKey->level = level;
    // tmpKey->name = clName;
    return filterStore.getByKey(&tmpKey) != NULL;
}



/**
 *  Find node with class name and level. Set filter value 
 *  true - for show message, false for dont show.
 *  @param className - class name 
 *  @param level - log level
 */
void LogFilter::setFilter(const char* clName, LogLevel level, bool value) {
    compaundTreeKey tmpKey(clName,level);
    // tmpKey->level = level;
    // tmpKey->name = clName;
    FilterEL* el = filterStore.getByKey(&tmpKey);
    if ( el == NULL) {
        compaundTreeKey* newKey = new compaundTreeKey(clName,level);
        filterStore.add(newKey,new FilterEL(value));
    }
    else {
        el->show=value;
    }
}

/**
 *  Find node by regexp compare with class name and level. Set filter value 
 *  true - for show message, false for dont show.
 *  @param className - class name 
 *  @param level - log level
 */
void LogFilter::setFilterEX(const char* classNameRX, LogLevel level, bool value) {
    filterListAction fla(classNameRX,level, true);
    filterStore.inOrderTraversal(&fla);
}

/**
 * Select node with defined class name ans defined level
 * 
 * @param className  Class name
 * @param level Log level
 * @return TRUE if selected class name with level  is market for show
 */
bool LogFilter::isShow(const char* clName, LogLevel level) {
    compaundTreeKey tmpKey(clName,level);
    // tmpKey->level = level;
    // tmpKey->name = className;
    FilterEL* el = filterStore.getByKey(&tmpKey);

    if (el == 0 ) return getDefView(level);;
    return el->show;
}

/**
 *   Retrun show value for default class and level.  
 *   Default mean not presented in filter list
 *   @param level get default value for level
 *   @return 
 *       TRUE - show log message
 *       FALSE - do not show
 */
bool LogFilter::getDefView(LogLevel level) {
    switch (level) {
        case INFO: 
        return DEF_LOG_PRINT_INFO;
        case DEBUG:
        return DEF_LOG_PRINT_DEBUG;
        case ERROR:
        return DEF_LOG_PRINT_ERROR;
        case WARNING:
        return DEF_LOG_PRINT_WARN;        
        default:
        return false;
    }
}


LogFilter logFilter;
