#ifndef __AUTOMATIC_SIGNAL_H__
#define __AUTOMATIC_SIGNAL_H__

#define AUTOMATIC_SIGNAL uCondition bench;

#define WAITUNTIL(pred, before, after) \
    if (!pred) {\
        before;\
        while(!bench.empty()) {\
            bench.signal();\
        }\
        do {\
            bench.wait();\
        }\
        while(!pred);\
        after;\
    }

#define RETURN(expr...) \
    while(! bench.empty()) {\
        bench.signal();\
    }\
    return expr;
#endif