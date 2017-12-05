//--------------------------------------------------------------------------------------------------------------------
// Define the interface to the Student
// Student buys soda, and borrows money from the bank
//--------------------------------------------------------------------------------------------------------------------

#ifndef __STUDENT_H__
#define __STUDENT_H__

#include "printer.h"
#include "bank.h"
#include "nameServer.h"
#include "watcardOffice.h"
#include "groupoff.h"
#include "watcard.h"
#include "vendingMachine.h"
#include "MPRNG.h"

_Task Student {
      Printer &prt;                         // print state info
      NameServer & nameServer;              // get vending machine
      WATCardOffice & cardOffice;           // process watcard
      Groupoff & groupoff;                  // group off to get giftcard
      unsigned int id;                      // id of the student
      unsigned int maxPurchases;            // max number of bottles to purchase
      unsigned int purchaseNum;             // number of bottles to purchase
      VendingMachine::Flavours flavour;     // favourite soda to purchase
      WATCard::FWATCard watcard;            // from wardcard office
      WATCard::FWATCard giftcard;           // from groupoff
      VendingMachine *vendingMachine;       // vending machine to buy soda
      void main();
    public:
      Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
               unsigned int id, unsigned int maxPurchases );
};

#endif