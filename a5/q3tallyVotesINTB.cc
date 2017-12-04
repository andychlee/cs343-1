#include <iostream>
#include "q3tallyVotes.h"
#include "q3voter.h"
#include "q3printer.h"
#include "q3utils.h"

using namespace std;

// neccessary functions to simulate a java monitor
void TallyVotes::wait() {
    bench.wait();                            // wait until signalled
    while ( rand() % 2 == 0 ) {              // multiple bargers allowed
        _Accept( vote ) {                    // accept barging callers
        } _Else {                            // do not wait if no callers
        } // _Accept
    } // while
}

void TallyVotes::signalAll() {               // also useful
    while ( ! bench.empty() ) bench.signal();// drain the condition
}

TallyVotes::Tour TallyVotes::vote(unsigned int id, TallyVotes::Ballot ballot) {
		arrive_id ++;								// give the new guy a id, first guy has id 1, and increment every time.
		unsigned int my_id = arrive_id;				// store this on the local stack, as if the arrive_id will be updated by other tasks
		while (my_id > max_id_this_round) {			// this guy is barging, as he should belong to the next group
			printer.print(id, Voter::States::Barging);
			wait();
		}

		// now now we can vote
	    picture += ballot.picture;
        statue += ballot.statue;
        giftshop += ballot.giftshop;
        printer.print(id, Voter::States::Vote, ballot);
        waiting ++;
        if (waiting < group) {						//  not the last one, does not have result yet
			printer.print(id, Voter::States::Block, waiting);
        	wait();									//  go to sleep and only wake up when we have a result
			waiting --;
            printer.print(id, Voter::States::Unblock, waiting);
        } else {									// last guy, should have a result
        	result = get_result(picture, statue, giftshop);

	        // clear everything for the next group
	        picture = 0;
	        giftshop = 0;
	        statue = 0;
	    	printer.print(id, Voter::States::Complete);
	    	waiting --;
			signalAll();						// last guy is responsible for waking everybody
        }
        // to solve the problem that there may be barging
        if (waiting == 0) {						// this is the last guy in this group, he has some extra thing to do
	    	max_id_this_round += group;			// prepare for next round, raise the bar and let the next group to come in
        	signalAll();						// if we do not do these, next group maybe in trouble, we should wake those guys
        }

        return TallyVotes::Tour(result);
}

TallyVotes::TallyVotes( unsigned int group, Printer & printer ): arrive_id(0), max_id_this_round(group), group(group), picture(0), statue(0), giftshop(0), waiting(0), printer(printer) {}
