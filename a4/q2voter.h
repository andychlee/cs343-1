#ifndef __Q2_VOTER_H__
#define __Q2_VOTER_H__

#include "q2tallyVotes.h"
#include "MPRNG.h"

extern MPRNG mprng;

_Task Voter {
    unsigned int id;
    TallyVotes& voteTallier;
    Printer &printer;

    // Choose ranking of picture tour, then relationship of statue to gift shop.
    TallyVotes::Ballot cast() {         // cast 3-way vote
        static unsigned int voting[3][2][2] = { { {2,1}, {1,2} }, { {0,2}, {2,0} }, { {0,1}, {1,0} } };
        unsigned int picture = mprng( 2 ), statue = mprng( 1 );
        return (TallyVotes::Ballot){ picture, voting[picture][statue][0], voting[picture][statue][1] };
    }

    // the main function that starts running
    void main();
  public:
    enum States { Start = 'S', Vote = 'V', Block = 'B', Unblock = 'U', Barging = 'b',
                   Complete = 'C', Finished = 'F' };
    Voter( unsigned int id, TallyVotes & voteTallier, Printer & printer ): id(id), voteTallier(voteTallier), printer(printer) {};
};

#endif