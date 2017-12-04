#include <iostream>
#include "q3tallyVotes.h"
#include "q3voter.h"
#include "q3printer.h"
#include "q3utils.h"

using namespace std;

TallyVotes::Tour TallyVotes::vote(unsigned int id, TallyVotes::Ballot ballot) {
        // now this function should act only as an interface
        // take in parameters, copy to server(task TallyVotes), and let task TallyVotes do the job
        voter_id = id;
        voter_ballot = ballot;
        bench.wait(voter_id);            // needs to pass this voter_id as when we wake him up, we need to know which task we are waking up to print

        return Tour(TallyVotes::result); // after woke up, result shoulb be ready
}

void TallyVotes::main() {
    for(;;) {
        _Accept(~TallyVotes) {
            return;
        } or _Accept(vote) {
            // now we vote
            picture += voter_ballot.picture;
            statue += voter_ballot.statue;
            giftshop += voter_ballot.giftshop;
            printer.print(voter_id, Voter::States::Vote, voter_ballot);

            // Since every voter blocks, we should print out block
            // note this time for everyone in the group
            waiting ++;
            printer.print(voter_id, Voter::States::Block, waiting);

            // looping util the next everybody is here, so the next if is true, which is we have a group 
            if (waiting == group) {
                // we shoud have the result
                // first calculate the result, store in the result variable
                // one by one wake them up and let them go

                result = get_result(picture, statue, giftshop);
                picture = 0;
                giftshop = 0;
                statue = 0;

                for (unsigned int i=0; i< group; i++) {
                    waiting --;
                    unsigned int cur_id = bench.front();
                    if (i == group-1) {
                        printer.print(cur_id, Voter::States::Complete);              // in the executable given, complete is printed by the last guy before printing unlock
                    }
                    printer.print(cur_id, Voter::States::Unblock, waiting);
                    bench.signalBlock();
                }
            }

        }
    }
}

TallyVotes::TallyVotes( unsigned int group, Printer & printer ): group(group), picture(0), statue(0), giftshop(0), waiting(0), printer(printer) {}
