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
	  Printer &prt;
	  NameServer & nameServer;
	  WATCardOffice & cardOffice;
	  Groupoff & groupoff;
	  unsigned int id;
	  unsigned int maxPurchases;
	  unsigned int purchaseNum;
	  VendingMachine::Flavours flavour;
	  WATCard::FWATCard watcard;
	  WATCard::FWATCard giftcard;
	  VendingMachine *vendingMachine;
	  void main();
	public:
	  Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
	           unsigned int id, unsigned int maxPurchases );
};

#endif