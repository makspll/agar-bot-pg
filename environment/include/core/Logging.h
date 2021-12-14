#ifndef LOGGING_H
#define LOGGING_H
#include <string>
#include <iostream>
#include <ctime>
#include "core/IPrintable.h"



enum class LOG_LEVEL{
    DEBUG = 0,
    INFO = 1,
    ERROR = 2,
};

class Logging {
    public:

    static LOG_LEVEL GLOBAL_LOG_LEVEL;

    static void LOG(LOG_LEVEL level, std::string msg);

    static void LOG(LOG_LEVEL level,const IPrintable& to_print);
};

#endif // LOGGING_H