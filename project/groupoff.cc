//--------------------------------------------------------------------------------------------------------------------
// Implementation to the Groupoff Task
//--------------------------------------------------------------------------------------------------------------------
#include "groupoff.h"

//--------------------------------------------------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------------------------------------------------
Groupoff::Groupoff(Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay):
prt(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay), calledUpon(0) {
    prt.print(Printer::Kind::Groupoff, 'S');
} // Groupoff::Groupoff

//--------------------------------------------------------------------------------------------------------------------
// Destructor
//--------------------------------------------------------------------------------------------------------------------
Groupoff::~Groupoff() {
    for ( ;; ) {                                                        // free memory
        if ( watCards.empty() ) break;
        delete watCards.back();
        watCards.pop_back();
    } // while
} // Groupoff::~Groupoff

//--------------------------------------------------------------------------------------------------------------------
// Return the next Job request
//--------------------------------------------------------------------------------------------------------------------
WATCard::FWATCard Groupoff::giftCard() {
    WATCard::FWATCard w;
    giftCards.push_back(w);
    return w;
} // Groupoff::giftCard


//--------------------------------------------------------------------------------------------------------------------
// The main function of the Groupoff
//--------------------------------------------------------------------------------------------------------------------
void Groupoff::main() {
    for (;;) {                                                          // for each student, accept one giftCard call
        if (calledUpon == numStudents) break;                           // each student has called giftCard routine once
        _Accept(giftCard) {
            calledUpon ++;
        } // _Accept
    } // for

    for (;;) {
        _Accept(~Groupoff) {
            // it is possible that not all students got a gift card
            // it is important the destructor of Groupoff is called only after all student tasks are finished
            break;
        } _Else {   // busy waiting
            if (giftCards.size() == 0) break;                          // only do this if there is still value to assign
            yield(groupoffDelay);                                      // yield a fixed amount of time
            int random_index = mprng(0, (int)giftCards.size()-1);      // choose a random value to assign.
            WATCard *w = new WATCard();
            watCards.push_back(w);                                     // add pointer of watcard to watCards, in order to free memory later
            w->deposit(sodaCost);                                      // put some money in the watCard
            giftCards[random_index].delivery(w);                       // dilevery the real value
            prt.print(Printer::Kind::Groupoff, 'D', sodaCost);         // print that we assigned a cost
            giftCards.erase(giftCards.begin() + random_index);         // remove it from the vector
        }  // _Accept
    } // for
    prt.print(Printer::Kind::Groupoff, 'F');                           // finished
} // Groupoff::main