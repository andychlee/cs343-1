//--------------------------------------------------------------------------------------------------------------------
// Implementation to the Printer
//--------------------------------------------------------------------------------------------------------------------
#include <iostream>
#include "printer.h"

using namespace std;

//--------------------------------------------------------------------------------------------------------------------
// Constructor of Printer::Info
//--------------------------------------------------------------------------------------------------------------------
Printer::Info::Info() : isSet( false ), state( 0 ), value1( -1 ), value2( -1 ) {}

//--------------------------------------------------------------------------------------------------------------------
// Print the information of the instance
//--------------------------------------------------------------------------------------------------------------------
void Printer::Info::printInfo() {
    cout << state;                                      // print state
    if ( value1 != -1 ) {                               // print value1 if applied
        cout << value1;
    } // if
    if ( value2 != -1 ) {                               // print value2 if applied
        cout << "," << value2;
    } // if
} // Printer::Info::printInfo

//--------------------------------------------------------------------------------------------------------------------
// Print all the stored information and reset the isSet flag
//--------------------------------------------------------------------------------------------------------------------
void Printer::flush() {
    // calculate the last column with stored information
    int lastCol = 0;
    for ( int i = numInstances - 1; i >= 0; i -= 1 ) {
        if ( buffer[i].isSet ) {                        // found and last column with information, then break
            lastCol = i;
            break;
        } // if
    } // for

    // print all the stored information
    for ( int i = 0; i < numInstances; i += 1 ) {
        if ( buffer[i].isSet ) {                        // if has information, print it and reset values
            buffer[i].printInfo();
            buffer[i].isSet = false;
            buffer[i].value1 = -1;
            buffer[i].value2 = -1;
        } // if

        if ( i < lastCol ) {                            // don't print tab after the last stored information
            cout << "\t";
        } // if
    } // for

    cout << endl;
} // Printer::flush

//--------------------------------------------------------------------------------------------------------------------
// Constructor of the printer
//--------------------------------------------------------------------------------------------------------------------
Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) :
    numStudents( numStudents ), numVendingMachines( numVendingMachines ), numCouriers( numCouriers ) {
    // calcu total number of columns, 6 includes parent, groupoff, WATCard office, name server, truck, bottling plant
    numInstances = 6 + numStudents + numVendingMachines + numCouriers;
    // print title of objects with single instance
    cout << "Parent\tGropoff\tWATOff\tNames\tTruck\tPlant\t";

    // print title of objects with multiple instances
    for ( unsigned int i = 0; i < numStudents; i += 1 ) {
        cout << "Stud" << i << "\t";
    } // for

    for ( unsigned int i = 0; i < numVendingMachines; i += 1 ) {
        cout << "Mach" << i << "\t";
    } // for

    for ( unsigned int i = 0; i < numCouriers; i += 1 ) {
        cout << "Cour" << i;
        if ( i != numCouriers - 1 ) {                   // don't print tab at the end of line
            cout << "\t";
        } // if
    } // for

    cout << endl;

    for ( int i = 0; i < numInstances; i += 1 ) {       // print second line
        cout << "*******";
        if ( i != numInstances - 1 ) {                  // don't print tab at the end of line
            cout << "\t";
        } // if
    } // for
    cout << endl;

    buffer = new Info[numInstances];                    // initiazlie the buffer
} // Printer::Printer

//--------------------------------------------------------------------------------------------------------------------
// Destructor of the Printer
//--------------------------------------------------------------------------------------------------------------------
Printer::~Printer() {
    flush();                                            // flush all the stored information
    delete buffer;                                      // free memory
    cout << "***********************" << endl;          // more output at the end
} // Printer::~Printer

//--------------------------------------------------------------------------------------------------------------------
// Given kind and state, store the information into the buffer, flush data if needs
//--------------------------------------------------------------------------------------------------------------------
void Printer::print( Kind kind, char state ) {
    // flush the buffered data if the column is about to be overwritten
    if ( buffer[kind].isSet ) {
        flush();
    }

    // store the information
    buffer[kind].isSet = true;
    buffer[kind].state = state;
} // Printer::print

//--------------------------------------------------------------------------------------------------------------------
// Given kind, state and vlaue1, store the information into the buffer, flush data if needs
//--------------------------------------------------------------------------------------------------------------------
void Printer::print( Kind kind, char state, int value1 ) {
    print( kind, state );
    buffer[kind].value1 = value1;
} // Printer::print

//--------------------------------------------------------------------------------------------------------------------
// Given kind, state, value1 and value2, store the information into the buffer, flush data if needs
//--------------------------------------------------------------------------------------------------------------------
void Printer::print( Kind kind, char state, int value1, int value2 ) {
    print( kind, state, value1 );
    buffer[kind].value2 = value2;
} // Printer::print

//--------------------------------------------------------------------------------------------------------------------
// PRIVATE METHOD
// Given kind, state and local identifier, store the information into the buffer, flush data if needs
// Return the index of the instance in the buffer
//--------------------------------------------------------------------------------------------------------------------
int Printer::printAndReturnIndex( Kind kind, unsigned int lid, char state ) {
    // calcu the index of instance in the buffer
    int index = 6;
    switch ( kind ) {
        case Kind::Student:
            index += lid;
            break;
        case Kind::Vending:
            index += numStudents + lid;
            break;
        case Kind::Courier:
            index += numStudents + numVendingMachines + lid;
            break;
        default: // invalid kind
            cerr << "passed invalid kind" << endl;
            break;
    }

    // flush the buffered data if the column is about to be overwritten
    if ( buffer[index].isSet ) {
        flush();
    }

    // store the information
    buffer[index].isSet = true;
    buffer[index].state = state;
    return index;
} // Printer::printAndReturnIndex

//--------------------------------------------------------------------------------------------------------------------
// Given kind, state, local identifier, store the information into the buffer, flush data if needs
//--------------------------------------------------------------------------------------------------------------------
void Printer::print( Kind kind, unsigned int lid, char state ) {
    printAndReturnIndex( kind, lid, state);
} // Printer::print

//--------------------------------------------------------------------------------------------------------------------
// Given kind, state, local identifier and value1, store the information into the buffer, flush data if needs
//--------------------------------------------------------------------------------------------------------------------
void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
    int index = printAndReturnIndex( kind, lid, state);
    buffer[index].value1 = value1;
} // Printer::print

//--------------------------------------------------------------------------------------------------------------------
// Given kind, state, local identifier, value1 and value2, store the information into the buffer, flush data if needs
//--------------------------------------------------------------------------------------------------------------------
void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
    int index = printAndReturnIndex( kind, lid, state);
    buffer[index].value1 = value1;
    buffer[index].value2 = value2;
} // Printer::print
