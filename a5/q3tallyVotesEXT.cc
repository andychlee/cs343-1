#include <iostream>
#include "q2tallyVotes.h"
#include "q2voter.h"
#include "q2printer.h"

using namespace std;
MPRNG mprng;

TallyVotes::Tour TallyVotes::vote(unsigned int id, TallyVotes::Ballot ballot) {
        if (barging_bench.counter() == 0) printer.print(id, Voter::States::Barging);
        barging_bench.P(); 
        mlk.P();
                                         // after wake up, first lock the door

            // now we vote
            picture += ballot.picture;
            statue += ballot.statue;
            giftshop += ballot.giftshop;
            printer.print(id, Voter::States::Vote, ballot);
            waiting ++;
            // after we vote, there are two possible cases
            // one is that we are missing someone, so we block and wait for them, (print B n -> U n -> F result)
            // the other one is that this task is that last one, so it just go ahead sum the result, (print C -> F result)

            if (waiting == group) {                         // I am the last guy, so I should print C and get the result and wake up someone in the waiting bench
                result = get_result(picture, statue, giftshop);

                // clear everything for the next group
                picture = 0;
                giftshop = 0;
                statue = 0;
                printer.print(id, Voter::States::Complete);
            } else {                                            // still need wait for more, sleep on the waiting bench
                barging_bench.V();
                mlk.V();                                     
                printer.print(id, Voter::States::Block, waiting);
                waiting_bench.P();
                mlk.P();
                printer.print(id, Voter::States::Unblock, waiting);
            }

            // when control reaches here, we should have a result, wake up someone in the waiting bench if there is any
            if (!waiting_bench.empty()) {                      // waiting is non-zero
                waiting_bench.V();
            } else {
                barging_bench.V();
            }
            waiting --;
        mlk.V();
        return TallyVotes::Tour(result);
    }