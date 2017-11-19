#ifndef __Q2_TALLYVOTES_H__
#define __Q2_TALLYVOTES_H__

#include "MPRNG.h"

#if defined(IMPLTYPE_SEM )           // semaphore solution
    #include <uSemaphore.h>
#endif


#if defined(IMPLTYPE_BAR )           // barrier solution
    #include <uBarrier.h>
#endif


using namespace std;

_Monitor Printer;

#if defined( IMPLTYPE_MC )              // mutex/condition solution
// includes for this kind of vote-tallier
class TallyVotes {
    // private declarations for this kind of vote-tallier
    uOwnerLock mlk;
    uCondLock waiting_bench;
    uCondLock barging_bench;
    bool in_use;
#elif defined( IMPLTYPE_BAR )           // barrier solution
// includes for this kind of vote-tallier
_Cormonitor TallyVotes : public uBarrier {
    // private declarations for this kind of vote-tallier
#elif defined( IMPLTYPE_SEM )           // semaphore solution
// includes for this kind of vote-tallier
class TallyVotes {
    // private declarations for this kind of vote-tallier
    uSemaphore mlk;
    uSemaphore barging_bench;
    uSemaphore waiting_bench;
    bool in_use;
    int barging_count;
#else
    #error unsupported voter type
#endif
    // common declarations
    unsigned int group;
    unsigned int picture;
    unsigned int statue;
    unsigned int giftshop;
    unsigned int waiting;               // how many are waiting, this gets increamented when one has voted and blocked
    int result;
    Printer &printer;

  public:                               // common interface
    
        #if defined( IMPLTYPE_MC )
            TallyVotes( unsigned int group, Printer & printer ): in_use(false), group(group), picture(0), statue(0), giftshop(0), waiting(0), printer(printer) {}

        #elif defined( IMPLTYPE_BAR )
            TallyVotes( unsigned int group, Printer & printer ): uBarrier(group), group(group), picture(0), statue(0), giftshop(0), waiting(0), printer(printer) {}

        #elif defined (IMPLTYPE_SEM)
            TallyVotes( unsigned int group, Printer & printer ): in_use(false), group(group), picture(0), statue(0), giftshop(0), waiting(0), printer(printer) {
                // Initialize both of them to be locked at the beginning
                waiting_bench.P();
            }

        #else
            #error unsupported voter type
        #endif

    struct Ballot { unsigned int picture, statue, giftshop; };
    enum Tour { Picture = 'p', Statue = 's', GiftShop = 'g' };
    Tour vote( unsigned int id, Ballot ballot );
};


#endif