#ifndef DEBUGLOG_H
#define DEBUGLOG_H

//#define DEBUG

#ifdef DEBUG
    #include <iostream>
    #define DEBUG_LOG(message) std::cout << "LOG: " << message << std::endl;
#else
    #define DEBUG_LOG(message)
#endif

#endif // DEBUGLOG_H
