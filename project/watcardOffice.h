//--------------------------------------------------------------------------------------------------------------------
// Define the interface to the watcardOffice
// An administrator task used by a student to transfer funds from bank account to WATCard
//--------------------------------------------------------------------------------------------------------------------

#ifndef __WATCardOffice_H__
#define __WATCardOffice_H__

#include <list>
#include "printer.h"
#include "bank.h"
#include "watcard.h"

using namespace std;

_Task WATCardOffice {
    Printer & prt;                              // printer to print states info
    Bank & bank;                                // bank to endow gifts
    unsigned int numCouriers;                   // number of couriers
    struct Job {                                // marshalled arguments and return future
        struct Args {
            unsigned int sid;                   // student id
            unsigned int amount;                // amount to transfer
            WATCard *card;                      // WATCard card
            Args( unsigned int sid, unsigned int amount, WATCard *card ) :
                sid( sid ), amount( amount ), card( card ) {}
        };
        Args args;                              // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;               // return future
        Job( Args args ) : args( args ) {}      // constructor of Job
    };
    _Task Courier {                             // communicates with bank
        Printer & prt;                          // printer to print states info
        Bank & bank;                            // bank to endow gifts
        WATCardOffice & cardOffice;             // WATCardOffice to request work
        unsigned lid;                           // local identifier of the courier
        void main();
      public:
        Courier( Printer &prt, Bank &bank, WATCardOffice &cardOffice, unsigned int lid );
    };
    Courier ** courierPool;                     // courier tasks to communicate with the bank
    list<Job*> requests;                        // work requests
    list<WATCard*> cardList;                    // save the pointers of the WATCard created, for memory free
    void main();
  public:
    _Event Lost {};                             // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

#endif
