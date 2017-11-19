#include <iostream>
#include "q2tallyVotes.h"
#include "q2voter.h"
#include "q2printer.h"

using namespace std;

MPRNG mprng;
string special_key = "NONE";            // indicate that there is no info for this voter

/*
 * Printer related functions
 */

string get_final_choice(TallyVotes::Tour tour) {
    string result;
    string t = to_string(tour);

    if(t == "0") {
        result = "p";
    } else if (t == "1") {
        result = "s";
    } else {
        result = "g";
    }
    return result;
}

string get_state(Voter::States state) {
    switch(state){
        case Voter::States::Start:
            return "S";
        case Voter::States::Vote:
            return "V";
        case Voter::States::Block:
            return "B";
        case Voter::States::Unblock:
            return "U";
        case Voter::States::Barging:
            return "b";
        case Voter::States::Complete:
            return "C";
        default:
            return "F";
    }
}

// constructor
Printer::Printer( unsigned int voters ): voters(voters) {
    // first construct the necessary data structure to hold the voters info.
    buffer = new string[voters];
    for (unsigned int i=0; i < voters; i++) {            
        buffer[i] = "NONE";                                                    // in the beginning , all voters have no info in the buffer
    }

    // print header of the output
    for (unsigned int i=0; i < voters; i++) {
        cout << "V" << i;
        if (i != voters - 1) cout << "\t";
    }
    cout << endl;
    for (unsigned int i=0; i < voters; i++) {
        cout << "*******";
        if (i != voters -1) cout << "\t";
    }
    cout << endl;
}

// destructor
Printer::~Printer() {
    flush("NONE", 0);                                                       // print the last F g as no one will try to overite it, so we manually flush it
    // now print the ending message
    cout << "*****************" << endl;
    cout << "All tours started" << endl; 
    delete [] buffer;
}

// overriding printers
void Printer::print( unsigned int id, Voter::States state) {
    if (buffer[id] == "NONE") buffer[id] = get_state(state);
    else flush(get_state(state), id);
}

void Printer::print( unsigned int id, Voter::States state, TallyVotes::Tour tour ) {
    string temp_output = get_state(state) + " " + get_final_choice(tour);
    if (buffer[id] == "NONE") buffer[id] = temp_output;
    else flush(temp_output, id);
};

void Printer::print( unsigned int id, Voter::States state, TallyVotes::Ballot ballot ) {
    string temp_output = get_state(state) + " " + to_string(ballot.picture) + "," + to_string(ballot.statue) + "," + to_string(ballot.giftshop);
    if (buffer[id] == "NONE") buffer[id] = temp_output;
    else flush(temp_output, id);
};

void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked ) {
    string temp_output = get_state(state) + " " + to_string(numBlocked);
    if (buffer[id] == "NONE") buffer[id] = temp_output;
    else flush(temp_output, id); 
};

// flush function in Printer, the one that actually prints
void Printer::flush(string new_data, unsigned int pos) {
    // first, we need to find the right most position that should stop printing
    unsigned int right_most = voters - 1;
    for (int i = (unsigned int)right_most; i >= 0; i--) {
        if (buffer[i] != special_key) {
            right_most = (unsigned int)i;
            break;
        }
    }

    // print the output to std out
    for (unsigned int i =0; i <= right_most; i++) {
        if (buffer[i] != special_key) cout << buffer[i];
        if (i != right_most) cout << "\t";
    }
    cout << endl;

    // restore the NONE to the buffer to indicate that old data has been flushed
    for (unsigned int i=0; i < voters; i++) {
        buffer[i] = "NONE";
        if (i == pos) buffer[i] = new_data;
    }
}

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


/*
 *  Tallyvotes related functions
 */

int get_result(int picture, int statue, int giftshop) {
    int Picture = 0, Statue =1, GiftShop = 2;
    if (picture > giftshop) {
        if (picture >= statue) return Picture;                   // picture has higher priority than statue 
        else return Statue;                                      // Statue has the highest ranking so we choose it
    } else if (picture == giftshop) {
        if (picture < statue) return Statue;                     // Statue has the highest ranking so we choose it
        else return GiftShop;                                    // gift shop has highest priority
    } else {                                                     // gift shop > picture case
        if (statue > giftshop) return Statue;                    // Statue has the highest ranking so we choose it
        else return GiftShop;                                    // gift shop has the highest priotity
    }
}


#if defined( IMPLTYPE_MC )              // mutex/condition solution
TallyVotes::Tour TallyVotes::vote(unsigned int id, TallyVotes::Ballot ballot) {
    mlk.acquire();
    try {
        if (in_use) {                                        // barging prevention
            printer.print(id, Voter::States::Barging);       // tell the world we are barging
            barging_bench.wait(mlk);                 
        }
        in_use = true;                                       // after wake up, first lock the door

        // now we vote
        picture += ballot.picture;
        statue += ballot.statue;
        giftshop += ballot.giftshop;
        printer.print(id, Voter::States::Vote, ballot);

        // after we vote, there are two possible cases
        // one is that we are missing someone, so we block and wait for them, (print B n -> U n -> F result)
        // the other one is that this task is that last one, so it just go ahead sum the result, (print C -> F result)

        if (waiting == group - 1) {                         // I am the last guy, so I should print C and get the result and wake up someone in the waiting bench
            result = get_result(picture, statue, giftshop);

            // clear everything for the next group
            picture = 0;
            giftshop = 0;
            statue = 0;
            printer.print(id, Voter::States::Complete);
        } else {                                            // still need wait for more, sleep on the waiting bench
            waiting ++;                                     
            printer.print(id, Voter::States::Block, waiting);
            if (!barging_bench.empty()){
                barging_bench.signal();
            } else {
                in_use = false;
            }
            waiting_bench.wait(mlk);

            // wake up from sleeping, printing the still blocked number
            waiting --;
            printer.print(id, Voter::States::Unblock, waiting);
        }

        // when control reaches here, we should have a result, wake up someone in the waiting bench if there is any
        if (!waiting_bench.empty()) {
            in_use = true;                  // as long as there is somebody in the waiting bench, next group's guys should not come int
            waiting_bench.signal();
        }
        // now we compute the final result and print it
        in_use = false;

    } _Finally {
        // in case there is next group's guys are waiting in the barging bench, wake one of them up
        if (!barging_bench.empty()) barging_bench.signal();
        mlk.release();
    }
    return TallyVotes::Tour(result);
}
#elif defined( IMPLTYPE_SEM )           // semaphore solution
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
#elif defined(IMPLTYPE_BAR)
    TallyVotes::Tour TallyVotes::vote(unsigned int id, TallyVotes::Ballot ballot) {

        // now we vote
        picture += ballot.picture;
        statue += ballot.statue;
        giftshop += ballot.giftshop;
        printer.print(id, Voter::States::Vote, ballot);

        // after we vote, there are two possible cases
        // one is that we are missing someone, so we block and wait for them, (print B n -> U n -> F result)
        // the other one is that this task is that last one, so it just go ahead sum the result, (print C -> F result)

        if (waiters() == total() - 1) {                         // I am the last guy, so I should print C and get the result and wake up someone in the waiting bench
            result = get_result(picture, statue, giftshop);

            // clear everything for the next group
            picture = 0;
            giftshop = 0;
            statue = 0;
            block();                                            // wait fot the next n threadds
            printer.print(id, Voter::States::Complete);
        } else {                                                // still need wait for more, sleep on the waiting bench
            printer.print(id, Voter::States::Block, waiters()+1);
            block();                                    
            printer.print(id, Voter::States::Unblock, waiters());
        }

        // in case there is next group's guys are waiting in the barging bench, wake one of them up
        return TallyVotes::Tour(result);
    }
#else 
    #error unsupported voter type
#endif

/*
 * main program
 */

void usage() {
    cerr << "Usage: ./voteMC  Voters (> 0 & V mod G = 0, default 6)  Group (> 0, default 3)  Seed (> 0)" << endl;
    exit(-1);
}

int main(int argc, char * argv[]) {

    // read in input
    int voters_count = 6, group = 3, r_seed = getpid();

    switch(argc) {
        case 4:
            try {
                r_seed = stoi(argv[3]);
                if (r_seed <= 0) throw 1;
            } catch(...) {
                cerr << "the seed must be a positive integer" << endl;
                usage();
            }
        case 3:
            try {
                group = stoi(argv[2]);
                if (group <= 0) throw 1;
            } catch(...) {
                cerr << "the group must be a positive integer" << endl;
                usage();
            }
        case 2:
            try {
                voters_count = stoi(argv[1]);
                if (voters_count <= 0 || voters_count % group != 0) throw 1;
            } catch(...) {
                cerr << "the voters must be a positive integer and it must be divisable by group" << endl;
                usage();
            }
        case 1:
            break;

        default:
            cerr << "invalid number of arguments" << endl;
            usage();

    }



    Printer printer((unsigned int)voters_count);
    TallyVotes tally_votes(group, printer);
    Voter* voters[voters_count];

    mprng.set_seed(r_seed);

    // creating tasks
    for (int i = 0; i < voters_count; i++) {
        voters[i] = new Voter(i, tally_votes, printer);
    }

    // ending tasks
    for (int i = 0; i < voters_count; i++) {
        delete voters[i];
    }

}