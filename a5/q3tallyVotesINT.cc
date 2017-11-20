#include <iostream>
#include "q3tallyVotes.h"
#include "q3voter.h"
#include "q3printer.h"
#include "q3utils.h"

using namespace std;

TallyVotes::Tour TallyVotes::vote(unsigned int id, TallyVotes::Ballot ballot) {

		// we first vote
	    picture += ballot.picture;
        statue += ballot.statue;
        giftshop += ballot.giftshop;
        printer.print(id, Voter::States::Vote, ballot);
        waiting ++;
        if (waiting < group) {				//  not the last one, does not have result yet
			printer.print(id, Voter::States::Block, waiting);
        	vote_bench.wait();				//  go to sleep and only wake up when we have a result
			waiting --;
            printer.print(id, Voter::States::Unblock, waiting);
        } else {							// last guy, should have a result
        	result = get_result(picture, statue, giftshop);

	        // clear everything for the next group
	        picture = 0;
	        giftshop = 0;
	        statue = 0;
	    	printer.print(id, Voter::States::Complete);
	    	waiting --;
        }

        vote_bench.signal();					// tell someone to wake up

        return TallyVotes::Tour(result);
}

TallyVotes::TallyVotes( unsigned int group, Printer & printer ): group(group), picture(0), statue(0), giftshop(0), waiting(0), printer(printer) {}
