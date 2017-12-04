//--------------------------------------------------------------------------------------------------------------------
// Define the interface to the NameServer
// An administrator task used to manage the vending-machine names
//--------------------------------------------------------------------------------------------------------------------

#ifndef __NAME_SERVER_H__
#define __NAME_SERVER_H__

#include "printer.h"
_Task NameServer;							// pre-declare
#include "vendingMachine.h"

_Task NameServer {
	Printer & prt;
	unsigned int numVendingMachines; 		// number of vending machines
	unsigned int numStudents;		 		// number of students
	unsigned int numMachineRegistered; 		// number of VM registered

	unsigned int * positions;				// array of positions of the VM for students
	VendingMachine ** machines;				// array of pointers to vending machines

	void main();
  public:
	NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
	~NameServer();
	void VMregister( VendingMachine *vendingmachine );	// register VM
	VendingMachine * getMachine( unsigned int id );		// find a vending machine
	VendingMachine ** getMachineList();					// get machines
};

#endif
