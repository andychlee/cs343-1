//--------------------------------------------------------------------------------------------------------------------
// Define the interface to the Bank
// Behaves like a server, that manages student-account information for all students
//--------------------------------------------------------------------------------------------------------------------
#ifndef __BANK_H__
#define __BANK_H__

_Monitor Bank {
    unsigned int * accounts;                                // student-account information for all students
  public:
    Bank( unsigned int numStudents );                       // constructor
    ~Bank();                                                // destructor
    void deposit( unsigned int id, unsigned int amount );   // deposit money to account id
    void withdraw( unsigned int id, unsigned int amount );  // withdraw money to account id
};

#endif
