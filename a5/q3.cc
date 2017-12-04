#include <iostream>
#include "q3tallyVotes.h"
#include "q3voter.h"
#include "q3printer.h"

using namespace std;
/*
 * main program
 */

MPRNG mprng;

void usage() {
    cerr << "Usage: ./vote  Voters (> 0 & V mod G = 0, default 6)  Group (> 0, default 3)  Seed (> 0)" << endl;
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
    TallyVotes tally_votes((unsigned int)group, printer);
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