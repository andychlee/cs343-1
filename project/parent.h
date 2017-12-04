//--------------------------------------------------------------------------------------------------------------------
// Define the interface to the Parent task
// Periodically gives a random number of money [$1, $3] to a random student
//--------------------------------------------------------------------------------------------------------------------
#ifndef __PARENT_H__
#define __PARENT_H__

#include "printer.h"
#include "bank.h"

_Task Parent {
	Printer & prt;          		// printer to print states info
	Bank & bank;					// bank to endow gifts
	unsigned int numStudents;		// number of students
	unsigned int parentalDelay; 	// time to yield

    void main();					// main
  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
};

#endif