//--------------------------------------------------------------------------------------------------------------------
// Define the interface to the Groupoff
// Accept a call from each student to obtain a giftcard
//--------------------------------------------------------------------------------------------------------------------

#ifndef __GROUP_OFF_H__
#define __GROUP_OFF_H__

#include <vector>
#include "printer.h"
#include "watcard.h"
#include "MPRNG.h"

using namespace std;

_Task Groupoff {
      Printer &prt;
      unsigned int numStudents;                     // number of stduents
      unsigned int sodaCost;                        // cost of sode
      unsigned int groupoffDelay;                   // delay between delivery the real value in the future
      vector<WATCard::FWATCard> giftCards;          // a list of gift cards
      vector<WATCard*> watCards;                    // a list of real watcards
      unsigned int calledUpon;                      // how many times the giftCard routine is called
      void main();
    public:
      Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
      ~Groupoff();
      WATCard::FWATCard giftCard();                 // interface function that return a future of watcard pointer
};

#endif