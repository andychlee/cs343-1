//--------------------------------------------------------------------------------------------------------------------
// Implementation to the Bank monitor
//--------------------------------------------------------------------------------------------------------------------
#include "bank.h"

//--------------------------------------------------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------------------------------------------------
Bank::Bank( unsigned int numStudents ) {
    accounts = new unsigned int[numStudents];               // initialize array to store student-account info
    for ( unsigned int i = 0; i < numStudents; i += 1 ) {
        accounts[i] = 0;                                    // each account initially starts with a balance of $0
    }
} // Bank::Bank

//--------------------------------------------------------------------------------------------------------------------
// Destructor
//--------------------------------------------------------------------------------------------------------------------
Bank::~Bank() {
    delete accounts;                                        // free memory
} // Bank::~Bank

//--------------------------------------------------------------------------------------------------------------------
// Given student id, and amount, deposit money
//--------------------------------------------------------------------------------------------------------------------
void Bank::deposit( unsigned int id, unsigned int amount ) {
    accounts[id] += amount;
} // Bank::deposit

//--------------------------------------------------------------------------------------------------------------------
// Given student id and amount, withdraw money
//--------------------------------------------------------------------------------------------------------------------
void Bank::withdraw( unsigned int id, unsigned int amount ) {
    // waits until enough money has been deposited
    for ( ;; ) {
        if ( amount <= accounts[id] ) break;
        _Accept( deposit );
    }

    accounts[id] -= amount;
} // Bank::withdraw
