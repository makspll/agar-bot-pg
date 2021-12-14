#include "core/Logging.h"

LOG_LEVEL Logging::GLOBAL_LOG_LEVEL = LOG_LEVEL::ERROR;

void Logging::LOG(LOG_LEVEL level, std::string msg){
    if((int)level <  (int)GLOBAL_LOG_LEVEL)
        return;

    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);

    auto stream = &std::cout;
    if(level == LOG_LEVEL::ERROR){
        stream = &std::cerr;
    }

    *stream << '(' << now->tm_hour <<':' <<now->tm_min << ':' << now->tm_sec << ") " << msg << '\n';
}

void Logging::LOG(LOG_LEVEL level,const IPrintable& to_print){
    if((int)level <  (int)GLOBAL_LOG_LEVEL)
        return;

    std::time_t t = std::time(0); // get time now
    std::tm* now = std::localtime(&t);

    auto stream = &std::cout;
    if(level == LOG_LEVEL::ERROR){
        stream = &std::cerr;
    }

    *stream << '(' << now->tm_hour <<':' <<now->tm_min << ':' << now->tm_sec << ") ";
    to_print.print(*stream);
    *stream << '\n';
}
