#include <iostream>
#include "q3tallyVotes.h"
#include "q3voter.h"
#include "q3printer.h"

using namespace std;

/*
 * Printer related functions
 */
string special_key = "NONE";            // indicate that there is no info for this voter

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