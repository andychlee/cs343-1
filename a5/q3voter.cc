#include "q3voter.h"
#include "q3printer.h"

/*
 * Voter related functions
 */


void Voter::main() {

    // starting stage, need to yield a random time until start
    yield(mprng(19));
    printer.print(id, Voter::States::Start);
    yield(1);

    // create a ballot for voting, and call vote routine in the TallyVotes routine
    TallyVotes::Ballot ballot = Voter::cast();
    TallyVotes::Tour result = voteTallier.vote(id, ballot);
    // printer.print(id, Voter::States::Vote, ballot);
    yield(1);
    printer.print(id, Voter::States::Finished, result);
}