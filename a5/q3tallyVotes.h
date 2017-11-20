#ifndef __Q3_TALLYVOTES_H__
#define __Q3_TALLYVOTES_H__

#include "MPRNG.h"

using namespace std;

_Monitor Printer;

#if defined( IMPLTYPE_EXT )                  // external scheduling monitor solution
// includes for this kind of vote-tallier
_Monitor TallyVotes {                       
    // private declarations for this kind of vote-tallier
    // does not need anything as all we need is just _Accept to achieve external scheduling
#elif defined( IMPLTYPE_INT )                // internal scheduling monitor solution
    // includes for this kind of vote-tallier
_Monitor TallyVotes {
    uCondition vote_bench;

    // private declarations for this kind of vote-tallier
#elif defined( IMPLTYPE_INTB )               // internal scheduling monitor solution with barging
// includes for this kind of vote-tallier
_Monitor TallyVotes {
    // private declarations for this kind of vote-tallier
    uCondition bench;                        // only one condition variable (you may change the variable name)
    void wait();                             // barging version of wait
    void signalAll();                        // unblock all waiting tasks
    unsigned int arrive_id;                  // give each task an arrive_id by the order they arrive, the first guy has id 1, and second guys has id 2 and e.t.c
    unsigned int max_id_this_round;          // use this to prevent barging, only allow a guy to vote if its id smaller than this number

#elif defined( IMPLTYPE_AUTO )               // automatic-signal monitor solution
// includes for this kind of vote-tallier
_Monitor TallyVotes {
    // private declarations for this kind of vote-tallier
#elif defined( IMPLTYPE_TASK )               // internal/external scheduling task solution
_Task TallyVotes {
    // private declarations for this kind of vote-tallier
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
  public:                                    // common interface
    TallyVotes( unsigned int group, Printer & printer );
    struct Ballot { unsigned int picture, statue, giftshop; };
    enum Tour { Picture = 'p', Statue = 's', GiftShop = 'g' };
    Tour vote( unsigned int id, Ballot ballot );
};

#endif